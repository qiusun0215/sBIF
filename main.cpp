#include <iostream>
#include <time.h>
#include "optimization.h"
#include "dumping.h"
#include <getopt.h>
#include "parsingargs.h"
#include <string.h>
#include <sstream>
 
 
using namespace std;
 
 
int main(int argc, char * argv[])
{
 
    string tmpPara = "";
    for(int i=1;i <argc; i++)
    {
        //cout << i << "=" << argv[i] <<"---"<< endl;
        if(strlen(argv[i]) == 0) 
        {
            cout << "find NULL" << endl;
            tmpPara += char(31);
        }
        else
        {
            tmpPara += argv[i];
        }
        tmpPara += " ";
    }
    std::map<std::string, std::vector<std::string> > result;
    ParsingArgs pa;
    pa.AddArgType('i',"inter_file", ParsingArgs::MUST_VALUE);
    pa.AddArgType('c',"chrom", ParsingArgs::MUST_VALUE);
    pa.AddArgType('f',"chrmfile", ParsingArgs::MUST_VALUE);
    pa.AddArgType('s',"start", ParsingArgs::MUST_VALUE);
    pa.AddArgType('e',"end", ParsingArgs::MUST_VALUE);
    pa.AddArgType('o',"out_folder", ParsingArgs::MUST_VALUE);
    pa.AddArgType('r',"resolution", ParsingArgs::MAYBE_VALUE);
    pa.AddArgType('d',"fiber_density", ParsingArgs::MAYBE_VALUE);
    pa.AddArgType('n',"n_samples", ParsingArgs::MAYBE_VALUE);
    pa.AddArgType('l',"n_runs", ParsingArgs::MAYBE_VALUE);
    pa.AddArgType('m',"n_sphere", ParsingArgs::MAYBE_VALUE);
    pa.AddArgType('k',"ki_dist", ParsingArgs::MAYBE_VALUE);
    pa.AddArgType('t',"max_trials", ParsingArgs::MAYBE_VALUE);
    pa.AddArgType('g',"n_iter", ParsingArgs::MAYBE_VALUE);
    pa.AddArgType('j',"job_prefix", ParsingArgs::MAYBE_VALUE);
    pa.AddArgType('p',"threads", ParsingArgs::MAYBE_VALUE);
    

    result.clear();
    //cout << "Input is:" << tmpPara << endl;
    std::string errPos;
    int iRet = pa.Parse(tmpPara,result, errPos);
    if(0>iRet)
    {
        cout << "Error" << iRet << errPos << endl;
    }
    else
    {
        string inter_file;
        string chrom;
        string chrmfile;
        unsigned start;
        unsigned end;
        string out_folder;
        unsigned resolution = 2000;
        double fiber_density = 0.2368;
        unsigned n_samples = 50000;
        unsigned n_samples_per_run = 100;
        unsigned n_sphere = 50;
        unsigned ki_dist = 80;
        unsigned max_trials= 100;
        unsigned n_iter=100;
        unsigned threads=10;
        string job_prefix="test";
        
        
        map<std::string, std::vector<std::string> >::iterator it = result.begin();
        for(; it != result.end(); ++it)
        {
            if (it->first.compare("i")==0)
                inter_file=it->second[0];
            if (it->first.compare("c")==0)
                chrom=it->second[0];
            if (it->first.compare("f")==0)
                chrmfile=it->second[0];
            if (it->first.compare("s")==0)
            {
                std::stringstream item;
                item<<it->second[0];
                item>>start;
            }
            if (it->first.compare("e")==0)
            {
                std::stringstream item;
                item<<it->second[0];
                item>>end;
            }
            if (it->first.compare("o")==0)
                out_folder=it->second[0];
            if (it->first.compare("r")==0)
            {
                std::stringstream item;
                item<<it->second[0];
                item>>resolution;
            }
            if (it->first.compare("d")==0)
            {
                std::stringstream item;
                item<<it->second[0];
                item>>fiber_density;
            }
            if (it->first.compare("n")==0)
            {
                std::stringstream item;
                item<<it->second[0];
                item>>n_samples;
            }
            if (it->first.compare("l")==0)
            {
                std::stringstream item;
                item<<it->second[0];
                item>>n_samples_per_run;
            }
            if (it->first.compare("m")==0)
            {
                std::stringstream item;
                item<<it->second[0];
                item>>n_sphere;
            }
            if (it->first.compare("k")==0)
            {
                std::stringstream item;
                item<<it->second[0];
                item>>ki_dist;
            }
            if (it->first.compare("t")==0)
            {
                std::stringstream item;
                item<<it->second[0];
                item>>max_trials;
            }
            if (it->first.compare("g")==0)
            {
                std::stringstream item;
                item<<it->second[0];
                item>>n_iter;
            }
            if (it->first.compare("p")==0)
            {
                std::stringstream item;
                item<<it->second[0];
                item>>threads;
            }
            if (it->first.compare("j")==0)
                job_prefix=it->second[0];
        }
        double diam=getDiam(resolution,fiber_density);
        unsigned n_runs=n_samples/n_samples_per_run;
        string command;
        command = "mkdir -p " + out_folder;  
        system(command.c_str());
        cout<< "Parameters: "<<endl;
        cout<< "interaction file :"<< inter_file << endl;
        cout<< "chromosome :"<< chrom << endl;
        cout<< "chrom_sizes_file :"<< chrmfile << endl;
        cout<< "start position:"<< start << endl;
        cout<< "end position :"<< end << endl;
        cout<< "out_folder :"<< out_folder << endl;
        cout<< "resolution :"<< resolution << endl;
        cout<< "fiber density :"<< fiber_density << endl;
        cout<< "number of samples :"<< n_samples << endl;
        cout<< "number of samples per run :"<< n_samples_per_run << endl;
        cout<< "number of sphere points :"<< n_sphere << endl;
        cout<< "knockin distance :"<< ki_dist << endl;
        cout<< "maximum trials :"<< max_trials << endl;
        cout<< "number of iteractions :"<< n_iter << endl;
        cout<< "job prefix :"<< job_prefix << endl;
        cout<< "number of threads :"<< threads << endl;
        cout<<"Bead diameter: "<<diam<<endl;
        unsigned region_size=end-start;
        unsigned n_nodes= (region_size % resolution == 0) ? (region_size / resolution) : (region_size / resolution + 1);
        vectord2d weights(n_nodes,vectord(n_nodes));
        const char * inter_file_char=inter_file.c_str();
        const char * chrmfile_char=chrmfile.c_str();
        const char * chrom_char = chrom.c_str();
        const char * out_folder_char =out_folder.c_str();
        const char * job_prefix_char = job_prefix.c_str();
        vectord2d inter = readInterFourCols(inter_file_char,weights,chrom_char,chrmfile_char,start,end,resolution);
        getInterNum(inter, n_samples_per_run, false, 1);
        
    
        clock_t begin,finish;
    
        double totaltime;
        begin=clock();
        #pragma omp parallel for num_threads(threads)

        for (int i=0; i<n_runs; ++i)
        {
            my_ensemble chains = SBIF(inter, weights, n_samples_per_run, n_sphere, diam, diam, ki_dist, max_trials, n_iter);
            for (unsigned j=0; j!=n_samples_per_run; j++)
                dumpSingleChain(chains[j], out_folder_char, i*n_samples_per_run+j, job_prefix_char);
        }
        finish=clock();
        totaltime=(double)(finish-begin)/CLOCKS_PER_SEC;
        cout<<"Total cost "<<totaltime<<" seconds!"<<endl;
         
    }
    return 0;
}
