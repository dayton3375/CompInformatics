#pragma once
#include <string>
#include <vector>
#include <fstream>
#include "cell.h"

#define MINIMUM -100000 // this is like negative infinity for the Initilization

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

    // statistics
    int matches;
    int mismatches;
    int opening_gaps;
    int gap_extensions;

    int maxAlignLocal; // largest score during local align
    int alignLocal_i;
    int alignLocal_j;

    vector<vector<cell> > vect;

    // builds a table
    Table(string &inputFileName, string &paramFileName, ScoreTable &sc)
    {
        s1 = "";
        s2 = "";
        match = sc.match;
        mismatch = sc.mismatch;
        h = sc.h;
        g = sc.g;

        matches = 0;
        mismatches = 0;
        opening_gaps = 0;
        gap_extensions = 0;

        maxAlignLocal = INT_MIN;
        alignLocal_i = 0;
        alignLocal_j = 0;

        readFile(inputFileName);
    }

    // reads the input file and fills s1 and s2
    void readFile(string &inputFileName)
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
                return;
            }
        }
    }

    // sets all cell values to 0
    void initialize()
    {
        width = s1.length() + 1;
        height = s2.length() + 1;
        vector<vector<cell> > v1(width, vector<cell>(height));
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

        cout << "INPUT:" << endl
             << endl;
        cout << "Scores:   "
             << "match = " << match << "   ";
        cout << "mismatch = " << mismatch << "   ";
        cout << "h = " << h << "   ";
        cout << "g = " << g << endl
             << endl;
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
        vect[0][0].Dscore = 0;
        vect[0][0].Iscore = 0;
        vect[0][0].Sscore = 0;

        for (int i = 1; i < vect.size(); ++i)
        {
            vect[i][0].Sscore = MINIMUM;
            vect[i][0].Dscore = 0;
            vect[i][0].Iscore = MINIMUM;
        }
        for (int j = 1; j < vect[0].size(); ++j)
        {
            vect[0][j].Sscore = MINIMUM;
            vect[0][j].Dscore = MINIMUM;
            vect[0][j].Iscore = 0;
        }
    }

    // Fills the table
    void globalAlign()
    {
        for (int i = 1; i < vect.size(); ++i)
        {
            for (int j = 1; j < vect[0].size(); ++j)
            {
                vect[i][j].Sscore = Get_S(i, j);
                vect[i][j].Dscore = Get_D(i, j);
                vect[i][j].Iscore = Get_I(i, j);
            }
        }
        int m = vect.size() - 1;
        int n = vect[0].size() - 1;
        cout << "Global Score: " << vect[m][n].Sscore << endl;
    }

    void localAlign()
    {
        int S = 0;
        int D = 0;
        int I = 0;
        int maxScore = INT_MIN;

        for (int i = 1; i < vect.size(); ++i)
        {
            for (int j = 1; j < vect[0].size(); ++j)
            {
                S = Get_S(i, j);
                D = Get_D(i, j);
                I = Get_I(i, j);

                vect[i][j].Sscore = S;
                vect[i][j].Dscore = D;
                vect[i][j].Iscore = I;

                zeroIfNegative(i, j); // zero scores that are negative

                maxScore = max(S, max(D, I)); // get the largest score

                if (maxScore > maxAlignLocal)
                {
                    maxAlignLocal = maxScore;
                    alignLocal_i = i;
                    alignLocal_j = j;
                }
            }
        }

        int m = vect.size() - 1;
        int n = vect[0].size() - 1;
        cout << "Global Score: " << vect[m][n].Sscore << endl;
    }

    // this zeros out negative score values
    // ONLY use during LOCAL align
    void zeroIfNegative(const int i, const int j)
    {
        if (vect[i][j].Sscore < 0)
        {
            vect[i][j].Sscore = 0;
        }
        if (vect[i][j].Dscore < 0)
        {
            vect[i][j].Dscore = 0;
        }
        if (vect[i][j].Iscore < 0)
        {
            vect[i][j].Iscore = 0;
        }
    }

    int Get_S(const int i, const int j)
    {
        bool isMatch = false;
        int result;
        cell c = vect[i - 1][j - 1];

        if (s1[i] == s2[j])
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

    char GetNextMove(const int i, const int j)
    {
        char result = '\0';
        cell c = vect[i][j];

        int maxVal = max(c.Dscore, max(c.Iscore, c.Sscore));

        if (c.Sscore == maxVal)
        {
            result = 'S';
        }
        else if (c.Iscore == maxVal)
        {
            result = 'I';
        }
        else if (c.Dscore == maxVal)
        {
            result = 'D';
        }
        else
        {
            cout << "Error in GetNextMove of value: " << maxVal << endl;
            return '\0';
        }

        return result;
    }

    void retraceTable(const algorithm alg)
    {
        char nextMove = '\0'; // should be I for Insert, S for Substitute, or D for Delete
        int i = 0;
        int j = 0;

        if (alg == GLOBAL)
        {
            // set indexes to bottom right corner of table
            i = vect.size() - 1;
            j = vect[0].size() - 1;
        }
        else // alg == LOCAL
        {
            i = alignLocal_i;
            j = alignLocal_j;
        }

        // these will be the strings printed to the screen
        string s1_Line = "";
        string midLine = "";
        string s2_Line = "";

        bool isGap = false;

        while ((i >= 0) && (j >= 0))
        {
            nextMove = GetNextMove(i, j); // I, S, or D

            switch (nextMove)
            {
            case 'D': // Delete
                if (isGap == false)
                {
                    isGap = true;
                    ++opening_gaps;
                }
                else
                {
                    ++gap_extensions;
                }
                s1_Line = s1[i] + s1_Line;
                midLine = " " + midLine;
                s2_Line = "-" + s2_Line;
                --i;
                break;

            case 'I': // Insert
                if (isGap == false)
                {
                    isGap = true;
                    ++opening_gaps;
                }
                else
                {
                    ++gap_extensions;
                }
                s1_Line = "-" + s1_Line;
                midLine = " " + midLine;
                s2_Line = s2[j] + s2_Line;
                --j;
                break;

            case 'S': // Substitute
                isGap = false;
                s1_Line = s1[i] + s1_Line;

                if (s1[i] == s2[j])
                {
                    midLine = "|" + midLine;
                    matches++;
                }
                else
                {
                    midLine = " " + midLine;
                    mismatches++;
                }

                s2_Line = s2[j] + s2_Line;
                --i;
                --j;
                break;

            default:
                cout << "error in retrace, switch value: " << nextMove << endl;
                return;
            }
        }
    }
};