#ifndef FORBIDDEN_H
#define FORBIDDEN_H
#include <vector>
#include "mgraph.h"
class Forbidden
{
public:
    Forbidden();
    static vector<MGraph> posibleSolutions(MGraph forbidden);

    static MGraph forbiddenWeight(const MGraph *input, vector<MGraph> forbidden);
    static MGraph forbiddenWeight2(const MGraph *input, vector<MGraph> forbidden);
};

#endif // FORBIDDEN_H
