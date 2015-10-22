#ifndef FORBIDDEN_H
#define FORBIDDEN_H
#include <vector>
#include "mgraph.h"
class Forbidden
{
public:
    Forbidden();
    static vector<MGraph> posibleSolutions(MGraph forbidden);
};

#endif // FORBIDDEN_H
