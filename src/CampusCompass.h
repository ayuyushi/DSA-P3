#pragma once
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <vector>
#include <fstream>
#include <sstream>
#include "Student.h"

using namespace std;


struct Edge
{
    int neighbor;
    int weight;
};


class CampusCompass {

    unordered_map<int, vector<Edge>> adj;
    unordered_map<int, string> locations;
    unordered_set<string> classCodes;
    StudentManager manager;

public:
    CampusCompass();
    bool ParseCSV(const string &edges_filepath, const string &classes_filepath)
    {
        ifstream classfile(classes_filepath);
        if (!classfile.is_open())
        {
            return false;
        }
        string line;
        getline(classfile, line);
        while (getline(classfile, line))
        {
            ifstream ss(line);
            string classCode;
            getline(ss, classCode, ',');
            classCodes.insert(classCode);
        }

        ifstream edgefile(edges_filepath);
        if (!edgefile.is_open())
        {
            return false;
        }
        string line1;
        string location1, location2, name1, name2, timeStr;
        getline(classfile, line1);
        while (getline(classfile, line1))
        {
            ifstream ss(line1);
            string classCode;
            getline(ss, location1, ',');
            getline(ss, location2, ',');
            getline(ss, name1, ',');
            getline(ss, name2, ',');
            getline(ss, timeStr, ',');

            int u = stoi(location1);
            int v = stoi(location2);
            int timeInt = stoi(timeStr);

            locations[u] = name1;
            locations[v] = name2;

            Edge uToV = {v, timeInt};
            Edge vToU = {u, timeInt};
            adj[u].push_back(uToV);
            adj[v].push_back(vToU);
        }
        return true;
    }

    bool ParseCommand(const string &command)
    {

    }

    // helper function to check if class code is in classes.csv
    bool isValidClass(const string& classCode)
    {
        if (classCodes.find(classCode) == classCodes.end()){
            return false;
        }
        return true;
    }
};


// verifies if UF ID is valid
string verifyNum(const string& number)
{
    // UFID must be 8 digits
    if (number.length() != 8)
    {
        return "unsuccessful";
    }

    for (char c : number)
    {
        if (!isdigit(c)) // c++ documentation, check if char is a digit
        {
            return "unsuccessful";
        }
    }
    return number;
}

// helper function to verify if name is valid
string verifyName(const string& name)
{
    string test = "unsuccessful";

    for (char c : name)
    {

        if (!isalpha(c)) // c++ documentation, check if char is a letter
        {
            if (c == ' ') // if there is a space, loop should continue
            {
                continue;
            }
            return test;
        }
    }
    return name;
}

// helper function to verify if class code is valid
string verifyClassCode(const string& classCode, CampusCompass& compass)
{
    // class code must be 7 characters
    if (classCode.length() != 7)
    {
        return "unsuccessful";
    }

    for (int i = 0; i < 7; i++)
    {
        if (i < 3) // for first three characters
        {
            if (!isupper(classCode[i])) // c++ documentation, check if char is a capital letter
            {
                return "unsuccessful";
            }
        }
        else // for remaining four characters
        {
            if (!isdigit(classCode[i])) // c++ documentation, check if char is a digit
            {
                return "unsuccessful";
            }
        }
    }
    if (!compass.isValidClass(classCode)) // uses helper function from CampusCompass to verify if the class code is in classes.csv
    {
        return "unsuccessful";
    }

    return classCode;
}


