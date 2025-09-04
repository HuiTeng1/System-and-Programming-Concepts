#pragma once

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
#include <conio.h>

using namespace std;

using namespace std;

// Forward declarations
struct WeddingEvent;
struct CurrentUser;
struct Vendor;
struct Organizer;
struct Admin;
struct PaymentTransaction;
struct Service;
struct Product;
struct BaseInfo;

// Struct definitions
struct Service {
    string serviceId;
    string serviceName;
    string description;
    string type;
    double price;
    int quantity;
    bool available;

    string toFileString() const;
    static Service fromFileString(string& str);
};

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

struct Product {
    string productId;
    string productName;
    string description;
    string type;
    int quantity;
    double price;
    bool available;

    string toFileString() const;
    static Product fromFileString(string& str);
};

struct BaseInfo {
    string name;
    string email;
    string phoneNum;
    string password;

    string toFileString() const;
    static BaseInfo fromFileString(string& str);
};

struct Vendor {
    BaseInfo baseInfo;
    string vendorId;
    string companyName;
    string companyContactNum;
    string type;
    vector<Service> serviceHasProvide;
    vector<Product> productHasProvide;
    int totalServicesProvided;
    int totalProductProvided;

    string toFileString() const;
    static Vendor fromFileString(string& str);
};

struct Organizer {
    BaseInfo baseInfo;
    string organizerId;
    string currentWeddingId;
    vector<int> bookedServices;

    string toFileString() const;
    static Organizer fromFileString(string& str);
};

struct Admin {
    BaseInfo baseInfo;
    string adminId;

    string toFileString() const;
    static Admin fromFileString(string& str);
};

enum UserType {
    NONE,
    ADMIN,
    ORGANIZER,
    VENDOR
};

struct CurrentUser {
    UserType type = NONE;
    int userIndex = -1;
    string userId = "";
    string userName = "";
    string currentWeddingId;
};

struct PaymentTransaction {
    string transactionId;
    string userId;
    string userName;
    string weddingId;
    double amount;
    string paymentMethod;
    string paymentStatus;
    string transactionDate;
    string transactionTime;

    string toFileString() const;
    static PaymentTransaction fromFileString(string& str);
};

// Function prototypes
string generateId(const string& prefix, size_t counter);
void clearScreen();
void pauseScreen();
bool isValidEmail(const string& email);
bool isValidPhoneNumber(const string& phone);
bool isValidPassword(const string& password);
bool isValidName(const string& name);
bool isValidBudget(double budget);
bool isValidDate(const string& date);
bool isValidWeddingDate(const string& date);
bool isDateAvailable(const string& date, const vector<WeddingEvent>& events, const string& venue = "");
double calculateServiceCost(const vector<int>& serviceIds, const vector<Vendor>& vendorList);
void loadEventsFromFile(vector<WeddingEvent>& events, const string& filename);
void saveEventsToFile(const vector<WeddingEvent>& events, const string& filename);
void createNewWedding(CurrentUser& currentUser, vector<WeddingEvent>& events, vector<Vendor>& vendorList, vector<Organizer>& organizerList);
void bookServicesForWedding(CurrentUser& currentUser, vector<WeddingEvent>& events, vector<Vendor>& vendorList, vector<Organizer>& organizerList);
void viewAllWeddings(CurrentUser& currentUser, const vector<WeddingEvent>& events, const vector<Vendor>& vendorList);
void manageMyWeddings(CurrentUser& currentUser, vector<WeddingEvent>& events, vector<Vendor>& vendorList, vector<Organizer>& organizerList);
void generateInvitationCard(const CurrentUser& currentUser, const vector<WeddingEvent>& events, const vector<Organizer>& organizerList);
void viewPaymentHistory(CurrentUser& currentUser);
void paymentAndReportingMenu(CurrentUser& currentUser, WeddingEvent* selectedEvent, vector<WeddingEvent>& events, const vector<Vendor>& vendorList);
void viewPaymentSummary(const CurrentUser& currentUser, const WeddingEvent* selectedEvent, const vector<Vendor>& vendorList);
void generateReport(const CurrentUser& currentUser, const WeddingEvent* selectedEvent, const vector<Vendor>& vendorList);
void getBaseUserInfo(BaseInfo& baseInfo);
void getVendorInfo(Vendor& vendor, vector<Vendor>& vendorList);
void getOrganizerInfo(Organizer& organizer, vector<Organizer>& organizerList);
bool getAdminInfo(Admin& admin, vector<Admin>& adminList);
template <typename T>
void saveUserIntoFile(const vector<T>& data, const string& fileName);
template <typename T>
void loadUserFromFile(vector<T>& data, const string& fileName);
void loadAllData(vector<Vendor>& vendorList, vector<Organizer>& organizerList, vector<Admin>& adminList);
void saveAllData(vector<Vendor>& vendorList, vector<Organizer>& organizerList, vector<Admin>& adminList);
bool login(vector<Vendor>& vendorList, vector<Organizer>& organizerList, vector<Admin>& adminList, CurrentUser& currentUser);
void logout(CurrentUser& currentUser);
void addService(CurrentUser& currentUser, vector<Vendor>& vendorList);
void addProduct(CurrentUser& currentUser, vector<Vendor>& vendorList);
void displayUserProfile(CurrentUser& currentUser, vector<Vendor>& vendorList, vector<Organizer>& organizerList, vector<Admin>& adminList);
void displayAllServices(vector<Vendor>& vendorList);
void userRegister(vector<Vendor>& vendorList, vector<Organizer>& organizerList, vector<Admin>& adminList);
void organizerMenu(CurrentUser& currentUser, vector<Vendor>& vendorList, vector<Organizer>& organizerList, vector<Admin>& adminList, vector<WeddingEvent>& events);
void adminMenu(CurrentUser& currentUser, vector<Vendor>& vendorList, vector<Organizer>& organizerList, vector<Admin>& adminList, vector<WeddingEvent>& events);
void vendorMenu(CurrentUser& currentUser, vector<Vendor>& vendorList, vector<Organizer>& organizerList, vector<Admin>& adminList);
void mainMenu(CurrentUser& currentUser, vector<Vendor>& vendorList, vector<Organizer>& organizerList, vector<Admin>& adminList, vector<WeddingEvent>& events);
