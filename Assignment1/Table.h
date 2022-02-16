#pragma once
#include <iostream>
#include <string>
#include <vector>

using namespace std;

class Table
{
public:
    string s1;
    string s2;

    int width;
    int height;

    vector<vector<int> > vect;

    Table(int w, int h)
    {
        width = w;
        height = h;
        vector<vector <int> > v(w , vector<int> (h));
        vect = v;
        
    }
};