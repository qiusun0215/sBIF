# sequential Bayesian Inference Framework (sBIF)
A method to reconstruct 3D chromatin structures of modeled single cells from Hi-C data.

## Compile: 
```Bash
     cmake .
     make
```
## Usage: 
```Bash
     sBIF [options] -i <inter_file> -c <chrm> -l <chrmLens> -s <start> -e <end> -o <out_folder>
```
## Options: <shortterm | longterm> 

        -i  | --inter     REQUIRED, Hi-C interaction file in a Tab delimited format, 
                          contains 5 columns: chrm, startpos, endpos, freq and weight
        -c  | --chrom     REQUIRED, chromosome name, must be consistent with the names
                          in chromosome length file" << endl;
        -l  | --chrlens   REQUIRED, chromosome length file, can be downloaded via fetchChromSizes
                          contains 2 columns: chrm and length
        -s  | --start     REQUIRED, start position
        -e  | --end       REQUIRED, end position
        -o  | --out       REQUIRED, output folder
        -r  | --res       resolution, default: 2000
        -d  | --fibdens   fiber density, default: 0.2368
        -ns | --nsamp     total number of samples, default: 50000
        -nr | --nruns     number of samples per run, default: 100
        -ni | --niter     number of iterations, default: 100
        -n  | --nsphere   number of sphere points chosen as the candidates for the next node
                          default: 50
        -k  | --kidist    knock-in distance threshold (nm), default: 80
        -m  | --maxtrial  maximum number of trials, default: 100
        -j  | --jobpre    job prefix, default: test
        -p  | --threads   number of threads, default: 1
        -h  | --help      print the available options
