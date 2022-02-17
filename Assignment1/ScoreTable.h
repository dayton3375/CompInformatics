#pragma once
#include <iostream>
#include <string>
#include <fstream>
#include <sstream>

using namespace std;

class ScoreTable
{
public:
    int match;
    int mismatch;
    int h; // opening gap
    int g; // gap extension

    // Stores scoring rules from file
    ScoreTable(string & filename)
    {
        ifstream inStream;
        inStream.open(filename, ifstream::in);
        if (inStream.is_open() == false)
        {
            cout << "Error reading parameters file: " << filename << endl;
        }

        string line;
        string var;
        string value;

        while (getline(inStream, line))
        {
            if (line == "")
            {
                continue;
            }
            // delimits the spaces
            stringstream stream(line);
            stream >> var;
            stream >> value;

            if (var == "match")
            {
                match = stoi(value);
            }
            else if (var == "mismatch")
            {
                mismatch = stoi(value);
            }
            else if (var == "h")
            {
                h = stoi(value);
            }
            else if (var == "g")
            {
                g = stoi(value);
            }
            else
            {
                cout << "Error getting values from parameters file, encountered string: " << line << endl;
                return;
            }
        }
    }
};