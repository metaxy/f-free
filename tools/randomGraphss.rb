(0..1).step(0.2).each do |p|
  `mkdir #{p}`
  (10..100).step(10).each do |n|
    `ruby randomGraph.rb -n #{n} -p #{p} > #{p}/n_#{n}.txt`
  end
end