require 'fileutils'
require 'optparse'
def main()
  options = {}
  OptionParser.new do |opts|
    opts.banner = "Usage: normalize.rb [options]"

    opts.on("-p", "--path PATH", "Path to the graphs") do |p|
      options[:path] = p
    end
  end.parse!
  
  Dir.entries(options[:path]).each do |graph|
    next if graph == "k.txt" or graph == "k-cvd.txt" or graph.end_with? ".zip" 
    next if graph.start_with? "."
    next if not (graph.end_with? ".txt" or graph.end_with? ".graph")
    
    nodes = []
    out = ""
    File.readlines(options[:path]+"/"+graph).each do |line|
      next if line.start_with? "#"
      sp = line.split("\s")
      next if sp.length != 2
      nodes << sp[0] if(!nodes.include? sp[0])
      nodes << sp[1] if(!nodes.include? sp[1])
      out += nodes.index(sp[0]).to_s + " " + nodes.index(sp[1]).to_s + "\n"
    end
    File.write(options[:path]+"/"+graph, out)
  end
    
end

main()