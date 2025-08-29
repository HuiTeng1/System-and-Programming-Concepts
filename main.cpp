#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <fstream>
#include <iomanip>
#include <algorithm>
#include <limits>
using namespace std;


struct Service {
    string serviceName;
    string description;
    string type; ///not sure -- like for planning , food 
    double price;
    int quantity;
    bool available;
};

struct Product {
    string productName;
    string description;
    string type; // not sure
    string type;
    int quantity;
    double price;
};

struct Vendor {
    string vendorId;
    string name; // use companyName or vendor he own name?
    string contactNum;
    string type; // not sure will use or not
    vector<Service> serviceHasProvide;
    vector<Product> productHasProvide;
};

struct Organization {
    string name;
    string contactNum;
    string email;
    string ic;
    string address;
};

struct Date {

    int day;
    int month;
    int year;

};

struct LoginCredentials {

    string username;
    string password;
    string lastLogin;
};

struct Client {

    string clientId;
    string name;
    string email;
    string phone;
    Date weddingDate;
    Venue venue;
    Catering catering;
    double totalCost = 0.0;
    LoginCredentials login;
};

struct Venue {

    int id;
    string name;
    string location;
    int capacity;
    double price;
};

struct Catering {

    int id;
    string packageName;
    string menu;
    int guests;
    double price;
};


vector<Venue> venues;
vector<Client> clients;
vector<Catering> cateringPackages;
Client* currentUser = nullptr;


void initializeData() {

    // use for initial venue for the structure

    venues = {

        {1, "Emperor Palace ", "Kuala Lumpur", 300, 15000.00},
        {2, "Lexis Hotel", "Selangor", 150, 8000.00},
        {3, "Avillion Cove Resort ", "Penang", 200, 12000.00},
        {4, "The First Hotel", "Johor Bahru", 250, 10000.00},

    };

    cateringPackages = {

        {1, "Basic Package", "Buffet Dinner", 50, 2000.00},
        {2, "Premium Package", "5 Course Meal", 50, 5000.00},
        {3, "VIP Package", "7 Course Gourmet", 50, 9000.00},
        {4, "Emperor Package", "12 Course Gourmet", 50, 12000.00}
    };
}




// Event Registeration
void eventRegisteration() {


    cout << " <-------- Event Registeration Menu --------> " << endl;
    cout << endl;
    cout << endl;

    // Venue 

    cout << " ===== Available Venues ===== " << endl;
    for (const auto& venue : venues) {
        cout << venue.id << ". " << venue.name << " (" << venue.location
            << ") - Capacity: " << venue.capacity
            << " - Price: " << formatCurrency(venue.price) << endl;
    }

    int venueChoice;

    do {

        cout << "Please select the venue: " << venues.size() << "): ";
        cin >> venueChoice;

    } while (venueChoice < 1 || venueChoice > venues.size());


    // Catering Menu

    cout << " ===== Catering Menu ===== " << endl;
    for (const auto& catering : cateringPackages) {
        cout << catering.id << ". " << catering.packageName
            << " - Menu: " << catering.menu
            << " - Price per 50 guests: " << formatCurrency(catering.price) << endl;
    }

    int cateringChoice;

    do {

        cout << "Select catering package (1-" << cateringPackages.size() << "): ";
        cin >> cateringChoice;

    } while (cateringChoice < 1 || cateringChoice > cateringPackages.size());

    // This is for the calculation price for the catering menu...

    int guestCount;

    do {
        cout << "Enter number of guests: ";
        cin >> guestCount;
    } while (guestCount < 1 || guestCount > currentUser->venue.capacity);

}


void viewAllBookings() {

    if (clients.empty()) {

        cout << "\nNo bookings found!\n";
        return;

    }

    cout << "\n========== ALL WEDDING BOOKINGS ==========\n";

    for (const auto& client : clients) {

        if (client.weddingDate.day != 0) {
            cout << "Client ID: " << client.clientId << endl;
            cout << "Name: " << client.name << endl;
            cout << "Wedding Date: " << dateToString(client.weddingDate) << endl;
            cout << "Venue: " << client.venue.name << endl;
            cout << "Catering: " << client.catering.packageName << endl;
            cout << "Total Cost: " << formatCurrency(client.totalCost) << endl;
            cout << "----------------------------------------\n";
        }

    }
}



// This is function for formatting value

string formatCurrency(double amount) {

    stringstream ss;
    ss << fixed << setprecision(2) << "RM" << amount;
    return ss.str();

}

string dateToString(const Date& date) {

    return to_string(date.day) + "/" + to_string(date.month) + "/" + to_string(date.year);

}





