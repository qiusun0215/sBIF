#include <iostream>
#include <time.h>
#include "MC.h"
#include "dumping.h"

int main()
{
    //const char * inter_file = "data/Nonsynchro_pvalue_nonzero_FDR_0.01_freq_enrich.txt";
    const char * inter_file = "../data/kc167_all_freq.txt";
    //const char * inter_file = "../data/Nonsynchro_res_5k_freq_enrich.txt";
    const char *chrom = "chr3L";
    const char *chrmfile = "../data/dm3_chrm_sizes";
    const unsigned start = 0;
    const unsigned end = 600000;
    const unsigned resolution = 2000;
    const double fiber_density = 0.2368;
    unsigned n_samples = 100;
    unsigned n_sphere = 50;
    double diam=getDiam(resolution,fiber_density);
    double ki_dist = 70;
    unsigned max_trials = 100;
    unsigned n_iter= 100;

    const char *out_folder = "../output_spec_naive/kc167/chr3L_5600_5900/txt";
    const char *job_prefix = "kc167";
    //const char *time_file = "../time/time_100_spec_naive.txt";

    unsigned region_size=end-start;
    unsigned n_nodes= (region_size % resolution == 0) ? (region_size / resolution) : (region_size / resolution + 1);
    vectord2d weights(n_nodes,vectord(n_nodes));
    vectord2d inter = readInterFourCols(inter_file,weights,chrom,chrmfile,start,end,resolution);
    getInterNum(inter, n_samples, false, 1);
    
    clock_t begin,finish;
    
    double totaltime;
    begin=clock();
    #pragma omp parallel for num_threads(30)

    for (int i=0; i<500; ++i)
    {
        my_ensemble chains = MC(inter, weights, n_samples, n_sphere, diam, diam, ki_dist, max_trials, n_iter);
        for (unsigned j=0; j!=n_samples; j++)
           dumpSingleChain(chains[j], out_folder, i*n_samples+j, job_prefix);
    }
    finish=clock();
    totaltime=(double)(finish-begin)/CLOCKS_PER_SEC;
    cout<<"Total cost "<<totaltime<<" seconds!"<<endl;
 
    //FILE *output = fopen(time_file,"w");
    //if (output==NULL)
    //{
    //   fprintf(stderr, "The output file can not be opened!\n");
    //    exit(1);
    //}
    //fprintf(output,"%f\n", totaltime);
    //fclose(output);
    return 0;
}
