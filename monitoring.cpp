#include <iostream>
#include <iomanip>
#include <ctime>
#include <vector>
#include <fstream>
#include <string>
#include <sstream>
#include <limits>
#include <algorithm>
#include "UserModule.h"
#include "EventModule.h"
#include "monitoring.h"
#include "Payment.h"

using namespace std;

// print time
string returnTime(Participant p)
{
    char buf[20];
    strftime(buf, sizeof(buf), "%Y-%m-%d %H:%M:%S", localtime(&p.attendance.checkInTime));
    return buf;
}

bool checkIsExist(vector<Participant> participant, string name)
{
    for (auto &p : participant)
    {
        if (p.name == name)
        {
            return true; // true represent is exist
        }
    }
    return false; // false represent not exist
}

// [Monitoring] return integer after verify
int returnInt()
{
    int selection;

    cout << "Enter your choice > ";
    cin >> selection;

    if (cin.fail())
    {
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        cout << "Invalid input. Please enter a number.\n\n";
        return -1;
    }

    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    return selection;
}

// [Monitoring] return upper case char after verify
char returnUpperChar()
{

    char selection;
    cout << "Enter your choice > ";
    cin >> selection;

    if (cin.fail())
    {
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        cout << "Invalid input. Please enter a character.\n\n";
        return '\0'; // special value meaning "invalid"
    }

    cin.ignore(numeric_limits<streamsize>::max(), '\n'); // clean up buffer
    return toupper(selection);
}

// [Monitoring] update the file after push back the vector
void updateFile(ofstream &outFile, vector<Participant> &participants)
{
    for (auto &p : participants)
    {
        outFile << p.name << "|"
                << (p.isVip ? 1 : 0) << "|"
                << (p.attendance.isPresent ? 1 : 0) << "|"
                << p.attendance.checkInTime
                << "\n";
    }
}

// [Monitoring example][R]
time_t weddingStartDate()
{
    tm date = {};
    date.tm_year = 2025 - 1900; // year since 1900
    date.tm_mon = 8;            // September (0-based, so 8 = Sept)
    date.tm_mday = 1;
    date.tm_hour = 12;
    date.tm_min = 0;
    date.tm_sec = 0;
    return mktime(&date);
}

time_t weddingEndDate()
{
    return weddingStartDate() + 24 * 60 * 60; // + 1 day in seconds
}

// [Monitoring][R][E]
void printParticipant(Participant p)
{
    time_t now = time(nullptr);
    time_t weddingDate = weddingStartDate();

    // Name
    cout << "| " << setw(30) << left << p.name;

    // VIP
    cout << "| " << setw(20) << left << (p.isVip ? "Yes" : "No");

    // Present
    if (now < weddingDate)
    {
        cout << "| " << setw(30) << left << "Haven't reach the date";
    }
    else
    {
        cout << "| " << setw(30) << left << (p.attendance.isPresent ? "Yes" : "No");
    }

    // Check-in Time
    if (now < weddingDate)
    {
        cout << "| " << setw(26) << left << "Haven't reach the date";
    }
    else if (p.attendance.checkInTime == -1)
    {
        cout << "| " << setw(26) << left << "Not checked in";
    }
    else
    {
        cout << "| " << setw(26) << left << returnTime(p);
    }

    cout << "|\n";
}

void deleteParticipants(vector<Participant> &participants)
{
    // enter the name, if searched then show the information, then ask to edit what
    string name, input;
    int selection = 0;
    bool found = false;
    Participant participant;

    if (participants.empty())
    {
        cout << "There are no participants in the file.\n\n";
        return;
    }

    while (true)
    {
        cout << "Please enter the participant's name to delete. (E.g. Cai Xu Kun)\n";
        getline(cin, name);

        if (cin.fail())
        {
            cout << "Please enter number or character only.\n\n";
            continue;
        }

        // Table header
        cout << string(115, '-') << "\n";
        cout << "| " << setw(30) << left << "Name"
             << "| " << setw(20) << left << "isVip"
             << "| " << setw(30) << left << "Present"
             << "| " << setw(26) << left << "Check-in Time"
             << "|\n";
        cout << string(115, '-') << "\n";

        for (auto &p : participants)
        {
            if (p.name == name)
            {
                printParticipant(p);
                participant.name = p.name;
                participant.isVip = p.isVip;
                participant.attendance.isPresent = p.attendance.isPresent;
                participant.attendance.checkInTime = p.attendance.checkInTime;
                found = true;
            }
        }

        if (!found)
        {
            cout << "There are no participants names " << name << " in the file.\n\n";
        }
        else
        {
            cout << string(115, '-') << "\n";

            ofstream outFile("participants.txt");

            if (!outFile)
            {
                cerr << "Cant open file. Location: deleteParticipants.\n";
                cout << "Sorry you cant delete now, There some error here.\n\n";
                return;
            }

            while (true)
            {
                cout << "Do you comfirm want to delete " << name << " ? (Yes: y, No: n)\n\n";
                selection = returnUpperChar();

                if (selection == '\0')
                {
                    continue;
                }

                if (selection == 'Y')
                {
                    for (int i = 0; i < participants.size(); i++)
                    {
                        if (participants[i].name == name)
                        {
                            participants.erase(participants.begin() + i);
                            cout << name << " has been deleted successfully.\n\n";
                            break;
                        }
                    }
                    break;
                }
                else if (selection == 'N')
                {
                    cout << name << " will not be deleted.\n";
                    break;
                }
                else
                {
                    cout << "Please choose the option given.\n";
                }
            }
            outFile.close();
        }
        break;
    }
}

//[Monitoring][E]
void editParticipantsName(vector<Participant> &participants, Participant participant, ofstream &outFile)
{
    string newname;

    while (true)
    {
        cout << "Please enter the new participant's name. (E.g. Cai Xu Kun)\n";
        getline(cin, newname);

        if (cin.fail())
        {
            cout << "Please enter number or character only.\n\n";
            continue;
        }

        if (!checkIsExist(participants, newname))
        {
            // Find and update the participant in the vector
            for (auto &p : participants)
            {
                if (p.name == participant.name)
                {
                    p.name = newname;
                    updateFile(outFile, participants);
                    cout << "Participant name updated successfully from '" << participant.name << "' to '" << newname << "'.\n\n";
                    break;
                }
            }
        }
        else
        {
            cout << "Error: A participant with the name '" << newname << "' already exists.\n\n";
        }
        break;
    }
}

//[Monitoring][E]
void editParticipantsVip(vector<Participant> &participants, Participant participant, ofstream &outFile)
{
    char selection;

    while (true)
    {
        cout << "Current Edit Participant: " << participant.name << "\nIs Vip Now: " << (participant.isVip ? "Yes" : "No") << "\nDo you want to change vip status? (Yes: y, No: n)\n";
        selection = returnUpperChar();

        if (selection == '\0')
        {
            continue;
        }

        if (selection == 'Y')
        {
            // Find and update the participant in the vector
            for (auto &p : participants)
            {
                if (p.name == participant.name)
                {
                    p.isVip = !p.isVip;
                    updateFile(outFile, participants);
                    cout << "Participant isVip status changes successfully from '" << (!p.isVip ? "Yes" : "No") << "' to '" << (p.isVip ? "Yes" : "No") << "'.\n\n";
                    break;
                }
            }
        }
        else
        {
            cout << "Participant isVip status retain.\n\n";
        }
        break;
    }
}

bool askForTime(Participant participant, tm &t)
{
    while (true)
    {
        string input;
        cout << "Current Edit Participant: " << participant.name << "\nCheck-in Time: " << returnTime(participant) << "\nPlease enter check-in time. (format: YYYY-MM-DD HH:MM:SS) (0: exit) \n> ";

        getline(cin, input);

        if (cin.fail())
        {
            cout << "Please enter number or character only.\n\n";
            continue;
        }

        if (input == "0")
        {
            cout << "Participant check-in time retain.\n\n";
            return false;
        }

        // Convert string to time_t

        istringstream ss(input);
        ss >> get_time(&t, "%Y-%m-%d %H:%M:%S"); // parse input
        if (ss.fail())
        {
            cerr << "Invalid date format! Enter again.\n\n"
                 << endl;
            continue;
        }
        else
        {
            return true;
        }
    }
}

//[Monitoring][E]
void editParticipantsPresent(vector<Participant> &participants, Participant participant, ofstream &outFile)
{
    char selection;
    tm t = {};

    while (true)
    {

        cout << "Current Edit Participant: " << participant.name << "\nIs Present Now: " << (participant.attendance.isPresent ? "Yes" : "No") << "\nDo you want to change present status? (Yes: y, No: n)\n";
        selection = returnUpperChar();

        if (selection == '\0')
        {
            continue;
        }

        if (selection == 'Y')
        {
            // if yes -> no, clear the check in time
            if (participant.attendance.isPresent)
            {
                for (auto &p : participants)
                {
                    if (p.name == participant.name)
                    {
                        p.attendance.checkInTime = -1;
                        p.attendance.isPresent = !p.attendance.isPresent;
                        updateFile(outFile, participants);
                        cout << "Participant check-in time clear successfully.\n";
                        cout << "Participant present status changes successfully from '" << (!p.attendance.isPresent ? "Yes" : "No") << "' to '" << (p.attendance.isPresent ? "Yes" : "No") << "'.\n\n";
                        break;
                    }
                }
            }
            else
            { // no -> yes, need to insert the time
                if (askForTime(participant, t))
                {
                    // Find and update the participant in the vector
                    for (auto &p : participants)
                    {
                        if (p.name == participant.name)
                        {
                            p.attendance.checkInTime = mktime(&t);
                            p.attendance.isPresent = !p.attendance.isPresent;
                            updateFile(outFile, participants);
                            cout << "Participant check-in time changes successfully from '" << returnTime(participant) << "' to '" << returnTime(p) << "'.\n";
                            cout << "Participant present status changes successfully from '" << (!p.attendance.isPresent ? "Yes" : "No") << "' to '" << (p.attendance.isPresent ? "Yes" : "No") << "'.\n\n";
                            break;
                        }
                    }
                }
                else
                {
                    cout << "Cause check-in time didnt insert. So, Participant present status retain.\n\n";
                }
            }
        }
        else if (selection == 'N')
        {
            cout << "Participant present status retain.\n\n";
        }
        break;
    }
}

//[Monitoring][E]
void editParticipantsCheckInTime(vector<Participant> &participants, Participant participant, ofstream &outFile)
{
    tm t = {};

    if (askForTime(participant, t))
    {
        // Find and update the participant in the vector
        for (auto &p : participants)
        {
            if (p.name == participant.name)
            {
                p.attendance.checkInTime = mktime(&t);
                p.attendance.isPresent = true;
                updateFile(outFile, participants);
                cout << "Participant check-in time changes successfully from '" << returnTime(participant) << "' to '" << returnTime(p) << "'.\n";
                cout << "Participant present status set as \"Yes\" successfully.\n\n";
                break;
            }
        }
    }
}

//[Monitoring][E]
void updateParticipants(vector<Participant> &participants)
{
    // enter the name, if searched then show the information, then ask to edit what
    string name, input;
    int selection = 0;
    bool found = false;
    Participant participant;

    if (participants.empty())
    {
        cout << "There are no participants in the file.\n\n";
        return;
    }

    while (true)
    {
        cout << "Please enter the participant's name to edit. (E.g. Cai Xu Kun)\n";
        getline(cin, name);

        if (cin.fail())
        {
            cout << "Please enter number or character only.\n\n";
            continue;
        }

        // Table header
        cout << string(115, '-') << "\n";
        cout << "| " << setw(30) << left << "Name"
             << "| " << setw(20) << left << "isVip"
             << "| " << setw(30) << left << "Present"
             << "| " << setw(26) << left << "Check-in Time"
             << "|\n";
        cout << string(115, '-') << "\n";

        for (auto &p : participants)
        {

            if (p.name == name)
            {
                printParticipant(p);
                participant.name = p.name;
                participant.isVip = p.isVip;
                participant.attendance.isPresent = p.attendance.isPresent;
                participant.attendance.checkInTime = p.attendance.checkInTime;
                found = true;
            }
        }

        if (!found)
        {
            cout << "There are no participants names " << name << " in the file.\n\n";
        }
        else
        {
            cout << string(115, '-') << "\n";

            ofstream outFile("participants.txt");
            if (!outFile)
            {
                cerr << "Cant open file. Location: updateParticipants.\n";
                cout << "Sorry you cant edit now, There some error here.\n\n";
                return;
            }

            cout << "What the information you going to edit.\n1. Name\n2. Vip (yes or not)\n3. Present (yes or not)\n4. Check-In Time\n\n";
            selection = returnInt();

            switch (selection)
            {
            case 1:
                cout << "Edit Name selected.\n\n";
                editParticipantsName(participants, participant, outFile);
                break;
            case 2:
                cout << "Edit Vip Status selected.\n\n";
                editParticipantsVip(participants, participant, outFile);
                break;
            case 3:
                cout << "Edit Present Status selected.\n\n";
                editParticipantsPresent(participants, participant, outFile);
                break;
            case 4:
                cout << "Edit Check-in Time selected.\n\n";
                editParticipantsCheckInTime(participants, participant, outFile);
                break;
            case 5:
                cout << "Exit Monitoring Module selected.\n\n";
                return;
            default:
                cout << "Invalid selection. Try again.\n\n";
                break;
            }
            outFile.close();
        }
        break;
    }
}

// [Monitoring][C][Mark][D] check user enter participants list
bool checkParticipantsList(stringstream &buffer)
{
    string name;
    cout << "Enter participant names (type 'done' on the end to finish).\n";

    // check is it "done" inside
    while (true)
    {
        // let user enter the name list
        getline(cin, name);
        if (name == "done")
        {
            return true;
        }

        if (!name.empty())
        {
            buffer << name << "|";
        }
    }
    return false;
}

// [Monitoring][C][Mark] show just the name of the buffer
void readBuffer(stringstream &buffer)
{
    string name;

    streampos pos = buffer.tellg();

    // print the name list
    while (getline(buffer, name, '|'))
    {
        if (!name.empty())
        {
            cout << name << endl;
        }
    }

    buffer.clear();
    buffer.seekg(pos);
}

// [Mark Attendance][U]
void markAttendance(vector<Participant> &participants)
{
    string name;
    stringstream buffer;
    char selection;
    bool anyMarked = false;
    tm t = {};

    // open file
    ofstream outFile("participants.txt");
    if (!outFile)
    {
        cerr << "Cant open file. Location: markAttendance.\n";
        return;
    }

    // get the buffer
    if (!checkParticipantsList(buffer))
    {
        cout << "Please insert \" done \" on the end of list.\n Back to previous page...\n";
        return;
    }

    // ask the user comfirm to mark
    while (true)
    {
        cout << "\nThese name going to mark as attend.\n";
        readBuffer(buffer);

        cout << "\nDo you want to mark these name as attend ? (Yes: y, No: n)\n";
        selection = returnUpperChar();

        if (selection == '\0')
        {
            continue;
        }

        if (selection == 'Y')
        {
            // mark participants attendance
            while (getline(buffer, name, '|'))
            {
                if (!name.empty())
                {
                    for (auto &p : participants)
                    {
                        if (name == p.name)
                        {
                            if (p.attendance.isPresent)
                            {
                                cout << " The user already take attendance on" << returnTime(p) << "\n\n";
                            }
                            else
                            {
                                p.attendance.isPresent = true;
                                p.attendance.checkInTime = mktime(&t);
                                cout << name << "'s attendance was successfully marked\n\n";
                                anyMarked = true;
                            }
                        }
                    }
                }
            }

            if (anyMarked)
            {
                updateFile(outFile, participants);
            }
            break;
        }
        else if (selection == 'N')
        {
            cout << "Attendance of these name will not be marked.\n";
            break;
        }
        else
        {
            cout << "Please choose the option given.\n";
        }
    }
    outFile.close();
}

// [Search][R]
void searchParticipantOneByOne(vector<Participant> participants)
{
    string name, input, compare;
    int found = 0;

    if (participants.empty())
    {
        cout << "There are no participants in the file.\n\n";
        return;
    }

    while (true)
    {
        cout << "Please enter the participants name to search. (E.g. Cai Xu Kun)\n";
        getline(cin, name);

        transform(name.begin(), name.end(), name.begin(), ::tolower);

        if (cin.fail())
        {
            cout << "Please enter number or character only.\n\n";
            continue;
        }

        // Table header
        cout << string(115, '-') << "\n";
        cout << "| " << setw(30) << left << "Name"
             << "| " << setw(20) << left << "isVip"
             << "| " << setw(30) << left << "Present"
             << "| " << setw(26) << left << "Check-in Time"
             << "|\n";
        cout << string(115, '-') << "\n";

        for (auto &p : participants)
        {
            string compare = p.name;
            transform(compare.begin(), compare.end(), compare.begin(), ::tolower);

            if (compare.find(name) != string::npos)
            {
                printParticipant(p);
                found++;
            }
        }

        if (found < 1)
        {

            cout << "There are no participants names " << name << " in the file.\n\n";
        }
        else
        {
            cout << string(115, '-') << "\n";
            cout << found << " result" << (found > 1 ? "s" : "") << " of participant" << (found > 1 ? "s" : "") << " names " << name << " or similar in the file. ( Press Enter to continue .... )\n";
            do
            {
                getline(cin, input);
            } while (!input.empty());
        }
        break;
    }
}

// [Monitoring][R] show all the participants
void showAllParticipants(vector<Participant> participants)
{
    string input;

    if (participants.empty())
    {
        cout << "There are no participants in the file.\n\n";
        return;
    }

    // Table header
    cout << string(115, '-') << "\n";
    cout << "| " << setw(30) << left << "Name"
         << "| " << setw(20) << left << "isVip"
         << "| " << setw(30) << left << "Present"
         << "| " << setw(26) << left << "Check-in Time"
         << "|\n";
    cout << string(115, '-') << "\n";

    // Table rows
    while (!participants.empty())
    {
        Participant p = participants.back();
        participants.pop_back();

        printParticipant(p);
    }

    cout << string(115, '-') << "\n";
    cout << "Press Enter to continue .... )\n";
    do
    {
        getline(cin, input);
    } while (!input.empty());
}

// [Monitoring][R] search participants menu
void searchParticipantsMenu(vector<Participant> &participants)
{
    int selection;

    while (true)
    {
        cout << "1. Show all the participants\n2. Search Participant by name\n3. Exit\n\n";
        selection = returnInt();

        if (selection == -1)
        {
            continue;
        }

        switch (selection)
        {
        case 1:
            showAllParticipants(participants);
            break;
        case 2:
            searchParticipantOneByOne(participants);
            break;
        case 3:
            cout << "Exit successful\nBack To Monitoring Menu\n\n";
            return;
        default:
            cout << "Please choose only the option given.\n";
            break;
        }
    }
}

// [Monitoring][C] add participants to list
void addParticipantsToList(vector<Participant> &participants, stringstream &buffer, ofstream &outFile)
{
    string name;
    char selection;
    bool anyAdded = false;

    while (true)
    {
        cout << "\nThese name going to add to the list\n";
        readBuffer(buffer);

        cout << "\nDo you want to add these name into the list ? (Yes: y, No: n)\n";
        selection = returnUpperChar();

        if (selection == '\0')
        {
            continue;
        }

        if (selection == 'Y')
        {
            while (getline(buffer, name, '|'))
            {
                if (!name.empty())
                {
                    if (!checkIsExist(participants, name))
                    {
                        Participant p;
                        p.name = name;
                        p.isVip = false;
                        p.attendance.isPresent = false;
                        p.attendance.checkInTime = -1;

                        participants.push_back(p);
                        cout << p.name << " was successfully added.\n";
                        anyAdded = true;
                    }
                    else
                    {
                        cout << "Error: A participant with the name \"" << name << "\" already exists.\n\n";
                    }
                }
            }

            if (anyAdded)
            {
                updateFile(outFile, participants);
                cout << "All the name was successfully added.\n\n";
            }
            break;
        }
        else if (selection == 'N')
        {
            cout << "All the name will not add to the list.\n";
            break;
        }
        else
        {
            cout << "Please choose the option given.\n";
        }
    }
}

void addParticipantsByList(vector<Participant> &participants)
{
    string name;
    stringstream buffer;
    char selection;

    ofstream outFile("participants.txt");
    if (!outFile)
    {
        cerr << "Cant open file. Location: addParticipantsByList.\n";
        return;
    }

    if (!checkParticipantsList(buffer))
    {
        cout << "Please insert \" done \" on the end of list.\n Back to previous page...\n";
        return;
    }

    while (true)
    {
        if (buffer.str().empty())
        {
            return;
        }

        cout << "Do you want to check name list one by one ? (Yes: y, No: n)\n\n";
        selection = returnUpperChar();

        if (selection == '\0')
        {
            continue;
        }

        if (selection == 'Y')
        {
            while (getline(buffer, name, '|'))
            {

                cout << "Do you want to add " << name << " ? (Yes: y, No: n, Add All: A, Remove the other: R) \n";
                selection = returnUpperChar();

                if (selection == '\0')
                {
                    continue;
                }

                if (selection == 'Y')
                {
                    if (!checkIsExist(participants, name))
                    {
                        Participant p;
                        p.name = name;
                        p.isVip = false;
                        p.attendance.isPresent = false;
                        p.attendance.checkInTime = -1;

                        participants.push_back(p);
                        updateFile(outFile, participants);
                        cout << p.name << " was successfully added.\n\n";
                    }
                    else
                    {
                        cout << "Error: A participant with the name \"" << name << "\" already exists.\n\n";
                    }
                }
                else if (selection == 'N')
                {
                    cout << name << " will not add to the list.\n";
                }
                else if (selection == 'A')
                {
                    addParticipantsToList(participants, buffer, outFile);
                }
                else if (selection == 'R')
                {
                    cout << "The else will not add to the list\n";
                    buffer.str("");
                    buffer.clear();
                }
            }
            break;
        }
        else if (selection == 'N')
        {
            addParticipantsToList(participants, buffer, outFile);
            break;
        }
        else
        {
            cout << "Please enter the option given.\n";
        }
    }
    outFile.close();
}

void addParticipantOneByOne(vector<Participant> &participants)
{
    string name;
    char selection;
    int isVip = 0;
    Participant p;

    ofstream outFile("participants.txt");
    if (!outFile)
    {
        cerr << "Cant open file. Location: addParticipantsByList.\n";
        return;
    }

    cout << "Enter the participants name you going to add.\n> ";
    getline(cin, name);

    if (cin.fail())
    {
        cout << "Please enter number or character only.\n\n";
        return;
    }

    if (!checkIsExist(participants, name))
    {
        while (true)
        {
            cout << "\nIs " << name << " a VIP ? (Yes: y, No: n)\n";
            selection = returnUpperChar();

            if (selection == '\0')
            {
                continue;
            }

            if (selection == 'Y')
            {
                isVip = 1;
            }
            break;
        }
    }
    else
    {
        cout << "Error: A participant with the name \"" << name << "\" already exists.\n\n";
    }

    while (true)
    {
        cout << "Are you confirm to add \"" << name << "\"" << " which is " << (isVip == 1 ? "VIP" : "not VIP") << " ? (Yes: y, No: n)\n";
        selection = returnUpperChar();

        if (selection == '\0')
        {
            continue;
        }
        break;
    }

    switch (selection)
    {
    case 'Y':
        p.name = name;
        p.isVip = isVip;
        p.attendance.isPresent = false;
        p.attendance.checkInTime = -1;

        participants.push_back(p);
        updateFile(outFile, participants);
        cout << name << " was successfully added.\n\n";
        break;
    case 'N':
        cout << name << " will not add to the list.\n";
        break;
    default:
        cout << "There was some error occur Location: addParticipantOneByOne\n\n";
        break;
    }
}

// CRUD
/* void add Participants
user enter the name of the participants
allowed user to set as vip
*/

void addParticipantsMenu(vector<Participant> &participants)
{
    int selection;

    while (true)
    {
        cout << "1. Add Participants by list \n2. Add Participant one by one\n3. Exit\n\n";
        selection = returnInt();

        if (selection == -1)
        {
            continue;
        }

        switch (selection)
        {
        case 1:
            addParticipantsByList(participants);
            break;
        case 2:
            addParticipantOneByOne(participants);
            break;
        case 3:
            cout << "Exit successful\n\n";
            return;
        default:
            cout << "Please choose only the option given.\n";
            break;
        }
    }
}

/*
void setattendance when timesup set all attendance as absent
void take attendance
user search name, take the att, record time of now

most important is file

*/

bool addDefaultParticipants(vector<Participant> &participants)
{
    ifstream inFile("participants.txt");

    if (!inFile)
    {
        cerr << "Error: Could not open participants.txt\n";
        return false;
    }

    string line;

    while (getline(inFile, line))
    {
        if (line.empty())
            continue;

        stringstream ss(line);
        string name, vipStr, presentStr, timeStr;

        getline(ss, name, '|'); // split by |
        getline(ss, vipStr, '|');
        getline(ss, presentStr, '|');
        getline(ss, timeStr, '|');

        Participant p;
        p.name = name;
        p.isVip = (stoi(vipStr) == 1);
        p.attendance.isPresent = (stoi(presentStr) == 1);
        p.attendance.checkInTime = stoll(timeStr);

        participants.push_back(p);
    }
    inFile.close();
    return true;
}

void menu(vector<Participant> &participants)
{
    int selection = 0;
    time_t now = time(nullptr); // current time

    while (true)
    {
        cout << "1. Add Participants\n2. Search Participants\n3. Mark Attendance\n4. Edit Participants Information\n5. Delete Participants\n6. Exit Monitoring Module\n\n";
        selection = returnInt();

        if (selection == -1)
        {
            continue;
        }

        switch (selection)
        {
        case 1:
            cout << "Add Participants selected.\n\n";
            addParticipantsMenu(participants);
            break;
        case 2:
            cout << "Search Participants selected.\n\n";
            searchParticipantsMenu(participants);
            break;
        case 3:
            if (now > weddingStartDate())
            {
                if (now < weddingEndDate())
                {
                    cout << "Mark Attendance selected.\n\n";
                    markAttendance(participants);
                }
                else
                {
                    cout << "Wedding already end. You are not allowed to mark attendance.\n";
                }
            }
            else
            {
                cout << "Wedding have not start yet. You are not allowed to mark attendance.\n";
            }
            break;
        case 4:
            cout << "Update Participants details selected.\n\n";
            updateParticipants(participants);
            break;
        case 5:
            cout << "Delete Monitoring Module selected.\n\n";
            deleteParticipants(participants);
            break;
        case 6:
            cout << "Exit Monitoring Module selected.\n\n";
            return;
        default:
            cout << "Invalid selection. Try again.\n\n";
            break;
        }
    }
}

