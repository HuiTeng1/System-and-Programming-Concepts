#include <iostream>
#include <vector>
#include "UserModule.h"
#include "EventModule.h"
#include "monitoring.h"
#include "Payment.h"
#include "UserModule.h" // Ensure this header declares saveAllData


using namespace std;

int main() {
    vector<Vendor> vendorList;
    vector<Organizer> organizerList;
    vector<Admin> adminList;
    vector<WeddingEvent> events;
    vector<Participant> participants;
    CurrentUser currentUser;

    // Load data from files
    loadAllData(vendorList, organizerList, adminList);
    loadEventsFromFile(events, "events.txt");
    addDefaultParticipants(participants);

    // Start main menu
    mainMenu(currentUser, vendorList, organizerList, adminList, events, participants);

    // Save data before exit
    saveAllData(vendorList, organizerList, adminList);
    saveEventsToFile(events, "events.txt");

    return 0;
}