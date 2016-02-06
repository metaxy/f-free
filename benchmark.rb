require 'date'
require 'json'
require 'fileutils'
require 'optparse'
require_relative 'utils'

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
  puts "RUN TESTS on #{forbidden} for #{forbidden}"
  if(!File.exists? "#{instances}/#{File.basename(forbidden)}.k.json")
    puts("no optimal results computed for #{forbidden} on #{instances}")
    return
  end
  
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
  count_quality = Hash.new
  count = Hash.new
  time = Hash.new
  failed = Hash.new
  
  entries = Dir.entries(instances+"/")
  entries_size = entries.size
  current_file = 0
  i = 0

          
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
      command = create_command(prog, instances+"/"+graph, forbidden, config["max_time"], config["seed"])
      simple_command = create_simple_command(prog, instances+"/"+graph, forbidden, config["max_time"], config["seed"])
      
      start = Time.now
      ret = `#{command}`
      finish = Time.now
      i += 1
      result_file_name = "output_#{graph}_#{prog}__#{i}.json"
      File.write(bench_folder+"/"+result_file_name, ret.split("\n").select{ |line| ! line.start_with?("#")}.map{ |line| line.split(" ")}) # write result
      
      k = get_k(ret) # calculated distance by prog

      run_time = finish - start
      
      if(output['results'][graph].nil?)
        output['results'][graph] = []
      end
      
      output['results'][graph] << {
        "prog" => prog,
        "graph" => graph,
        "metrics" => get_metrics(k, kcorrect),  
        "time" => run_time,
        "simple_command" => simple_command,
        "time_log" => parse_time($TMP_FILE+'.time'),
        "log_output" => File.read($TMP_FILE+'.log'),
        "result_file_name" => bench_folder+"/"+result_file_name,
        "model_file_name" => instances+"/"+graph,
        "debug_out" => get_debug(ret)
      }
      
      puts "[#{prog}] k: #{k} of #{kcorrect}"
      FileUtils.rm($TMP_FILE+'.time') # cleanup temp files
      FileUtils.rm($TMP_FILE+'.log')
      
      File.write(fileName,  JSON.pretty_generate(output)) # save log in case that the script fails
    end
  end
  output['end_time'] = Time.now.to_s
  
  output['stats'] = {}
  
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

def calculate_stats(data)
  ret = {}
  data["config"]["progs"].each do |prog|
    results = output["results"].select {|k,v| v["prog"] == prog}
    metrics = results.to_a.map {|a| a[1]["metrics"]}
    hasOptimal = metrics.select {|x| x["no_correct"] == false}
    hasNoOptimal = metrics.select {|x| x["no_correct"] == true}
    failed = metrics.select {|x| x["solved"] == false}.length
    
    ret[prog] = {
      "failed" => failed,
      "failed_percent" => (failed.to_f / results.length.to_f) * 100,
      "avg_absolut" => 
    }
  end
  return ret
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

