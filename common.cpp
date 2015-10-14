#include "common.h"
#include <iostream>
#include <sstream>
#include <fstream>
#include <iomanip>
#include "anyoption.h"
using namespace std;
Common::Common()
{
}


Graph Common::graphFromStdin()
{
    Graph g;
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
Graph Common::graphFromFile(string fileName)
{
    Graph g;
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
    opt->setOption( "seed" );

    /* go through the command line and get the options  */
    opt->processCommandArgs( argc, argv );
    if( opt->getValue( "seed" ) != NULL) {
        config["seed"] = opt->getValue( "seed" );
    }
    /* 8. DONE */
    delete opt;
    return config;
}

