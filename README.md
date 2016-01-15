# Approximation of H-Free Editing

# Structure

 + `config/` contains a cofigurations for benchmarks
 + `data_benchmarks/` contains results of benchmarks
 + `forbidden/` contains a defintions of forbidden subgraphs
 + `model/` contains graph datasets
 + `tools/` some tools for graphs generations or tests

# Compile
```sh
mkdir build
cd build
cmake ..
make
```
# Run benchmark

## Create a benchmark config file
Create a json file in `config/`.

```json
{
  "instances" : ["list of graph data sets"],
  "forbidden" : [ "list of forbidden subgraphs"],
  "progs" : ["list of programms to test"],
  "seed" : "initial seed",
  "max_time" : "maximal runtime in seconds"
}
```

For example
```json
{
  "instances" : ["./model/albert_barabasi", "./model/newman_watts_strogatz",],
  "forbidden" : [ "./forbidden/claw", "./forbidden/cluster"],
  "progs" : ["grow_reduce", "grow_reduce2", "bottom --useWeight 0"],
  "seed" : 5489,
  "max_time" : 5
}
```

The binary file should be in `build/` and begin with `ffree_`. So if in the config we have `"grow_reduce"` then `ffree_grow_reduce` will be called.

## Run Benchmark
`ruby ruby benchmark.rb -c config/your_config_file.json`

Every programm will be tested against every instance with all the possible forbidden subgraphs.

## View Results 
The results are in `data_benchmarks/`.
Every result single (all programs tested against one instance and one forbidden subgraph) will habe its own folder by date.

There will be some files. The most interesing will be results.json