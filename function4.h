#include <iostream>
#include <vector>
#include <string>
#include <chrono>
#include <iomanip>
#include <ctime> 
#include <sstream>
#include "function2.h"
#include "function3.h"
using namespace std;

void timeCmp(string a , string b){
    // Define two date strings in dd/mm format
    string completeDateStr1 = a;
    string completeDateStr2 = b;

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

    ss2 >> timeinfo2.tm_mday;
    ss2.ignore(1); // Skip the '/'
    ss2 >> timeinfo2.tm_mon;
    timeinfo2.tm_mon--; // Months are 0-based in struct tm
    

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
            cout << "=> This project has been submitted on time"<< endl;
    } 
    else if (time1 > time2) {
            cout << "=> This group has been submitted late" << endl;
        } 
}


void functoin4_1(){
    int a;
    cout << "Enter the Project Number to search: ";
    cin >> a;
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

            if (project.number == a) {
                projects.push_back(project); // Only add the matching project to the vector
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


    for (const Project& project : projects) {

        cout << "Project Details for Project " << project.number << ":\n";
        cout << project.description << endl;
        cout << project.deadline << endl;
        cout << "========================"<<endl;
        for (const GroupSubmission& GS : groupSubmissions) {
        for (const Project& project : projects) {
            if ((project.number == a) && (GS.projectnumber == a)) {
                cout << "Group: " << GS.groupNumber << " - Project " << project.number << ": "<<endl;
                cout <<"Submission Date: "<< GS.submissionDate<<endl;
                cout << "Project Deadline: " << project.deadline<<endl;
                timeCmp(GS.submissionDate,project.deadline);

        cout << "--------------------------------------" <<endl;
            }
        }
        }
    }
}


void function4_2(){
        // Open the text file for reading
    ifstream file("GroupSubmissions.txt");
    if (!file.is_open()) {
        cerr << "Failed to open the file." << endl;
    }
    //Input group number
    int search_term;
    cout << "Enter the Group Number to search: ";
    cin >> search_term;
    cout << "Projects Group " << search_term << " has submitted: " << endl;
    cout << "=====================================" << endl;
    
    //Read 2 txt file to get data for 2 vector
    vector<GroupSubmission> submissions;
    GroupSubmission GS;
    string line;
    while (getline(file, line)) {
        if (line.find("Group Number: ") != string::npos) {
            if (GS.groupNumber != 0) {
                submissions.push_back(GS);
            }
            GS.groupNumber = stoi(line.substr(13)); // Extract group number
        } else if (line.find("Project Number: ") != string::npos) {
            GS.projectnumber = stoi(line.substr(15)); // Extract project number
        } else if (!line.empty()) {
            GS.submissionDate = line;
        }
    }
        submissions.push_back(GS);
    file.close();

    vector<Project> projects;   
    Project project; 
    ifstream infile("Prj.txt");
    if (infile.is_open()) {
    while (getline(infile, line)) {
        Project project;
        size_t pos = line.find("Project Number: ");
        if (pos != string::npos) {
            project.number = stoi(line.substr(pos + 15)); // Extract number from "Project Number: X"
            // Read description
            getline(infile, project.description);

            // Read deadline
            getline(infile, project.deadline);
            projects.push_back(project); // Only add the matching project to the vector
        }
    }
    infile.close();
    }
    
    //Display submitted prj 
    bool found = false;
    for (const GroupSubmission& submission : submissions) {
    if (submission.groupNumber == search_term) {
        bool projectFound = false;
        for (const Project& project : projects) {
            if (submission.projectnumber == project.number) {
                cout << "Project Number: " << submission.projectnumber << endl;
                cout << project.description << endl;
                cout << "Submission Date: " << submission.submissionDate << endl;
                cout << "Deadline: " << project.deadline << endl;
                timeCmp(submission.submissionDate, project.deadline);
                cout << "-------------------------------------" << endl;
                found = true;
                projectFound = true;
            }
        }
        if (!projectFound) {
            cout << "Project Number: " << submission.projectnumber << endl;
            cout << "Not submitted yet!" << endl;
            cout << "-------------------------------------" << endl;
        }
    }
    }
    // Display other projects that the group has not submitted yet
    if (found) {
    for (const Project& project : projects) {
        bool projectSubmitted = false;
        for (const GroupSubmission& submission : submissions) {
            if (submission.groupNumber == search_term && submission.projectnumber == project.number) {
                projectSubmitted = true;
                break;
            }
        }
        if (!projectSubmitted) {
            cout << "Project Number: " << project.number << endl;
            cout << project.description << endl;
            cout << "Deadline: " << project.deadline << endl;
            cout << "Not submitted yet!" << endl;
            cout << "-------------------------------------" << endl;
        }
    }
}

// Not found
if (!found) {
    cout << "This group has not submitted any project!" << endl;
}

}

