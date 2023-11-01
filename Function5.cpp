#include <iostream>
#include <stdlib.h>
#include <vector>
#include <string>
#include <chrono>
#include <iomanip>
#include <ctime>
#include <regex> 
#include <sstream>
#include "function2.h"
#include "function3.h"
using namespace std;
bool isValidDateFormat(const string& input) {
    // Regular expression pattern for DD/MM/YYYY format
    regex pattern(R"(^(0[1-9]|1\d|2\d|3[01])/(0[1-9]|1[0-2])/(\d{4})$)");

    // Check if the input matches the pattern
    return regex_match(input, pattern);
}
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
    ifstream infile("Prj.txt");
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
void function5(string userDeadline){
vector<Project> projects; 
DateValidation(userDeadline); 
ifstream infile("Prj.txt");
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
    ifstream submissionFile("GroupSubmissions.txt");

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

// Create a vector to store the project numbers and submission information
vector<vector<string>> submissionInfo;
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

// Display the header row
cout << setw(10) << "Group/Project";
for (const Project& project : projects) {
    cout << setw(22) << "Project " << project.number;
}
cout << endl;

// Display the submission information for each group
for (const GroupSubmission& GS : groupSubmissions) {
    cout << setw(10) << "Group " << GS.groupNumber;

    // Find the submission information for the current group
    vector<string> submissionRow;
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
}
}
int main(){
    string userDeadline,b;
    int a;
    cout <<"1: from begin to specificdate" << endl << " 2: from begin up till now" << endl;
    cin >> a;

    if(a ==1){
    string userDeadline;
    bool validFormat = false;

    while (!validFormat) {
        cout << "These projects which have deadline before inputted date will be displyed (DD/MM/YYYY): ";
        cin >> userDeadline;

        validFormat = isValidDateFormat(userDeadline);

        if (!validFormat) {
            cout << "Invalid input format. Please use DD/MM/YYYY format." << endl;
        }
    }

        function5(userDeadline);
    }
    
    if(a ==2){
        setCurrentTime(b);
        function5(b);
    }
    
    
}