# sBIF
sequential Bayesian Inference framework
A method to construct 3D chromatin structures of single cells from Hi-C data.

## Compile: 
g++ --std=c++11 -fopenmp check.cpp loading.cpp dumping.cpp optimization.cpp parsingargs.cpp main.cpp -o sbif

## Usage: 
sbif -i inter_file -o out_folder -c chrom -f chrom_sizes -s start -e end -k ki_dist -n num_samples

## Parameters: shortterm | longterm: description  
    i | inter_file:  Hi-C file with 4 columes, chrm|start|end|weight 
    c | chrom: chromosome
    f | chrmfile: file of chromosome sizes
    s | start: start position
    e | end: end position
    o | out_folder: output folder
    r | resolution: resolution
    d | fiber_density: fiber density for chromatin 11nm fiber 
    n | n_samples: number of samples
    l | n_samples_per_run: number of samples generated for each single run
    m | n_sphere: number of sphere points chosen as the candidates for the next node
    k | ki_dist: knock in distance
    t | max_trials: maximum number of trials
    g | n_iter: number of iteractions
    j | job_prefix: job prefix 
    p | threads: number of threads 
