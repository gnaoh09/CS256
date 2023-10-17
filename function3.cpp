#include <iostream>
#include <vector>
#include <string>
#include <fstream>
using namespace std;
struct Project {
    int number;
    int projectNumber;
    string description;
    string deadline;
};

struct GroupSubmission {
    int groupNumber;
    int projectNumber;
    string submissionDate;
};
// Create a vector to store group submissions
    vector<GroupSubmission> groupSubmissions;
int main() {
    using namespace std;

    // Create a vector of Project structures
    vector<Project> projects;
    ifstream file("Prj.txt");
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
    string submissionDate;

    cout << "Enter the group number: ";
    cin >> groupNumber;

    cout << "Enter the project number: ";
    cin >> projectNumber;

    cout << "Enter the submission date (dd-mm-yyyy): ";
    cin >> submissionDate;

    bool found = false;
    for (const Project& project : projects) {
        if (project.number == projectNumber) {
            GroupSubmission submission;
            submission.groupNumber = groupNumber;
            submission.projectNumber = projectNumber;
            submission.submissionDate = submissionDate;
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
    ofstream infile("GroupSubmissions.txt");

    if (infile.is_open()) {
        for (const GroupSubmission& submission : groupSubmissions) {
            infile << "Group Number: " << submission.groupNumber << "\n";
            infile << "Project Number: " << submission.projectNumber << "\n";
            infile << "Submission Date: " << submission.submissionDate << "\n\n";
        }

        file.close();
        cout << "Group submissions have been saved to GroupSubmissions.txt.\n";
    } else {
        cerr << "Unable to open GroupSubmissions.txt for writing.\n";
    }
}
