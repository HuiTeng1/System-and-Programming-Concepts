#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <fstream>
#include <iomanip>
#include <algorithm>
#include <limits>
#include <ctime>
#include <iomanip>
using namespace std;

// ... [Previous struct definitions remain the same until Event-related structs] ...

struct Date {
    int day;
    int month;
    int year;
    
    string toString() const {
        return to_string(day) + "/" + to_string(month) + "/" + to_string(year);
    }
    
    bool isValid() const {
        if (month < 1 || month > 12) return false;
        if (day < 1) return false;
        
        int daysInMonth[] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
        if (month == 2 && (year % 4 == 0 && (year % 100 != 0 || year % 400 == 0))) {
            return day <= 29;
        }
        return day <= daysInMonth[month - 1];
    }
    
    bool isFutureDate() const {
        time_t now = time(0);
        tm* currentTime = localtime(&now);
        int currentYear = currentTime->tm_year + 1900;
        int currentMonth = currentTime->tm_mon + 1;
        int currentDay = currentTime->tm_mday;
        
        if (year > currentYear) return true;
        if (year == currentYear && month > currentMonth) return true;
        if (year == currentYear && month == currentMonth && day > currentDay) return true;
        return false;
    }
};

struct Venue {
    int id;
    string name;
    string location;
    int capacity;
    double price;
    vector<Date> bookedDates;
    
    bool isAvailable(const Date& date) const {
        for (const auto& bookedDate : bookedDates) {
            if (bookedDate.day == date.day && bookedDate.month == date.month && bookedDate.year == date.year) {
                return false;
            }
        }
        return true;
    }
    
    string toFileString() const {
        string bookedDatesStr;
        for (const auto& date : bookedDates) {
            if (!bookedDatesStr.empty()) bookedDatesStr += "#";
            bookedDatesStr += to_string(date.day) + "," + to_string(date.month) + "," + to_string(date.year);
        }
        return to_string(id) + "|||" + name + "|||" + location + "|||" + 
               to_string(capacity) + "|||" + to_string(price) + "|||" + bookedDatesStr;
    }
    
    static Venue fromFileString(const string& str) {
        Venue v;
        stringstream ss(str);
        string temp;
        
        getline(ss, temp, '|'); v.id = stoi(temp); ss.ignore(2);
        getline(ss, v.name, '|'); ss.ignore(2);
        getline(ss, v.location, '|'); ss.ignore(2);
        getline(ss, temp, '|'); v.capacity = stoi(temp); ss.ignore(2);
        getline(ss, temp, '|'); v.price = stod(temp); ss.ignore(2);
        
        // Parse booked dates
        string datesStr;
        getline(ss, datesStr);
        if (!datesStr.empty()) {
            stringstream datesSS(datesStr);
            string dateStr;
            while (getline(datesSS, dateStr, '#')) {
                if (!dateStr.empty()) {
                    stringstream dateSS(dateStr);
                    string part;
                    Date date;
                    getline(dateSS, part, ','); date.day = stoi(part);
                    getline(dateSS, part, ','); date.month = stoi(part);
                    getline(dateSS, part); date.year = stoi(part);
                    v.bookedDates.push_back(date);
                }
            }
        }
        
        return v;
    }
};

struct CateringPackage {
    int id;
    string packageName;
    string description;
    int minGuests;
    int maxGuests;
    double basePrice;
    double pricePerGuest;
    
    string toFileString() const {
        return to_string(id) + "|||" + packageName + "|||" + description + "|||" + 
               to_string(minGuests) + "|||" + to_string(maxGuests) + "|||" + 
               to_string(basePrice) + "|||" + to_string(pricePerGuest);
    }
    
    static CateringPackage fromFileString(const string& str) {
        CateringPackage cp;
        stringstream ss(str);
        string temp;
        
        getline(ss, temp, '|'); cp.id = stoi(temp); ss.ignore(2);
        getline(ss, cp.packageName, '|'); ss.ignore(2);
        getline(ss, cp.description, '|'); ss.ignore(2);
        getline(ss, temp, '|'); cp.minGuests = stoi(temp); ss.ignore(2);
        getline(ss, temp, '|'); cp.maxGuests = stoi(temp); ss.ignore(2);
        getline(ss, temp, '|'); cp.basePrice = stod(temp); ss.ignore(2);
        getline(ss, temp); cp.pricePerGuest = stod(temp);
        
        return cp;
    }
};

struct EventBooking {
    string bookingId;
    string organizerId;
    string eventType;
    Date eventDate;
    int venueId;
    int cateringId;
    int numberOfGuests;
    double totalCost;
    string status; // "pending", "confirmed", "cancelled"
    vector<int> serviceIds;
    
    string toFileString() const {
        string serviceIdsStr;
        for (size_t i = 0; i < serviceIds.size(); ++i) {
            serviceIdsStr += to_string(serviceIds[i]);
            if (i < serviceIds.size() - 1) serviceIdsStr += ",";
        }
        
        return bookingId + "|||" + organizerId + "|||" + eventType + "|||" + 
               to_string(eventDate.day) + "," + to_string(eventDate.month) + "," + to_string(eventDate.year) + "|||" + 
               to_string(venueId) + "|||" + to_string(cateringId) + "|||" + 
               to_string(numberOfGuests) + "|||" + to_string(totalCost) + "|||" + 
               status + "|||" + serviceIdsStr;
    }
    
    static EventBooking fromFileString(const string& str) {
        EventBooking eb;
        stringstream ss(str);
        string temp;
        
        getline(ss, eb.bookingId, '|'); ss.ignore(2);
        getline(ss, eb.organizerId, '|'); ss.ignore(2);
        getline(ss, eb.eventType, '|'); ss.ignore(2);
        
        // Parse date
        string dateStr;
        getline(ss, dateStr, '|');
        stringstream dateSS(dateStr);
        getline(dateSS, temp, ','); eb.eventDate.day = stoi(temp);
        getline(dateSS, temp, ','); eb.eventDate.month = stoi(temp);
        getline(dateSS, temp); eb.eventDate.year = stoi(temp);
        ss.ignore(2);
        
        getline(ss, temp, '|'); eb.venueId = stoi(temp); ss.ignore(2);
        getline(ss, temp, '|'); eb.cateringId = stoi(temp); ss.ignore(2);
        getline(ss, temp, '|'); eb.numberOfGuests = stoi(temp); ss.ignore(2);
        getline(ss, temp, '|'); eb.totalCost = stod(temp); ss.ignore(2);
        getline(ss, eb.status, '|'); ss.ignore(2);
        
        // Parse service IDs
        string servicesStr;
        getline(ss, servicesStr);
        if (!servicesStr.empty()) {
            stringstream servicesSS(servicesStr);
            string serviceIdStr;
            while (getline(servicesSS, serviceIdStr, ',')) {
                if (!serviceIdStr.empty()) {
                    eb.serviceIds.push_back(stoi(serviceIdStr));
                }
            }
        }
        
        return eb;
    }
};

// Global vectors for event management
vector<Venue> venues;
vector<CateringPackage> cateringPackages;
vector<EventBooking> eventBookings;

// Function declarations
void initializeEventData();
void saveEventData();
void loadEventData();
void eventRegistrationMenu(CurrentUser& currentUser, vector<Organizer>& organizerList, vector<Vendor>& vendorList);
void eventBookingMenu(CurrentUser& currentUser, vector<Organizer>& organizerList, vector<Vendor>& vendorList);
void viewMyBookings(CurrentUser& currentUser);
void manageVenues(CurrentUser& currentUser);
void manageCatering(CurrentUser& currentUser);
Date getDateInput(const string& prompt);
bool validateDate(const Date& date);
double calculateCateringCost(int cateringId, int guests);
void displayAvailableVenues(const Date& date);
void displayCateringPackages();

// Initialize event data
void initializeEventData() {
    venues = {
        {1, "Emperor Palace", "Kuala Lumpur", 300, 15000.00, {}},
        {2, "Lexis Hotel", "Selangor", 150, 8000.00, {}},
        {3, "Avillion Cove Resort", "Penang", 200, 12000.00, {}},
        {4, "The First Hotel", "Johor Bahru", 250, 10000.00, {}}
    };

    cateringPackages = {
        {1, "Basic Package", "Buffet Dinner with basic beverages", 50, 200, 2000.00, 40.00},
        {2, "Premium Package", "5 Course Meal with premium beverages", 30, 150, 5000.00, 80.00},
        {3, "VIP Package", "7 Course Gourmet Meal with fine wines", 20, 100, 9000.00, 150.00},
        {4, "Emperor Package", "12 Course Gourmet Experience", 10, 50, 12000.00, 250.00}
    };
}

// Save event data to files
void saveEventData() {
    ofstream venueFile("venues.txt");
    ofstream cateringFile("catering.txt");
    ofstream bookingFile("bookings.txt");
    
    for (const auto& venue : venues) {
        venueFile << venue.toFileString() << endl;
    }
    
    for (const auto& catering : cateringPackages) {
        cateringFile << catering.toFileString() << endl;
    }
    
    for (const auto& booking : eventBookings) {
        bookingFile << booking.toFileString() << endl;
    }
    
    venueFile.close();
    cateringFile.close();
    bookingFile.close();
}

// Load event data from files
void loadEventData() {
    ifstream venueFile("venues.txt");
    ifstream cateringFile("catering.txt");
    ifstream bookingFile("bookings.txt");
    
    venues.clear();
    cateringPackages.clear();
    eventBookings.clear();
    
    string line;
    while (getline(venueFile, line)) {
        if (!line.empty()) {
            venues.push_back(Venue::fromFileString(line));
        }
    }
    
    while (getline(cateringFile, line)) {
        if (!line.empty()) {
            cateringPackages.push_back(CateringPackage::fromFileString(line));
        }
    }
    
    while (getline(bookingFile, line)) {
        if (!line.empty()) {
            eventBookings.push_back(EventBooking::fromFileString(line));
        }
    }
    
    venueFile.close();
    cateringFile.close();
    bookingFile.close();
}

// Event Registration Module
void eventRegistrationMenu(CurrentUser& currentUser, vector<Organizer>& organizerList, vector<Vendor>& vendorList) {
    if (currentUser.type != ORGANIZER) {
        cout << "This feature is only available for organizers!" << endl;
        pauseScreen();
        return;
    }
    
    int choice;
    do {
        clearScreen();
        cout << "=== EVENT REGISTRATION MENU ===" << endl;
        cout << "1. Create New Event Booking" << endl;
        cout << "2. View My Bookings" << endl;
        cout << "3. Cancel Booking" << endl;
        cout << "4. Return to Main Menu" << endl;
        cout << "Enter your choice: ";
        cin >> choice;
        cin.ignore();
        
        switch (choice) {
            case 1:
                eventBookingMenu(currentUser, organizerList, vendorList);
                break;
            case 2:
                viewMyBookings(currentUser);
                break;
            case 3:
                // Cancel booking function would go here
                cout << "Cancel booking feature coming soon!" << endl;
                pauseScreen();
                break;
            case 4:
                return;
            default:
                cout << "Invalid choice!" << endl;
                pauseScreen();
        }
    } while (choice != 4);
}

// Event Booking Module
void eventBookingMenu(CurrentUser& currentUser, vector<Organizer>& organizerList, vector<Vendor>& vendorList) {
    clearScreen();
    cout << "=== CREATE NEW EVENT BOOKING ===" << endl;
    
    EventBooking newBooking;
    newBooking.bookingId = generateId("B", eventBookings.size() + 1);
    newBooking.organizerId = currentUser.userId;
    newBooking.status = "pending";
    
    // Get event type
    cout << "Enter event type (wedding, conference, birthday, etc.): ";
    getline(cin, newBooking.eventType);
    
    // Get event date
    newBooking.eventDate = getDateInput("Enter event date (DD MM YYYY): ");
    if (!validateDate(newBooking.eventDate)) {
        cout << "Invalid date or date is not in the future!" << endl;
        pauseScreen();
        return;
    }
    
    // Venue selection
    displayAvailableVenues(newBooking.eventDate);
    cout << "Select venue ID: ";
    cin >> newBooking.venueId;
    cin.ignore();
    
    // Validate venue selection
    bool validVenue = false;
    for (auto& venue : venues) {
        if (venue.id == newBooking.venueId && venue.isAvailable(newBooking.eventDate)) {
            validVenue = true;
            venue.bookedDates.push_back(newBooking.eventDate);
            break;
        }
    }
    
    if (!validVenue) {
        cout << "Invalid venue selection or venue not available on selected date!" << endl;
        pauseScreen();
        return;
    }
    
    // Catering selection
    displayCateringPackages();
    cout << "Select catering package ID: ";
    cin >> newBooking.cateringId;
    cin.ignore();
    
    // Validate catering selection
    bool validCatering = false;
    for (const auto& catering : cateringPackages) {
        if (catering.id == newBooking.cateringId) {
            validCatering = true;
            break;
        }
    }
    
    if (!validCatering) {
        cout << "Invalid catering package selection!" << endl;
        pauseScreen();
        return;
    }
    
    // Number of guests
    cout << "Enter number of guests: ";
    cin >> newBooking.numberOfGuests;
    cin.ignore();
    
    // Calculate total cost
    double venueCost = 0;
    for (const auto& venue : venues) {
        if (venue.id == newBooking.venueId) {
            venueCost = venue.price;
            break;
        }
    }
    
    double cateringCost = calculateCateringCost(newBooking.cateringId, newBooking.numberOfGuests);
    newBooking.totalCost = venueCost + cateringCost;
    
    // Display summary
    clearScreen();
    cout << "=== BOOKING SUMMARY ===" << endl;
    cout << "Event Type: " << newBooking.eventType << endl;
    cout << "Date: " << newBooking.eventDate.toString() << endl;
    cout << "Venue ID: " << newBooking.venueId << endl;
    cout << "Catering Package ID: " << newBooking.cateringId << endl;
    cout << "Number of Guests: " << newBooking.numberOfGuests << endl;
    cout << "Total Cost: RM " << fixed << setprecision(2) << newBooking.totalCost << endl;
    cout << "=======================" << endl;
    
    char confirm;
    cout << "Confirm booking? (y/n): ";
    cin >> confirm;
    cin.ignore();
    
    if (tolower(confirm) == 'y') {
        newBooking.status = "confirmed";
        eventBookings.push_back(newBooking);
        saveEventData();
        cout << "Booking confirmed successfully!" << endl;
    } else {
        // Remove venue booking if cancelled
        for (auto& venue : venues) {
            if (venue.id == newBooking.venueId) {
                auto it = find(venue.bookedDates.begin(), venue.bookedDates.end(), newBooking.eventDate);
                if (it != venue.bookedDates.end()) {
                    venue.bookedDates.erase(it);
                }
                break;
            }
        }
        cout << "Booking cancelled." << endl;
    }
    
    pauseScreen();
}

// View user's bookings
void viewMyBookings(CurrentUser& currentUser) {
    clearScreen();
    cout << "=== MY EVENT BOOKINGS ===" << endl;
    
    bool found = false;
    for (const auto& booking : eventBookings) {
        if (booking.organizerId == currentUser.userId) {
            found = true;
            cout << "Booking ID: " << booking.bookingId << endl;
            cout << "Event Type: " << booking.eventType << endl;
            cout << "Date: " << booking.eventDate.toString() << endl;
            cout << "Venue ID: " << booking.venueId << endl;
            cout << "Catering ID: " << booking.cateringId << endl;
            cout << "Guests: " << booking.numberOfGuests << endl;
            cout << "Total Cost: RM " << fixed << setprecision(2) << booking.totalCost << endl;
            cout << "Status: " << booking.status << endl;
            cout << "------------------------" << endl;
        }
    }
    
    if (!found) {
        cout << "No bookings found!" << endl;
    }
    
    pauseScreen();
}

// Helper functions
Date getDateInput(const string& prompt) {
    Date date;
    cout << prompt;
    cin >> date.day >> date.month >> date.year;
    cin.ignore();
    return date;
}

bool validateDate(const Date& date) {
    return date.isValid() && date.isFutureDate();
}

double calculateCateringCost(int cateringId, int guests) {
    for (const auto& catering : cateringPackages) {
        if (catering.id == cateringId) {
            if (guests < catering.minGuests) guests = catering.minGuests;
            if (guests > catering.maxGuests) guests = catering.maxGuests;
            return catering.basePrice + (guests * catering.pricePerGuest);
        }
    }
    return 0.0;
}

void displayAvailableVenues(const Date& date) {
    cout << "\n=== AVAILABLE VENUES ON " << date.toString() << " ===" << endl;
    for (const auto& venue : venues) {
        if (venue.isAvailable(date)) {
            cout << "ID: " << venue.id << " | " << venue.name 
                 << " | " << venue.location << " | Capacity: " << venue.capacity
                 << " | Price: RM " << fixed << setprecision(2) << venue.price << endl;
        }
    }
}

void displayCateringPackages() {
    cout << "\n=== CATERING PACKAGES ===" << endl;
    for (const auto& catering : cateringPackages) {
        cout << "ID: " << catering.id << " | " << catering.packageName 
             << " | " << catering.description << " | Guests: " << catering.minGuests 
             << "-" << catering.maxGuests << " | Base: RM " << fixed << setprecision(2) << catering.basePrice
             << " | Per Guest: RM " << catering.pricePerGuest << endl;
    }
}

// Add these to your main menu options
// In your main function or main menu, add:
// case 4: eventRegistrationMenu(currentUser, organizerList, vendorList); break;
// case 5: // Event management for admin would go here
