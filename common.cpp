#include "common.h"
#include <iostream>
#include <sstream>
#include <fstream>
#include <iomanip>
#include <dirent.h>
#include <string>
#include <vector>
#include "anyoption.h"
using namespace std;
Common::Common()
{
}


GGraph Common::graphFromStdin()
{
    GGraph g;
    string line, node1, node2;
    while (getline(cin, line))
    {
        int i;
        if ((i = line.find_first_of("#")) != string::npos) {
            line = line.substr(0, i);
        }
        stringstream ss(line);
        ss >> node1 >> node2;
        if (node1 == "") continue;
        if (node2 == "")
        {
            cout << "Eingabefehler" << endl;
            return g;
        }
        g.insertEdge(node1, node2);
    }
    return g;
}
GGraph Common::graphFromFile(string fileName)
{
    GGraph g;
    string line, node1, node2;
    std::ifstream infile(fileName);
    if(!infile) {
        clog << "coud not read file " << fileName << endl;
        exit(-1);
    }
    while (std::getline(infile, line))
    {
        int i;
        if ((i = line.find_first_of("#")) != string::npos) {
            line = line.substr(0, i);
        }
        stringstream ss(line);
        ss >> node1 >> node2;
        if (node1 == "") continue;
        if (node2 == "")
        {
            cout << "Eingabefehler" << endl;
            return g;
        }
        g.insertEdge(node1, node2);
    }
    return g;
}

void Common::printMatrix(int **matrix, unsigned int size)
{
#ifdef _DEBUG
    for(int i = 0; i < size; i++) {
        for(int y = 0; y < size; y++) {
            clog << std::setw(2) << matrix[i][y] << " ";
        }
        clog << endl;
    }
#endif
}

void Common::printVector(int *vector, unsigned int size)
{
#ifdef _DEBUG
    for(int i = 0; i < size; i++) {
        clog << i <<"="<< vector[i] << " ";
    }
    clog << endl;
#endif
}
void Common::printSubgraph(Subgraph *subgraph)
{
#ifdef _DEBUG
    for(Edge e: *subgraph) {
         clog << e.first  << "<->" << e.second << ",";
    }
    clog <<endl;
#endif
}

map<string, string> Common::parseConfig(int argc, char* argv[])
{
    map<string,string> config;
    AnyOption *opt = new AnyOption();
    opt->noPOSIX();
    opt->addUsage( "" );
    opt->addUsage( "Usage: " );
    opt->addUsage( "" );
    opt->addUsage( "" );
    opt->setFlag( "help", 'h' );
    opt->setOption( "input", 'i' );
    opt->setOption( "forbidden", 'f' );
    opt->setOption( "seed" );
    opt->setOption( "rounds" );
    opt->setOption( "random2_subgraph_batch" );
    opt->setOption( "print_output" );
    opt->setOption( "print_input" );

    /* go through the command line and get the options  */
    opt->processCommandArgs( argc, argv );
    if( opt->getValue( "seed" ) != NULL) {
        config["seed"] = opt->getValue( "seed" );
    }
    if( opt->getValue( "input" ) != NULL) {
        config["input"] = opt->getValue( "input" );
    }
    if( opt->getValue( "forbidden" ) != NULL) {
        config["forbidden"] = opt->getValue( "forbidden" );
    }
    if( opt->getValue( "rounds" ) != NULL) {
        config["rounds"] = opt->getValue( "rounds" );
    }
    if( opt->getValue( "random2_subgraph_batch" ) != NULL) {
        config["random2_subgraph_batch"] = opt->getValue( "random2_subgraph_batch" );
    }

    if( opt->getValue( "print_output" ) != NULL) {
        config["print_output"] = opt->getValue( "print_output" );
    }
    if( opt->getValue( "print_input" ) != NULL) {
        config["print_input"] = opt->getValue( "print_input" );
    }
    /* 8. DONE */
    delete opt;
    return config;
}
map<string, string> Common::parseConfigOptions(int argc, char* argv[], vector<string> options)
{
    map<string,string> config;
    AnyOption *opt = new AnyOption();
    opt->noPOSIX();
    opt->addUsage( "" );
    opt->addUsage( "Usage: " );
    opt->addUsage( "" );
    opt->addUsage( "" );
    opt->setFlag( "help", 'h' );
    for(string option: options) {
        opt->setOption( option.c_str() );
    }

    opt->processCommandArgs( argc, argv );
    for(string option: options) {
        if( opt->getValue( option.c_str() ) != NULL) {
            config[option] = opt->getValue( option.c_str() );
        }
    }

    delete opt;
    return config;
}
string Common::dotColor(float id, int size)
{
    float hue_factor = 1 / (float)size;
    float saturation = 1 - ((1 / (size)));
    //return std::to_string(id*hue_factor) + " " +std::to_string((id % 10) *saturation) + " " + std::to_string(0.999);
    return std::to_string((id+1)*hue_factor) + " " +std::to_string(0.99) + " " + std::to_string(0.999);

}

vector<string> Common::listFiles(string path)
{
    DIR*    dir;
    dirent* pdir;
    vector<string> files;

    dir = opendir(path.c_str());
    if(!dir)  {
        clog << "cloud not open " << path << endl;
        exit(-1);
    }

    while (pdir = readdir(dir)) {
        files.push_back(pdir->d_name);
    }

    return files;
}

Edge Common::transformEdge(const Edge &e, NodeMapping *mapping)
{
    /*clog << e.first << " " << e.second << endl;
    for(auto a: *mapping) {
        clog << a.first << " -> " << a.second << endl;
    }*/
    return Edge(mapping->at(e.first), mapping->at(e.second));
}

double Common::getDouble(Config *m_config, const string &name, double def)
{
    auto iter = m_config->find(name);
    if(iter != m_config->end()) {
        std::istringstream i(iter->second);
        double x;
        if (!(i >> x))
            return def;
        return x;
    }
    return def;
}
int Common::getInt(Config *m_config, const string &name, int def)
{
    auto iter = m_config->find(name);
    if(iter != m_config->end()) {
        std::istringstream i(iter->second);
        int x;
        if (!(i >> x))
            return def;
        return x;
    }
    return def;
}
