# sBIF
sequential Bayesian Inference framework: 
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

        -i  | --inter     REQUIRED, Hi-C interaction file in a Tab delimited format, " << endl;
                          contains 5 columns: chrm, startpos, endpos, freq and weight" << endl;
        -c  | --chrom     REQUIRED, chromosome name, must be consistent with the names" <<endl;
                          in chromosome length file" << endl;
        -l  | --chrlens   REQUIRED, chromosome length file, can be downloaded via fetchChromSizes" << endl;
                          contains 2 columns: chrm and length" << endl;
        -s  | --start     REQUIRED, start position" << endl;
        -e  | --end       REQUIRED, end position" << endl;
        -o  | --out       REQUIRED, output folder" << endl;
        -r  | --res       resolution, default: 2000" << endl;
        -d  | --fibdens   fiber density, default: 0.2368" << endl;
        -ns | --nsamp     total number of samples, default: 50000" << endl;
        -nr | --nruns     number of samples per run, default: 100" << endl;
        -ni | --niter     number of iterations, default: 100" << endl;
        -n  | --nsphere   number of sphere points chosen as the candidates for the next node" << endl;
                          default: 50" << endl;
        -k  | --kidist    knock-in distance threshold (nm), default: 80" << endl;
        -m  | --maxtrial  maximum number of trials, default: 100" << endl;
        -j  | --jobpre    job prefix, default: test" << endl;
        -p  | --threads   number of threads, default: 1" << endl;
        -h  | --help      print the available options" << endl;
