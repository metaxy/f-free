require 'date'
require 'json'
require 'fileutils'

$BIN_PATH = "./build"
$INSTANCES = "./model/cluster_very_small"
$FORBIDDEN = "./forbidden/cluster"
$PROGS = ["random --rounds 1", "blp"]
$SEED = "5489"
$MAX_TIME=5
$TMP_FILE = DateTime.now.strftime("%Y_%m_%d__%H_%M_%S_")+rand(1 .. 500000000).to_s

def create_command(name, input, forbidden, timeout)
  format_string_time = '{"elapsed_time": "%E", "kernel_time" : "%S", "user_time" : "%U", "cpu_usage" : "%P", "max_memory" : "%M", "page_faults" : "%F", "context_switches_forced" : "%c", "context_switches": "%w", "io_input": "%I", "io_output": "%O"}'
  return "/usr/bin/time -f '#{format_string_time}' --output '#{$TMP_FILE}.time' timeout  #{timeout}s #{$BIN_PATH}/ffree_#{name} --input '#{input}' --forbidden '#{forbidden}' --seed #{$SEED} 2>#{$TMP_FILE}.log"
end

def create_simple_command(name, input, forbidden, timeout)
  return "timeout #{timeout}s #{$BIN_PATH}/ffree_#{name} --input '#{input}' --forbidden '#{forbidden}' --seed #{$SEED}"
end

def parse_log(file)
end


def check_env()
  if(`git status -s` != "")
    puts "Please commit first"
    exit
  end
  `cd '$BIN_PATH' && make`
end

def parse_time(file_name)
  file = File.read(file_name)
  if(file.starts_with? 'Command exited')
    lines = file.lines
    data = JSON.parse(lines[1])
    data['extra'] = lines[0]
    return data
  else
    return JSON.parse(file)
  end
end


def main()
  
  fileName = DateTime.now.strftime("benchmarks/bench_%Y_%m_%d__%H_%M_%S.json")  
  output = {}
  
  
  output['instances'] = $INSTANCES
  output['forbidden'] = $FORBIDDEN
  output['start_time'] = Time.now.to_s
  output['git_hash'] = `git rev-parse --verify HEAD`
  output['commit_message'] = `git log -1 --pretty=%B`
  output['seed'] = $SEED
  output['max_time'] = $MAX_TIME
  
  output['results'] = []
  quality = Hash.new
  count = Hash.new
  time = Hash.new
  failed = Hash.new
  
  entries = Dir.entries($INSTANCES+"/")
  entries_size = entries.size
  current_file = 0
  entries.each do |graph|
    current_file += 1
    
    next if graph == "k.txt" or graph == "k-cvd.txt" or graph.end_with? ".zip" 
    next if graph.start_with? "."
    next if not (graph.end_with? ".txt" or graph.end_with? ".graph")
    grep = `grep #{graph} #{$INSTANCES}/all.k`
    kcorrect = grep.split()[1].to_i
    puts "# File #{current_file} of #{entries_size}"
    $PROGS.each do |prog|
      command = create_command(prog, $INSTANCES+"/"+graph, $FORBIDDEN, $MAX_TIME)
      simple_command = create_simple_command(prog, $INSTANCES+"/"+graph, $FORBIDDEN, $MAX_TIME)
      
      start = Time.now
      ret = `#{command}`
      
      finish = Time.now
      if(ret.chomp == "")
        k = -1
        qual = 0
      else
        k = ret.split("\n").select{ |line| ! line.start_with?("#")}.size
        if(kcorrect != 0)
          qual = kcorrect.to_f/k.to_f
        else
          if(k == 0)
            qual = 1
          else
            qual = (kcorrect+0.1)/(k+0.1)
          end
        end
      end
      diff = finish - start
      if(quality[prog].nil?)
        quality[prog] = 0.0
        count[prog] = 0
        time[prog] = 0.0
        failed[prog] = 0
      end
      quality[prog] += qual
      count[prog] += 1
      if(k == -1)
        failed[prog] += 1
      end
      time[prog] += diff
      output['results'] << {
        "prog" => prog,
        "graph" => graph,
        "k" => k,
        "kcorrect" => kcorrect,
        "time" => diff,
        "quality" => qual,
        "command" => command,
        "simple_command" => simple_command,
        "time_log" => parse_time($TMP_FILE+'.time'),
        "log_output" => File.read($TMP_FILE+'.log')
      }
      FileUtils.rm($TMP_FILE+'.time')
      FileUtils.rm($TMP_FILE+'.log')
      
      File.write(fileName,  JSON.pretty_generate(output))
    end
  end
  output['end_time'] = Time.now.to_s
  
  output['stats'] = {}
  $PROGS.each do |prog|
    output['stats'][prog] = {
      "quality" => (quality[prog]/count[prog].to_f)*100,
      "failed" => failed[prog],
      "failed_percent" => (failed[prog]/count[prog].to_f)*100,
      "mean_time" => (time[prog]/count[prog].to_f)
    }
  end
  
  File.write(fileName,  JSON.pretty_generate(output))
end


main()

