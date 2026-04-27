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

// edge struct for path finding
struct Edge
{
    int neighbor;
    int weight;
    bool isClosed = false;
};

// node struc for path finding
struct Node
{
    int node;
    int dist;
};

// simulates min heap for path finding
// compares node distances
struct Compare
{
    bool operator()(const Node& n1, const Node& n2)
    {
        return n1.dist > n2.dist;
    }
};

// time struct to store class times
struct classTime
{
    int start;
    int end;
};

// compare struct to compare class start times
struct CompareByStart {
    const unordered_map<string, classTime>& classToTime;

    bool operator()(const string& a, const string& b) const {
        return classToTime.at(a).start < classToTime.at(b).start;
    }
};


class CampusCompass {

    unordered_map<int, vector<Edge>> adj; // campus graph
    unordered_map<int, string> locations; // stores locations
    unordered_map<string, int> classToLocation; // maps classes to locations
    unordered_map<string, classTime> classToTime; // maps classes to times
    unordered_set<string> classCodes; // stores class codes
    StudentManager manager;

public:
    CampusCompass() {}

    // return student object
    Student* getStudent(int id)
    {
        return manager.getStudent(id);
    }

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

    // verify if name is valid
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

    // verify if class code is valid
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

    // parses through edges.csv and classes.csv and extracts appropriate data
    bool ParseCSV(const string &edges_filepath, const string &classes_filepath)
    {
        // classes.csv
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
            string classCode, classLocation, startStr, endStr;
            getline(ss, classCode, ',');
            getline(ss, classLocation, ',');
            getline(ss, startStr, ',');
            getline(ss, endStr, ',');

            classCodes.insert(classCode);
            classToLocation[classCode] = stoi(classLocation);

            auto toMinutes = [](const string& t) {
                int h = stoi(t.substr(0, 2));
                int m = stoi(t.substr(3, 2));
                return h * 60 + m;
            };
            classToTime[classCode] = {toMinutes(startStr), toMinutes(endStr)};
        }

        // edges.csv
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

    // finds existing edges
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

    // checks if edge is closed
    bool isEdgeClosed(Edge& edge)
    {
        return edge.isClosed;
    }

    // toggles edge stage
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

    // checks edge stage
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

    // are two locations connected? return true if path is found and false if not
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

    // helper function for Dijkstra's algo
    // used my project 2 (maze runners) as a reference
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

    // returns the shortest distance from a vector of classes
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
            if (classToLocation.find(classCode) == classToLocation.end())
                continue;
            int location = classToLocation[classCode];

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

    // helper function
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

        for (int i = 0; i < static_cast<int>(nodesGraph.size()); i++)
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

    // helper function for studentZone
    int studentZone(int& studentID)
    {
        Student* current = manager.getStudent(studentID);
        if (!current) return 0;

        int residence = current->getResidenceID();
        unordered_set<string> classes = current->getClasses();

        unordered_map<int, int> parent;
        auto distance = Dijkstra(residence, parent);

        const int INF = numeric_limits<int>::max();

        unordered_set<int> nodesGraph;
        nodesGraph.insert(residence);

        for (auto &classCode : classes)
        {
            if (classToLocation.find(classCode) == classToLocation.end())
                continue;

            int now = classToLocation[classCode];

            if (distance[now] == INF)
                continue;

            int target = now;

            while (target != residence &&
                   parent.find(target) != parent.end() &&
                   parent[target] != -1)
            {
                int par = parent[target];

                nodesGraph.insert(target);
                nodesGraph.insert(par);

                target = par;
            }
        }

        unordered_map<int, vector<Edge>> edges;

        for (int u : nodesGraph)
        {
            for (auto &edge : adj[u])
            {
                if (nodesGraph.count(edge.neighbor) && !edge.isClosed)
                {
                    edges[u].push_back(edge);
                }
            }
        }

        return PrimsMST(edges, nodesGraph);
    }

    // extra credit method
    string verifySchedule(int studentID)
    {
        Student* student = manager.getStudent(studentID);
        if (!student)
        {
            cout << "unsuccessful" << endl;
            return "unsuccessful";
        }

        const auto& classes = student->getClasses();

        if (classes.size() < 2)
        {
            cout << "unsuccessful" << endl;
            return "unsuccessful";
        }

        vector<string> sorted(classes.begin(), classes.end());
        sort(sorted.begin(), sorted.end(), CompareByStart{classToTime});

        cout << "Schedule Check for " << student->getName() << ":" << endl;

        for (size_t i = 0; i + 1 < sorted.size(); i++)
        {
            const string& from = sorted[i];
            const string& to   = sorted[i + 1];

            int fromLoc = classToLocation[from];
            int toLoc   = classToLocation[to];

            int gap = classToTime[to].start - classToTime[from].end;

            unordered_map<int, int> parent;
            auto dist = Dijkstra(fromLoc, parent);

            const int INF = numeric_limits<int>::max();
            int pathTime = (dist.count(toLoc) && dist[toLoc] != INF) ? dist[toLoc] : INF;

            if (gap >= pathTime)
                cout << from << " - " << to << ": successful" << endl;
            else
                cout << from << " - " << to << ": unsuccessful" << endl;
        }

        return "successful";
    }

    // function to parse commands
    // i put this function in campuscompass.h so that main.cpp does not directly interact with objects from internal memory
    // referenced project 1 for parsing help

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

            string idStr;
            int id, residence, n;

            if (!(in >> idStr >> residence >> n))
            {
                cout << "unsuccessful" << endl;
                return "unsuccessful";
            }

            if (n <= 0 || n >= 6)
            {
                cout << "unsuccessful" << endl;
                return "unsuccessful";
            }
            if (verifyNum(idStr) == "unsuccessful")
            {
                cout << "unsuccessful" << endl;
                return "unsuccessful";
            }

            id = stoi(idStr);


            if (verifyName(name) == "unsuccessful")
            {
                cout << "unsuccessful" << endl;
                return "unsuccessful";
            }

            unordered_set<string> classes;

            if (classes.size() > 6)
            {
                cout << "unsuccessful" << endl;
                return "unsuccessful";
            }

            string classCode;


            for (int i = 0; i < n; i++)
            {
                if (!(in >> classCode))
                {
                    cout << "unsuccessful" << endl;
                    return "unsuccessful";
                }

                if (verifyClassCode(classCode) == "unsuccessful")
                {
                    cout << "unsuccessful" << endl;
                    return "unsuccessful";
                }

                if (classes.find(classCode) != classes.end())
                {
                    cout << "unsuccessful" << endl;
                    return "unsuccessful";
                }

                classes.insert(classCode);
            }


            if (!(manager.insert(name, id, residence, classes)))
            {
                cout << "unsuccessful" << endl;
                return "unsuccessful";
            }
            else
            {
                cout << "successful" << endl;
                return "successful";

            }
        }

    // remove
    else if (func == "remove")
    {
        int id;
        if (!(in >> id))
        {
            cout << "unsuccessful" << endl;
            return "unsuccessful";
        }

        if ( manager.remove(id))
        {
            cout << "successful" << endl;
            return "successful";
        }
        else
        {
            cout << "unsuccessful" << endl;
            return "unsuccessful";
        }

    }

    // drop class
    else if (func == "dropClass")
    {
        int id;
        string classCode;

        if (!(in >> id >> classCode))
        {
            cout << "unsuccessful" << endl;
            return "unsuccessful";
        }

        if (verifyClassCode(classCode) == "unsuccessful")
        {
            cout << "unsuccessful" << endl;
            return "unsuccessful";
        }

        if ( manager.dropClass(id, classCode))
        {
            cout << "successful" << endl;
            return "successful";
        }
        else
        {
            cout << "unsuccessful" << endl;
            return "unsuccessful";
        }
    }

    // replace class
    else if (func == "replaceClass")
    {
        int id;
        string oldCode, newCode;

        if (!(in >> id >> oldCode >> newCode))
        {
            cout << "unsuccessful" << endl;
            return "unsuccessful";
        }

        if (verifyClassCode(newCode) == "unsuccessful")
        {
            cout << "unsuccessful" << endl;
            return "unsuccessful";
        }

        if ( manager.replaceClass(id, oldCode, newCode))
        {
            cout << "successful" << endl;
            return "successful";
        }
        else
        {
            cout << "unsuccessful" << endl;
            return "unsuccessful";
        }
    }

    // remove class in general
    else if (func == "removeClass")
    {
        string classCode;

        if (!(in >> classCode))
        {
            cout << "unsuccessful" << endl;
            return "unsuccessful";
        }

        if (verifyClassCode(classCode) == "unsuccessful")
        {
            cout << "unsuccessful" << endl;
            return "unsuccessful";
        }

        int removed = manager.removeClass(classCode);

        if (removed != 0)
        {
            cout << removed << endl;
            return "successful";
        }
        else
        {
            cout << "unsuccessful" << endl;
            return "unsuccessful";
        }
    }

    // toggle edges
    else if (func == "toggleEdgesClosure")
    {
        int n;
        if (!(in >> n))
        {
            cout << "unsuccessful" << endl;
            return "unsuccessful";
        }

        vector<int> edges;
        int x;

        while (in >> x)
        {
            edges.push_back(x);
        }

        toggleEdgeClosure(n, edges);
        cout << "successful" << endl;
        return "successful";
    }

    // check edge status
    else if (func == "checkEdgeStatus")
    {
        int x, y;
        if (!(in >> x >> y))
        {
            cout << "unsuccessful" << endl;
            return "unsuccessful";
        }

        string result = checkEdgeStatus(x, y);
        cout << result << endl;
        return result;
    }

    // is connected
    else if (func == "isConnected")
    {
        int x, y;
        if (!(in >> x >> y))
        {
            cout << "unsuccessful" << endl;
            return "unsuccessful";
        }

        if ( isConnected(x, y))
        {
            cout << "successful" << endl;
            return "successful";
        }
        else
        {
            cout << "unsuccessful" << endl;
            return "unsuccessful";
        }

    }

    // print shortest edges
    else if (func == "printShortestEdges")
    {
        int id;
        if (!(in >> id))
        {
            cout << "unsuccessful" << endl;
            return "unsuccessful";
        }

        Student* student = manager.getStudent(id);
        if (!student)
        {
            cout << "unsuccessful" << endl;
            return "unsuccessful";
        }

        vector<string> classes(student->getClasses().begin(), student->getClasses().end());

        sort(classes.begin(), classes.end());

        vector<int> distances = shortestEdges(id, classes);

        cout << "Time For Shortest Edges: " << student->getName() << endl;

        for (size_t i = 0; i < classes.size(); i++)
        {
            cout << classes[i] << ": " << distances[i] << endl;
        }

        return "successful";
    }

    // print student zone
    else if (func == "printStudentZone")
    {
        int id;

        if (!(in >> id))
        {
            cout << "unsuccessful" << endl;
            return "unsuccessful";
        }

        Student* student = manager.getStudent(id);
        if (!student)
        {
            cout << "unsuccessful" << endl;
            return "unsuccessful";
        }

        int cost = studentZone(id);
        cout << "Student Zone Cost For " << student->getName() << ": " << cost << endl;
        return "successful";
    }

    // extra credit function
    else if (func == "verifySchedule")
    {
        int id;
        if (!(in >> id))
        {
            cout << "unsuccessful" << endl;
            return "unsuccessful";
        }
        return verifySchedule(id);
    }

    // invalid commands
    else
    {
        cout << "unsuccessful" << endl;
        return  "unsuccessful";
    }
}

};
