#ifndef WEDDING_MANAGEMENT_SYSTEM_H
#define WEDDING_MANAGEMENT_SYSTEM_H

#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <fstream>
#include <iomanip>
#include <algorithm>
#include <limits>
#include <cctype>

using namespace std;

// ==================== Data Structures ====================

struct Service {
    string serviceId;
    string serviceName;
    string description;
    string type; // e.g., planning, food
    double price;
    int quantity;
    bool available;

    string toFileString();
    static Service fromFileString(const string &str);
};

struct BaseInfo {
    string name;
    string email;
    string phoneNum;
    string password;

    string toFileString();
    static BaseInfo fromFileString(const string &str);
};

struct Vendor {
    BaseInfo baseInfo;
    string vendorId;
    string companyName;
    string companyContactNum;
    string type; // e.g., individual, company
    vector<Service> serviceHasProvide;
    int totalServicesProvided;

    string toFileString();
    static Vendor fromFileString(const string &str);
};

struct Organizer {
    BaseInfo baseInfo;
    string organizerId;
    string currentWeddingId;
    vector<int> bookedServices;

    string toFileString();
    static Organizer fromFileString(const string &str);
};

struct Admin {
    BaseInfo baseInfo;
    string adminId;

    string toFileString();
    static Admin fromFileString(const string &str);
};

// ==================== User Management ====================

enum UserType { NONE, ADMIN, ORGANIZER, VENDOR };

struct CurrentUser {
    UserType type = NONE;
    int userIndex = -1;
    string userId = "";
    string userName = "";
    string currentWeddingId;
};

// ==================== Function Prototypes ====================

string generateId(const string &prefix, int counter);
void clearScreen();
void pauseScreen();

void getBaseUserInfo(BaseInfo &baseInfo);
void getVendorInfo(Vendor &vendor, vector<Vendor> &vendorList);
void getOrganizerInfo(Organizer &organizer, vector<Organizer> &organizerList);
bool getAdminInfo(Admin &admin, vector<Admin> &adminList);

template <typename T>
void saveUserIntoFile(const vector<T> &data, const string &fileName);

template <typename T>
void loadUserFromFile(vector<T> &data, const string &fileName);

void loadAllData(vector<Vendor> &vendorList, vector<Organizer> &organizerList, vector<Admin> &adminList);
bool login(vector<Vendor> &vendorList, vector<Organizer> &organizerList, vector<Admin> &adminList, CurrentUser &currentUser);
void logout(CurrentUser &currentUser);
void userRegister(vector<Vendor> &vendorList, vector<Organizer> &organizerList, vector<Admin> &adminList);
void saveAllData(vector<Vendor> &vendorList, vector<Organizer> &organizerList, vector<Admin> &adminList);

void displayUserProfile(CurrentUser &currentUser, vector<Vendor> &vendorList, vector<Organizer> &organizerList, vector<Admin> &adminList);
bool deleteOwnAccount(CurrentUser &currentUser, vector<Vendor> &vendorList, vector<Organizer> &organizerList, vector<Admin> &adminList);
void updateBaseInfo(BaseInfo &baseInfo);
void updateAdminInfo(Admin &admin);
void updateOrganizerInfo(Organizer &organizer);
void updateVendorInfo(Vendor &vendor);

void vendorMenu(CurrentUser &currentUser, vector<Vendor> &vendorList, vector<Organizer> &organizerList, vector<Admin> &adminList);
void organizerMenu(CurrentUser &currentUser, vector<Vendor> &vendorList, vector<Organizer> &organizerList, vector<Admin> &adminList);
void adminMenu(CurrentUser &currentUser, vector<Vendor> &vendorList, vector<Organizer> &organizerList, vector<Admin> &adminList);
void mainMenu(CurrentUser &currentUser, vector<Vendor> &vendorList, vector<Organizer> &organizerList, vector<Admin> &adminList);

void addService(CurrentUser &currentUser, vector<Vendor> &vendorList);
void updateUserProfile(CurrentUser &currentUser, vector<Vendor> &vendorList, vector<Organizer> &organizerList, vector<Admin> &adminList);
void displayAllServices(vector<Vendor> &vendorList);
void displayServicesByVendor(vector<Vendor> &vendorList);
void displayServicesByType(vector<Vendor> &vendorList);
void displayBookedServices(CurrentUser &currentUser, vector<Organizer> &organizerList, vector<Vendor> &vendorList);
void deleteOwnService(CurrentUser &currentUser, vector<Vendor> &vendorList);
void UpdateWeddingMenu(CurrentUser &currentUser, vector<Vendor> &vendorList, vector<Organizer> &organizerList, vector<Admin> &adminList);
void organizerMenu(CurrentUser &currentUser, vector<Vendor> &vendorList, vector<Organizer> &organizerList, vector<Admin> &adminList);

#endif // WEDDING_MANAGEMENT_SYSTEM_H