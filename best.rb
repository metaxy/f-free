require 'date'
require 'json'
require 'fileutils'
require 'optparse'
require_relative 'utils'
$BIN_PATH = "./build"

def main()
  tools = ["extend", "grow_reduce3", "bottom", "random"];

  options = {}
  OptionParser.new do |opts|
    opts.banner = "Usage: best.rb [options]"

    opts.on("-i", "--input INPUT", "Which graph do you want to test?") do |v|
      options[:input] = v
    end
  end.parse!
  abort("no input") if options[:input].nil?
  results = {}
  Dir.entries("forbidden/").each do |forbidden|
    next if forbidden.start_with? "."
    puts "Testing #{forbidden}"
    tools.each do |prog|
      k = run(options[:input], "forbidden/"+forbidden, prog, "3")
      if(k >= 0) 
        if(results.has_key? forbidden)
          if(results[forbidden] > k)
            results[forbidden] = k
          end
        else
          results[forbidden] = k
        end
      end  
      
    end
  end
  results.sort_by{|k,v| v}.to_h.each do |k,v|
    puts "#{v}   #{k}"  
  end
end 

def run(input, forbidden, prog, timeout)
  ret = `timeout #{timeout}s #{$BIN_PATH}/ffree_#{prog} --input '#{input}' --forbidden '#{forbidden}'`
  return get_k(ret)
end

main();