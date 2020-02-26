#ifndef SBIF_OPTIMIZATION_H
#define SBIF_OPTIMIZATION_H

#include <random>
#include <iostream>
#include "loading.h"



indices getNextNodesEvenEnsemb(my_ensemble& chains, my_ensemble& candi, vectori3d& sim_ligcs,
    const double diam, unsigned n_sphere, unsigned node_idx,
    unsigned pre_node_idx, const double clash_dist, const double ki_dist,
    unsigned head, unsigned tail);

vectori calcuInitiFreqs(vectori3d& sim_ligcs, indices& total_valid_idx, indices& select_idx,
    unsigned head, unsigned tail, unsigned n_samples, unsigned n_nodes);

double calcuProb(vectori& select_ligc, unsigned node_idx, unsigned head, unsigned tail,
    vectord2d& inter, vectord2d& weights);

double calcuNewProb(vectori& select_ligc, vectori3d& sim_ligcs, indices& total_valid_idx,
    indices& select_idx, unsigned node_idx, unsigned head, unsigned tail,
    unsigned old_select_idx, unsigned new_valid_idx, vectord2d& inter,
    vectord2d& weights);

bool acceptance(double& old_p, double& new_p);

my_ensemble SBIF(vectord2d& inter, vectord2d& weights, unsigned n_samples, unsigned n_sphere,
    const double diam, const double clash_dist, const double ki_dist, unsigned max_trials,
    unsigned n_iter);

#endif //SBIF_OPTIMIZATION_H

