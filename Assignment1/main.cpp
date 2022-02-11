#include <iostream>
#include <string>
#include <sstream>

using namespace std;

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

    // if user included the optional argument of a param file, otherwise default file
    string paramFileName = "parameters.config";
    if (argc == 4)
    {
        paramFileName = argv[4];
    }

    return 0;
}