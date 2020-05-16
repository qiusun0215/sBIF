# sequential Bayesian Inference Framework (sBIF)

A method to reconstruct 3D chromatin structures of modeled single cells from Hi-C data.

## Dependencies: 


This script has been tested on Linux system with CMake (version>=2.8.12) installed. All C++ standard template libraries are supposed to be installed already.


## Compile: 

```Bash
     cmake .
     make
```

## Usage: 

```Bash
     sBIF [options] -i <inter_file> -c <chrm> -l <chrmLens> -s <start> -e <end> -o <out_folder>
```

sBIF takes the Hi-C interaction file as input, it can be used to construct a chromatin polymer ensemble that best describe the Hi-C data. In each output file of Tab delimited format, there are three columns that representing the x, y and z coordinates of nodes in the simulated chromatin polymer chain.


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
        
        
## Example:

```Bash
     cd example
     ./drosChr2L.sh
```
This example code will generate 500,000 chromatin structures in the region of chr2L 11,000,000-12,000,000 for Drosophila cell line S2R+. The output files are in tab-delimited plain format with three columns, representing x, y and z coordinates of each chain.
