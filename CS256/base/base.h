#ifndef BASE_H
#define BASE_H

#include <iostream>
#include <stdlib.h>
#include <vector>
#include <string>
#include <chrono>
#include <iomanip>
#include <ctime>
#include <regex> 
#include <sstream>
using namespace std;


bool isValidDateFormat(const string& input) {
    // Regular expression pattern for DD/MM/YYYY format
    regex pattern(R"(^(0[1-9]|1\d|2\d|3[01])/(0[1-9]|1[0-2])/(\d{4})$)");

    // Check if the input matches the pattern
    return regex_match(input, pattern);
}

struct Project {
    int number;
    int projectNumber;
    string description;
    string deadline;
};

struct GroupSubmission {
    int groupNumber;
    int projectnumber;
    string submissionDate;
};

void DateValidation(string input) {
    vector<Project> projects;
    ifstream infile("./data/Prj.txt");
    if (infile.is_open()) {
        string line;
        while (getline(infile, line)) {
            Project project;

            size_t pos = line.find("Project Number: ");
            if (pos != string::npos) {
                project.number = stoi(line.substr(pos + 15)); // Extract number from "Project Number: X"

                // Read description
                getline(infile, project.description);

                // Read deadline
                getline(infile, project.deadline);

                projects.push_back(project);
            }
        }

        infile.close();
    }

    // Find the project with the earliest deadline
    Project earliestDeadlineProject;
    bool isFirstProject = true;
    time_t earliestDeadlineEpoch = 0;

    for (const Project& project : projects) {
        // Parse the deadline into a tm structure
        tm deadlineTime = {};
        istringstream ss1(project.deadline);
        ss1 >> deadlineTime.tm_mday;
        ss1.ignore(1); // Skip the '/'
        ss1 >> deadlineTime.tm_mon;
        deadlineTime.tm_mon--; // Months are 0-based in struct tm
        ss1.ignore(1);
        ss1 >> deadlineTime.tm_year;
        deadlineTime.tm_year -= 1900; // Years are relative to 1900 in struct tm

        // Convert the deadline tm structure to time since epoch
        time_t deadlineEpoch1 = mktime(&deadlineTime);


        tm deadlineInput = {};
        istringstream ss(input);
        ss >> deadlineInput.tm_mday;
        ss.ignore(1); // Skip the '/'
        ss >> deadlineInput.tm_mon;
        deadlineInput.tm_mon--; // Months are 0-based in struct tm
        ss.ignore(1);
        ss >> deadlineInput.tm_year;
        deadlineInput.tm_year -= 1900; // Years are relative to 1900 in struct tm

        // Convert the deadline tm structure to time since epoch
        time_t deadlineEpoch2 = mktime(&deadlineInput);
        
        
        if (isFirstProject || deadlineEpoch1  < earliestDeadlineEpoch) {
        earliestDeadlineEpoch = deadlineEpoch1;
        earliestDeadlineProject = project;
        isFirstProject = false;
        }


        if (!isFirstProject && earliestDeadlineEpoch > deadlineEpoch2){
            cout << "There are no any project have deadline before " << input;
            exit(0);
        }
    }
}
#endif