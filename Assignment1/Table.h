#pragma once
#include <string>
#include <vector>
#include <fstream>
#include "cell.h"

#define MINIMUM -10000

using namespace std;

class Table
{
public:
    string s1;
    string s2;

    int width;
    int height;

    // scoring rules
    int match;
    int mismatch;
    int h;
    int g;

    vector<vector<cell> > vect;

    // builds a table
    Table(string & inputFileName, string & paramFileName, ScoreTable & sc)
    {
        s1 = "";
        s2 = "";
        match = sc.match;
        mismatch = sc.mismatch;
        h = sc.h;
        g = sc.g;
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

    // sets all cell values to 0
    void initialize()
    {
        width = s1.length();
        height = s2.length();
        vector<vector <cell> > v1(width , vector<cell> (height));
        vect = v1;

        // initialize the table cells
        for (vector<cell> v : vect)
        {
            for (cell c : v)
            {
                c.Sscore = 0;
                c.Dscore = 0;
                c.Iscore = 0;
            }
        }
    }

    // initializes the first rows of the table
    void globalInit()
    {
        vect[0][0].Dscore = 0;
        vect[0][0].Iscore = 0;
        vect[0][0].Sscore = 0;

        for (int i = 1; i < vect.size(); ++i)
        {
            vect[i][0].Sscore = MINIMUM;
            vect[i][0].Dscore = h + (i * g);
            vect[i][0].Iscore = MINIMUM;
        }
        for (int j = 1; j < vect[0].size(); ++j)
        {
            vect[0][j].Sscore = MINIMUM;
            vect[0][j].Dscore = MINIMUM;
            vect[0][j].Iscore = h + (j * g);
        }
    }

    void localInit()
    {

    }

    void globalAlign()
    {
        for (int i = 1; i < vect.size(); ++i)
        {
            if (i == vect.size() - 1)
            {
                int x = 1;
            }
            for (int j = 1; j < vect[0].size(); ++j)
            {
                vect[i][j].Sscore = Get_S(i, j);
                vect[i][j].Dscore = Get_D(i, j);
                vect[i][j].Iscore = Get_I(i, j);
            }
        }
    }

    int Get_S(const int i, const int j)
    {
        bool isMatch = false;
        int result;
        cell c = vect[i - 1][j - 1];

        if (s1[i] == s1[j])
        {
            isMatch = true;
        }

        int S = c.Sscore;
        int D = c.Dscore;
        int I = c.Iscore;

        int maxVal = max(D, max(S, I));
        
        if (isMatch)
        {
            result = maxVal + match;
        }
        else
        {
            result = maxVal + mismatch;
        }

        return result;
    }

    int Get_D(const int i, const int j)
    {
        cell c = vect[i - 1][j];

        int D = c.Dscore + g;
        int S = c.Sscore + h + g;
        int I = c.Iscore + h + g;

        int maxVal = max(D, max(S, I));
        return maxVal;
    }

    int Get_I(const int i, const int j)
    {
        cell c = vect[i][j - 1];

        int I = c.Iscore + g;
        int S = c.Sscore + h + g;
        int D = c.Dscore + h + g;

        int maxVal = max(I, max(S, D));
        return maxVal;
    }
};