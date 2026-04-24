#include <iostream>
#include <fstream>
#include "CampusCompass.h"

using namespace std;

int main() {
    // initialize main project object
    CampusCompass compass;

    // break if CSV data does not exist
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
        // parse command is a function within campuscompass.h that handles command processing
        compass.ParseCommand(line);
        commands--;
    }
}