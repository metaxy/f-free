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
    opt->setFlag( "input", 'i' );
    opt->setFlag( "forbidden", 'f' );
    opt->setOption( "seed" );

    /* go through the command line and get the options  */
    opt->processCommandArgs( argc, argv );
    if( opt->getValue( "seed" ) != NULL) {
        config["seed"] = opt->getValue( "seed" );
    }
    if( opt->getValue( "input" ) != NULL) {
        config["input"] = opt->getValue( "input" );
    }
    /* 8. DONE */
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

    while (pdir = readdir(dir)) {
        files.push_back(pdir->d_name);
    }

    return files;
}
