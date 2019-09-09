#ifndef SBIF_DUMPING_H
#define SBIF_DUMPING_H

#include <cstdio>
#include <cstring>
#include <string>
#include <iostream>
#include "loading.h"
#include "check.h"

char *getOutfile(const char *out_folder, unsigned rep_id, const char *job_prefix);

void dumpSingleChain(my_chain &chain, const char *out_folder, unsigned rep_id, const char *job_prefix);

void dumpEnsemble(my_ensemble &chains, const char *out_folder, const char *job_prefix);


#endif //SBIF_DUMPING_H
