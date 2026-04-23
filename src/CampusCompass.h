#pragma once
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <vector>
#include <fstream>
#include <queue>
#include <sstream>
#include "Student.h"
#include <limits>

using namespace std;


struct Edge
{
    int neighbor;
    int weight;
    bool isClosed = false;
};


struct Node
{
    int node;
    int dist;
};

struct Compare //min heap
{
    bool operator()(const Node& n1, const Node& n2)
    {
        return n1.node > n2.node;
    }
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

    // helper function to check if class code is in classes.csv
    bool isValidClass(const string& classCode)
    {
        if (classCodes.find(classCode) == classCodes.end()){
            return false;
        }
        return true;
    }

    Edge* findEdge(int u, int v)
    {
        for (auto &edge : adj[u])
        {
            if (edge.neighbor == v)
            {
                return &edge;
            }
        }
        return nullptr;
    }

    bool isEdgeClosed(Edge& edge)
    {
        return edge.isClosed;
    }

    bool toggleEdgeClosure(int n, const vector<int> &edges)
    {
        for (int i = 0; i < 2 * n; i+=2)
        {
            int a = edges[i];
            int b = edges[i+1];
            Edge* e1 = findEdge(a, b);
            Edge* e2 = findEdge(b, a);
            if (e1)
            {
                e1->isClosed = !e1->isClosed;
            }
            if (e2)
            {
                e2->isClosed = !e2->isClosed;
            }
        }
        return true;
    }

    string checkEdgeStatus(int locationX, int locationY)
    {
        Edge* edge = findEdge(locationX, locationY);
        if (!edge)
        {
            return "DNE";
        }
        if (edge->isClosed)
        {
            return "closed";
        }
        return "open";
    }

    bool isConnected(int locationX, int locationY)
    {
        unordered_set<int> visited;
        queue<int> check;

        check.push(locationX);
        visited.insert(locationX);

        while (!check.empty())
        {
            int current = check.front();
            check.pop();
            if (current == locationY)
            {
                return true;
            }
            for (auto& edge : adj[current])
            {
                if (edge.isClosed)
                {
                    continue;
                }
                if (visited.find(edge.neighbor) == visited.end())
                {
                    check.push(edge.neighbor);
                    visited.insert(edge.neighbor);
                }
            }
        }
        return false;
    }

    unordered_map<int, int> Dijkstra(int start)
    {
        unordered_map<int, int> distance;
        unordered_set<int> visited;

        for (auto &edge : adj)
        {
            distance[edge.first] = std::numeric_limits<int>::max(); // c++ documentation; integer representation of infinity
        }
        priority_queue <Node, vector<Node>, Compare> openSet; //c++ documentation

        distance[start] = 0;
        openSet.push({start, 0});
        while (!openSet.empty())
        {
            Node current = openSet.top();
            openSet.pop();
            int u = current.node;
            if (visited.count(u))
            {
                continue;
            }
            visited.insert(u);
            for (auto &edge : adj[u])
            {
                if (edge.isClosed)
                {
                    continue;
                }

                int v = edge.neighbor;
                int w = edge.weight;

                if (distance[u]!= numeric_limits<int>::max() && distance[v] > distance[u] + 0w...00
            }
        }
        return distance;

    }



    int shortestEdges(int& studentID)
    {
        Student* current = manager.getStudent(studentID);
        vector<string> classes = current->getClasses();
        int residence = current->getResidenceID();
        return -1;
    }
















    bool ParseCommand(const string &command)
    {
        return false;
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


