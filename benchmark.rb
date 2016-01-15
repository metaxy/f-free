require 'date'
require 'json'
require 'fileutils'
require 'optparse'

$BIN_PATH = "./build"
$TMP_FILE = DateTime.now.strftime("./tmp/tmp_%Y_%m_%d__%H_%M_%S_")+rand(1 .. 500000000).to_s
$BENCHMARKS_PATH = "./data_benchmarks"

def create_command(name, input, forbidden, timeout, seed)
  format_string_time = '{"elapsed_time": "%E", "kernel_time" : "%S", "user_time" : "%U", "cpu_usage" : "%P", "max_memory" : "%M", "page_faults" : "%F", "context_switches_forced" : "%c", "context_switches": "%w", "io_input": "%I", "io_output": "%O"}'
  return "/usr/bin/time -f '#{format_string_time}' --output '#{$TMP_FILE}.time' timeout  #{timeout}s #{$BIN_PATH}/ffree_#{name} --input '#{input}' --forbidden '#{forbidden}' --seed #{seed} 2>#{$TMP_FILE}.log"
end

def create_simple_command(name, input, forbidden, timeout, seed)
  return "timeout #{timeout}s #{$BIN_PATH}/ffree_#{name} --input '#{input}' --forbidden '#{forbidden}' --seed #{seed}"
end

def parse_log(file)
end


def check_env()
  if(`git status -s` != "")
    puts "Please commit first"
    exit
  end
  puts "Compiling..."
  puts `cd '#{$BIN_PATH}' && make`
  FileUtils.mkpath './tmp'
end

def parse_time(file_name)
  file = File.read(file_name)
  if(file.start_with? 'Command exited')
    lines = file.split(/(\n)/)
    data = JSON.parse(lines[2])
    data['extra'] = lines[0]
    return data
  else
    return JSON.parse(file)
  end
end

def run_a_config(config, options, forbidden, instances)
  bench_folder = DateTime.now.strftime("#{$BENCHMARKS_PATH}/%Y_%m_%d__%H_%M_%S")
  time_string = DateTime.now.strftime("%Y.%m.%d %H:%M:%S")
   
  fileName = "#{bench_folder}/results.json"
  FileUtils.mkpath(bench_folder)
  output = {}
  output['options'] = options
  output['config'] = config.dup
  output['config']['forbidden'] = forbidden
  output['config']['instances'] = instances
  
  output['start_time'] = Time.now.strftime "%Y-%m-%d %H:%M:%S"
  output['git_hash'] = `git rev-parse --verify HEAD`
  output['commit_message'] = `git log -1 --pretty=%B`.strip!
  
  output['results'] = {}
  output['graphs'] = {}
  quality = Hash.new
  count = Hash.new
  time = Hash.new
  failed = Hash.new
  
  entries = Dir.entries(instances+"/")
  entries_size = entries.size
  current_file = 0
  i = 0
  if(!File.exists? "#{instances}/#{File.basename(forbidden)}.k.json")
    abort("no optimal results computed for #{forbidden} on #{instances}")
  end
          
  sols = JSON.parse(File.read("#{instances}/#{File.basename(forbidden)}.k.json"))
  entries.each do |graph|
    current_file += 1
    
    next if graph == "k.txt" or graph == "k-cvd.txt" or graph.end_with? ".zip" 
    next if graph.start_with? "."
    next if not (graph.end_with? ".txt" or graph.end_with? ".graph")
    
    kcorrect = sols[graph]['min_k'].to_i
    puts "# File #{current_file} of #{entries_size} (#{graph})"
    
    graph_content = File.read(instances+"/"+graph)
    
    no_correct = false
    no_correct = true if kcorrect == -1
    
    output['graphs'][graph] = sols[graph]
    config["progs"].each do |prog|
      puts "this is one is not solved" if no_correct
      command = create_command(prog, instances+"/"+graph, forbidden, config["max_time"], config["seed"])
      simple_command = create_simple_command(prog, instances+"/"+graph, forbidden, config["max_time"], config["seed"])
      
      start = Time.now
      ret = `#{command}`
      finish = Time.now
      i += 1
      result_file_name = "output_#{graph}_#{prog}__#{i}.json"
      File.write(bench_folder+"/"+result_file_name, ret.split("\n").select{ |line| ! line.start_with?("#")}.map{ |line| line.split(" ")})
      
      debug_out = {}
      if(ret.chomp == "")
        k = -1
        qual = 0
      else
        k = ret.split("\n").select{ |line| ! line.start_with?("#")}.size
        debug_outstring = ret.split("\n").select{ |line| line.start_with?("#debug:")}
        if(debug_outstring.size() > 0)
          debug_out = JSON.parse(debug_outstring[0].slice(8,debug_outstring[0].size))
        end
        
        if(no_correct == false)
          if(kcorrect != 0)
            qual = kcorrect.to_f/k.to_f
          else # it has to be k == 0, because they should solve right
            qual = 1
          end
        else
          qual = 1
        end
      end
      diff = finish - start
      
      if(quality[prog].nil?)
        quality[prog] = 0.0
        count[prog] = 0
        time[prog] = 0.0
        failed[prog] = 0
      end
      
      if(!no_correct)
        quality[prog] += qual
        count[prog] += 1
      end
      
      if(k == -1)
        failed[prog] += 1
      else
        time[prog] += diff
      end
      
      if(output['results'][graph].nil?)
        output['results'][graph] = []
      end
      output['results'][graph] << {
        "prog" => prog,
        "graph" => graph,
        "k" => k,
        "kcorrect" => kcorrect,
        "time" => diff,
        "quality" => qual,
        "simple_command" => simple_command,
        "time_log" => parse_time($TMP_FILE+'.time'),
        "log_output" => File.read($TMP_FILE+'.log'),
        "result_file_name" => bench_folder+"/"+result_file_name,
        "model_file_name" => instances+"/"+graph,
        "debug_out" => debug_out
      }
      puts "[#{prog}] k: #{k} of #{kcorrect} (#{qual*100}%)"
      FileUtils.rm($TMP_FILE+'.time')
      FileUtils.rm($TMP_FILE+'.log')
      
      File.write(fileName,  JSON.pretty_generate(output))
    end
  end
  output['end_time'] = Time.now.to_s
  
  output['stats'] = {}
  
  config["progs"].each do |prog|
    
    output['stats'][prog] = 
    {
      "quality" => (quality[prog]/count[prog].to_f)*100,
      "failed" => failed[prog],
      "failed_percent" => (failed[prog]/count[prog].to_f)*100,
      "mean_time" => (time[prog]/(count[prog]-failed[prog]).to_f)
    }
    puts "[#{prog}] qual: #{output['stats'][prog]["quality"]}, qualSolved: #{output['stats'][prog]["quality_solved"]}, failed:  #{output['stats'][prog]["failed"]}"
  end
  
  File.write(fileName,  JSON.pretty_generate(output))
  
  if(File.exists? $BENCHMARKS_PATH+'/list.json')
    list = JSON.parse(File.read($BENCHMARKS_PATH+'/list.json'))
  else
    list = []
  end
  list << {"fileName" => fileName, 
           "time" => time_string, 
           "progs" =>  config["progs"], 
           "forbidden" => forbidden, 
           "instances" => instances}
  File.write($BENCHMARKS_PATH+'/list.json',  JSON.pretty_generate(list))
  
end


def main()
  check_env()

  options = {}
  OptionParser.new do |opts|
    opts.banner = "Usage: benchmark.rb [options]"

    opts.on("-c", "--config CONFIG", "Which config file should be used for this benchmark") do |v|
      options[:config] = v
    end
  end.parse!
  abort("config option no set") if options[:config].nil?
  c = File.read(options[:config])
  abort("config file not found") if c.nil?
  config = JSON.parse(c)
  
  config["instances"].each do |instance|
    config["forbidden"].each do |forbidden|
      run_a_config(config, options, forbidden, instance)
    end
  end
  
end


main()

