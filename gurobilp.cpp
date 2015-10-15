#include "gurobilp.h"
#include "gurobi_c++.h"

using namespace std;
GurobiLP::GurobiLP(int nodeCount) : m_nodeCount(nodeCount)
{
    try {
        m_env = new GRBEnv();
        m_model = new GRBModel(*m_env);
        m_model->getEnv().set(GRB_IntParam_OutputFlag, 0);
/*    m_model.getEnv().set(GRB_IntParam_Threads, 1);
    m_model.getEnv().set(GRB_IntParam_Method, 1);
    m_model.getEnv().set(GRB_DoubleParam_NodefileStart, 0.5);
*/
        m_vars =  new GRBVar*[nodeCount];
        for (int i=0; i < nodeCount; i++) {
            m_vars[i] = new GRBVar[nodeCount];
        }
    } catch(GRBException e) {
        cout << "Error code = " << e.getErrorCode() << endl;
        cout << e.getMessage() << endl;
    } catch(...) {
        cout << "Exception during optimization" << endl;
    }

}
inline GRBVar GurobiLP::e(NodeT x, NodeT y)
{
    if(y > x)
        return m_vars[x][y];
    return m_vars[y][x];
}
inline GRBVar GurobiLP::e(Edge ed)
{
    return e(ed.first, ed.second);
}

inline int GurobiLP::weight(NodeT x, NodeT y)
{
    int a = e(x,y).get(GRB_DoubleAttr_X);
    if(a < 0.5) return -1;
    return 1;
}
inline int GurobiLP::weight(Edge edge)
{
    return weight(edge.first, edge.second);
}

double GurobiLP::weightRelaxed(NodeT x, NodeT y)
{
    clog <<  (double)e(x,y).get(GRB_DoubleAttr_X) << endl;
    return e(x,y).get(GRB_DoubleAttr_X);
}
double GurobiLP::weightRelaxed(Edge edge)
{
    return weightRelaxed(edge.first, edge.second);
}
void GurobiLP::addVar(Edge e)
{
    m_vars[e.first][e.second] = m_model->addVar(0.0, 1.0, 0.0, GRB_BINARY);
}
void GurobiLP::addVarRelaxed(Edge e)
{
    m_vars[e.first][e.second] = m_model->addVar(0.0, 1.0, 0.0, GRB_CONTINUOUS);
}
void GurobiLP::addVars(vector<Edge> edges)
{
    for(const auto &i : edges)
        addVar(i);
}
void GurobiLP::addModelVars(Model weights)
{
    for(const auto &kv : weights) {
        addVar(kv.first);
    }
    m_model->update();
}
void GurobiLP::addModelVarsRelaxed(Model weights)
{
    for(const auto &kv : weights) {
        addVarRelaxed(kv.first);
    }
    m_model->update();
}
void GurobiLP::setObjective(Model weights)
{
    de("set objective");
    try {
        GRBLinExpr min = 0;
        for(const auto &kv : weights) {
            min -= kv.second * e(kv.first);
        }
        m_model->setObjective(min, GRB_MINIMIZE);
        m_weights = weights;
    } catch(GRBException e) {
        clog << "set Objective: Error code = " << e.getErrorCode() << endl;
        clog << e.getMessage() << endl;
        exit(-1);
    }
}
void GurobiLP::addConstraint(P3 p3)
{
    NodeT u = get<0>(p3);
    NodeT v = get<1>(p3);
    NodeT w = get<2>(p3);
    m_model->addConstr(e(u,v) + e(v,w) - e(u,w) <= 1);
    m_model->addConstr(e(u,v) - e(v,w) + e(u,w) <= 1);
    m_model->addConstr(-e(u,v) + e(v,w) + e(u,w) <= 1);
}
void GurobiLP::addConstraints(vector<P3> p3s)
{
    de("add constainst");
    for(const auto &i : p3s)
        addConstraint(i);
}
Model GurobiLP::optimize()
{
    m_model->optimize();
    int optimstatus = m_model->get(GRB_IntAttr_Status);

    if (optimstatus == GRB_OPTIMAL) {
    } else if (optimstatus == GRB_INFEASIBLE) {
        exit(-1);
    } else if (optimstatus == GRB_UNBOUNDED) {
        exit(-1);
    } else {
        exit(-1);
    }
    Model ret;
    for(const auto &i : m_weights) {
        ret[i.first] = weight(i.first);
    }
    return ret;
}
ModelRelaxed GurobiLP::optimizeRelaxed()
{
    m_model->optimize();
    int optimstatus = m_model->get(GRB_IntAttr_Status);

    if (optimstatus == GRB_OPTIMAL) {
    } else if (optimstatus == GRB_INFEASIBLE) {
        exit(-1);
    } else if (optimstatus == GRB_UNBOUNDED) {
        exit(-1);
    } else {
        exit(-1);
    }
    ModelRelaxed ret;
    for(const auto &i : m_weights) {
        ret[i.first] = weightRelaxed(i.first);
     //   clog << i.first.first<< " " << i.first.second << " = " << weightRelaxed(i.first) << endl; 
    }
    return ret;
}
