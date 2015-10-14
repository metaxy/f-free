require 'date'

$BIN_PATH = "./build"
$INSTANCES = "./model/cluster_very_small"
$FORBIDDEN = "./forbidden/cluster"
$PROGS = ["random --rounds 10", "random2 --rounds 10"]
$f = nil
$SEED = "5489"
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
  fileName = DateTime.now.strftime("benchmarks/bench_%Y_%m_%d__%H_%M_%S.txt")  
  
  $f = open(fileName, 'a');
  entries = Dir.entries($INSTANCES+"/")
  
  putf "#INSTANCES: #{$INSTANCES}"
  putf "#FORBIDDEN: #{$FORBIDDEN}"
  putf "#Start Time: #{Time.now.to_s}"
  putf "#Git Hash: #{`git rev-parse --verify HEAD`}"
  putf "#Git Commit Message: #{`git log -1 --pretty=%B`}"
  putf "#Format: Progname, graph file, distance, correct distance, time, quality"
  
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
      ret = run_prog(prog, $INSTANCES+"/"+graph, $FORBIDDEN, 10)
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
        time[prog] = 0
        failed[prog] = 0
      end
      quality[prog] += qual
      count[prog] += 1
      failed[prog] += (k == -1)
      time[prog] += diff
      putf "#{prog};#{graph};#{k},#{kcorrect};#{diff}s;#{qual}"
    end
  end
  $PROGS.each do |prog|
    putf "#Quality of #{prog}: #{(quality[prog]/count[prog].to_f)*100}% failed: #{failed[porg]}"
    putf "#Mean Time of #{prog}: #{(time[prog]/count[prog].to_f)*100}%"
  end
  putf "#End Time: #{Time.now.to_s}"
end

def putf(string)
  puts string
  $f.puts string
end

main()

