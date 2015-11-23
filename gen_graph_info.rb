require 'date'
require 'json'
require 'fileutils'
require 'optparse'

def main()
  
  options = {}
  OptionParser.new do |opts|
    opts.banner = "Usage: gen_graph_info.rb [options]"
    opts.on("-i", "--instances CONFIG", "Which folder to generate") do |v|
      options[:instances] = v
    end
  end.parse!
  
  out = {}
  
  entries = Dir.entries(options[:instances]+"/").each do |graph|
    next if graph == "k.txt" or graph == "k-cvd.txt" or graph.end_with? ".zip" 
    next if graph.start_with? "."
    next if not (graph.end_with? ".txt" or graph.end_with? ".graph")
    
    ret = `python3 tools/graph_info.py < "#{options[:instances]}/#{graph}"`
    puts "#{options[:instances]}/#{graph}"
    puts ret
    out[graph] = JSON.parse(ret)
  end
  File.write(options[:instances]+"/graph_info.json",  JSON.pretty_generate(out))
  
end

main()

