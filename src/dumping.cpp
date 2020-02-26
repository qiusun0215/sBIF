#include "dumping.h"

char* getOutfile(const char* out_folder, unsigned rep_id, const char* job_prefix)
{
    const char* seperator = "/";
    char rep[10];
    sprintf(rep, "%d", rep_id);
    char* out_file = (char*)malloc(sizeof(char) * MAX_CHAR);
    strcpy(out_file, out_folder);
    if (out_folder[strlen(out_folder) - 1] != *seperator)
    {
        strcat(out_file, seperator);
    }
    strcat(out_file, job_prefix);
    strcat(out_file, ".");
    strcat(out_file, rep);
    strcat(out_file, ".txt");
    return out_file;

}

void dumpSingleChain(my_chain& chain, const char* out_folder, unsigned rep_id, const char* job_prefix)
{
    char* out_file = getOutfile(out_folder, rep_id, job_prefix);
    FILE* output = fopen(out_file, "w");
    if (output == NULL)
    {
        fprintf(stderr, "The output file can not be opened!\n");
        exit(1);
    }
    for (Node node : chain)
    {
        fprintf(output, "%f\t%f\t%f\n", node.x, node.y, node.z);
    }
    fclose(output);
    //cout<<"Writing sample "<<rep_id<<" ..."<<endl;
}

void dumpEnsemble(my_ensemble& chains, const char* out_folder, const char* job_prefix)
{
    auto n_samples = chains.size();
    for (unsigned i = 0; i != n_samples; ++i)
    {
        char* out_file = getOutfile(out_folder, i, job_prefix);
        FILE* output = fopen(out_file, "w");
        if (output == NULL)
        {
            fprintf(stderr, "The output file can not be opened!\n");
            exit(1);
        }
        for (Node node : chains[i])
        {
            fprintf(output, "%f\t%f\t%f\n", node.x, node.y, node.z);
        }
        fclose(output);
        //cout<<"Writing sample "<<i<<" ..."<<endl;
    }
}