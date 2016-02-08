$JSON_TIME = '{"elapsed_time": "%E", "kernel_time" : "%S", "user_time" : "%U", "cpu_usage" : "%P", "max_memory" : "%M", "page_faults" : "%F", "context_switches_forced" : "%c", "context_switches": "%w", "io_input": "%I", "io_output": "%O"}'
$TMP_FILE = DateTime.now.strftime("./tmp/tmp_%Y_%m_%d__%H_%M_%S_")+rand(1 .. 500000000).to_s
$BIN_PATH = "./build"
$BENCHMARKS_PATH = "./data_benchmarks"

module Enumerable

  def sum
    return self.inject(0){|accum, i| accum + i }
  end

  def mean
    return self.sum / self.length.to_f
  end

  def sample_variance
    m = self.mean
    sum = self.inject(0){|accum, i| accum + (i - m) ** 2 }
    return sum / (self.length - 1).to_f
  end

  def standard_deviation
    return Math.sqrt(self.sample_variance)
  end

end

def run_prog(name, input, forbidden, timeout, seed)
  if(name == "mover") 
    return run_mover(name, input, forbidden, timeout, seed)
  else
    return run_ffree(name, input, forbidden, timeout, seed)
  end
end


def run_ffree(name, input, forbidden, timeout, seed)
  cmd = "/usr/bin/time -f '#{$JSON_TIME}' --output '#{$TMP_FILE}.time' timeout  #{timeout}s #{$BIN_PATH}/ffree_#{name} --input '#{input}' --forbidden '#{forbidden}' --seed #{seed} 2>#{$TMP_FILE}.log"
  start = Time.now
  ret = `#{cmd}`
  finish = Time.now
  k = get_k(ret) # calculated distance by prog
  run_time = finish - start
  
  run = {
    'k' => k,
    'run_time' => run_time,
    'time_log' => parse_time($TMP_FILE+'.time'),
    'log_output' => File.read($TMP_FILE+'.log'),
    'debug' => get_debug(ret),
    'command' =>  "timeout #{timeout}s #{$BIN_PATH}/ffree_#{name} --input '#{input}' --forbidden '#{forbidden}' --seed #{seed}",
    'result' => ret.split("\n").select{ |line| ! line.start_with?("#")}.map{ |line| line.split(" ")},
    'ret' => ret
  }
  FileUtils.rm($TMP_FILE+'.time') # cleanup temp files
  FileUtils.rm($TMP_FILE+'.log')
  return run    
end

def run_mover(name, input, forbidden, timeout, seed)
  cmd = "/usr/bin/time -f '#{$JSON_TIME}' --output '#{$TMP_FILE}.time' timeout  #{timeout}s python3 other/mover/mover.py '#{input}'  2>#{$TMP_FILE}.log"
  start = Time.now
  ret = `#{cmd}`
  finish = Time.now
  k = get_k(ret) # calculated distance by prog
  run_time = finish - start
  run = {
    'k' => k,
    'run_time' => run_time,
    'time_log' => parse_time($TMP_FILE+'.time'),
    'log_output' => File.read($TMP_FILE+'.log'),
    'debug' => [],
    'command' =>  "timeout #{timeout}s python3 other/mover/mover.py '#{input}'",
    'result' => ret.split("\n").select{ |line| ! line.start_with?("#")}.map{ |line| line.split(" ")},
    'ret' => ret
  }
  FileUtils.rm($TMP_FILE+'.time') # cleanup temp files
  FileUtils.rm($TMP_FILE+'.log')
  return run    
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

def get_k(output)
  return -1 if output.chomp == ""
  return output.split("\n").select{ |line| ! line.start_with?("#")}.size
end

def get_debug(output)
  return {} if output.chomp == ""
  debug_outstring = output.split("\n").select{ |line| line.start_with?("#debug:")}
  if(debug_outstring.size() > 0)
    return JSON.parse(debug_outstring[0].slice(8, debug_outstring[0].size))
  end
  return {}      
end

def get_metrics(k, kcorrect)
  solved = k != -1
  if(kcorrect == -1) # we have no correct result
    return {
      "absolut" => k,
      "no_correct" => true,
      "solved" => solved,
      "k_correct" => kcorrect
    }
  else
    if(kcorrect != 0)
      qual = kcorrect.to_f / k.to_f
      qual_inv = k.to_f / kcorrect.to_f
    else # it has to be k == 0, because they should solve right
      qual = 1
      qual_inv = 1
    end
    
    return {
      "absolut" => k,
      "no_correct" => false,
      "quality" => qual,
      "quality_inv" => qual_inv,
      "distance" => kcorrect - k,
      "solved" => solved,
      "k_correct" => kcorrect
    
    }
  end
  
end