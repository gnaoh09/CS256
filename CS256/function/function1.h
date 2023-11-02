#ifndef FUNCTION1_H
#define FUNCTION1_H

#include <iostream>
#include <fstream>
#include <sstream>
#include <limits>
#include <vector>
#include <chrono>
#include <ctime>
#include <iomanip>
#include "requiredSubmitFunc.h"
using namespace std;
// datatype
struct Member
{
    string groupNumber;
    string name;
    string studentId;
};

vector<Member> groupData;
int numberOfGroups, peopleInAGroup;
string groupNum;
string trimString(string &str)
{
    string whiteSpaces = " \n\r\t\f\v"; // all types of possible spaces to remove
    // Remove leading whitespace
    size_t first_non_space = str.find_first_not_of(whiteSpaces);
    str.erase(0, first_non_space);

    // Remove trailing whitespace
    size_t last_non_space = str.find_last_not_of(whiteSpaces);
    str.erase(last_non_space + 1);
    return str;
}
bool checkExistStudent(vector<Member> &groupData, string id, int size)
{
    bool check = false;
    for (size_t i = 0; i < size; i++)
    {
        if (groupData[i].studentId == id)
        {
            cout << "Student already exist!";
            check = true;
            break;
        }
    }
    return check;
}
bool existGroup(string groupNum, vector<Member> &groupData)
{
    bool check = false;
    for (size_t i = 0; i < groupData.size(); i++)
    {
        if (groupData[i].groupNumber == groupNum)
        {
            cout << "Group already exist!";
            check = true;
            break;
        }
    }
    return check;
}
void createMaxLength(string namelength, int mark, vector<int> &maxLength, vector<string> &headers)
{
    int currentLength = (int)namelength.length();
    if (currentLength >= maxLength[mark] && currentLength >= headers[mark].length())
    {
        maxLength[mark] = currentLength;
    }
    if (headers[mark].length() >= maxLength[mark])
    {
        maxLength[mark] = headers[mark].length();
    }
}
void selectionSort(vector<Member> &groupData, int size)
{
    int startScan, minIndex, minValue;

    for (startScan = 0; startScan < (size - 1); startScan++)
    {
        minIndex = startScan;
        minValue = stoi(groupData[startScan].groupNumber);

        for (int index = startScan + 1; index < size; index++)
        {
            if (stoi(groupData[index].groupNumber) < minValue)
            {
                minValue = stoi(groupData[index].groupNumber);
                minIndex = index;
            }
        }
        Member temp = groupData[minIndex];
        groupData[minIndex] = groupData[startScan];
        groupData[startScan] = temp;
    }
}

void addStudent()
{
    ifstream inputFile("./data/group.txt"); // read file
    if (!inputFile)
    {
        cerr << "Failed to open the file." << endl;
    }
    inputFile.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); // ignore the first line
    string line;
    while (getline(inputFile, line))
    {
        istringstream iss(line);
        char delimiter = ',';
        Member member;
        getline(iss, member.groupNumber, delimiter);

        getline(iss, member.name, delimiter);

        getline(iss, member.studentId);

        groupData.push_back(member);
    }
    inputFile.close();
    Member member;
    bool repeat;
    do
    {
        repeat = false;
        do
        {
            cout << "Enter student ID: ";
            cin >> member.studentId;
            trimString(member.studentId);                                                   // trim spaces
        } while (checkExistStudent(groupData, member.studentId, groupData.size()) == true); // check Id of exist student if matched enter id again

        cin.sync(); // delete key buffer character
        do
        {
            cout << "Input the group number: ";
            cin >> member.groupNumber;
            trimString(member.groupNumber);
        } while (stoi(member.groupNumber) < 1);
        cin.sync(); // delete key buffer character
        cout << "Enter student name: ";
        getline(cin, member.name);
        trimString(member.name);
        groupData.push_back(member);
        cin.sync(); // delete key buffer character
        char decision;
        cout << "Continue input Y or N:";
        cin >> decision;
        if (tolower(decision) == 'y')
        {
            repeat = true;
        }

    } while (repeat);
    selectionSort(groupData, groupData.size()); // sort same group
    // Wrtie to file
    string fileName;
    ofstream writeFile("./data/group.txt", ios::out);
    if (!writeFile)
    {
        cerr << "Error writing to file: " << endl;
    }
    writeFile << "Group,Full name,Student Id" << endl;
    for (size_t i = 0; i < groupData.size(); i++)
    {
        writeFile << groupData[i].groupNumber << "," + groupData[i].name + "," + groupData[i].studentId << endl;
        writeFile.flush();
    }
    writeFile.close();

    
    groupData.clear();
}

void displayAll()
{
    vector<string> headers = {"Group", "Full Name", "Student Id"};
    vector<int> maxLength(3, 0); // for good looking spacing purposes
    char delimiter = ',';
    string line;
    ifstream inputFile("./data/group.txt"); // read file
    if (!inputFile)
    {
        cerr << "Failed to open the file." << endl;
    }
    inputFile.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); // ignore the first line
    while (getline(inputFile, line))
    {
        istringstream iss(line);
        Member member;
        if (getline(iss, member.groupNumber, delimiter))
        {
            int mark = 0; // mark value for groupNumber to use in createMaxLength func
            createMaxLength(member.groupNumber, mark, maxLength, headers);
        }
        if (getline(iss, member.name, delimiter))
        {
            int mark = 1; // mark value for groupName to use in createMaxLength func

            createMaxLength(member.name, mark, maxLength, headers);
        }
        if (getline(iss, member.studentId))
        {
            int mark = 2; // mark value for student Id to use in createMaxLength func
            createMaxLength(member.studentId, mark, maxLength, headers);
        }
        groupData.push_back(member);
    }
    inputFile.close();
    // display
    for (size_t x = 0; x < headers.size(); x++)
    {
        string spacing(abs(maxLength[x] - (int)headers[x].length()), ' '); // calculate the number of space
        cout << " | " << headers[x] + spacing;
    }
    cout << " |\n";
    for (size_t index = 0; index < groupData.size(); index++)
    {
        string currentGroupSpace(abs(maxLength[0] - (int)groupData[index].groupNumber.length()), ' ');
        string currentnameSpace(abs(maxLength[1] - (int)groupData[index].name.length()), ' ');    // calculate the number of space between the longest string name and the current string name
        string currentIdSpace(abs(maxLength[2] - (int)groupData[index].studentId.length()), ' '); // calculate the number of space between the longest string name and the current string name
        cout << " | " << groupData[index].groupNumber + currentGroupSpace
             << " | " + groupData[index].name + currentnameSpace + " | " + groupData[index].studentId + currentIdSpace + " |\n";
    };
    groupData.clear();

}

void groupDisplay(){
    vector<string> headers = {"Group", "Full Name", "Student Id"};
    vector<int> maxLength(3, 0); // for good looking spacing purposes
    char delimiter = ',';
    std::string line;
    ifstream inputFile("./data/group.txt"); // read file
    if (!inputFile)
    {
        cerr << "Failed to open the file." << endl;
    }
    inputFile.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); // ignore the first line
    while (getline(inputFile, line))
    {
        istringstream iss(line);
        Member member;
        if (getline(iss, member.groupNumber, delimiter))
        {
            int mark = 0;
            createMaxLength(member.groupNumber, mark, maxLength, headers);
        }
        if (getline(iss, member.name, delimiter))
        {
            int mark = 1;

            createMaxLength(member.name, mark, maxLength, headers);
        }
        if (getline(iss, member.studentId))
        {
            int mark = 2;
            createMaxLength(member.studentId, mark, maxLength, headers);
        }
        groupData.push_back(member);
    }
    inputFile.close();
    string groupName;
    cout << "Enter which group to display: ";
    cin >> groupName;
    // display
    for (size_t x = 0; x < headers.size(); x++)
    {
        string spacing(abs(maxLength[x] - (int)headers[x].length()), ' ');
        cout << " | " << headers[x] + spacing;
    }
    cout << " |\n";
    bool found = false;
    for (size_t index = 0; index < groupData.size(); index++)
    {
        if (groupName == groupData[index].groupNumber)
        {
            string currentGroupSpace(abs(maxLength[0] - (int)groupData[index].groupNumber.length()), ' ');
            string currentnameSpace(abs(maxLength[1] - (int)groupData[index].name.length()), ' ');    // calculate the number of space between the longest string name and the current string name
            string currentIdSpace(abs(maxLength[2] - (int)groupData[index].studentId.length()), ' '); // calculate the number of space between the longest string name and the current string name
            cout << " | " << groupData[index].groupNumber + currentGroupSpace
                 << " | " + groupData[index].name + currentnameSpace + " | " + groupData[index].studentId + currentIdSpace + " |\n";
            found = true;
        };
    }
    if (found == false)
    {
        cout << "Group does not exist! ";
    }
    groupData.clear();
   
}


void createGroupFile(){
    cout << "Input the number of groups: ";
    cin >> numberOfGroups;
    for (int i = 0; i < numberOfGroups; i++)
    {
        do
        {
            cout << "Input the group number: ";
            cin >> groupNum;
        } while (stoi(groupNum) < 1 || existGroup(groupNum, groupData));
        do
        {
            cout << "Input the number of people in this group: ";
            cin >> peopleInAGroup;
        } while (peopleInAGroup < 1);

        for (size_t i = 0; i < peopleInAGroup; i++)
        {
            Member member;
            cout << "Enter student ID #" << i + 1 << " : ";
            cin >> member.studentId;
            trimString(member.studentId);                                                 // trim spaces
            if (checkExistStudent(groupData, member.studentId, groupData.size()) == true) // check Id of exist student if matched enter id again
            {
                cout << endl;
                continue;
            }
            member.groupNumber = groupNum;
            cin.sync(); // delete key buffer character
            trimString(member.groupNumber);
            cout << "Enter your name #" << i + 1 << " : ";
            getline(cin, member.name);
            trimString(member.name);
            groupData.push_back(member);
        }
    }
    selectionSort(groupData, groupData.size()); // sort same group
    // Wrtie to file
    string fileName;
    cout << "Name your output file: ";
    cin >> fileName;
    ofstream writeFile(fileName, ios::out);
    if (!writeFile)
    {
        cerr << "Error writing to file: " << endl;
    }
    writeFile << "Group,Full name,Student Id" << endl;
    for (size_t i = 0; i < groupData.size(); i++)
    {
        writeFile << groupData[i].groupNumber << "," + groupData[i].name + "," + groupData[i].studentId << endl;
        writeFile.flush();
    }
    writeFile.close();
    groupData.clear();
}

void function1(){
    int a;
    cout << "choose 1 2 3 4" << endl;
    cin >> a;
    switch (a)
    {
    case 1:
        addStudent();
        break;
    case 2:
        groupDisplay();
        break;
    case 3:
        createGroupFile();
        break;
    case 4:
        displayAll();
        break;  
    }
}

#endif