#ifndef STATE_APPROX_2K_H
#define STATE_APPROX_2K_H
#include "state.h"
#include <list>
#include "common.h"
#include "mergegraph.h"
class StateApprox2K
{
public:
     StateApprox2K();
     MergeGraph solve(MergeGraph graph);
private:
     double m_2k_diff;

};

#endif // STATE_APPROX_2K_H
