#include <iostream>
#include <string>
#include "ScoreTable.h"
#include "Table.h"

using namespace std;

enum algorithm
{
    GLOBAL,
    LOCAL
};

int main(int argc, char *argv[])
{
    // make sure there's enough parameters
    if (argc < 3 || argc > 4)
    {
        cout << "Invalid # of cmd arguments: " << argc - 1 << " were given. 2 are needed" << endl;    
        cout << "try: <executable name> <input file containing both s1 and s2> <0: global, 1: local> <optional: path to parameters config file>" << endl;
        return -1; 
    }

    // gather program arguments
    string inputFileName = argv[1];
    char seqType = argv[2][0];
    algorithm alg = LOCAL;

    if (seqType == '0')
    {
        alg = GLOBAL;
    }

    // if user included the optional argument of a param file, otherwise default file
    string paramFileName = "parameters.config";
    if (argc == 4)
    {
        paramFileName = argv[4];
    }
    ScoreTable score(paramFileName);
    Table t(inputFileName, paramFileName, score);
    t.initialize();

    if (alg == GLOBAL)
    {
        t.globalInit();
        t.globalAlign();
    }
    else // alg == LOCAL
    {
        t.localInit();
        // t.localAlign();
    }

    t.retraceTable();
    
    return 0;
}