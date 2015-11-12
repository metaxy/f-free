require 'date'
require 'json'
require 'fileutils'
require 'optparse'

$BIN_PATH = "./build"

def create_command(name, input, forbidden, timeout)
  return "timeout  #{timeout}s #{$BIN_PATH}/ffree_#{name} --input '#{input}' --forbidden '#{forbidden}' 2>/dev/null"
end


def main()
  
  options = {}
  options[:time] = 60*5
  OptionParser.new do |opts|
    opts.banner = "Usage: benchmark.rb [options]"

    opts.on("-i", "--instances CONFIG", "Which folder to generate") do |v|
      options[:instances] = v
    end
    opts.on("-f", "--forbidden CONFIG", "Which structures to forbid") do |v|
      options[:forbidden] = v
    end
    opts.on("-p", "--prog CONFIG", "Which structures to forbid") do |v|
      options[:prog] = v
    end
    opts.on("-t", "--t CONFIG", "Max time in seconds") do |v|
      options[:time] = t
    end
  end.parse!
  
  
  entries = Dir[options[:instances]+'/*'].sort_by{ |f| File.size(f) }
  entries_size = entries.size
  current_file = 0
  
  
  notsolved = []
  sols = JSON.parse(File.read("#{config["instances"]}/#{File.basename(config["forbidden"])}.k.json"))
  output = sols
  entries.each do |graph|
    current_file += 1
    next if graph.start_with? "."
    next if not (graph.end_with? ".txt" or graph.end_with? ".graph")
    next if sols[graph] != -1
    
    puts "# File #{current_file} of #{entries_size}"
    command = create_command(options[:prog], graph, options[:forbidden], options[:time])
    ret = `#{command}`
    puts ret
    if(ret.chomp == "")
      k = -1
      notsolved << graph
    else
      k = ret.split("\n").select{ |line| ! line.start_with?("#")}.size
    end
    output[graph] = k
    
  end
  puts "Notsolved:"
  p notsolved
  File.write(options[:instances]+"/"+File.basename(options[:forbidden])+".k.json",  JSON.pretty_generate(output))
  
end


main()

