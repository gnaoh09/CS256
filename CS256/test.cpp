#include <iostream>
#include <stdlib.h>
#include <vector>
#include <string>
#include <chrono>
#include <fstream>
#include <iomanip>
#include <set>
#include <ctime>
#include <regex> 
#include <sstream>
#include "./base/base.h"
#include "./function/function2.h" 
#include "./function/function3.h" 
#include "./function/function4.h" 

using namespace std;
void setCurrentTime(string& a) {
    // Get the current time
    time_t currentTime = time(nullptr);

    // Convert the current time to a string
    tm* timeinfo = localtime(&currentTime);
    char buffer[11]; // Buffer size for "DD/MM/YYYY\0"
    strftime(buffer, sizeof(buffer), "%d/%m/%Y", timeinfo);
    a = buffer;
}
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
// Create a vector to store the project numbers and submission information
vector<vector<string>> submissionInfo;
void StaticToTable(string userDeadline){
vector<Project> projects; 
DateValidation(userDeadline); 
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

            // Parse the user deadline into a tm structure
            tm userDeadlineTime = {};
            istringstream ss(userDeadline);
            ss >> userDeadlineTime.tm_mday;
            ss.ignore(1); // Skip the '/'
            ss >> userDeadlineTime.tm_mon;
            userDeadlineTime.tm_mon--; // Months are 0-based in struct tm
            ss.ignore(1);
            ss >> userDeadlineTime.tm_year;
            userDeadlineTime.tm_year -= 1900; // Years are relative to 1900 in struct tm

            // Convert the user deadline tm structure to time since epoch
            time_t userDeadlineEpoch = mktime(&userDeadlineTime);

            // Parse the project deadline into a tm structure
            tm projectDeadlineTime = {};
            istringstream ss2(project.deadline);
            ss2 >> projectDeadlineTime.tm_mday;
            ss2.ignore(1); // Skip the '/'
            ss2 >> projectDeadlineTime.tm_mon;
            projectDeadlineTime.tm_mon--; // Months are 0-based in struct tm
            ss2.ignore(1);
            ss2 >> projectDeadlineTime.tm_year;
            projectDeadlineTime.tm_year -= 1900; // Years are relative to 1900 in struct tm

            // Convert the project deadline tm structure to time since epoch
            time_t projectDeadlineEpoch = mktime(&projectDeadlineTime);

            // Only add the project to the vector if it is before the user-specified deadline
            if (userDeadlineEpoch > projectDeadlineEpoch) {
                projects.push_back(project);
            }
        }
    }

    infile.close();
}
    vector<GroupSubmission> groupSubmissions;
    ifstream submissionFile("./data/GroupSubmissions.txt");

    if (submissionFile.is_open()) {
        string line;
        GroupSubmission GS;
        bool newGroup = true;

        while (getline(submissionFile, line)) {
    if (newGroup) {
        if (line.find("Group Number: ") == 0) {
            GS.groupNumber = stoi(line.substr(13)); // Extract number from "Group Number: X"
            newGroup = false;
        } 
    } else {
        if (line.find("Project Number: ") == 0) {
            // Read the project number
            GS.projectnumber = stoi(line.substr(15)); // Extract number from "Project Number: X"
            
            // Read the submission date
            if (getline(submissionFile, GS.submissionDate)) {
                // Successfully read the submission date
                groupSubmissions.push_back(GS);
            } else {
                cerr << "Error reading submission date for Group " << GS.groupNumber << " - Project " << GS.projectnumber << endl;
                break;  
            }
        } else {
            cerr << "Error: Line does not have the expected format for project number." << endl;
        }
        newGroup = true;
    }
}

        submissionFile.close();
    }


for (const GroupSubmission& GS : groupSubmissions) {
    vector<string> row;
    row.push_back("Group " + to_string(GS.groupNumber));

    for (const Project& project : projects) {
        string info = "";

        // Check if the project has been submitted by the group
        bool projectSubmitted = false;
        for (const GroupSubmission& submission : groupSubmissions) {
            if (submission.projectnumber == project.number && submission.groupNumber == GS.groupNumber) {
                projectSubmitted = true;
                break;
            }
        }

        if (projectSubmitted) {
            // Define two date strings in dd/mm format
            string completeDateStr1 = GS.submissionDate;
            string completeDateStr2 = project.deadline;

            // Get the current year to create complete date strings
            time_t now = time(0);
            tm* ltm = localtime(&now);
            int currentYear = 1900 + ltm->tm_year;

            // Parse the complete date strings into tm structures
            tm timeinfo1 = {};
            tm timeinfo2 = {};

            istringstream ss1(completeDateStr1);
            istringstream ss2(completeDateStr2);

            ss1 >> timeinfo1.tm_mday;
            ss1.ignore(1); // Skip the '/'
            ss1 >> timeinfo1.tm_mon;
            timeinfo1.tm_mon--; // Months are 0-based in struct tm
            ss1.ignore(1);
            ss1 >> timeinfo1.tm_year;

            ss2 >> timeinfo2.tm_mday;
            ss2.ignore(1); // Skip the '/'
            ss2 >> timeinfo2.tm_mon;
            timeinfo2.tm_mon--; // Months are 0-based in struct tm
            ss2.ignore(1);
            ss2 >> timeinfo2.tm_year;

            if (ss1.fail() || ss2.fail()) {
                cerr << "Date parsing failed." << endl;
            }

            // Set the year to the current year
            timeinfo1.tm_year = ltm->tm_year;
            timeinfo2.tm_year = ltm->tm_year;

            // Convert tm structures to time since epoch
            time_t time1 = mktime(&timeinfo1);
            time_t time2 = mktime(&timeinfo2);

            if (time1 <= time2) {
                info = "on time";
            } else {
                info = "late";
            }
        } else {
            info = "not submitted yet";
        }

        row.push_back(info);
    }

    submissionInfo.push_back(row);
}

// Create a set to keep track of processed group numbers
set<int> processedGroups;

// Display the header row
cout << setw(10) << "Group/Project";
for (const Project& project : projects) {
    cout << setw(22) << "Project " << project.number;
}
cout << endl;

// Display the submission information for each group
vector<string> submissionRow; // Declare submissionRow outside the if statement
for (const GroupSubmission& GS : groupSubmissions) {
    // Check if the current group has already been processed
    if (processedGroups.count(GS.groupNumber) > 0) {
        continue; // Skip the group if it has already been processed
    }

    cout << setw(10) << "Group " << GS.groupNumber;

    submissionRow.clear(); // Clear submissionRow before populating it for each group

    // Find the submission information for the current group
    for (const vector<string>& row : submissionInfo) {
        if (row[0] == "Group " + to_string(GS.groupNumber)) {
            submissionRow = vector<string>(row.begin() + 1, row.end());
            break;
        }
    }

    // Display the submission state for each project
    for (const string& info : submissionRow) {
        cout << setw(24) << info;
    }
    cout << endl;

    // Add the current group to the set of processed groups
    processedGroups.insert(GS.groupNumber);
}

    ofstream outfile("./data/OverallStatistic.txt");

if (outfile.is_open()) {
    // Create a set to keep track of processed group numbers
    set<int> processedGroups;

    // Write the project information
    outfile << setw(10) << "Group/Project";
    for (const Project& project : projects) {
        outfile << setw(22) << "Project " << project.number;
    }
    outfile << endl;

    // Write the submission information for each group
    for (const GroupSubmission& GS : groupSubmissions) {
        // Check if the current group has already been processed
        if (processedGroups.count(GS.groupNumber) > 0) {
            continue; // Skip the group if it has already been processed
        }

        outfile << setw(10) << "Group " << GS.groupNumber;

        // Find the submission information for the current group
        vector<string> submissionRow;
        for (const vector<string>& row : submissionInfo) {
            if (row[0] == "Group " + to_string(GS.groupNumber)) {
                submissionRow = vector<string>(row.begin() + 1, row.end());
                break;
            }
        }

        // Write the submission state for each project
        for (const string& info : submissionRow) {
            outfile << setw(24) << info;
        }
        outfile << endl;

        // Add the current group to the set of processed groups
        processedGroups.insert(GS.groupNumber);
    }

    outfile.close();
    cout << "Data has been written to OverallStatistic.txt." << endl;
}
    }

void printSubmissionStatus(const vector<vector<string>>& submissionInfo) {
    // Create a set to keep track of processed group numbers
    set<int> processedGroups;

    for (const vector<string>& row : submissionInfo) {
        // Extract the group number from the first element of the row
        int groupNumber = stoi(row[0].substr(6)); // Assuming the format is "Group X"

        // Check if the current group has already been processed
        if (processedGroups.count(groupNumber) > 0) {
            continue; // Skip the group if it has already been processed
        }

        // Initialize counters for different submission statuses
        int notSubmittedCount = 0;
        int lateCount = 0;
        int onTimeCount = 0;

        // Check the submission status for each project
        for (size_t i = 1; i < row.size(); i++) {
            const string& submissionStatus = row[i];

            if (submissionStatus == "not submitted yet") {
                notSubmittedCount++;
            } else if (submissionStatus == "late") {
                lateCount++;
            } else if (submissionStatus == "on time") {
                onTimeCount++;
            }
        }

        // Print the submission status for the group
        cout << "Group " << groupNumber << ":" << endl;
        cout << "  Not Submitted: " << notSubmittedCount << " project(s)" << endl;
        cout << "  Late: " << lateCount << " project(s)" << endl;
        cout << "  On Time: " << onTimeCount << " project(s)" << endl;
        cout << endl;

        // Add the current group to the set of processed groups
        processedGroups.insert(groupNumber);
    }
}
int main(){
    string b;
    b = "31/12/2023";
    StaticToTable(b);
    printSubmissionStatus(submissionInfo);
    
}