require 'date'
require 'optparse'


def main()
  
  options = {}
  OptionParser.new do |opts|
    opts.banner = "Usage: benchmark.rb [options]"

    opts.on("-n", "--nodes CONFIG", "Node Count") do |v|
      options[:nodes] = v.to_i
    end
    opts.on("-p", "--prob CONFIG", "Probility") do |v|
      options[:prob] = v.to_f
    end
  end.parse!
  
  
  options[:nodes].times do |x|
    options[:nodes].times do |y|
      puts "#{x} #{y}" if rand < options[:prob]
    end
  end
        
  
end


main()

