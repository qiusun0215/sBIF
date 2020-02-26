#include "help.h"

//print help file...
void printHelp()
{

    cout << "*****sequential Bayesian Inference Framework (sBIF)*****" << endl;
    
    cout << "Usage : sBIF [options] -i <inter_file> -c <chrm> -l <chrmLens> -s <start> -e <end> -o <out_folder>" << endl;
    cout << endl;
    cout << endl;
    cout << "Options: <short | long>" << endl;

    cout << "     i  | inter     REQUIRED, Hi-C interaction file in a Tab delimited format, " << endl;
    cout << "                    contains 5 columns: chrm, startpos, endpos, freq and weight" << endl;
    cout << "     c  | chrom     REQUIRED, chromosome name, must be consistent with the names" <<endl;
    cout << "                    in chromosome length file" << endl;
    cout << "     l  | chrlens   REQUIRED, chromosome length file, can be downloaded via fetchChromSizes" << endl;
    cout << "                    contains 2 columns: chrm and length" << endl;
    cout << "     s  | start     REQUIRED, start position" << endl;
    cout << "     e  | end       REQUIRED, end position" << endl;
    cout << "     o  | out       REQUIRED, output folder" << endl;
    cout << "     r  | res       resolution, default: 2000" << endl;
    cout << "     d  | fibdens   fiber density, default: 0.2368" << endl;
    cout << "     ns | nsamp     total number of samples, default: 50000" << endl;
    cout << "     nr | nruns     number of samples per run, default: 100" << endl;
    cout << "     ni | niter     number of iterations, default: 100" << endl;
    cout << "     n  | nsphere   number of sphere points chosen as the candidates for the next node" << endl;
    cout << "                    default: 50" << endl;
    cout << "     k  | kidist    knock-in distance threshold (nm), default: 80" << endl;
    cout << "     m  | maxtrial  maximum number of trials, default: 100" << endl;
    cout << "     j  | jobpre    job prefix, default: test" << endl;
    cout << "     p  | threads   number of threads, default: 1" << endl;
    cout << "     h  | help      print the available options" << endl;
  

}
