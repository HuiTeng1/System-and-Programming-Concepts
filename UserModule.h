#ifndef USERMODULE_H
#define USERMODULE_H

#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <fstream>
#include <iomanip>
#include <algorithm>
#include <limits>
#include <cctype>

// Forward declarations for any external types
struct WeddingEvent;
struct Participant;

// Structures
struct Service {
    string serviceId;
    string serviceName;
    string description;
    string type;
    double price;
    int quantity;
    bool available;
    string toFileString();
    static Service fromFileString(string &str);
};

struct BaseInfo {
    string name;
    string email;
    string phoneNum;
    string password;
    string toFileString();
    static BaseInfo fromFileString(string &str);
};

struct Vendor {
    BaseInfo baseInfo;
    string vendorId;
    string companyName;
    string companyContactNum;
    string type;
    vector<Service> serviceHasProvide;
    int totalServicesProvided;
    string toFileString();
    static Vendor fromFileString(string &str);
};

struct Organizer {
    BaseInfo baseInfo;
    string organizerId;
    string currentWeddingId;
    vector<int> bookedServices;
    string toFileString();
    static Organizer fromFileString(string &str);
};

struct Admin {
    BaseInfo baseInfo;
    string adminId;
    string toFileString();
    static Admin fromFileString(string &str);
};

enum UserType { NONE, ADMIN, ORGANIZER, VENDOR };

struct CurrentUser {
    UserType type;
    int userIndex; 
    string userId;
    string userName;
    string currentWeddingId;
};

// Function prototypes
string generateId(const string &prefix, int counter);

void clearScreen();
void pauseScreen();

void getBaseUserInfo(BaseInfo &baseInfo);
void getVendorInfo(Vendor &vendor, vector<Vendor> &vendorList);
void getOrganizerInfo(Organizer &organizer, vector<Organizer> &organizerList);
bool getAdminInfo(Admin &admin, vector<Admin> &adminList);
template <typename T>
void saveUserIntoFile(vector<T> &data, string fileName);
template <typename T>
void loadUserFromFile(vector<T> &data, string fileName);
template <typename T>
void saveUserIntoFile(vector<T> &data, string fileName){
    ofstream write(fileName);

    if (!write)
    {
        cout << "Error opening file." << endl;
        return;
    }
    // 'auto' means the compiler will automatically figure out the type of this variable based on its value
    for (auto &obj : data)
    {
        write << obj.toFileString() << endl;
    }
    write.close();
}
template void saveUserIntoFile<Vendor>(vector<Vendor> &data, string fileName);
template void saveUserIntoFile<Organizer>(vector<Organizer> &data, string fileName);
template void saveUserIntoFile<Admin>(vector<Admin> &data, string fileName);

template <typename T>
void loadUserFromFile(vector<T> &data, string fileName)
{
    ifstream read(fileName);
    if (!read)
        return; // File doesn't exist yet

    data.clear();
    string line;
    while (getline(read, line))
    {
        if (!line.empty())
        {
            try
            {
                data.push_back(T::fromFileString(line));
            }
            catch (...)
            {
                cout << "Error parsing line in " << fileName << endl;
            }
        }
    }
    read.close();
}
template void loadUserFromFile<Vendor>(vector<Vendor> &data, string fileName);
template void loadUserFromFile<Organizer>(vector<Organizer> &data, string fileName);
template void loadUserFromFile<Admin>(vector<Admin> &data, string fileName);


void loadAllData(vector<Vendor> &vendorList, vector<Organizer> &organizerList, vector<Admin> &adminList);

bool login(vector<Vendor> &vendorList, vector<Organizer> &organizerList, vector<Admin> &adminList, CurrentUser &currentUser);
void logout(CurrentUser &currentUser);

void addService(const CurrentUser &currentUser, vector<Vendor> &vendorList);
void saveAllData(vector<Vendor> &vendorList, vector<Organizer> &organizerList, vector<Admin> &adminList);
void displayUserProfile(CurrentUser &currentUser, vector<Vendor> &vendorList, vector<Organizer> &organizerList, vector<Admin> &adminList);
void listAllUsers(vector<Vendor> &vendorList, vector<Organizer> &organizerList, vector<Admin> &adminList);
void userRegister(vector<Vendor> &vendorList, vector<Organizer> &organizerList, vector<Admin> &adminList);

void updateBaseInfo(BaseInfo &baseInfo);
void updateAdminInfo(Admin &admin);
void updateOrganizerInfo(Organizer &organizer);
void updateVendorInfo(Vendor &vendor);
void updateUserProfile(const CurrentUser &currentUser, vector<Vendor> &vendorList, vector<Organizer> &organizerList, vector<Admin> &adminList);

bool deleteOwnAccount(const CurrentUser &currentUser, vector<Vendor> &vendorList, vector<Organizer> &organizerList, vector<Admin> &adminList);
void deleteOwnService(const CurrentUser &currentUser, vector<Vendor> &vendorList);

void mainMenu(const CurrentUser &currentUser, vector<Vendor> &vendorList, vector<Organizer> &organizerList, vector<Admin> &adminList, vector<WeddingEvent>& events, vector<Participant>& participants);
void organizerMenu(const CurrentUser &currentUser, vector<Vendor> &vendorList, vector<Organizer> &organizerList, vector<Admin> &adminList, vector<WeddingEvent> &events, vector<Participant> &participants);
void adminMenu(const CurrentUser &currentUser, vector<Vendor> &vendorList, vector<Organizer> &organizerList, vector<Admin> &adminList);
void vendorMenu(const CurrentUser &currentUser, vector<Vendor> &vendorList, vector<Organizer> &organizerList, vector<Admin> &adminList);
void updateWeddingMenu(const CurrentUser &currentUser, vector<Vendor> &vendorList, vector<Organizer> &organizerList, vector<Admin> &adminList,vector<WeddingEvent> &events);

#endif // USERMODULE_H