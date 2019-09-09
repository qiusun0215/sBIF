#include "optimization.h"

//https://stackoverflow.com/questions/9600801/evenly-distributing-n-points-on-a-sphere

indices getNextNodesEvenEnsemb(my_ensemble &chains, my_ensemble &candi, vectori3d &sim_ligcs,
                            const double diam, unsigned n_sphere, unsigned node_idx,
                            unsigned pre_node_idx, const double clash_dist, const double ki_dist,
                            unsigned head,unsigned tail)
{
    auto n_samples=chains.size();
    double offset=2.0/n_sphere;
    double increment = PI * (3.0 - sqrt(5.0));
    double x,y,z,r,phi;
    indices total_valid_idx(0);

    for (unsigned k=0; k<n_samples; ++k)
        for (unsigned i=0; i<n_sphere; ++i)
        {
            y = ((i * offset) - 1) + (offset / 2);
            r = sqrt(1 - pow(y, 2));
            phi = (i % n_sphere) * increment;
            x = cos(phi) * r;
            z = sin(phi) * r;
            candi[k][i].x = x * diam + chains[k][pre_node_idx].x;
            candi[k][i].y = y * diam + chains[k][pre_node_idx].y;
            candi[k][i].z = z * diam + chains[k][pre_node_idx].z;

            if (checkClash(chains[k], candi[k][i], node_idx, sim_ligcs[k][i],
                    clash_dist, ki_dist, head, tail))
            {
                total_valid_idx.push_back(pair<unsigned,unsigned> (k,i));

            }
        }
    return total_valid_idx;
}

vectori calcuInitiFreqs(vectori3d &sim_ligcs, indices &total_valid_idx, indices &select_idx,
                        unsigned head, unsigned tail, unsigned n_samples, unsigned n_nodes)
{
    assert(sim_ligcs.size()==n_samples);
    assert(sim_ligcs[0][0].size()==n_nodes);
    auto n_valid=(unsigned)total_valid_idx.size();
    random_device rd;
    mt19937 eng(rd());
    uniform_int_distribution<unsigned> select(0,n_valid-1);
    vectori select_ligc(n_nodes,0);

    for(unsigned i=0; i<n_samples; ++i)
    {
        unsigned idx=select(eng);
        select_idx[i]=total_valid_idx[idx];
        for (unsigned j=head; j<tail; ++j)
            select_ligc[j]+=sim_ligcs[total_valid_idx[idx].first][total_valid_idx[idx].second][j];
    }
    return select_ligc;
}

double calcuProb(vectori &select_ligc, unsigned node_idx, unsigned head, unsigned tail,
                 vectord2d &inter, vectord2d &weights)
{
    double p=0;
    for (unsigned i=head; i<tail; ++i)
    {
        if (inter[node_idx][i]==0) continue;
        if (i==node_idx) continue;
        if (i==node_idx+1) continue;
        if (i==node_idx-1) continue;
        //p+=pow(select_ligc[i]-inter[node_idx][i],2);
        p+=log(poisson_pmf(select_ligc[i],inter[node_idx][i]))*weights[node_idx][i];
    }
    return p;
}

double calcuNewProb(vectori &select_ligc, vectori3d &sim_ligcs, indices &total_valid_idx,
                     indices &select_idx, unsigned node_idx, unsigned head, unsigned tail,
                     unsigned old_select_idx, unsigned new_valid_idx, vectord2d &inter,
                     vectord2d &weights)
{
    double p=0;
    for (unsigned i=head; i<tail; ++i)
    {
        if (inter[node_idx][i]==0) continue;
        if (i==node_idx) continue;
        if (i==node_idx+1) continue;
        if (i==node_idx-1) continue;
        unsigned new_num=select_ligc[i]-sim_ligcs[select_idx[old_select_idx].first][select_idx[old_select_idx].second][i];
        new_num+=sim_ligcs[total_valid_idx[new_valid_idx].first][total_valid_idx[new_valid_idx].second][i];
        p+=log(poisson_pmf(new_num,inter[node_idx][i]))*weights[node_idx][i];
        //p+=pow(new_num-inter[node_idx][i],2);
    }
    return p;
}

bool acceptance(double &old_p, double &new_p)
{
    return new_p>=old_p;
    //if (new_p>=old_p) return true;
    //random_device rd;
    //mt19937 eng(rd());
    //uniform_real_distribution<double> distr(0.0,1.0);
    //double u=distr(eng);
    //if (u<=(exp(new_p)/exp(old_p))) return true;
    //else return false;
    //return false;
}

my_ensemble SBIF(vectord2d &inter, vectord2d &weights, unsigned n_samples, unsigned n_sphere,
               const double diam, const double clash_dist, const double ki_dist, unsigned max_trials,
               unsigned n_iter)
{
    auto n_nodes=(unsigned)inter.size();

    random_device rd;
    mt19937 eng(rd());
    uniform_int_distribution<unsigned> direc(0,1);
    uniform_int_distribution<unsigned> pick_node(0,n_nodes-1);
    uniform_int_distribution<unsigned> pick_select_idx(0,n_samples-1);

    Node initi_node={0,0,0};
    my_ensemble empty_chains(0);
    indices total_valid_idx;
    vectori select_ligc;
    unsigned n_trials=0;
    unsigned count=0;
    unsigned head=0;
    unsigned tail=0;
    unsigned init_idx,dir_idx,node_idx,pre_node_idx;
    unsigned old_select_idx, new_valid_idx;
    bool shiftFlag;
    bool validFlag;
    vectord final_p;
    while (n_trials<max_trials)
    {
        //cout<<"Trial "<<n_trials+1<<" starts!"<<endl;

        my_ensemble old_chains(n_samples,my_chain(n_nodes,initi_node));
        my_ensemble new_chains(n_samples,my_chain(n_nodes,initi_node));
        my_ensemble candi(n_samples,my_chain(n_sphere,initi_node));
        vectori3d sim_ligcs(n_samples,vectori2d(n_sphere,vectori(n_nodes)));
        indices select_idx(n_samples);
        double old_p,new_p;
        shiftFlag=true;
        validFlag=true;

        init_idx=pick_node(eng);
		head=init_idx;
        tail=init_idx+1;
        //cout<<"Intial Node: "<<init_idx<<endl;

        for (unsigned i=0; i<n_nodes-1; ++i)
        {
            dir_idx=direc(eng);
            if (dir_idx==0)
            {
                if (head==0)
                {
                    node_idx=tail;
                    pre_node_idx=tail-1;
                }
                else
                {
                    node_idx=head-1;
                    pre_node_idx=head;
                }
            }
            else
            {
                if (tail==n_nodes)
                {
                    node_idx=head-1;
                    pre_node_idx=head;
                }
                else
                {
                    node_idx=tail;
                    pre_node_idx=tail-1;
                }
            }
            if (shiftFlag)
            {
                total_valid_idx = getNextNodesEvenEnsemb(old_chains, candi, sim_ligcs, diam, n_sphere,
                                                         node_idx, pre_node_idx, clash_dist, ki_dist,
                                                         head, tail);
                auto n_valid = total_valid_idx.size();
                if (total_valid_idx.empty())
                {
                    validFlag = false;
                    break;
                }
                uniform_int_distribution<unsigned> pick_valid_idx(0, (unsigned) n_valid - 1);
                select_ligc = calcuInitiFreqs(sim_ligcs, total_valid_idx, select_idx,
                                              head, tail, n_samples, n_nodes);
                double total_p=0;
                for (unsigned k=head; k<tail; ++k)
                {
                    if (k==node_idx) continue;
                    if (k==node_idx-1) continue;
                    if (k==node_idx+1) continue;
                    total_p+=inter[node_idx][k];
                }
                if (total_p>0)
                {
                    old_p = calcuProb(select_ligc, node_idx, head, tail, inter, weights);

                    final_p.clear();
                    final_p.push_back(old_p);

                    count=0;
                    while (count < n_iter)
                    {
                        old_select_idx = pick_select_idx(eng);
                        new_valid_idx = pick_valid_idx(eng);
                        new_p = calcuNewProb(select_ligc, sim_ligcs, total_valid_idx, select_idx,
                                             node_idx, head, tail, old_select_idx, new_valid_idx, inter, weights);
                        //cout<<"Node_idx "<<node_idx<<" old_p "<<old_p<<" new_p "<<new_p<<" count "<<count<<" final_p.size() "<<final_p.size()<<endl;
                        if (acceptance(old_p, new_p))
                        {
                            for (unsigned k = head; k < tail; ++k)
                            {
                                if (k == node_idx) continue;
                                if (k == node_idx + 1) continue;
                                if (k == node_idx - 1) continue;
                                select_ligc[k] = select_ligc[k] - sim_ligcs[select_idx[old_select_idx].first][select_idx[old_select_idx].second][k]
                                                 + sim_ligcs[total_valid_idx[new_valid_idx].first][total_valid_idx[new_valid_idx].second][k];
                            }
                            select_idx[old_select_idx] = total_valid_idx[new_valid_idx];
                            final_p.push_back(new_p);
                            old_p = new_p;
                        } else final_p.push_back(old_p);
                        assert(final_p.size()>=2);
                        if (final_p[-2]==0)
                        {
                            count += 1;
                            continue;
                        }
                        if (abs(final_p[final_p.size()-1] - final_p[final_p.size()-2]) < 0.000001 * abs(final_p[final_p.size()-2]))
                            count += 1;
                    }
                }
                for (unsigned j = 0; j < n_samples; ++j)
                {
                    for (unsigned k = head; k < tail; ++k)
                        new_chains[j][k] = old_chains[select_idx[j].first][k];
                    new_chains[j][node_idx] = candi[select_idx[j].first][select_idx[j].second];
                }
                shiftFlag = false;
            }
            else
            {
                total_valid_idx = getNextNodesEvenEnsemb(new_chains, candi, sim_ligcs, diam, n_sphere,
                                                         node_idx, pre_node_idx, clash_dist, ki_dist,
                                                         head, tail);
                auto n_valid = total_valid_idx.size();
                if (total_valid_idx.empty())
                {
                    validFlag = false;
                    break;
                }
                uniform_int_distribution<unsigned> pick_valid_idx(0, (unsigned) n_valid - 1);
                select_ligc = calcuInitiFreqs(sim_ligcs, total_valid_idx, select_idx,
                                              head, tail, n_samples, n_nodes);

                double total_p=0;
                for (unsigned k=head; k<tail; ++k)
                {
                    if (k==node_idx) continue;
                    if (k==node_idx-1) continue;
                    if (k==node_idx+1) continue;
                    total_p+=inter[node_idx][k];
                }
                if (total_p>0)
                {
                    old_p = calcuProb(select_ligc, node_idx, head, tail, inter, weights);

                    final_p.clear();
                    final_p.push_back(old_p);

                    count=0;
                    while (count < n_iter)
                    {
                        old_select_idx = pick_select_idx(eng);
                        new_valid_idx = pick_valid_idx(eng);
                        new_p = calcuNewProb(select_ligc, sim_ligcs, total_valid_idx, select_idx,
                                             node_idx, head, tail, old_select_idx, new_valid_idx, inter, weights);
                        //cout<<"Node_idx "<<node_idx<<" old_p "<<old_p<<" new_p "<<new_p<<" count "<<count<<" final_p.size() "<<final_p.size()<<endl;
                        if (acceptance(old_p, new_p))
                        {
                            for (unsigned k = head; k < tail; ++k)
                            {
                                if (k == node_idx) continue;
                                if (k == node_idx + 1) continue;
                                if (k == node_idx - 1) continue;
                                select_ligc[k] = select_ligc[k] - sim_ligcs[select_idx[old_select_idx].first][select_idx[old_select_idx].second][k]
                                                 + sim_ligcs[total_valid_idx[new_valid_idx].first][total_valid_idx[new_valid_idx].second][k];
                            }
                            select_idx[old_select_idx] = total_valid_idx[new_valid_idx];
                            final_p.push_back(new_p);
                            old_p = new_p;
                        } else final_p.push_back(old_p);
                        assert(final_p.size()>=2);
                        if (final_p[-2]==0)
                        {
                            count += 1;
                            continue;
                        }
                        if (abs(final_p[final_p.size()-1] - final_p[final_p.size()-2]) < 0.000001 * abs(final_p[final_p.size()-2]))
                            count += 1;
                    }
                }
                for (unsigned j = 0; j < n_samples; ++j)
                {
                    for (unsigned k = head; k < tail; ++k)
                        old_chains[j][k] = new_chains[select_idx[j].first][k];
                    old_chains[j][node_idx] = candi[select_idx[j].first][select_idx[j].second];
                }
                shiftFlag = true;
            }
            if (dir_idx==0)
            {
                if (head == 0)
                    tail = tail + 1;
                else
                    head = head - 1;
            }
            else
            {
                if (tail==n_nodes)
                    head=head-1;
                else
                    tail=tail+1;
            }
            //cout<<"Node "<<node_idx<<" has been generated!"<<endl;
        }
        if (validFlag)
        {
            if (shiftFlag)
                return old_chains;
            else
                return new_chains;
        }
        n_trials+=1;
    }
    return empty_chains;
}
