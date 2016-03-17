#include "reducednodemapping.h"
#include <algorithm>
ReducedNodeMapping::ReducedNodeMapping() : m_max(-1) {

}

ReducedNodeMapping::ReducedNodeMapping(int max) : m_max(max) {
}


bool ReducedNodeMapping::add(NodeMapping mapping) {
    string key = "";
    /* m.first is the node in the forbidden graph,
     * m.second is the node in the input graph*/
    vector<int> keys;
    for(const auto &m : mapping) {
        keys.push_back(m.second);
    }
    sort(keys.begin(), keys.end());
    for(int k : keys) {
        key += "."+std::to_string(k);
    }
    //clog << key << endl;
    /* so if a graph has some automorphism, than we will store only one NodeMapping*/
    m_data[key] = mapping;

    if(m_max == -1) {
        return false; //do not stop
    } else {
        if(m_max - 1 == m_data.size())
            return true; // stop, its enought
        return false;
    }
}

vector<NodeMapping> ReducedNodeMapping::get() const {
    vector<NodeMapping> ret;
    for(const auto &m : m_data) {
        ret.push_back(m.second);
    }
    return ret;
}
