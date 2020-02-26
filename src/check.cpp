#include "check.h"

bool checkSphere(Node& node1, const double radius)
{
    Node origin = { 0.0,0.0,0.0 };
    double dist = calcuDist(node1, origin);
    return (dist <= radius);
}


bool checkClash(my_chain& chain, Node& node1, unsigned idx, vectori& sim_ligc,
    const double clash_dist, const double ki_dist, unsigned head, unsigned tail)
{

    for (unsigned i = head; i < tail; ++i)

    {
        if (i == idx) continue;
        if (i == idx - 1) continue;
        if (i == idx + 1) continue;
        double dist = calcuDist(node1, chain[i]);
        if (dist < clash_dist)
        {
            return false;
        }
        sim_ligc[i] = (dist <= ki_dist) ? 1 : 0;
    }
    return true;
}