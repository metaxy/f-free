require 'date'
require 'json'
require 'fileutils'
require 'optparse'
require_relative 'utils'

def check_env()
  if(`git status -s` != "")
    puts "Please commit first"
    exit
  end
  puts "Compiling..."
  puts `cd '#{$BIN_PATH}' && make`
  FileUtils.mkpath './tmp'
end


def run_a_config(config, options, forbidden, instances)
  puts "RUN TESTS on #{instances} for #{forbidden}"
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
      run = run_prog(prog, instances+"/"+graph, forbidden, config["max_time"], config["seed"])
      i += 1
      result_file_name = "output_#{graph}_#{prog}__#{i}.json"
      File.write(bench_folder+"/"+result_file_name, run['result']) # write result

      if(output['results'][graph].nil?)
        output['results'][graph] = []
      end
      
      output['results'][graph] << {
        "prog" => prog,
        "graph" => graph,
        "metrics" => get_metrics(run['k'], kcorrect),  
        "time" => run['run_time'],
        "simple_command" => run['command'],
        "time_log" => run['time_log'],
        "log_output" => run['time_log'],
        "result_file_name" => bench_folder+"/"+result_file_name,
        "model_file_name" => instances+"/"+graph,
        "debug_out" => run['debug_out']
      }
      
      puts "[#{prog}] k: #{run['k']} of #{kcorrect}"
    
      
      File.write(fileName,  JSON.pretty_generate(output)) # save log in case that the script fails
    end
  end
  output['end_time'] = Time.now.to_s
  
  output['stats'] = calculate_stats(output)
  
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
    results = []
    data["results"].each do |k,rs|
      rs.each do |result|
        results << result if result["prog"] == prog
      end
    end
    
    metrics = results.map {|a| a["metrics"]}
    hasOptimal = metrics.select {|x| x["no_correct"] == false}
    hasNoOptimal = metrics.select {|x| x["no_correct"] == true}
    failed = metrics.select {|x| x["solved"] == false}.length
    
    quality = hasOptimal.map{|a| a["quality"]}
    qualityInv = hasOptimal.map{|a| a["quality_inv"]}
    distance = hasOptimal.map{|a| a["distance"]}
    absolut = metrics.map{|a| a["absolut"]}
    time = results.map{ |a| a["time"]}
    
    wins = 0
    results.each do |result|
      other = data["results"][results["graph"]].select {|x| x["prog"] != prog}
      winning = true
      other.each do |o|
        winning = false if o["metrics"]["absolut"] <= result["metrics"]["absolut"]
      end
      wins += 1 if winning

    end
    ret[prog] = {
      "failed" => failed,
      "failed_percent" => (failed.to_f / results.length.to_f) * 100,
      "quality" =>  quality.mean,
      "qualityStd" =>  quality.standard_deviation,
      "distance" =>  distance.mean,
      "distanceStd" =>  distance.standard_deviation,
      "qualityInv" =>  qualityInv.mean,
      "qualityInvStd" =>  qualityInv.standard_deviation,
      "absolut" =>  absolut.mean,
      "absolutStd" =>  absolut.standard_deviation,
      "time" => time.mean,
      "timeStd" => time.standard_deviation,
      "winning" => wins
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

