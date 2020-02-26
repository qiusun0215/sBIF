#ifndef SBIF_LOADING_H
#define SBIF_LOADING_H

#include <map>
#include <string>
#include "check.h"

#define MAX_CHAR 10000

map<const string, unsigned int> getChrmLens(const char* chrmfile);


vectord2d readInterFiveCols(const char* inter_file, vectord2d& weights, const char* chrom,
    const char* chrmfile, unsigned start, unsigned end, unsigned resolution);

void getInterNum(vectord2d& inter, unsigned n_samples, bool scaling = false, const unsigned scale_diagonal = 1);

#endif //SBIF_LOADING_H once
