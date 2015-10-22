require 'date'
require 'json'

$BIN_PATH = "./build"
$INSTANCES = "./model/cluster_very_small"
$FORBIDDEN = "./forbidden/cluster"
$PROGS = ["random --rounds 1","random --rounds 10", "random2 --rounds 10", "random2 --rounds 10 --random2_subgraph_batch=100"]
$SEED = "5489"
$MAX_TIME=2
def run_prog(name, input, forbidden, timeout)
  ret = `timeout #{timeout}s  #{$BIN_PATH}/ffree_#{name} --input #{input} --forbidden #{forbidden} --seed #{$SEED}`
  return ret
end

def main()
  
  if(`git status -s` != "")
    puts "Please commit first"
    exit
  end
  
  `cd $BIN_PATH && make`
  fileName = DateTime.now.strftime("benchmarks/bench_%Y_%m_%d__%H_%M_%S.json")  
  output = {}
  entries = Dir.entries($INSTANCES+"/")
  
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
      start = Time.now
      ret = run_prog(prog, $INSTANCES+"/"+graph, $FORBIDDEN, $MAX_TIME)
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
        "quality" => qual
      }
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

