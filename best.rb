require 'date'
require 'json'
require 'fileutils'
require 'optparse'
require_relative 'utils'
$BIN_PATH = "./build"

def main()
 # tools = ["extend", "grow_reduce3", "grow_reduce_boost", "random"];
  tools = ["grow_reduce_boost","extend"];
  forbiddenList = ["2k2_c4_c5","2p3","c4","splitcluster","cograph"]

  options = {}
  OptionParser.new do |opts|
    opts.banner = "Usage: best.rb [options]"

    opts.on("-i", "--input INPUT", "Which graph do you want to test?") do |v|
      options[:input] = v
    end
  end.parse!
  abort("no input") if options[:input].nil?
  
  
  results = {}
  
  forbiddenList.each do |forbidden|
    next if forbidden.start_with? "."
    Dir.entries(options[:input]).each do |input|
      next if input.start_with? "."
      next if not (input.end_with? ".txt" or input.end_with? ".graph")
      
      
      min = -1
      n = nodeCount(options[:input]+input)
      n2 = n*n
      
      tools.each do |prog|
        k = run(options[:input]+input, "forbidden/"+forbidden, prog, 3).to_f 
        if(k >= 0) 
          if(min == -1)
            min = k
          elsif(min > k)
            min = k
          end
        end
      end
      
      if(min != -1)
        normed = (min.to_f/n2.to_f)
        if(results.has_key? forbidden)
          results[forbidden] << normed
        else
          results[forbidden] = [normed]
        end
      else
        normed = 1
      end
      
     
      puts "Testing #{forbidden} on #{input} (#{normed})"
      
    end
  end
  puts "\n\nSorted by Size"
  results.sort_by{|k,v| v.mean}.to_h.each do |graph,value|
    puts "#{graph} #{value.mean.round(1)} #{value.standard_deviation.round(1)}"  
  end
  
  puts "\n\nSorted by Name"
  results.sort_by{|k,v| k}.to_h.each do |graph,value|
    puts "#{graph} #{value.mean.round(1)} #{value.standard_deviation.round(1)}"  
  end
  
  puts "\n\npython"
  print "mean = ("
  results.sort_by{|k,v| k}.to_h.each do |graph,value|
    print value.mean.round(4)
    print ", "
  end
  print ")\n"
  print "std = ("
  results.sort_by{|k,v| k}.to_h.each do |graph,value|
    print value.standard_deviation.round(4)
    print ", "
  end
  print ")\n"
end 

def nodeCount(fileName)
  File.read(fileName).split("\n").select{ |line| ! line.start_with?("#")}.map{ |line| line.split(" ")}.flatten.uniq.length
end

def run(input, forbidden, prog, timeout)
  ret = `timeout #{timeout*2}s #{$BIN_PATH}/ffree_#{prog} --input '#{input}' --forbidden '#{forbidden}' --timeout #{timeout} 2>/dev/null`
  return get_k(ret)
end

main();
