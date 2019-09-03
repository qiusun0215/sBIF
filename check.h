#ifndef SBIF_CHECK_H
#define SBIF_CHECK_H

#include <cmath>
#include <vector>
#include <cassert>
using namespace std;
typedef struct {
    double x ;
    double y ;
    double z ;
} Node;

typedef vector<Node> my_chain;
typedef vector<my_chain> my_ensemble;
typedef vector<double> vectord;
typedef vector<unsigned> vectori;
typedef vector<vectord> vectord2d;
typedef vector<vectori> vectori2d;
typedef vector<vectori2d> vectori3d;
typedef vector<pair<unsigned,unsigned>> indices;
#define PI 3.14159265358979

inline double calcuDist( const Node &node1, const Node &node2)
{
    return sqrt((node1.x - node2.x)*(node1.x - node2.x) + (node1.y - node2.y)*(node1.y - node2.y) + (node1.z - node2.z)*(node1.z - node2.z));
}

inline double getDiam( const unsigned resolution, const double fiber_density)
{
    return pow(resolution/fiber_density*6/PI,(1.0/3));
}

inline double poisson_pmf(const double k, const double lambda)
{
    return exp(k * log(lambda) - lgamma(k + 1.0) - lambda);
}

bool checkSphere(Node &node1, const double radius);

bool checkClash(my_chain &chain, Node &node1, unsigned idx, vectori &sim_ligc,
              const double clash_dist, const double ki_dist, unsigned head, unsigned tail);

#endif //SBIF_CHECK_H
