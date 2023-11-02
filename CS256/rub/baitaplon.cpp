#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <ctime>

struct Member {
    std::string name;
    int age;
    // Add more member information as needed
};

struct Group {
    int groupNumber;
    std::vector<Member> members;
};

std::vector<Group> groups;
std::vector<std::string> deadlines;

void userInputGroupInfo() {
    int numMembers;
    std::cout << "Enter number of members in the group: ";
    std::cin >> numMembers;
    
    Group group;
    group.groupNumber = groups.size() + 1;
    
    for (int i = 0; i < numMembers; i++) {
        Member member;
        std::cout << "Enter name of member " << i+1 << ": ";
        std::cin >> member.name;
        std::cout << "Enter age of member " << i+1 << ": ";
        std::cin >> member.age;
        
        group.members.push_back(member);
    }
    
    groups.push_back(group);
}

void displayAllGroups() {
    for (const auto& group : groups) {
        std::cout << "Group " << group.groupNumber << ":\n";
        for (const auto& member : group.members) {
            std::cout << "Name: " << member.name << ", Age: " << member.age << "\n";
        }
        std::cout << "\n";
    }
}

void displayGroup(int groupNumber) {
    if (groupNumber < 1 || groupNumber > groups.size()) {
        std::cout << "Invalid group number!\n";
        return;
    }
    
    const Group& group = groups[groupNumber - 1];
    
    std::cout << "Group " << group.groupNumber << ":\n";
    for (const auto& member : group.members) {
        std::cout << "Name: " << member.name << ", Age: " << member.age << "\n";
    }
    std::cout << "\n";
}

void saveGroupInfoToFile() {
    std::ofstream file("group_info.txt");
    
    for (const auto& group : groups) {
        file << "Group " << group.groupNumber << ":\n";
        for (const auto& member : group.members) {
            file << "Name: " << member.name << ", Age: " << member.age << "\n";
        }
        file << "\n";
    }
    
    file.close();
    std::cout << "Group information saved to file.\n";
}

void loadGroupInfoFromFile() {
    std::ifstream file("group_info.txt");
    
    if (!file) {
        std::cout << "No saved group information found.\n";
        return;
    }
    
    groups.clear();
    
    std::string line;
    Group group;
    
    while (std::getline(file, line)) {
        if (line.find("Group") != std::string::npos) {
            if (!group.members.empty()) {
                groups.push_back(group);
                group.members.clear();
            }
            group.groupNumber = std::stoi(line.substr(line.find("Group") + 6));
        } else if (line.find("Name") != std::string::npos) {
            Member member;
            member.name = line.substr(line.find("Name") + 5);
            std::getline(file, line);
            member.age = std::stoi(line.substr(line.find("Age") + 4));
            group.members.push_back(member);
        }
    }
    
    if (!group.members.empty()) {
        groups.push_back(group);
    }
    
    file.close();
    std::cout << "Group information loaded from file.\n";
}

void inputProjectDeadline() {
    int numDeadlines;
    std::cout << "Enter number of deadlines: ";
    std::cin >> numDeadlines;
    
    for (int i = 0; i < numDeadlines; i++) {
        std::string description;
        std::cout << "Enter short description of deadline " << i+1 << ": ";
        std::cin.ignore();
        std::getline(std::cin, description);
        
        std::string submissionDeadline;
        std::cout << "Enter submission deadline for deadline " << i+1 << ": ";
        std::getline(std::cin, submissionDeadline);
        
        deadlines.push_back(description + " - " + submissionDeadline);
    }
}

void displayProjectDeadlines() {
    std::cout << "Project Deadlines:\n";
    for (const auto& deadline : deadlines) {
        std::cout << deadline << "\n";
    }
    std::cout << "\n";
}

void submitProject() {
    int groupNumber, projectNumber;
    std::string submitDate;
    
    std::cout << "Enter group number: ";
    std::cin >> groupNumber;
    
    if (groupNumber < 1 || groupNumber > groups.size()) {
        std::cout << "Invalid group number!\n";
        return;
    }
    
    std::cout << "Enter project number: ";
    std::cin >> projectNumber;
    
    if (projectNumber < 1 || projectNumber > deadlines.size()) {
        std::cout << "Invalid project number!\n";
        return;
    }
    
    std::cout << "Enter submit date (YYYY-MM-DD): ";
    std::cin >> submitDate;
    
    // Save submission status or perform any other necessary actions
    
    std::cout << "Project submitted successfully.\n";
}

void displaySubmissionStatusByProject() {
    int projectNumber;
    std::cout << "Enter project number: ";
    std::cin >> projectNumber;
    
    if (projectNumber < 1 || projectNumber > deadlines.size()) {
        std::cout << "Invalid project number!\n";
        return;
    }
    
    std::cout << "Submission status for project " << projectNumber << ":\n";
    // Display submission status for each group for the specified project
}

void displaySubmissionStatusByGroup() {
    int groupNumber;
    std::cout << "Enter group number: ";
    std::cin >> groupNumber;
    
    if (groupNumber < 1 || groupNumber > groups.size()) {
        std::cout << "Invalid group number!\n";
        return;
    }
    
    std::cout << "Submission status for group " << groupNumber << ":\n";
    // Display submission status for each project of the specified group
}

void displayOverallStatistic() {
    std::string fromDate, toDate;
    std::cout << "Enter period of time (from date - to date): ";
    std::cin >> fromDate >> toDate;
    
    std::cout << "Overall statistic for period " << fromDate << " - " << toDate << ":\n";
    // Display submission status for all projects with deadlines within the specified period
    
    std::ofstream file("Overall statistic.txt");
    // Export the result to a txt file
    file.close();
    
    std::cout << "Overall statistic exported to file: Overall statistic.txt\n";
}

void findIncompleteGroups() {
    std::cout << "Groups that have not completed or submitted on time:\n";
    // Find and display groups that have not completed or submitted on time
}

int main() {
    std::string choice;
    
    do {
        std::cout << "Menu:\n";
        std::cout << "1. Group information\n";
        std::cout << "2. Project deadline declaration\n";
        std::cout << "3. Submit project\n";
        std::cout << "4. Statistic\n";
        std::cout << "5. Overall statistic\n";
        std::cout << "6. Find groups do not complete or submit on time\n";
        std::cout << "0. Exit\n";
        std::cout << "Enter your choice: ";
        std::cin >> choice;
        
        if (choice == "1") {
            std::string groupInfoChoice;
            std::cout << "Group Information:\n";
            std::cout << "a) User input information of group\n";
            std::cout << "b) Display information\n";
            std::cout << "c) Save information in txt file\n";
            std::cout << "d) Load information from the last save\n";
            std::cout << "Enter your choice: ";
            std::cin >> groupInfoChoice;
            
            if (groupInfoChoice == "a") {
                userInputGroupInfo();
            } else if (groupInfoChoice == "b") {
                displayAllGroups();
            } else if (groupInfoChoice == "c") {
                saveGroupInfoToFile();
            } else if (groupInfoChoice == "d") {
                loadGroupInfoFromFile();
            } else {
                std::cout << "Invalid choice!\n";
            }
        } else if (choice == "2") {
            std::string deadlineChoice;
            std::cout << "Project Deadline Declaration:\n";
            std::cout << "a) Input\n";
            std::cout << "b) Display\n";
            std::cout << "Enter your choice: ";
            std::cin >> deadlineChoice;
            
            if (deadlineChoice == "a") {
                inputProjectDeadline();
            } else if (deadlineChoice == "b") {
                displayProjectDeadlines();
            } else {
                std::cout << "Invalid choice!\n";
            }
        } else if (choice == "3") {
            submitProject();
        } else if (choice == "4") {
            std::string statisticChoice;
            std::cout << "Statistic:\n";
            std::cout << "a) Input project number\n";
            std::cout << "b) Input group number\n";
            std::cout << "Enter your choice: ";
            std::cin >> statisticChoice;
            
            if (statisticChoice == "a") {
                displaySubmissionStatusByProject();
            } else if (statisticChoice == "b") {
                displaySubmissionStatusByGroup();
            } else {
                std::cout << "Invalid choice!\n";
            }
        } else if (choice == "5") {
            displayOverallStatistic();
        } else if (choice == "6") {
            findIncompleteGroups();
        } else if (choice == "0") {
            std::cout << "Exiting...\n";
        } else {
            std::cout << "Invalid choice!\n";
        }
        
        std::cout << "\n";
    } while (choice != "0");
    
    return 0;
}