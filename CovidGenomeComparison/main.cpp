#include <iostream>
#include <fstream>
#include <string>
#include <vector>

using namespace std;

// Takes a vector of filenames, reads the contents of those files, 
// and returns a vector of strings, each string being the contects of each file
vector<string> ReadFiles(vector<string> fileNames)
{
    vector<string> inputStrings;
    vector<fstream *> fileStreams;

    // open a file handle to each file name
    for (int i = 0; i < fileNames.size(); ++i)
    {
        string fileName = fileNames[i];
        fstream * f = new fstream(fileName, std::ios::in);
        fileStreams.push_back(f);
    }

    // read files into vector of lines
    vector<string> S;

    for (int i = 0; i < fileStreams.size(); ++i)
    {
        string line = "";
        getline(*fileStreams[i], line);
        string inputString = "";
        while (*fileStreams[i] >> line)
        {
            inputString += line;
        }

        S.push_back(inputString);
    }

    // close every stream to prevent memory leaks
    for (int i = 0; i < fileNames.size(); ++i)
    {
        fileStreams[i]->close();
        delete fileStreams[i];
    }

    return S;
}

int main(int argc, char* argv[])
{
    if (argc < 4)
    {
        cout << "[ERROR - not enough args]" << endl;
        return -1;
    }

    vector<string> fileNames;
    // get command line arguments (file names)
    for (int i = 1; argv[i]; ++i)
    {
        fileNames.push_back(string(argv[i]));
    }

    vector<string> S = ReadFiles(fileNames);

    return 0;
}