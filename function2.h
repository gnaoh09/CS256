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



void CreateNew(){
    vector<Project> projectVector;

    int numProjects;
    cout << "Enter the number of projects: ";
    cin >> numProjects;

    cin.ignore(); // Consume the newline character left in the input buffer

    for (int i = 0; i < numProjects; i++) {
        Project project;

        do {
            cout << "Project " << i + 1 << " - Enter project number: ";
            if (cin >> project.projectNumber) {
                // Successful integer input
                break; // Exit the loop
            } else {
                cin.clear(); // Clear the error flag
                cin.ignore(numeric_limits<streamsize>::max(), '\n'); // Clear the input buffer
                cout << "Invalid input. Please enter a valid integer for project number.\n";
            }
        } while (true);
        cin.ignore(); // Consume the newline character left in the input buffer

        cout << "Project " << i + 1 << " - Enter project description: ";
        getline(cin, project.description);

        bool validFormat = false;
    while (!validFormat) {
        cout << "Project " << i + 1 << " - Enter project deadline (DD/MM/YYYY): ";
        getline(cin, project.deadline);
        validFormat = isValidDateFormat(project.deadline);
        if (!validFormat) {
            cout << "Invalid input format. Please use DD/MM/YYYY format." << endl;
        }
    }

        
        projectVector.push_back(project);
    }


    // Write project information to "Prj.txt"
    ofstream file("Prj.txt");
    if (file.is_open()) {
        for (const Project& project : projectVector) {
            file << "Project Number: " << project.projectNumber << "\n";
            file << "Description: " << project.description << "\n";
            file << "Deadline: " << project.deadline << "\n\n";
        }
        file.close();
        cout << "Project information has been saved to Prj.txt.\n";
    } else {
        cerr << "Unable to open Prj.txt for writing.\n";
    }
}


void Display(){
    fstream newfile;
    newfile.open("Prj.txt",ios::in);
   if (newfile.is_open()){
      string tp;
      while(getline(newfile, tp)){ 
         cout << tp << "\n"; 
      }
      newfile.close();  
   }
}


void function2() {
    int a;
    cout << "Nhap lua chon: ";
    cin >> a;
    switch(a) {
  case 1:
    CreateNew();
    break;
  case 2:
    Display();
    break;

    }   
}
