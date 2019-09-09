#include <cstdlib>
#include <cstring>
#include <cstdio>
#include <map>
#include <ctime>
#include "loading.h"

map<const string, unsigned int> getChrmLens(const char* chrmfile)
{
    map<const string, unsigned int> all_chrm_lens;
    FILE *file = fopen(chrmfile, "r");
    if (file == NULL)
    {
        fprintf(stderr, "The inputfile can not be opened!\n");
        exit(1);
    }
    char line[MAX_CHAR];
    char chrm_name[MAX_CHAR / 2];
    unsigned int chrm_length;
    while (1)
    {
        if (fgets(line, MAX_CHAR, file) == NULL) break;
        int num = sscanf(line, "%s %u", chrm_name, &chrm_length);
        if (num != 2)
        {
            fprintf(stderr, "The chrmfile should have two columns, one represents names, the other represents sizes!\n");
            exit(1);
        }
        all_chrm_lens[chrm_name] = chrm_length;
    }
    fclose(file);
    return all_chrm_lens;
}


vectord2d readInterFourCols(const char * inter_file, vectord2d &weights,  const char *chrom,
                           const char *chrmfile, unsigned start, unsigned end,  unsigned resolution)
{
    map<const string, unsigned int> all_chrm_lens = getChrmLens(chrmfile);
    unsigned chrm_size = all_chrm_lens[chrom];
    if (chrm_size <= 0) throw runtime_error("The chromosome does not exist!");
    assert((start >= 0) && (start < chrm_size));
    assert((end > 0) && (end <= chrm_size));
    unsigned region_size = end - start;
    const unsigned bin_size = ((region_size % resolution == 0) ? (region_size / resolution) : (region_size / resolution + 1));
    const unsigned bin_start = start / resolution;
    vectord2d inter(bin_size,vectord(bin_size));
    FILE *file = fopen(inter_file, "r");
    if (file == NULL)
    {
        fprintf(stderr, "The interaction file can not be opened!\n");
        exit(1);
    }
    char line[MAX_CHAR];
    char chrm_name[50];
    unsigned int bin1;
    unsigned int bin2;
    double freq;
    double weight;
    while (1)
    {
        if (fgets(line, MAX_CHAR, file) == NULL) break;
        int num = sscanf(line, "%s %u %u %lf %lf",chrm_name, &bin1, &bin2, &freq, &weight);
        if (num != 5)
        {
            fprintf(stderr, "The interaction file should have four columns(chrm_name bin1 bin2 freq weight) !\n");
            exit(1);
        }
        if (strcmp(chrm_name, chrom) == 0)
        {
            bin1 = bin1 / resolution - bin_start;
            bin2 = bin2 / resolution - bin_start;
            if ((bin1<0)||(bin2<0)) continue;
            if ((bin1>=bin_size)||(bin2>=bin_size)) continue;
            if (weight<0) continue;
	        inter[bin1][bin2] = freq;
            inter[bin2][bin1] = freq;
            weights[bin1][bin2] = weight;
            weights[bin2][bin1] = weight;
        }
    }
    fclose(file);
    return inter;
}


void getInterNum(vectord2d &inter, unsigned n_samples, bool scaling, const unsigned scale_diagonal)
{   
    if (scaling)
    {
        auto locus_size = inter.size();
        double diag_value = 0;
        unsigned num = 0;
        for (unsigned i = 0; i != locus_size - scale_diagonal; ++i)
        {
            diag_value += inter[i][i + scale_diagonal];
            if (inter[i][i + scale_diagonal] != 0) num += 1;
        }
        diag_value /= num;
        for (unsigned i = 0; i != locus_size; ++i)
        {
            for (unsigned j = 0; j != locus_size; ++j)
            {
                if (i == j)
                {
                    inter[i][j] = -1;
                    continue;
                }
                if (((i - j) == scale_diagonal) || ((j - i) == scale_diagonal))
                {
                    inter[i][j] = -1;
                    continue;
                }
                inter[i][j] = ((inter[i][j] > diag_value) ? diag_value : inter[i][j]);
                inter[i][j] = round(inter[i][j] / diag_value * n_samples);
            }
        }
    }
    else
    {
        auto locus_size = inter.size();
        for (unsigned i=0; i!=locus_size; ++i)
            for (unsigned j=0; j!=locus_size; ++j)
            {
                if (i==j)
                {
                    inter[i][j]=-1;
                    continue;
                }
                if (fabs(i-j)==1)
                {
                    inter[i][j]=-1;
                    continue;
                }
                inter[i][j] = round(inter[i][j] * n_samples);
            }
    }
}

