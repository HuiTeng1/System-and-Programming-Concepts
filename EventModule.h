#ifndef WEDDING_EVENT_H
#define WEDDING_EVENT_H

#include <string>
#include <vector>


// Forward declarations
struct Vendor;
struct Organizer;
struct CurrentUser;

// WeddingEvent structure
struct WeddingEvent {
    std::string eventId;
    std::string organizerId;
    std::string groomName;
    std::string brideName;
    std::string weddingDate;
    std::string weddingVenue;
    std::string weddingTheme;
    double budget;
    double totalCost;
    std::string status;
    std::vector<int> bookedServices;

    std::string toFileString() const;
    static WeddingEvent fromFileString(std::string& str);
};

// Event utility functions
bool isValidDate(const std::string& date);
bool isDateAvailable(const std::string& date, const std::vector<WeddingEvent>& events, const std::string& venue = "");
double calculateServiceCost(const std::vector<int>& serviceIds, const std::vector<Vendor>& vendorList);
void loadEventsFromFile(std::vector<WeddingEvent>& events, const std::string& filename);
void saveEventsToFile(const std::vector<WeddingEvent>& events, const std::string& filename);
void createNewWedding(CurrentUser& currentUser, std::vector<WeddingEvent>& events,
    std::vector<Vendor>& vendorList, std::vector<Organizer>& organizerList);
void bookServicesForWedding(CurrentUser& currentUser, std::vector<WeddingEvent>& events,
    std::vector<Vendor>& vendorList, std::vector<Organizer>& organizerList);
void viewAllWeddings(CurrentUser& currentUser, const std::vector<WeddingEvent>& events,
    const std::vector<Vendor>& vendorList);
void manageMyWeddings(CurrentUser& currentUser, std::vector<WeddingEvent>& events,
    std::vector<Vendor>& vendorList, std::vector<Organizer>& organizerList);
void generateInvitationCard(const CurrentUser& currentUser, const std::vector<WeddingEvent>& events,
    const std::vector<Organizer>& organizerList);

#endif // WEDDING_EVENT_H
