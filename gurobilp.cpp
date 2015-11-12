#include "gurobilp.h"
#include "gurobi_c++.h"

using namespace std;
GurobiLP::GurobiLP(int nodeCount) : m_nodeCount(nodeCount)
{
    try {
        m_env = new GRBEnv();
        m_model = new GRBModel(*m_env);
        // no output from gurobi
        m_model->getEnv().set(GRB_IntParam_OutputFlag, 1);
        m_model->getEnv().set(GRB_IntParam_LogToConsole, 0);
        m_model.getEnv().set(GRB_IntParam_Threads, 8);

       /* m_model->getEnv().set(GRB_IntParam_Method, 1);
        m_model->addConstr(getEnv().set(GRB_DoubleParam_NodefileStart, 0.5);*/
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
void GurobiLP::addConstraint(MGraph *graph, MGraph *forbidden, NodeMapping *mapping)
{
    GRBLinExpr expr;
    for(Edge edge: forbidden->edges()) {
        Edge trans = Common::transformEdge(edge, mapping);
        if(graph->connected(trans)) {
            expr += 1 - e(trans);
        } else {
            expr += e(trans);
        }
    }
    m_model->addConstr(expr >= 1);
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
    }
    return ret;
}
