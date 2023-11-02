#ifndef FUNCTION3_H
#define FUNCTION3_H

#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include "../base/base.h"
using namespace std;





void function3() {
    // Create a vector to store group submissions
    vector<GroupSubmission> groupSubmissions;

    // Create a vector of Project structures
    vector<Project> projects;
    ifstream file("./data/Prj.txt");   
    if (file.is_open()) {
        while (!file.eof()) {
            Project project;
            string line;

            // Read project number
            getline(file, line);
            if (line.empty()) {
                break; // Stop if no more projects
            }
            project.number = stoi(line.substr(15)); // Extract number from "Project Number: X"

            // Read description
            getline(file, project.description);

            // Read deadline
            getline(file, project.deadline);
            projects.push_back(project);
        }
        file.close();

    

    int groupNumber, projectNumber;

    cout << "Enter the group number: ";
    cin >> groupNumber;

    cout << "Enter the project number: ";
    cin >> projectNumber;

    string SubmissionDate;
    bool validFormat = false;

    while (!validFormat) {
        cout << "Enter a date (DD/MM/YYYY): ";
        cin >> SubmissionDate;

        validFormat = isValidDateFormat(SubmissionDate);

        if (!validFormat) {
            cout << "Invalid input format. Please use DD/MM/YYYY format." << endl;
        }
    }



    bool found = false;
    for (const Project& project : projects) {
        if (project.number == projectNumber) {
            GroupSubmission submission;
            submission.groupNumber = groupNumber;
            submission.projectnumber = projectNumber;
            submission.submissionDate = SubmissionDate;
            groupSubmissions.push_back(submission);
            found = true;
            break;
        }
    }

    if (!found) {
        cout << "Project with number " << projectNumber << " not found.\n";
    } else {
        cout << "Submission recorded for Group " << groupNumber << " and Project " << projectNumber << ".\n";
    }
}
    ofstream infile("./data/GroupSubmissions.txt", ios::app);

    if (infile.is_open()) {
        for (const GroupSubmission& submission : groupSubmissions) {
            infile << "Group Number: " << submission.groupNumber << "\n";
            infile << "Project Number: " << submission.projectnumber << "\n";
            infile <<  submission.submissionDate << "\n\n";
        }

        file.close();
        cout << "Group submissions have been saved to GroupSubmissions.txt.\n";
    } else {
        cerr << "Unable to open GroupSubmissions.txt for writing.\n";
    }
}


#endif
