#pragma once
#include <string>
#include <vector>
#include <fstream>
#include "cell.h"

using namespace std;

class Table
{
public:
    string s1;
    string s2;

    int width;
    int height;

    vector<vector<cell> > vect;

    // builds a table
    Table(string & inputFileName)
    {
        s1 = "";
        s2 = "";
        readFile(inputFileName);
    }

    // reads the input file and fills s1 and s2
    void readFile(string & inputFileName)
    {
        ifstream inStream;
        inStream.open(inputFileName, ifstream::in);

        if (inStream.is_open() == false)
        {
            cout << "Error opening input file" << endl;
            return;
        }

        string line = "";
        bool isS1 = false;
        bool isS2 = false;
        // iterate through file line by line
        while (getline(inStream, line))
        {
            if (line[0] == '>' && isS1 == false)
            {
                isS1 = true;
                continue;
            }
            else if (line[0] == '>' && isS1 == true)
            {
                isS2 = true;
                isS1 = false;
                continue;
            }
            else if (line.empty())
            {
                continue;
            }
            else if (isS1)
            {
                s1 += line;
            }
            else if (isS2)
            {
                s2 += line;
            }
            else
            {
                cout << "Error in reading file" << endl;
            }
        }
    }

    void initialize()
    {
        width = s1.length();
        height = s2.length();
        vector<vector <cell> > v(width , vector<cell> (height));
        vect = v;

        
    }
};