#ifndef EVENTMODULE_H
#define EVENTMODULE_H

#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <fstream>
#include <iomanip>
#include <algorithm>
#include <limits>
#include <cctype>
#include <ctime>
#include <chrono>
#include <windows.h>
using namespace std;

// Forward declarations
struct WeddingEvent;
struct CurrentUser;
struct Vendor;
struct Organizer;
struct Admin;

// Event-related utility functions declarations
bool isValidDate(const string& date);
bool isDateAvailable(const string& date, const vector<WeddingEvent>& events, const string& venue = "");
double calculateServiceCost(const vector<int>& serviceIds, const vector<Vendor>& vendorList);
void loadEventsFromFile(vector<WeddingEvent>& events, const string& filename);
void saveEventsToFile(const vector<WeddingEvent>& events, const string& filename);
void createNewWedding(CurrentUser& currentUser, vector<WeddingEvent>& events,
    vector<Vendor>& vendorList, vector<Organizer>& organizerList);
void bookServicesForWedding(CurrentUser& currentUser, vector<WeddingEvent>& events,
    vector<Vendor>& vendorList, vector<Organizer>& organizerList);
void viewAllWeddings(CurrentUser& currentUser, const vector<WeddingEvent>& events,
    const vector<Vendor>& vendorList);
void manageMyWeddings(CurrentUser& currentUser, vector<WeddingEvent>& events,
    vector<Vendor>& vendorList, vector<Organizer>& organizerList);
void generateInvitationCard(const CurrentUser& currentUser, const vector<WeddingEvent>& events,
    const vector<Organizer>& organizerList);

// Enhanced validation functions
bool isValidWeddingDate(const string& date);

// WeddingEvent structure
struct WeddingEvent {
    string eventId;
    string organizerId;
    string groomName;
    string brideName;
    string weddingDate;
    string weddingVenue;
    string weddingTheme;
    double budget;
    double totalCost;
    string status;
    vector<int> bookedServices;

    string toFileString() const;
    static WeddingEvent fromFileString(string& str);
};

#endif // EVENTMODULE_H
