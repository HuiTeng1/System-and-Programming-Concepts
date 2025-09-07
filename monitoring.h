#ifndef MONITORING_H
#define MONITORING_H

#include <iostream>
#include <vector>
#include <string>
#include <ctime>
#include <fstream>
#include <sstream>

// Struct definitions
struct Attendance {
    bool isPresent;
    time_t checkInTime;
};

struct Participant {
    string name;
    bool isVip;
    Attendance attendance;
};

// Utility functions
string returnTime(Participant p);
bool checkIsExist(vector<Participant> participant, string name);

// Input validation functions
int returnInt();
char returnUpperChar();

// File operations
void updateFile(ofstream& outFile, vector<Participant>& participants);
bool addDefaultParticipants(vector<Participant>& participants);

// Date functions
time_t weddingStartDate();
time_t weddingEndDate();

// Display functions
void printParticipant(Participant p);
void showAllParticipants(vector<Participant> participants);

// CRUD operations
void addParticipantsMenu(vector<Participant>& participants);
void addParticipantsByList(vector<Participant>& participants);
void addParticipantOneByOne(vector<Participant>& participants);

void searchParticipantsMenu(vector<Participant>& participants);
void searchParticipantOneByOne(vector<Participant> participants);

void updateParticipants(vector<Participant>& participants);
void editParticipantsName(vector<Participant>& participants, Participant participant, ofstream& outFile);
void editParticipantsVip(vector<Participant>& participants, Participant participant, ofstream& outFile);
void editParticipantsPresent(vector<Participant>& participants, Participant participant, ofstream& outFile);
void editParticipantsCheckInTime(vector<Participant>& participants, Participant participant, ofstream& outFile);

void deleteParticipants(vector<Participant>& participants);

// Attendance functions
void markAttendance(vector<Participant>& participants);
bool checkParticipantsList(stringstream& buffer);
void readBuffer(stringstream& buffer);
void addParticipantsToList(vector<Participant>& participants, stringstream& buffer, ofstream& outFile);
bool askForTime(Participant participant, tm& t);

// Main menu
void menu(vector<Participant>& participants);

#endif


// // main.cpp
// #include <iostream>
// #include "monitoring.h"  // Include your header


// int main()
// {
//     vector<Participant> participants;

//     if (addDefaultParticipants(participants))
//     {
//         menu(participants);
//     }
//     else
//     {
//         cout << "Monitoring Module is close now.\n";
//     }

//     cout << "System close now.";

//     return 0;
// }