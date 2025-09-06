#include "UserModule.h"
#include "EventModule.h"
#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <fstream>
#include <iomanip>
#include <algorithm>
#include <ctime>
#include <windows.h>

using namespace std;

// WeddingEvent member functions
string WeddingEvent::toFileString() const {
    string servicesStr;
    for (size_t i = 0; i < bookedServices.size(); ++i) {
        servicesStr += to_string(bookedServices[i]);
        if (i < bookedServices.size() - 1) {
            servicesStr += ",";
        }
    }

    return eventId + "|" + organizerId + "|" + groomName + "|" + brideName + "|" +
           weddingDate + "|" + weddingVenue + "|" + weddingTheme + "|" +
           to_string(budget) + "|" + to_string(totalCost) + "|" + status + "|" + servicesStr;
}

WeddingEvent WeddingEvent::fromFileString(string& str) {
    WeddingEvent event;
    stringstream ss(str);
    string temp;

    getline(ss, event.eventId, '|');
    getline(ss, event.organizerId, '|');
    getline(ss, event.groomName, '|');
    getline(ss, event.brideName, '|');
    getline(ss, event.weddingDate, '|');
    getline(ss, event.weddingVenue, '|');
    getline(ss, event.weddingTheme, '|');

    getline(ss, temp, '|');
    event.budget = stod(temp);

    getline(ss, temp, '|');
    event.totalCost = stod(temp);

    getline(ss, event.status, '|');

    string servicesStr;
    getline(ss, servicesStr);
    event.bookedServices.clear();
    if (!servicesStr.empty()) {
        stringstream ssServices(servicesStr);
        string serviceIdStr;
        while (getline(ssServices, serviceIdStr, ',')) {
            if (!serviceIdStr.empty()) {
                event.bookedServices.push_back(stoi(serviceIdStr));
            }
        }
    }

    return event;
}

// Event utility functions
bool isValidDate(const string& date) {
    if (date.length() != 10) return false;
    if (date[4] != '-' || date[7] != '-') return false;

    for (int i = 0; i < 10; i++) {
        if (i == 4 || i == 7) continue;
        if (!isdigit(date[i])) return false;
    }

    time_t now = time(0);
    tm currentDate;
    localtime_s(&currentDate, &now);

    int year = stoi(date.substr(0, 4));
    int month = stoi(date.substr(5, 2));
    int day = stoi(date.substr(8, 2));

    if (month < 1 || month > 12) return false;
    if (day < 1 || day > 31) return false;

    if (year < (currentDate.tm_year + 1900)) return false;
    if (year == (currentDate.tm_year + 1900) && month < (currentDate.tm_mon + 1)) return false;
    if (year == (currentDate.tm_year + 1900) && month == (currentDate.tm_mon + 1) && day <= currentDate.tm_mday) return false;

    return true;
}

bool isValidWeddingDate(const string& date) {
    if (!isValidDate(date)) return false;

    int year = stoi(date.substr(0, 4));
    int month = stoi(date.substr(5, 2));
    int day = stoi(date.substr(8, 2));

    time_t now = time(0);
    tm futureDate;
    localtime_s(&futureDate, &now);
    futureDate.tm_mday += 30;
    mktime(&futureDate);

    tm inputDate = { 0 };
    inputDate.tm_year = year - 1900;
    inputDate.tm_mon = month - 1;
    inputDate.tm_mday = day;
    mktime(&inputDate);

    if (inputDate.tm_year < futureDate.tm_year ||
        (inputDate.tm_year == futureDate.tm_year && inputDate.tm_yday < futureDate.tm_yday)) {
        cout << "Wedding date must be at least 30 days from today!\n";
        return false;
    }

    return true;
}


bool isDateAvailable(const string& date, const vector<WeddingEvent>& events, const string& venue) {
    for (const auto& event : events) {
        if (event.weddingDate == date && event.status != "cancelled") {
            if (venue.empty() || event.weddingVenue == venue) {
                return false;
            }
        }
    }
    return true;
}

bool isValidBudget(double budget) {
    if (budget < 1000) {
        cout << "Budget must be at least RM1000!\n";
        return false;
    }
    if (budget > 1000000) {
        cout << "Budget cannot exceed RM1,000,000!\n";
        return false;
    }
    return true;
}

double calculateServiceCost(const vector<int>& serviceIds, const vector<Vendor>& vendorList) {
    double total = 0.0;
    for (int serviceId : serviceIds) {
        for (const auto& vendor : vendorList) {
            for (const auto& service : vendor.serviceHasProvide) {
                if (service.serviceId == "S" + to_string(serviceId)) {
                    total += service.price;
                    break;
                }
            }
        }
    }
    return total;
}

void loadEventsFromFile(vector<WeddingEvent>& events, const string& filename) {
    ifstream file(filename);
    if (!file) return;

    events.clear();
    string line;
    while (getline(file, line)) {
        if (!line.empty()) {
            try {
                events.push_back(WeddingEvent::fromFileString(line));
            }
            catch (...) {
                cout << "Error parsing event line: " << line << endl;
            }
        }
    }
    file.close();
}

void saveEventsToFile(const vector<WeddingEvent>& events, const string& filename) {
    ofstream file(filename);
    if (!file) {
        cout << "Error opening events file for writing." << endl;
        return;
    }

    for (const auto& event : events) {
        file << event.toFileString() << endl;
    }
    file.close();
}

void createNewWedding(CurrentUser& currentUser, vector<WeddingEvent>& events,vector<Vendor>& vendorList, vector<Organizer>& organizerList) {
    if (currentUser.type != ORGANIZER) {
        cout << "Only organizers can create weddings!" << endl;
        pauseScreen();
        return;
    }

    clearScreen();
    cout << "=== CREATE NEW WEDDING ===" << endl;

    WeddingEvent newEvent;
    newEvent.eventId = generateId("W", events.size() + 1);
    newEvent.organizerId = currentUser.userId;
    newEvent.status = "planning";
    newEvent.totalCost = 0.0;

    cout << "Groom's Name: ";
    getline(cin, newEvent.groomName);

    cout << "Bride's Name: ";
    getline(cin, newEvent.brideName);

    bool validDate = false;
    while (!validDate) {
        cout << "Wedding Date (YYYY-MM-DD): ";
        getline(cin, newEvent.weddingDate);

        if (!isValidWeddingDate(newEvent.weddingDate)) {
            cout << "Invalid date! Please use YYYY-MM-DD format and ensure it's at least 30 days from today.\n";
        }
        else {
            validDate = true;
        }
    }

    cout << "Wedding Venue: ";
    getline(cin, newEvent.weddingVenue);

    cout << "Wedding Theme: ";
    getline(cin, newEvent.weddingTheme);

    bool validBudget = false;
    while (!validBudget) {
        cout << "Budget (RM): ";
        string budgetStr;
        getline(cin, budgetStr);

        try {
            newEvent.budget = stod(budgetStr);
            if (!isValidBudget(newEvent.budget)) {
                cout << "Budget must be between RM1000 and RM1,000,000!\n";
            }
            else {
                validBudget = true;
            }
        }
        catch (...) {
            cout << "Invalid budget amount!\n";
        }
    }

    events.push_back(newEvent);

    for (auto& organizer : organizerList) {
        if (organizer.organizerId == currentUser.userId) {
            organizer.currentWeddingId = newEvent.eventId;
            break;
        }
    }

    saveEventsToFile(events, "events.txt");

    cout << "Wedding created successfully! Event ID: " << newEvent.eventId << endl;
    pauseScreen();
}

void bookServicesForWedding(CurrentUser& currentUser, vector<WeddingEvent>& events,vector<Vendor>& vendorList, vector<Organizer>& organizerList) {
    if (currentUser.type != ORGANIZER) {
        cout << "Only organizers can book services!" << endl;
        pauseScreen();
        return;
    }

    string currentWeddingId;
    for (const auto& organizer : organizerList) {
        if (organizer.organizerId == currentUser.userId) {
            currentWeddingId = organizer.currentWeddingId;
            break;
        }
    }

    if (currentWeddingId.empty()) {
        cout << "No active wedding! Please create a wedding first." << endl;
        pauseScreen();
        return;
    }

    WeddingEvent* currentEvent = nullptr;
    for (auto& event : events) {
        if (event.eventId == currentWeddingId) {
            currentEvent = &event;
            break;
        }
    }

    if (!currentEvent || currentEvent->status == "cancelled") {
        cout << "Wedding not found or cancelled!" << endl;
        pauseScreen();
        return;
    }

    clearScreen();
    cout << "=== BOOK SERVICES FOR WEDDING ===" << endl;
    cout << "Event: " << currentEvent->groomName << " & " << currentEvent->brideName << endl;
    cout << "Date: " << currentEvent->weddingDate << endl;
    cout << "Budget: RM" << fixed << setprecision(2) << currentEvent->budget << endl;
    cout << "Current Total: RM" << fixed << setprecision(2) << currentEvent->totalCost << endl;
    cout << "Remaining Budget: RM" << fixed << setprecision(2) << (currentEvent->budget - currentEvent->totalCost) << endl;
    cout << "==========================================" << endl;

    cout << "Service Categories:" << endl;
    cout << "1. Venue" << endl;
    cout << "2. Catering" << endl;
    cout << "3. Photography" << endl;
    cout << "4. Decoration" << endl;
    cout << "5. View All Services" << endl;
    cout << "0. Back to Main Menu" << endl;

    int choice;
    cout << "Select category: ";
    cin >> choice;
    cin.ignore();

    if (choice == 0) return;

    string serviceType;
    switch (choice) {
    case 1: serviceType = "venue"; break;
    case 2: serviceType = "catering"; break;
    case 3: serviceType = "photography"; break;
    case 4: serviceType = "decoration"; break;
    case 5: serviceType = ""; break;
    default:
        cout << "Invalid choice!" << endl;
        pauseScreen();
        return;
    }

    clearScreen();
    cout << "=== AVAILABLE SERVICES ===" << endl;

    vector<pair<size_t, size_t>> availableServices;
    int optionNum = 1;

    for (size_t i = 0; i < vendorList.size(); i++) {
        const auto& vendor = vendorList[i];
        for (size_t j = 0; j < vendor.serviceHasProvide.size(); j++) {
            const auto& service = vendor.serviceHasProvide[j];
            if ((serviceType.empty() || service.type == serviceType) &&
                service.available && service.quantity > 0) {

                cout << optionNum << ". " << service.serviceName << " (by " << vendor.baseInfo.name << ")" << endl;
                cout << "   Description: " << service.description << endl;
                cout << "   Type: " << service.type << endl;
                cout << "   Price: RM" << fixed << setprecision(2) << service.price << endl;
                cout << "   Available: " << service.quantity << endl;
                cout << "   " << string(40, '-') << endl;

                availableServices.push_back({ i, j });
                optionNum++;
            }
        }
    }

    if (availableServices.empty()) {
        cout << "No available services in this category!" << endl;
        pauseScreen();
        return;
    }

    int serviceChoice;
    cout << "Select service to book (0 to cancel): ";
    cin >> serviceChoice;
    cin.ignore();

    if (serviceChoice < 1 || serviceChoice > static_cast<int>(availableServices.size())) {
        cout << "Invalid selection!" << endl;
        pauseScreen();
        return;
    }

    auto& selected = availableServices[static_cast<size_t>(serviceChoice - 1)];
    Service& service = vendorList[selected.first].serviceHasProvide[selected.second];
    Vendor& vendor = vendorList[selected.first];

    double newTotal = currentEvent->totalCost + service.price;
    if (newTotal > currentEvent->budget) {
        cout << "Cannot book service! Exceeds budget by RM"
            << fixed << setprecision(2) << (newTotal - currentEvent->budget) << endl;
        pauseScreen();
        return;
    }

    cout << "\nConfirm booking:" << endl;
    cout << "Service: " << service.serviceName << endl;
    cout << "Vendor: " << vendor.baseInfo.name << endl;
    cout << "Price: RM" << fixed << setprecision(2) << service.price << endl;
    cout << "Proceed? (y/n): ";

    char confirm;
    cin >> confirm;
    cin.ignore();

    if (confirm != 'y' && confirm != 'Y') {
        cout << "Booking cancelled." << endl;
        pauseScreen();
        return;
    }

    service.quantity--;
    if (service.quantity == 0) {
        service.available = false;
    }

    int serviceIdNum = stoi(service.serviceId.substr(1));
    currentEvent->bookedServices.push_back(serviceIdNum);
    currentEvent->totalCost += service.price;

    for (auto& organizer : organizerList) {
        if (organizer.organizerId == currentUser.userId) {
            organizer.bookedServices.push_back(serviceIdNum);
            break;
        }
    }

    saveEventsToFile(events, "events.txt");

    cout << "Service booked successfully!" << endl;
    pauseScreen();
}

void viewAllWeddings(CurrentUser& currentUser, const vector<WeddingEvent>& events,const vector<Vendor>& vendorList) {
    clearScreen();
    cout << "=== ALL WEDDING EVENTS ===" << endl;

    if (events.empty()) {
        cout << "No wedding events found." << endl;
        pauseScreen();
        return;
    }

    for (const auto& event : events) {
        if (event.status == "cancelled") continue;

        cout << "Event ID: " << event.eventId << endl;
        cout << "Couple: " << event.groomName << " & " << event.brideName << endl;
        cout << "Date: " << event.weddingDate << endl;
        cout << "Theme: " << event.weddingTheme << endl;
        cout << "Budget: RM" << fixed << setprecision(2) << event.budget << endl;
        cout << "Total Cost: RM" << fixed << setprecision(2) << event.totalCost << endl;
        cout << "Status: " << event.status << endl;
        cout << string(50, '=') << endl;
    }

    pauseScreen();
}

void manageMyWeddings(CurrentUser& currentUser, vector<WeddingEvent>& events,vector<Vendor>& vendorList, vector<Organizer>& organizerList) {
    if (currentUser.type != ORGANIZER) {
        cout << "Only organizers can manage weddings!" << endl;
        pauseScreen();
        return;
    }

    vector<WeddingEvent*> myEvents;
    for (auto& event : events) {
        if (event.organizerId == currentUser.userId && event.status != "cancelled") {
            myEvents.push_back(&event);
        }
    }

    if (myEvents.empty()) {
        cout << "You have no wedding events!" << endl;
        pauseScreen();
        return;
    }

    clearScreen();
    cout << "=== MY WEDDING EVENTS ===" << endl;

    for (size_t i = 0; i < myEvents.size(); i++) {
        WeddingEvent* event = myEvents[i];
        cout << (i + 1) << ". " << event->groomName << " & " << event->brideName
            << " (" << event->weddingDate << ")" << endl;
        cout << "   Status: " << event->status << endl;
        cout << "   Cost: RM" << fixed << setprecision(2) << event->totalCost
            << " / RM" << fixed << setprecision(2) << event->budget << endl;
        cout << "   " << string(30, '-') << endl;
    }

    int choice;
    cout << "Select wedding to manage (0 to cancel): ";
    cin >> choice;
    cin.ignore();

    if (choice < 1 || choice > static_cast<int>(myEvents.size())) {
        cout << "Invalid selection!" << endl;
        pauseScreen();
        return;
    }

    WeddingEvent* selectedEvent = myEvents[static_cast<size_t>(choice - 1)];

    clearScreen();
    cout << "=== MANAGING: " << selectedEvent->groomName << " & " << selectedEvent->brideName << " ===" << endl;
    cout << "Date: " << selectedEvent->weddingDate << endl;
    cout << "Status: " << selectedEvent->status << endl;
    cout << "Budget: RM" << fixed << setprecision(2) << selectedEvent->budget << endl;
    cout << "Total Cost: RM" << fixed << setprecision(2) << selectedEvent->totalCost << endl;
    cout << "==========================================" << endl;

    cout << "Management Options:" << endl;
    cout << "1. Set as Current Wedding" << endl;
    cout << "2. Cancel Wedding" << endl;
    cout << "3. Mark as Completed" << endl;
    cout << "4. Generate Invitation Card" << endl;
    cout << "0. Back" << endl;

    int option;
    cout << "Select option: ";
    cin >> option;
    cin.ignore();

    switch (option) {
    case 1:
        for (auto& organizer : organizerList) {
            if (organizer.organizerId == currentUser.userId) {
                organizer.currentWeddingId = selectedEvent->eventId;
                break;
            }
        }
        saveUserIntoFile<Organizer>(organizerList, "organizers.txt");
        cout << "Wedding set as current wedding." << endl;
        pauseScreen();
        break;

    case 2:
        cout << "Are you sure you want to cancel this wedding? (y/n): ";
        char confirm;
        cin >> confirm;
        cin.ignore();

        if (confirm == 'y' || confirm == 'Y') {
            selectedEvent->status = "cancelled";
            saveEventsToFile(events, "events.txt");
            cout << "Wedding cancelled successfully." << endl;
        }
        else {
            cout << "Cancellation aborted." << endl;
        }
        pauseScreen();
        break;

    case 3:
        selectedEvent->status = "completed";
        saveEventsToFile(events, "events.txt");
        cout << "Wedding marked as completed." << endl;
        pauseScreen();
        break;

    case 4:
        generateInvitationCard(currentUser, events, organizerList);
        break;

    default:
        break;
    }
}

void generateInvitationCard(CurrentUser& currentUser, const vector<WeddingEvent>& events,const vector<Organizer>& organizerList) {
    if (currentUser.type != ORGANIZER) {
        cout << "Only organizers can generate invitation cards!" << endl;
        pauseScreen();
        return;
    }

    string currentWeddingId;
    for (const auto& organizer : organizerList) {
        if (organizer.organizerId == currentUser.userId) {
            currentWeddingId = organizer.currentWeddingId;
            break;
        }
    }

    if (currentWeddingId.empty()) {
        cout << "No active wedding! Please create a wedding first." << endl;
        pauseScreen();
        return;
    }

    const WeddingEvent* wedding = nullptr;
    for (const auto& event : events) {
        if (event.eventId == currentWeddingId && event.status != "cancelled") {
            wedding = &event;
            break;
        }
    }

    if (!wedding) {
        cout << "Wedding not found or cancelled!" << endl;
        pauseScreen();
        return;
    }

    clearScreen();
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

    SetConsoleTextAttribute(hConsole, 14);
    cout << "==================================================\n";
    cout << "           WEDDING INVITATION CARD\n";
    cout << "==================================================\n\n";

    SetConsoleTextAttribute(hConsole, 14);
    cout << "    You're Cordially Invited To The Wedding Of \n\n";

    SetConsoleTextAttribute(hConsole, 11);
    cout << "         " << wedding->groomName << "\n";
    cout << "            &\n";
    cout << "         " << wedding->brideName << "\n\n";

    SetConsoleTextAttribute(hConsole, 10);
    cout << "   Date: " << wedding->weddingDate << "\n";
    cout << "   Time: 2:00 PM (Please arrive by 1:30 PM)\n";
    cout << "   Venue: " << wedding->weddingVenue << "\n";
    cout << "   Theme: " << wedding->weddingTheme << "\n\n";

    SetConsoleTextAttribute(hConsole, 13);
    cout << "    Dress Code: Formal Attire \n";
    cout << "    Reception to Follow Immediately After \n\n";

    SetConsoleTextAttribute(hConsole, 14);
    cout << "   RSVP: " << currentUser.userName << "\n";
    cout << "   Contact: " << wedding->weddingVenue << "\n\n";

    SetConsoleTextAttribute(hConsole, 15);
    cout << "==================================================\n";
    cout << "   Your presence is the greatest gift we could ask for!\n";
    cout << "==================================================\n\n";

    ofstream inviteFile("invitation_" + wedding->eventId + ".txt");
    if (inviteFile) {
        inviteFile << "==================================================\n";
        inviteFile << "           WEDDING INVITATION CARD\n";
        inviteFile << "==================================================\n\n";
        inviteFile << "   You're Cordially Invited To The Wedding Of\n\n";
        inviteFile << "         " << wedding->groomName << "\n";
        inviteFile << "            &\n";
        inviteFile << "         " << wedding->brideName << "\n\n";
        inviteFile << "   Date: " << wedding->weddingDate << "\n";
        inviteFile << "   Time: 2:00 PM\n";
        inviteFile << "   Venue: " << wedding->weddingVenue << "\n";
        inviteFile << "   Theme: " << wedding->weddingTheme << "\n\n";
        inviteFile << "   Dress Code: Formal Attire\n";
        inviteFile << "   Reception to Follow Immediately After\n\n";
        inviteFile << "   RSVP: " << currentUser.userName << "\n";
        inviteFile << "   Contact: " << wedding->weddingVenue << "\n\n";
        inviteFile << "==================================================\n";
        inviteFile.close();

        cout << "Invitation card saved to: invitation_" << wedding->eventId << ".txt\n";
    }

    SetConsoleTextAttribute(hConsole, 7);
    pauseScreen();
}
