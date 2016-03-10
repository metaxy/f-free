require 'date'
require 'json'
require 'fileutils'
require 'optparse'

$BIN_PATH = "./build"

def create_command(name, input, forbidden, timeout)
  return "timeout  #{timeout}s #{$BIN_PATH}/ffree_#{name} --input '#{input}' --forbidden '#{forbidden}' 2>/dev/null"
end

def generate(instances, forbidden, prog, time)
  options[:instances] = instances
  options[:forbidden] = forbidden
  options[:prog] = prog
  options[:time] = time

  entries = Dir.entries(options[:instances])
  entries_size = entries.size
  current_file = 0
  
  output = {}
  notsolved = []
  if(File.exists?("#{options[:instances]}/#{File.basename(options[:forbidden])}.k.json"))
    sols = JSON.parse(File.read("#{options[:instances]}/#{File.basename(options[:forbidden])}.k.json"))
  else
    sols = {}
  end
  
  entries.each do |graph|
    current_file += 1
    next if graph.start_with? "."
    next if not (graph.end_with? ".txt" or graph.end_with? ".graph")
    
    puts "# File #{current_file} of #{entries_size}"
    if(not sols[graph].nil?) 
      output[graph] = sols[graph]
    else
      output[graph] = {}
    end
    
    if(sols[graph].nil? || sols[graph]['min_k'] == '-1')
      command = create_command(options[:prog], options[:instances]+'/'+graph, options[:forbidden], options[:time])
      ret = `#{command}`
      puts ret
      if(ret.chomp == "")
        k = -1
        notsolved << graph
      else
        k = ret.split("\n").select{ |line| ! line.start_with?("#")}.size
      end
      output[graph]['min_k'] = k
      if(k != -1)
        output[graph]['solution'] = ret.split("\n").select{ |line| ! line.start_with?("#")}.map{ |line| line.split(" ")}
      end
    end
    
  end
  puts "Notsolved:"
  p notsolved
  File.write(options[:instances]+"/"+File.basename(options[:forbidden])+".k.json",  JSON.pretty_generate(output))
 
end

def main()
  
  options = {}
  options[:time] = 60*5 # 5 min
  options[:prog] = "blp"
  
  options[:instances] = nil
  options[:forbidden] = nil

  OptionParser.new do |opts|
    opts.banner = "Usage: benchmark.rb [options]"

    opts.on("-i", "--instances PATH", "Which folder to generate") do |v|
      options[:instances] = v
    end
    opts.on("-f", "--forbidden PATH", "Which structures to forbid") do |v|
      options[:forbidden] = v
    end
    opts.on("-p", "--prog NAME", "Programm to run") do |v|
      options[:prog] = v
    end
    opts.on("-t", "--t TIM", "Max run time in seconds") do |v|
      options[:time] = v
    end
  end.parse!
  
  all_instances = Dir.entries("./model/")
  all_forbidden = Dir.entries("./model/")

  if(options[:instances] == nil)
      all_instances.each do |instance|
        if(options[:forbidden] == nil) 
          all_forbidden.each do |forbidden|
            generate(instance, forbidden, options[:prog], options[:time])
          end
        else
            generate(instance, options[:forbidden], options[:prog], options[:time])
        end
      end
  else
    if(options[:forbidden] == nil) 
      all_forbidden.each do |forbidden|
        generate(options[:instances], forbidden, options[:prog], options[:time])
      end
    else
      generate(options[:instances], options[:forbidden], options[:prog], options[:time])
    end
  end 
  
end


main()

