#ifndef REQUIREDSUBMITFUNC_H
#define REQUIREDSUBMITFUNC_H

#include <iostream>
#include <fstream>
#include <sstream>
#include <limits>
#include <vector>
#include <cstdio>
using namespace std;
void writeSubmitStatus(int numberOfPrj, int numberOfGroup, vector<vector<string>> &twoDVector) // function to write submit data
{
    ofstream write("SubmissionStatus.txt", ios::out);
    if (!write)
    {
        cerr << "Error write file: " << endl;
    }
    write << "Groups/Projects,";
    for (int i = 0; i < numberOfPrj; i++)
    {
        write << "Project " << i + 1;
        if (i < numberOfPrj - 1)
        {
            write << ",";
        }
    }
    write << endl;
    for (int i = 0; i < numberOfGroup; i++)
    {
        write << "Group " << i + 1 << ",";
        for (int j = 0; j < numberOfPrj; j++)
        {
            write << twoDVector[i][j];
            if (j < numberOfPrj - 1)
            {
                write << ",";
            }
        }
        write << endl;
    }
    write.close();
}
int get_Num_Of_Prj()
{
    int numberOfPrj = 0; // this equal to number of columns of vector

    string prjLine;
    ifstream readFile("Prj.txt"); // read the prj file to get number of prj
    if (!readFile)
    {
        cerr << "Error read file: " << endl;
    }
    while (getline(readFile, prjLine))
    {
        if (prjLine.find("Project Number:") != string::npos) // find the line that contain "Project Number:""
        {
            numberOfPrj++; // add one to count how many prj exists
        }
    }
    readFile.close();
    return numberOfPrj;
}
int get_Num_Of_Grp()
{
    int numberOfGroup = 0; // this equal to number of rows of vector
    string grpLine;
    ifstream readGrpFile("group.txt"); // read the grp file to get number of grp
    if (!readGrpFile)
    {
        cerr << "Error read file: " << endl;
    }
    readGrpFile.ignore(numeric_limits<streamsize>::max(), '\n'); // ignore the first line when read file

    while (getline(readGrpFile, grpLine))
    {
        numberOfGroup = grpLine[0] - '0'; // convert read char to int
    }
    readGrpFile.close();
    return numberOfGroup;
}
void readSubmit(vector<vector<string>> twoDVector, int numberOfGroup, int numberOfPrj) // dont use this function
{
    ifstream readSubmit("SubmissionStatus.txt"); // read the submission file to get the existed values
    if (!readSubmit)                             // if file doesnt exist create new submit values
    {
        cerr << "\nFile doesnt exist, create a new file " << endl;
        string myString = "Not Yet Submitted";
        for (int i = 0; i < numberOfGroup; i++)
        {
            fill(twoDVector[i].begin(), twoDVector[i].end(), myString); // fill all vector value
        }
    }
    if (readSubmit)
    {
        int count = 0;
        string submitLine;
        readSubmit.ignore(numeric_limits<streamsize>::max(), '\n'); // ignore the first line when read file
        while (getline(readSubmit, submitLine) && count < numberOfGroup)
        {
            string element = "";
            char delimiter = ',';
            istringstream iss(submitLine);
            getline(iss, element, delimiter); // use to skip the first element and the first comma on each read line
            for (size_t i = 0; i < numberOfPrj; i++)
            {
                getline(iss, twoDVector[count][i], delimiter); // read from the 2nd element to the end of each line to get existed submit value
            }
            count++;
        }
    }
    readSubmit.close();
    for (size_t i = 0; i < numberOfGroup; i++)
    {
        for (size_t j = 0; j < numberOfPrj; j++)
        {
            if (twoDVector[i][j].empty()) // search for emty value in vector
            {
                twoDVector[i][j] = "Not Yet Submitted"; // fill the emty position of vector if new group or new prj was created
            }
        }
    }
};
void deleteFile() // dont use yet
{
    const char *filename = "SubmissionStatus.txt";
    if (remove(filename) != 0)
    {
        printf("Error deleting file\n");
    }
    else
    {
        printf("File deleted successfully\n");
    }
}
void readSubmitStatus(vector<vector<string>> &twoDVector, int numberOfGroup, int numberOfPrj)
{
    ifstream readSubmit("SubmissionStatus.txt"); // read the submission file to get the existed values
    if (!readSubmit)                             // if file doesnt exist create new submit values
    {
        cerr << "\nError open file" << endl;
    }
    if (readSubmit)
    {
        int count = 0;
        string submitLine;
        readSubmit.ignore(numeric_limits<streamsize>::max(), '\n'); // ignore the first line when read file
        while (getline(readSubmit, submitLine) && count < numberOfGroup)
        {
            string element = "";
            char delimiter = ',';
            istringstream iss(submitLine);
            getline(iss, element, delimiter); // use to skip the first element and the first comma on each read line
            for (size_t i = 0; i < numberOfPrj; i++)
            {
                getline(iss, twoDVector[count][i], delimiter); // read from the 2nd element to the end of each line to get existed submit value
            }
            count++;
        }
    }
    readSubmit.close();
};

#endif