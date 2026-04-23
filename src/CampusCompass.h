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
#include <iostream>
#include <utility>
#include <algorithm>
#include <cctype>

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
        return n1.dist > n2.dist;
    }
};


class CampusCompass {

    unordered_map<int, vector<Edge>> adj;
    unordered_map<int, string> locations;
    unordered_map<string, int> classToLocation;
    unordered_set<string> classCodes;
    StudentManager manager;

public:
    CampusCompass() {}

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
    string verifyClassCode(const string& classCode)
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
        if (!isValidClass(classCode)) // uses helper function from CampusCompass to verify if the class code is in classes.csv
        {
            return "unsuccessful";
        }

        return classCode;
    }

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
            stringstream ss(line);
            string classCode;
            string classLocation;
            getline(ss, classCode, ',');
            getline(ss, classLocation, ',');
            classCodes.insert(classCode);
            classToLocation[classCode] = stoi(classLocation);
        }

        ifstream edgefile(edges_filepath);
        if (!edgefile.is_open())
        {
            return false;
        }
        string line1;
        string location1, location2, name1, name2, timeStr;
        getline(edgefile, line1);
        while (getline(edgefile, line1))
        {
            stringstream ss(line1);
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

    unordered_map<int, int> Dijkstra(int start, unordered_map<int, int>& parent)
    {
        const int INF = numeric_limits<int>::max(); // c++ documentation; integer representation of infinity

        unordered_map<int, int> distance;
        unordered_set<int> visited;

        for (auto &edge : adj)
        {
            distance[edge.first] = INF;
            parent[edge.first] = -1;

            for (auto &edge1 : edge.second)
            {
                distance[edge1.neighbor] = INF;
                parent[edge1.neighbor] = -1;
            }
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

                if (distance[u]!= INF && distance[v] > distance[u] + w)
                {
                    distance[v] = distance[u] + w;
                    parent[v] = u;
                    openSet.push({v, distance[v]});
                }
            }
        }
        return distance;
    }


    vector<int> shortestEdges(int& studentID, const vector<string>& sortedClasses)
    {
        const int INF = numeric_limits<int>::max(); // c++ documentation; integer representation of infinity

        vector<int> distances;

        Student* current = manager.getStudent(studentID);
        if (!current) return {};

        int residence = current->getResidenceID();

        unordered_map<int, int> parent;

        auto dist = Dijkstra(residence, parent);

        for (auto &classCode : sortedClasses)
        {
            int location = classToLocation.at(classCode);
            if (dist[location] == INF)
            {
                distances.push_back(-1);
            }
            else
            {
                distances.push_back(dist[location]);
            }
        }
        return distances;
    }

    int PrimsMST(unordered_map<int, vector<Edge>> edges, unordered_set<int> nodesGraph)
    {

        const int INF = numeric_limits<int>::max(); // c++ documentation; integer representation of infinity


        unordered_set<int> inMST;
        unordered_map<int, int> keys;
        for (auto &node : nodesGraph)
        {
            keys[node] = INF;
        }
        int start = *nodesGraph.begin();
        keys[start] = 0;
        int total = 0;

        for (int i = 0; i < (int)nodesGraph.size(); i++)
        {
            int u = -1;
            int best = INF;
            for (auto &v : nodesGraph)
            {
                if (!inMST.count(v) && keys[v] < best)
                {
                    best = keys[v];
                    u = v;
                }
            }
            if (u == -1)
            {
                break;
            }
            inMST.insert(u);
            if (keys[u] == INF)
            {
                continue;
            }
            total += keys[u];
            for (auto &edge : edges.at(u))
            {
                int v = edge.neighbor;
                if (!nodesGraph.count(v) || edge.isClosed || inMST.count(v))
                {
                    continue;
                }
                if (edge.weight < keys[v])
                {
                    keys[v] = edge.weight;
                }
            }
        }
        return total;
    }


    int studentZone(int& studentID)
    {
        Student* current = manager.getStudent(studentID);
        if (!current) return {};

        int residence = current->getResidenceID();
        unordered_set<string> classes = current->getClasses();

        unordered_map<int, int> parent;
        auto distance = Dijkstra(residence, parent);

        const int INF = numeric_limits<int>::max(); // c++ documentation; integer representation of infinity

        unordered_set<int> nodesGraph;
        unordered_map<int, vector<Edge>> edges;
        nodesGraph.insert(residence);

        for (auto &classCode : classes)
        {
            int now = classToLocation.at(classCode);

            if (distance[now] == INF)
            {
                continue;
            }

            int target = now;

            while (target != residence && parent[target] != -1)
            {
                int par = parent[target];
                nodesGraph.insert(target);
                nodesGraph.insert(par);

                for (auto& edge: adj[par])
                {
                    if (edge.neighbor == target && !edge.isClosed)
                    {
                        edges[par].push_back(edge);
                    }
                }

                for (auto& edge: adj[target])
                {
                    if (edge.neighbor == par && !edge.isClosed)
                    {
                        edges[target].push_back(edge);
                    }
                }

                target = par;
            }
        }
        return PrimsMST(edges, nodesGraph);
    }


    string ParseCommand(const string &command)
{
    stringstream in(command);
    string func;
    in >> func;

    // insert student
        if (func == "insert")
        {
            string temp, name;
            getline(in, temp, '"');
            getline(in, name, '"');

            int id, residence, n;

            if (!(in >> id >> residence >> n))
            {
                return "unsuccessful";
            }

            if (verifyName(name) == "unsuccessful")
            {
                return "unsuccessful";
            }

            unordered_set<string> classes;
            string classCode;

            for (int i = 0; i < n; i++)
            {
                if (!(in >> classCode))
                {
                    return "unsuccessful";
                }

                if (verifyClassCode(classCode) == "unsuccessful")
                {
                    return "unsuccessful";
                }

                classes.insert(classCode);
            }

            if ((int)classes.size() != n)
            {
                return "unsuccessful";
            }

            return manager.insert(name, id, residence, classes)
                   ? "successful"
                   : "unsuccessful";
        }

    // remove
    else if (func == "remove")
    {
        int id;
        if (!(in >> id)) return "unsuccessful";

        return manager.remove(id)
               ? "successful"
               : "unsuccessful";
    }

    // drop class
    else if (func == "dropClass")
    {
        int id;
        string classCode;

        if (!(in >> id >> classCode)) return "unsuccessful";

        if (verifyClassCode(classCode) == "unsuccessful")
            return "unsuccessful";

        return manager.dropClass(id, classCode)
               ? "successful"
               : "unsuccessful";
    }

    // replace class
    else if (func == "replaceClass")
    {
        int id;
        string oldCode, newCode;

        if (!(in >> id >> oldCode >> newCode))
            return "unsuccessful";

        if (verifyClassCode(newCode) == "unsuccessful")
            return "unsuccessful";

        return manager.replaceClass(id, oldCode, newCode)
               ? "successful"
               : "unsuccessful";
    }

    // remove class in general
    else if (func == "removeClass")
    {
        string classCode;

        if (!(in >> classCode)) return "unsuccessful";

        if (verifyClassCode(classCode) == "unsuccessful")
            return "unsuccessful";

        int removed = manager.removeClass(classCode);

        return (removed > 0) ? "successful" : "unsuccessful";
    }

    // toggle edges
    else if (func == "toggleEdgesClosure")
    {
        int n;
        if (!(in >> n)) return "unsuccessful";

        vector<int> edges;
        int x;

        while (in >> x)
            edges.push_back(x);

        if ((int)edges.size() != 2 * n)
            return "unsuccessful";

        toggleEdgeClosure(n, edges);
        return "successful";
    }

    // check edge status
    else if (func == "checkEdgeStatus")
    {
        int x, y;
        if (!(in >> x >> y)) return "unsuccessful";

        cout << checkEdgeStatus(x, y) << endl;
        return "successful";
    }

    // is connected
    else if (func == "isConnected")
    {
        int x, y;
        if (!(in >> x >> y)) return "unsuccessful";

        return isConnected(x, y)
               ? "successful"
               : "unsuccessful";
    }

    // print shortest edges
    else if (func == "printShortestEdges")
    {
        int id;
        if (!(in >> id)) return "unsuccessful";

        Student* student = manager.getStudent(id);
        if (!student) return "unsuccessful";

        vector<string> classes(student->getClasses().begin(),
                               student->getClasses().end());

        sort(classes.begin(), classes.end());

        vector<int> distances = shortestEdges(id, classes);

        cout << "Time For Shortest Edges: " << student->getName() << endl;

        for (size_t i = 0; i < classes.size(); i++)
            cout << classes[i] << ": " << distances[i] << endl;

        return "successful";
    }

    // print student zone
    else if (func == "printStudentZone")
    {
        int id;

        if (!(in >> id))
        {
            return "unsuccessful";
        }

        Student* student = manager.getStudent(id);
        if (!student)
        {
            cout << "unsuccessful" << endl;
            return "unsuccessful";
        }

        int cost = studentZone(id);

        cout << "Student Zone Cost: " << cost << endl;

        return "successful";
    }

    // ---------------- INVALID ----------------
    else
    {
        cout << "unsuccessful" << endl;
        return  "unsuccessful";
    }
}


};


