#include <iostream>
#include <fstream>
#include "CampusCompass.h"

using namespace std;

int main() {
    // initialize your main project object
    CampusCompass compass;

    // ingest CSV data
    if (!compass.ParseCSV("data/edges.csv", "data/classes.csv"))
    {
        return -1;
    };

    string line; // reading the number of commands
    getline(cin, line);
    int commands = stoi(line);

    while (commands != 0) // while loop that runs as many times as the number of commands
    {

        // reading the function itself
        getline(cin, line);
        if (line.empty())
        {
            continue;
        }
        compass.ParseCommand(line);
        commands--;
    }
}



