#ifndef REDUCEDNODEMAPPING_H
#define REDUCEDNODEMAPPING_H
#include <map>
#include "src/common/common.h"
class ReducedNodeMapping
{
public:
    ReducedNodeMapping();
    ReducedNodeMapping(int max);

    bool add(NodeMapping mapping);
    vector<NodeMapping> get() const;


private:
    int m_max;
    map<string, NodeMapping> m_data;
};

#endif // REDUCEDNODEMAPPING_H
