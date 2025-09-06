#include <windows.h>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <fstream>
#include <iomanip>
#include <algorithm>
#include <cctype>
#include <limits>
using namespace std;

struct Service{
    string serviceId;
    string serviceName;
    string description;
    string type;
    double price;
    int quantity;
    bool available;
    string toFileString(){
        return serviceId + "|" + serviceName + "|" + description + "|" + type + "|" +
               to_string(price) + "|" + to_string(quantity) + "|" + (available ? "1" : "0");
    }

    static Service fromFileString(string &str){
        Service s;
        stringstream ss(str);
        string temp;

        getline(ss, s.serviceId, '|');
        getline(ss, s.serviceName, '|');
        getline(ss, s.description, '|');
        getline(ss, s.type, '|');
        getline(ss, temp, '|');
        s.price = stod(temp);
        getline(ss, temp, '|');
        s.quantity = stoi(temp);
        getline(ss, temp);
        s.available = (temp == "1");

        return s;
    }
};
struct BaseInfo
{
    string name;
    string email;
    string phoneNum;
    string password;
    string toFileString()
    {
        return name + "|" + email + "|" + phoneNum + "|" + password;
    }

    static BaseInfo fromFileString(string &str)
    {
        BaseInfo b;
        stringstream ss(str);

        getline(ss, b.name, '|');
        getline(ss, b.email, '|');
        getline(ss, b.phoneNum, '|');
        getline(ss, b.password);

        return b;
    }
};
struct Vendor
{
    BaseInfo baseInfo;
    string vendorId;
    string companyName;
    string companyContactNum;
    string type; // not sure will use or not, "individual", "company", "partnership" or which type service can provide
    vector<Service> serviceHasProvide;
    int totalServicesProvided;
    string toFileString()
    {
        // baseInfo
        string baseStr = baseInfo.toFileString();
        // Vendor detail
        string vendorStr = vendorId + "|" + companyName + "|" + companyContactNum + "|" + type + "|" + to_string(totalServicesProvided);
        // service
        string servicesStr;
        for (auto &s : serviceHasProvide)
        {
            if (!servicesStr.empty())
                servicesStr += "##";
            servicesStr += s.toFileString();
        }

        return baseStr + "|" + vendorStr + "|" + servicesStr;
    }

    static Vendor fromFileString(string &str)
    {
        Vendor v;
        stringstream ss(str);
        string segment;

        getline(ss, v.baseInfo.name, '|');
        getline(ss, v.baseInfo.email, '|');
        getline(ss, v.baseInfo.phoneNum, '|');
        getline(ss, v.baseInfo.password, '|');

        // Parse Vendor fields
        getline(ss, v.vendorId, '|');
        getline(ss, v.companyName, '|');
        getline(ss, v.companyContactNum, '|');
        getline(ss, v.type, '|');
        getline(ss, segment, '|');
        v.totalServicesProvided = stoi(segment);

        // Parse services
        getline(ss, segment, '|');
        if (!segment.empty())
        {
            stringstream sss(segment);
            string sItem;
            while (getline(sss, sItem, '#'))
            {
                if (getline(sss, sItem, '#'))
                { // Skip the second #
                    if (!sItem.empty())
                    {
                        v.serviceHasProvide.push_back(Service::fromFileString(sItem));
                    }
                }
            }
        }
        return v;
    }
};
struct Organizer
{
    BaseInfo baseInfo;
    string organizerId;
    string currentWeddingId;
    vector<int> bookedServices;

    string toFileString()
    {
        // bookedServices
        string bookedServicesStr;
        for (size_t i = 0; i < bookedServices.size(); ++i)
        {
            bookedServicesStr += to_string(bookedServices[i]);
            if (i < bookedServices.size() - 1)
            {
                bookedServicesStr += ",";
            }
        }

        return baseInfo.toFileString() + "|" + organizerId + "|" + currentWeddingId + "|" + bookedServicesStr;
    }

    static Organizer fromFileString(string &str)
    {
        Organizer o;
        stringstream ss(str);

        getline(ss, o.baseInfo.name, '|');
        getline(ss, o.baseInfo.email, '|');
        getline(ss, o.baseInfo.phoneNum, '|');
        getline(ss, o.baseInfo.password, '|');
        // Parse Organizer fields
        getline(ss, o.organizerId, '|');
        getline(ss, o.currentWeddingId, '|');

        // Parse booked services
        string bookedStr;
        getline(ss, bookedStr);
        o.bookedServices.clear();
        if (!bookedStr.empty())
        {
            stringstream ssServices(bookedStr);
            string serviceStr;
            while (getline(ssServices, serviceStr, ','))
            {
                if (!serviceStr.empty())
                {
                    o.bookedServices.push_back(stoi(serviceStr));
                }
            }
        }

        return o;
    }
};
struct Admin
{
    BaseInfo baseInfo;
    string adminId;
    string toFileString()
    {
        return baseInfo.toFileString() + "|" + adminId;
    }

    static Admin fromFileString(string &str)
    {
        Admin a;
        stringstream ss(str);

        // Parse BaseInfo
        getline(ss, a.baseInfo.name, '|');
        getline(ss, a.baseInfo.email, '|');
        getline(ss, a.baseInfo.phoneNum, '|');
        getline(ss, a.baseInfo.password, '|');
        getline(ss, a.adminId);

        return a;
    }
};
// Current User Management
enum UserType{
    NONE,
    ADMIN,
    ORGANIZER,
    VENDOR
};
struct CurrentUser
{
    UserType type;
    int userIndex; // Index in the respective vector
    string userId;
    string userName;
    string currentWeddingId;

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

    // Method to serialize to file string
    string toFileString() {
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

    static WeddingEvent fromFileString(const string& str) {
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
};

struct Attendance {
    bool isPresent;
    time_t checkInTime;
};

struct Participant {
    string name;
    bool isVip;
    Attendance attendance;
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

    // Serialize to string
    string toFileString() {
        return transactionId + "|" + userId + "|" + userName + "|" + weddingId + "|" + to_string(amount) + "|" +
               paymentMethod + "|" + paymentStatus + "|" + transactionDate + "|" + transactionTime;
    }

    // Deserialize from string
    static PaymentTransaction fromFileString(const string& str) {
        PaymentTransaction pt;
        stringstream ss(str);
        string temp;
        getline(ss, pt.transactionId, '|');
        getline(ss, pt.userId, '|');
        getline(ss, pt.userName, '|');
        getline(ss, pt.weddingId, '|');
        getline(ss, temp, '|');
        pt.amount = stod(temp);
        getline(ss, pt.paymentMethod, '|');
        getline(ss, pt.paymentStatus, '|');
        getline(ss, pt.transactionDate, '|');
        getline(ss, pt.transactionTime, '|'); // specify delimiter '|'
        return pt;
    }
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


//void loadAllData(vector<Vendor> &vendorList, vector<Organizer> &organizerList, vector<Admin> &adminList);

bool login(vector<Vendor> &vendorList, vector<Organizer> &organizerList, vector<Admin> &adminList, CurrentUser &currentUser);
void logout(CurrentUser &currentUser);

void addService(const CurrentUser &currentUser, vector<Vendor> &vendorList);
//void saveAllData(vector<Vendor> &vendorList, vector<Organizer> &organizerList, vector<Admin> &adminList);
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

// Event utility functions
bool isValidDate(const string& date);
bool isDateAvailable(const string& date, const vector<WeddingEvent>& events, const string& venue = "");
double calculateServiceCost(const vector<int>& serviceIds, const vector<Vendor>& vendorList);
void loadEventsFromFile(vector<WeddingEvent>& events, const string& filename);
void saveEventsToFile(const vector<WeddingEvent>& events, const string& filename);
void createNewWedding(CurrentUser& currentUser, vector<WeddingEvent>& events,vector<Vendor>& vendorList, vector<Organizer>& organizerList);
void bookServicesForWedding(CurrentUser& currentUser, vector<WeddingEvent>& events,
    vector<Vendor>& vendorList, vector<Organizer>& organizerList);
void viewAllWeddings(CurrentUser& currentUser, const vector<WeddingEvent>& events,
    const vector<Vendor>& vendorList);
void manageMyWeddings(CurrentUser& currentUser, vector<WeddingEvent>& events,
    vector<Vendor>& vendorList, vector<Organizer>& organizerList);
void generateInvitationCard(CurrentUser& currentUser, const vector<WeddingEvent>& events,
    const vector<Organizer>& organizerList);

string returnTime(Participant p);
bool checkIsExist(vector<Participant> participant, string name);

// Input validation functions
int returnInt();
char returnUpperChar();

// File operations
void updateFile(ofstream& outFile, vector<Participant>& participants);
bool addDefaultParticipants(vector<Participant>& participants);

// Date functions
time_t weddingStartDate();
time_t weddingEndDate();

// Display functions
void printParticipant(Participant p);
void showAllParticipants(vector<Participant> participants);

// CRUD operations
void addParticipantsMenu(vector<Participant>& participants);
void addParticipantsByList(vector<Participant>& participants);
void addParticipantOneByOne(vector<Participant>& participants);

void searchParticipantsMenu(vector<Participant>& participants);
void searchParticipantOneByOne(vector<Participant> participants);

void updateParticipants(vector<Participant>& participants);
void editParticipantsName(vector<Participant>& participants, Participant participant, ofstream& outFile);
void editParticipantsVip(vector<Participant>& participants, Participant participant, ofstream& outFile);
void editParticipantsPresent(vector<Participant>& participants, Participant participant, ofstream& outFile);
void editParticipantsCheckInTime(vector<Participant>& participants, Participant participant, ofstream& outFile);

void deleteParticipants(vector<Participant>& participants);

// Attendance functions
void markAttendance(vector<Participant>& participants);
bool checkParticipantsList(stringstream& buffer);
void readBuffer(stringstream& buffer);
void addParticipantsToList(vector<Participant>& participants, stringstream& buffer, ofstream& outFile);
bool askForTime(Participant participant, tm& t);

// Main menu
void menu(vector<Participant>& participants);

void viewPaymentSummary(const CurrentUser& currentUser, const WeddingEvent& selectedEvent, const vector<Vendor>& vendorList);
void processPayment(CurrentUser& currentUser, WeddingEvent* selectedEvent, vector<PaymentTransaction>& transactions);
void paymentAndReportingMenu(CurrentUser& currentUser, WeddingEvent* selectedEvent, vector<WeddingEvent>& events, vector<Vendor>& vendorList, vector<PaymentTransaction>& transactions);
void viewPaymentHistory(CurrentUser& currentUser);

string generateId(const string &prefix, int counter){
    return prefix + to_string(10000 + counter);
}

void clearScreen()
{
    system("cls");
}

void pauseScreen()
{
    cout << "\nPress Enter to continue...";
    cin.get();
}

void getBaseUserInfo(BaseInfo &baseInfo)
{

    cout << "Enter name: ";
    getline(cin, baseInfo.name);

    cout << "Enter email: ";
    getline(cin, baseInfo.email);

    cout << "Enter phone: ";
    getline(cin, baseInfo.phoneNum);

    cout << "Enter password: ";
    getline(cin, baseInfo.password);
}

void getVendorInfo(Vendor &vendor, vector<Vendor> &vendorList)
{
    // Get base info
    getBaseUserInfo(vendor.baseInfo);

    // Get vendor-specific info
    vendor.vendorId = generateId("V", vendorList.size() + 1);

    cout << "Enter company name: ";
    getline(cin, vendor.companyName);

    cout << "Enter company contact number: ";
    getline(cin, vendor.companyContactNum);

    cout << "Enter vendor type (e.g., 'individual', 'company'): ";
    getline(cin, vendor.type);
};

void getOrganizerInfo(Organizer &organizer, vector<Organizer> &organizerList)
{
    string input;

    // Get base info
    getBaseUserInfo(organizer.baseInfo);

    organizer.organizerId = generateId("O", organizerList.size() + 1);

    // cout << "Enter groom's name: ";
    // getline(cin, organizer.groomName);

    // cout << "Enter bride's name: ";
    // getline(cin, organizer.brideName);

    // // Optional fields: ask user if they want to set or leave empty
    // cout << "Wedding date (leave empty if not set): ";
    // getline(cin, input);
    // organizer.weddingDate = input;

    // cout << "Wedding venue (leave empty if not set): ";
    // getline(cin, input);
    // organizer.weddingVenue = input;

    // cout << "Wedding theme (leave empty if not set): ";
    // getline(cin, input);
    // organizer.weddingTheme = input;

    // cout << "Budget (enter 0 if not set): ";
    // getline(cin, input);
    // try {
    //     organizer.budget = stod(input);
    // } catch (...) {
    //     organizer.budget = 0.0;
    // }

    // organizer.weddingStage = "planning";
}

bool getAdminInfo(Admin &admin, vector<Admin> &adminList)
{
    string secretPassword = "Secret";
    string inputPassword;
    getBaseUserInfo(admin.baseInfo);

    admin.adminId = generateId("A", adminList.size() + 1);

    cout << "Enter Secret Password to Register Admin: ";
    getline(cin, inputPassword);

    if (inputPassword != secretPassword)
    {
        cout << "Incorrect secret password! Registration failed." << endl;
        pauseScreen();
        return false;
    }
    return true;
}

bool login(vector<Vendor> &vendorList, vector<Organizer> &organizerList, vector<Admin> &adminList, CurrentUser &currentUser){
    clearScreen();
    cout << "=== LOGIN ===" << endl;

    string email, password;
    cout << "Enter email: ";
    getline(cin, email);

    cout << "Enter password: ";
    getline(cin, password);

    // Check admin
    for (size_t i = 0; i < adminList.size(); i++)
    {
        if (adminList[i].baseInfo.email == email && adminList[i].baseInfo.password == password)
        {
            currentUser.type = UserType::ADMIN;
            currentUser.userIndex = i;
            currentUser.userId = adminList[i].adminId;
            currentUser.userName = adminList[i].baseInfo.name;
            currentUser.currentWeddingId = "";
            cout << "Welcome Admin " << currentUser.userName << "!" << endl;
            // cin.get();
            pauseScreen();
            return true;
        }
    }

    // Check organizer
    for (size_t i = 0; i < organizerList.size(); i++)
    {
        if (organizerList[i].baseInfo.email == email && organizerList[i].baseInfo.password == password)
        {
            currentUser.type = UserType::ORGANIZER;
            currentUser.userIndex = i;
            currentUser.userId = organizerList[i].organizerId;
            currentUser.userName = organizerList[i].baseInfo.name;
            currentUser.currentWeddingId = organizerList[i].currentWeddingId;
            cout << "Welcome " << currentUser.userName << "!" << endl;
            pauseScreen();
            return true;
        }
    }

    // Check vendor
    for (size_t i = 0; i < vendorList.size(); i++){
        if (vendorList[i].baseInfo.email == email && vendorList[i].baseInfo.password == password){
            currentUser.type = UserType::VENDOR;
            currentUser.userIndex = i;
            currentUser.userId = vendorList[i].vendorId;
            currentUser.userName = vendorList[i].baseInfo.name;
            currentUser.currentWeddingId = "";
            cout << "Welcome " << currentUser.userName << "!" << endl;
            pauseScreen();
            return true;
        }
    }

    cout << "Invalid email or password!" << endl;
    cin.get();
    return false;
}

void logout(CurrentUser &currentUser)
{
    char confirmed;
    cout << "Are you sure you want to logout (Y/N)";
    cin >> confirmed;
    if (toupper(confirmed) == 'Y')
    {
        currentUser.type =  UserType::NONE;
        currentUser.userIndex = -1;
        currentUser.userId = "";
        currentUser.userName = "";
        cout << "Logged out successfully!" << endl;
        return;
    }
    else 
    {
        cout << "Logout Failed. Please try again.";
        return;
    }
}

void addService(CurrentUser &currentUser, vector<Vendor> &vendorList)
{
    if (currentUser.type != VENDOR)
        return;

    Service newService;
    newService.serviceId = generateId("S", vendorList[currentUser.userIndex].serviceHasProvide.size() + 1);
    cout << "=== ADD NEW SERVICE ===" << endl;

    cout << "Service name: ";
    getline(cin, newService.serviceName);

    cout << "Description: ";
    getline(cin, newService.description);

    cout << "Type (e.g., catering, photography, decoration): ";
    getline(cin, newService.type);

    cout << "Price: RM";
    cin >> newService.price;

    cout << "Quantity available: ";
    cin >> newService.quantity;
    cin.ignore();

    newService.available = true;

    // Add to current vendor's services
    vendorList[currentUser.userIndex].serviceHasProvide.push_back(newService);
    vendorList[currentUser.userIndex].totalServicesProvided++;

    saveUserIntoFile<Vendor>(vendorList, "vendors.txt");
    cout << "Service added successfully!" << endl;
    pauseScreen();
}

// void saveAllData(vector<Vendor> &vendorList, vector<Organizer> &organizerList, vector<Admin> &adminList)
// {
//     saveUserIntoFile<Admin>(adminList, "admins.txt");
//     saveUserIntoFile<Organizer>(organizerList, "organizers.txt");
//     saveUserIntoFile<Vendor>(vendorList, "vendors.txt");
// }

// void loadAllData(vector<Vendor> &vendorList, vector<Organizer> &organizerList, vector<Admin> &adminList){
//     loadUserFromFile<Admin>(adminList, "admins.txt");
//     loadUserFromFile<Organizer>(organizerList, "organizers.txt");
//     loadUserFromFile<Vendor>(vendorList, "vendors.txt");
// }
void displayUserProfile(CurrentUser &currentUser, vector<Vendor> &vendorList, vector<Organizer> &organizerList, vector<Admin> &adminList)
{
    clearScreen();
    cout << "=== MY PROFILE ===" << endl;

    if (currentUser.type == NONE)
    {
        cout << "No user logged in!" << endl;
        pauseScreen();
        return;
    }

    switch (currentUser.type)
    {
    case ADMIN:
    {
        Admin &admin = adminList[currentUser.userIndex];
        cout << "User Type: Administrator" << endl;
        cout << "Admin ID: " << admin.adminId << endl;
        cout << "Name: " << admin.baseInfo.name << endl;
        cout << "Email: " << admin.baseInfo.email << endl;
        cout << "Phone: " << admin.baseInfo.phoneNum << endl;
        break;
    }
    case ORGANIZER:
    {
        Organizer &org = organizerList[currentUser.userIndex];
        cout << "User Type: Wedding Organizer" << endl;
        cout << "Organizer ID: " << org.organizerId << endl;
        cout << "Name: " << org.baseInfo.name << endl;
        cout << "Email: " << org.baseInfo.email << endl;
        cout << "Phone: " << org.baseInfo.phoneNum << endl;
        // cout << "Groom: " << org.groomName << endl;
        // cout << "Bride: " << org.brideName << endl;
        // cout << "Wedding Date: " << (org.weddingDate.empty() ? "Not set" : org.weddingDate) << endl;
        // cout << "Venue: " << (org.weddingVenue.empty() ? "Not set" : org.weddingVenue) << endl;
        // cout << "Theme: " << (org.weddingTheme.empty() ? "Not set" : org.weddingTheme) << endl;
        // cout << "Budget: RM" << fixed << setprecision(2) << org.budget << endl;
        // cout << "Wedding Stage: " << org.weddingStage << endl;
        cout << "Booked Services: " << org.bookedServices.size() << endl;
        break;
    }
    case VENDOR:
    {
        Vendor &vendor = vendorList[currentUser.userIndex];
        cout << "User Type: Service Vendor" << endl;
        cout << "Vendor ID: " << vendor.vendorId << endl;
        cout << "Name: " << vendor.baseInfo.name << endl;
        cout << "Email: " << vendor.baseInfo.email << endl;
        cout << "Phone: " << vendor.baseInfo.phoneNum << endl;
        cout << "Company: " << vendor.companyName << endl;
        cout << "Company Phone: " << vendor.companyContactNum << endl;
        cout << "Vendor Type: " << vendor.type << endl;
        cout << "Services Provided: " << vendor.serviceHasProvide.size() << endl;
        break;
    }
    }
    pauseScreen();
}

void listAllUsers(vector<Vendor> &vendorList, vector<Organizer> &organizerList, vector<Admin> &adminList)
{
    clearScreen();
    cout << "=== ALL USERS LIST ===" << endl;

    cout << "\n--- ADMINISTRATORS ---" << endl;
    for (Admin &admin : adminList)
    {
        cout << "Admin ID: " << admin.adminId << " | Name: " << admin.baseInfo.name
             << " | Email: " << admin.baseInfo.email << endl;
    }

    cout << "\n--- ORGANIZERS ---" << endl;
    for (Organizer &org : organizerList)
    {
        cout << "Organizer ID: " << org.organizerId << " | Name: " << org.baseInfo.name
             << " | Email: " << org.baseInfo.email << endl;
    }

    cout << "\n--- VENDORS ---" << endl;
    for (Vendor &vendor : vendorList)
    {
        cout << "Vendor ID: " << vendor.vendorId << " | Name: " << vendor.baseInfo.name
             << " | Email: " << vendor.baseInfo.email
             << " | Company: " << vendor.companyName << endl;
    }

    pauseScreen();
}

void userRegister(vector<Vendor> &vendorList, vector<Organizer> &organizerList, vector<Admin> &adminList)
{
    int choice;
    clearScreen();
    cout << "=== USER REGISTER ===" << endl;
    cout << "1. Admin (System Administrator)" << endl;
    cout << "2. Organizer (Wedding Planner - Individual/Couple)" << endl;
    cout << "3. Supplier (Business/Service Provider)" << endl;
    cout << "Enter choice: ";
    cin >> choice;
    cin.ignore();

    switch (choice)
    {
    case 1:
    {
        Admin admin;
        if (getAdminInfo(admin, adminList))
        {
            adminList.push_back(admin);
        }
        break;
    }
    case 2:
    {
        Organizer organizer;
        getOrganizerInfo(organizer, organizerList);
        organizerList.push_back(organizer);
        break;
    }
    case 3:
    {
        Vendor vendor;
        getVendorInfo(vendor, vendorList);
        vendorList.push_back(vendor);
        break;
    }
    default:
        cout << "Invalid choice." << endl;
        break;
    }

    saveUserIntoFile<Admin>(adminList, "admins.txt");
    saveUserIntoFile<Organizer>(organizerList, "organizers.txt");
    saveUserIntoFile<Vendor>(vendorList, "vendors.txt");
}

void updateBaseInfo(BaseInfo &baseInfo)
{
    int choice;
    cout << "\n=== UPDATE BASE INFORMATION ===" << endl;
    cout << "Current Information:" << endl;
    cout << "1. Name" << endl;
    cout << "2. Email" << endl;
    cout << "3. Phone" << endl;
    cout << "4. Password" << endl;
    cout << "5. Update All Base Info" << endl;
    cout << "0. Back to main menu" << endl;
    cout << "Choose what to update: ";
    cin >> choice;
    cin.ignore();

    switch (choice)
    {
    case 1:
        cout << "Enter new name: ";
        getline(cin, baseInfo.name);
        cout << "Name updated successfully!" << endl;
        break;
    case 2:
        cout << "Enter new email: ";
        getline(cin, baseInfo.email);
        cout << "Email updated successfully!" << endl;
        break;
    case 3:
        cout << "Enter new phone: ";
        getline(cin, baseInfo.phoneNum);
        cout << "Phone updated successfully!" << endl;
        break;
    case 4:
        cout << "Enter new password: ";
        getline(cin, baseInfo.password);
        cout << "Password updated successfully!" << endl;
        break;
    case 5:
        getBaseUserInfo(baseInfo);
        cout << "All base information updated successfully!" << endl;
        break;
    case 0:
        return;
    default:
        cout << "Invalid choice!" << endl;
        break;
    }
    pauseScreen();
}

void updateAdminInfo(Admin &admin)
{
    int choice;
    cout << "\n=== UPDATE ADMIN INFORMATION ===" << endl;
    cout << "Current Information:" << endl;
    cout << "Admin ID: " << admin.adminId << " (Cannot be changed)" << endl;
    cout << "1. Update Base Information (Name, Email, Phone, Password)" << endl;
    cout << "0. Back to main menu" << endl;
    cout << "Choose what to update: ";
    cin >> choice;
    cin.ignore();

    switch (choice)
    {
    case 1:
        updateBaseInfo(admin.baseInfo);
        break;
    case 0:
        return;
    default:
        cout << "Invalid choice!" << endl;
        pauseScreen();
        break;
    }
}

void updateOrganizerInfo(Organizer &organizer)
{
    int choice;
    string input;

    cout << "\n=== UPDATE ORGANIZER INFORMATION ===" << endl;
    cout << "Current Information:" << endl;
    cout << "Organizer ID: " << organizer.organizerId << " (Cannot be changed)" << endl;
    cout << "1. Update Base Information (Name, Email, Phone, Password)" << endl;
    cout << "2. Groom Name" << endl;
    cout << "3. Bride Name" << endl;
    cout << "4. Wedding Date" << endl;
    cout << "5. Wedding Venue" << endl;
    cout << "6. Budget" << endl;
    cout << "7. Wedding Theme" << endl;
    cout << "8. Wedding Stage" << endl;
    cout << "9. Update All Organizer Info" << endl;
    cout << "0. Back to main menu" << endl;
    cout << "Choose what to update: ";
    cin >> choice;
    cin.ignore();

    switch (choice)
    {
    case 1:
        updateBaseInfo(organizer.baseInfo);
        break;
    // case 2:
    //     cout << "Enter new groom name: ";
    //     getline(cin, organizer.groomName);
    //     cout << "Groom name updated successfully!" << endl;
    //     pauseScreen();
    //     break;
    // case 3:
    //     cout << "Enter new bride name: ";
    //     getline(cin, organizer.brideName);
    //     cout << "Bride name updated successfully!" << endl;
    //     pauseScreen();
    //     break;
    // case 4:
    //     cout << "Enter new wedding date (leave empty to clear): ";
    //     getline(cin, organizer.weddingDate);
    //     cout << "Wedding date updated successfully!" << endl;
    //     pauseScreen();
    //     break;
    // case 5:
    //     cout << "Enter new wedding venue (leave empty to clear): ";
    //     getline(cin, organizer.weddingVenue);
    //     cout << "Wedding venue updated successfully!" << endl;
    //     pauseScreen();
    //     break;
    // case 6:
    //     cout << "Enter new budget (RM): ";
    //     cin >> organizer.budget;
    //     cin.ignore();
    //     cout << "Budget updated successfully!" << endl;
    //     pauseScreen();
    //     break;
    // case 7:
    //     cout << "Enter new wedding theme (leave empty to clear): ";
    //     getline(cin, organizer.weddingTheme);
    //     cout << "Wedding theme updated successfully!" << endl;
    //     pauseScreen();
    //     break;
    // case 8:
    //     cout << "Enter new wedding stage (e.g., planning, booked, completed): ";
    //     getline(cin, organizer.weddingStage);
    //     cout << "Wedding stage updated successfully!" << endl;
    //     pauseScreen();
    //     break;
    // case 9:
    //     updateBaseInfo(organizer.baseInfo);
    //     cout << "Enter new groom name: ";
    //     getline(cin, organizer.groomName);
    //     cout << "Enter new bride name: ";
    //     getline(cin, organizer.brideName);
    //     cout << "Wedding date (leave empty if not set): ";
    //     getline(cin, organizer.weddingDate);
    //     cout << "Wedding venue (leave empty if not set): ";
    //     getline(cin, organizer.weddingVenue);
    //     cout << "Wedding theme (leave empty if not set): ";
    //     getline(cin, organizer.weddingTheme);
    //     cout << "Budget (RM): ";
    //     cin >> organizer.budget;
    //     cin.ignore();
    //     cout << "Wedding stage: ";
    //     getline(cin, organizer.weddingStage);
    //     cout << "All organizer information updated successfully!" << endl;
    //     pauseScreen();
    //     break;
    case 0:
        return;
    default:
        cout << "Invalid choice!" << endl;
        pauseScreen();
        break;
    }
}

void updateVendorInfo(Vendor &vendor)
{
    int choice;

    cout << "\n=== UPDATE VENDOR INFORMATION ===" << endl;
    cout << "Current Information:" << endl;
    cout << "Vendor ID: " << vendor.vendorId << " (Cannot be changed)" << endl;
    cout << "1. Update Base Information (Name, Email, Phone, Password)" << endl;
    cout << "2. Company Name" << endl;
    cout << "3. Company Contact" << endl;
    cout << "4. Vendor Type" << endl;
    cout << "5. Update All Vendor Info" << endl;
    cout << "0. Back to main menu" << endl;
    cout << "Choose what to update: ";
    cin >> choice;
    cin.ignore();

    switch (choice)
    {
    case 1:
        updateBaseInfo(vendor.baseInfo);
        break;
    case 2:
        cout << "Enter new company name: ";
        getline(cin, vendor.companyName);
        cout << "Company name updated successfully!" << endl;
        pauseScreen();
        break;
    case 3:
        cout << "Enter new company contact number: ";
        getline(cin, vendor.companyContactNum);
        cout << "Company contact updated successfully!" << endl;
        pauseScreen();
        break;
    case 4:
        cout << "Enter new vendor type (e.g., 'individual', 'company'): ";
        getline(cin, vendor.type);
        cout << "Vendor type updated successfully!" << endl;
        pauseScreen();
        break;
    case 5:
        updateBaseInfo(vendor.baseInfo);
        cout << "Enter new company name: ";
        getline(cin, vendor.companyName);
        cout << "Enter new company contact number: ";
        getline(cin, vendor.companyContactNum);
        cout << "Enter new vendor type: ";
        getline(cin, vendor.type);
        cout << "All vendor information updated successfully!" << endl;
        pauseScreen();
        break;
    case 0:
        return;
    default:
        cout << "Invalid choice!" << endl;
        pauseScreen();
        break;
    }
}

void updateUserProfile(CurrentUser &currentUser, vector<Vendor> &vendorList, vector<Organizer> &organizerList, vector<Admin> &adminList)
{
    clearScreen();

    if (currentUser.type == NONE)
    {
        cout << "No user logged in!" << endl;
        pauseScreen();
        return;
    }

    cout << "=== UPDATE PROFILE ===" << endl;
    cout << "Logged in as: " << currentUser.userName << " (" << currentUser.userId << ")" << endl;

    bool updated = false;

    switch (currentUser.type)
    {
    case ADMIN:
    {
        updateAdminInfo(adminList[currentUser.userIndex]);
        updated = true;
        break;
    }
    case ORGANIZER:
    {
        updateOrganizerInfo(organizerList[currentUser.userIndex]);
        updated = true;
        break;
    }
    case VENDOR:
    {
        updateVendorInfo(vendorList[currentUser.userIndex]);
        updated = true;
        break;
    }
    }

    if (updated)
    {
        // Save the updated data to files
        saveUserIntoFile<Admin>(adminList, "admins.txt");
        saveUserIntoFile<Organizer>(organizerList, "organizers.txt");
        saveUserIntoFile<Vendor>(vendorList, "vendors.txt");
        cout << "\nProfile changes saved to file!" << endl;
    }
}

void updateOwnService(CurrentUser &currentUser, vector<Vendor> &vendorList)
{
    if (currentUser.type != VENDOR)
    {
        cout << "Only vendors can update services!" << endl;
        pauseScreen();
        return;
    }

    clearScreen();
    cout << "=== UPDATE SERVICES ===" << endl;

    Vendor &vendor = vendorList[currentUser.userIndex];

    if (vendor.serviceHasProvide.empty())
    {
        cout << "No services available to update!" << endl;
        pauseScreen();
        return;
    }

    cout << "\nYour Services:" << endl;
    int index = 1;
    for (auto &service : vendor.serviceHasProvide)
    {
        cout << index << ". " << service.serviceName
             << " - RM" << service.price
             << " (Qty: " << service.quantity << ")" << endl;
        index++;
    }

    int serviceIndex;
    cout << "Select service to update (1-" << vendor.serviceHasProvide.size() << "): ";
    cin >> serviceIndex;
    cin.ignore();

    if (serviceIndex < 1 || serviceIndex > (int)vendor.serviceHasProvide.size())
    {
        cout << "Invalid selection!" << endl;
        pauseScreen();
        return;
    }

    Service &service = vendor.serviceHasProvide[serviceIndex - 1];

    int updateChoice;
    cout << "\nCurrent Service Information:" << endl;
    cout << "1. Service Name: " << service.serviceName << endl;
    cout << "2. Description: " << service.description << endl;
    cout << "3. Type: " << service.type << endl;
    cout << "4. Price: RM" << service.price << endl;
    cout << "5. Quantity: " << service.quantity << endl;
    cout << "6. Availability: " << (service.available ? "Available" : "Not Available") << endl;
    cout << "7. Update All Service Info" << endl;
    cout << "0. Back" << endl;
    cout << "Choose what to update: ";
    cin >> updateChoice;
    cin.ignore();

    switch (updateChoice)
    {
    case 1:
        cout << "Enter new service name: ";
        getline(cin, service.serviceName);
        cout << "Service name updated successfully!" << endl;
        break;
    case 2:
        cout << "Enter new description: ";
        getline(cin, service.description);
        cout << "Description updated successfully!" << endl;
        break;
    case 3:
        cout << "Enter new type: ";
        getline(cin, service.type);
        cout << "Type updated successfully!" << endl;
        break;
    case 4:
        cout << "Enter new price: RM";
        cin >> service.price;
        cin.ignore();
        cout << "Price updated successfully!" << endl;
        break;
    case 5:
        cout << "Enter new quantity: ";
        cin >> service.quantity;
        cin.ignore();
        cout << "Quantity updated successfully!" << endl;
        break;
    case 6:
        service.available = !service.available;
        cout << "Availability changed to: " << (service.available ? "Available" : "Not Available") << endl;
        break;
    case 7:
        cout << "Enter new service name: ";
        getline(cin, service.serviceName);
        cout << "Enter new description: ";
        getline(cin, service.description);
        cout << "Enter new type: ";
        getline(cin, service.type);
        cout << "Enter new price: RM";
        cin >> service.price;
        cout << "Enter new quantity: ";
        cin >> service.quantity;
        cin.ignore();
        char availChoice;
        cout << "Is service available? (y/n): ";
        cin >> availChoice;
        service.available = (availChoice == 'y' || availChoice == 'Y');
        cout << "All service information updated successfully!" << endl;
        break;
    case 0:
        return;
    default:
        cout << "Invalid choice!" << endl;
        pauseScreen();
        return;
    }

    saveUserIntoFile<Vendor>(vendorList, "vendors.txt");
    loadUserFromFile<Vendor>(vendorList, "vendors.txt");
    pauseScreen();
}

void displayAllServices(vector<Vendor> &vendorList)
{
    bool hasServices = false;

    clearScreen();
    cout << "=== ALL AVAILABLE SERVICES ===" << endl;

    for (auto &vendor : vendorList)
    {
        if (!vendor.serviceHasProvide.empty())
        {
            int serviceNum = 1;
            hasServices = true;
            cout << "\n--- Services by " << vendor.baseInfo.name << " (" << vendor.companyName << ") ---" << endl;
            cout << "Vendor ID: " << vendor.vendorId << " | Contact: " << vendor.baseInfo.phoneNum << endl;

            for (auto &service : vendor.serviceHasProvide)
            {
                cout << serviceNum << ". Service: " << service.serviceName << endl;
                cout << "   Description: " << service.description << endl;
                cout << "   Type: " << service.type << endl;
                cout << "   Price: RM" << fixed << setprecision(2) << service.price << endl;
                cout << "   Quantity Available: " << service.quantity << endl;
                cout << "   Status: " << (service.available ? "Available" : "Not Available") << endl;
                cout << "   " << string(50, '-') << endl;
                serviceNum++;
            }
        }
    }

    if (!hasServices)
    {
        cout << "No services available at the moment." << endl;
    }

    pauseScreen();
}

void displayServicesByVendor(vector<Vendor> &vendorList)
{
    int vendorNum = 1;
    int choice;

    clearScreen();
    cout << "=== SERVICES BY VENDOR ===" << endl;

    if (vendorList.empty())
    {
        cout << "No vendors registered." << endl;
        pauseScreen();
        return;
    }

    // Display all vendors
    cout << "Available Vendors:" << endl;
    for (auto &vendor : vendorList)
    {
        cout << vendorNum << ". " << vendor.baseInfo.name << " (" << vendor.companyName << ")" << endl;
        cout << "   Vendor ID: " << vendor.vendorId << " | Services: " << vendor.serviceHasProvide.size() << endl;
        vendorNum++;
    }

    cout << "\nSelect vendor to view services (1-" << vendorList.size() << "): ";
    cin >> choice;
    cin.ignore();

    if (choice < 1 || choice > (int)vendorList.size())
    {
        cout << "Invalid selection!" << endl;
        pauseScreen();
        return;
    }

    Vendor &selectedVendor = vendorList[choice - 1];

    clearScreen();
    cout << "=== SERVICES BY " << selectedVendor.baseInfo.name << " ===" << endl;
    cout << "Company: " << selectedVendor.companyName << endl;
    cout << "Contact: " << selectedVendor.baseInfo.phoneNum << " | Email: " << selectedVendor.baseInfo.email << endl;
    cout << string(60, '=') << endl;

    if (selectedVendor.serviceHasProvide.empty())
    {
        cout << "This vendor has no services available." << endl;
    }
    else
    {
        int serviceNum = 1;
        for (auto &service : selectedVendor.serviceHasProvide)
        {
            cout << serviceNum << ". " << service.serviceName << endl;
            cout << "   Description: " << service.description << endl;
            cout << "   Type: " << service.type << endl;
            cout << "   Price: RM" << fixed << setprecision(2) << service.price << endl;
            cout << "   Quantity: " << service.quantity << endl;
            cout << "   Status: " << (service.available ? "Available" : "Not Available") << endl;
            cout << "   " << string(50, '-') << endl;
            serviceNum++;
        }
    }

    pauseScreen();
}

void displayServicesByType(vector<Vendor> &vendorList)
{
    clearScreen();
    cout << "=== SERVICES BY TYPE ===" << endl;

    // First, collect all unique service types
    vector<string> serviceTypes;
    for (auto &vendor : vendorList)
    {
        for (auto &service : vendor.serviceHasProvide)
        {
            bool typeExists = false;
            for (auto &existingType : serviceTypes)
            {
                if (existingType == service.type)
                {
                    typeExists = true;
                    break;
                }
            }
            if (!typeExists && !service.type.empty())
            {
                serviceTypes.push_back(service.type);
            }
        }
    }

    if (serviceTypes.empty())
    {
        cout << "No service types available." << endl;
        pauseScreen();
        return;
    }

    // Display available types
    cout << "Available Service Types:" << endl;
    int typeNum = 1;
    for (auto &type : serviceTypes)
    {
        cout << typeNum << ". " << type << endl;
        typeNum++;
    }

    int choice;
    cout << "\nSelect service type (1-" << serviceTypes.size() << "): ";
    cin >> choice;
    cin.ignore();

    if (choice < 1 || choice > (int)serviceTypes.size())
    {
        cout << "Invalid selection!" << endl;
        pauseScreen();
        return;
    }

    string selectedType = serviceTypes[choice - 1];

    clearScreen();
    cout << "=== " << selectedType << " SERVICES ===" << endl;
    cout << string(50, '=') << endl;

    bool found = false;
    for (auto &vendor : vendorList)
    {
        for (auto &service : vendor.serviceHasProvide)
        {
            if (service.type == selectedType)
            {
                found = true;
                cout << "Service: " << service.serviceName << endl;
                cout << "Vendor: " << vendor.baseInfo.name << " (" << vendor.companyName << ")" << endl;
                cout << "Description: " << service.description << endl;
                cout << "Price: RM" << fixed << setprecision(2) << service.price << endl;
                cout << "Quantity: " << service.quantity << endl;
                cout << "Contact: " << vendor.baseInfo.phoneNum << endl;
                cout << "Status: " << (service.available ? "Available" : "Not Available") << endl;
                cout << string(50, '-') << endl;
            }
        }
    }

    if (!found)
    {
        cout << "No services found for this type." << endl;
    }

    pauseScreen();
}

void displayBookedServices(CurrentUser &currentUser, vector<Organizer> &organizerList, vector<Vendor> &vendorList)
{
    if (currentUser.type != ORGANIZER)
    {
        cout << "Only organizers can view booked services!" << endl;
        pauseScreen();
        return;
    }

    clearScreen();
    cout << "=== MY BOOKED SERVICES ===" << endl;

    Organizer &organizer = organizerList[currentUser.userIndex];

    // cout << "Wedding: " << organizer.groomName << " & " << organizer.brideName << endl;
    // cout << "Date: " << (organizer.weddingDate.empty() ? "Not set" : organizer.weddingDate) << endl;
    // cout << "Venue: " << (organizer.weddingVenue.empty() ? "Not set" : organizer.weddingVenue) << endl;
    // cout << "Budget: RM" << fixed << setprecision(2) << organizer.budget << endl;
    cout << string(60, '=') << endl;

    if (organizer.bookedServices.empty())
    {
        cout << "No services booked yet." << endl;
    }
    else
    {
        cout << "Total Booked Services: " << organizer.bookedServices.size() << endl;
        cout << string(50, '-') << endl;

        double totalCost = 0.0;
        int serviceCount = 1;

        for (auto &serviceId : organizer.bookedServices)
        {
            // Find the service in vendors list
            bool serviceFound = false;
            for (auto &vendor : vendorList)
            {
                int currentServiceId = 1;
                for (auto &service : vendor.serviceHasProvide)
                {
                    if (currentServiceId == serviceId)
                    {
                        cout << serviceCount << ". " << service.serviceName << endl;
                        cout << "   Vendor: " << vendor.baseInfo.name << " (" << vendor.companyName << ")" << endl;
                        cout << "   Type: " << service.type << endl;
                        cout << "   Price: RM" << fixed << setprecision(2) << service.price << endl;
                        cout << "   Contact: " << vendor.baseInfo.phoneNum << endl;
                        totalCost += service.price;
                        serviceFound = true;
                        break;
                    }
                    currentServiceId++;
                }
                if (serviceFound)
                    break;
            }

            if (!serviceFound)
            {
                cout << serviceCount << ". Service ID " << serviceId << " (Service not found)" << endl;
            }

            cout << "   " << string(45, '-') << endl;
            serviceCount++;
        }

        cout << "\nTotal Cost: RM" << fixed << setprecision(2) << totalCost << endl;
        // cout << "Remaining Budget: RM" << fixed << setprecision(2) << (organizer.budget - totalCost) << endl;
    }

    pauseScreen();
}

bool deleteOwnAccount(CurrentUser &currentUser, vector<Vendor> &vendorList, vector<Organizer> &organizerList, vector<Admin> &adminList)
{
    clearScreen();
    cout << "=== DELETE MY ACCOUNT ===" << endl;

    if (currentUser.type == NONE)
    {
        cout << "No user logged in!" << endl;
        pauseScreen();
        return false;
    }

    // Show current user info
    cout << "You are about to delete your account:" << endl;
    cout << "User Type: ";
    switch (currentUser.type)
    {
    case ADMIN:
        cout << "Administrator";
        break;
    case ORGANIZER:
        cout << "Wedding Organizer";
        break;
    case VENDOR:
        cout << "Service Vendor";
        break;
    }
    cout << endl;
    cout << "Name: " << currentUser.userName << endl;
    cout << "User ID: " << currentUser.userId << endl;

    // Warning message
    cout << "\n"
         << string(50, '!') << endl;
    cout << "WARNING: This action cannot be undone!" << endl;
    cout << "All your data will be permanently deleted!" << endl;
    cout << string(50, '!') << endl;

    // Confirmation
    string confirmation;
    cout << "\nType 'DELETE' to confirm account deletion: ";
    getline(cin, confirmation);

    if (confirmation != "DELETE")
    {
        cout << "Account deletion cancelled." << endl;
        pauseScreen();
        return false;
    }

    // Final confirmation with password
    string password;
    cout << "Enter your password to confirm: ";
    getline(cin, password);

    bool passwordCorrect = false;

    switch (currentUser.type)
    {
    case ADMIN:
        passwordCorrect = (adminList[currentUser.userIndex].baseInfo.password == password);
        break;
    case ORGANIZER:
        passwordCorrect = (organizerList[currentUser.userIndex].baseInfo.password == password);
        break;
    case VENDOR:
        passwordCorrect = (vendorList[currentUser.userIndex].baseInfo.password == password);
        break;
    }

    if (!passwordCorrect)
    {
        cout << "Incorrect password! Account deletion cancelled." << endl;
        pauseScreen();
        return false;
    }

    // Delete the account
    switch (currentUser.type)
    {
    case ADMIN:
        adminList.erase(adminList.begin() + currentUser.userIndex);
        break;
    case ORGANIZER:
        organizerList.erase(organizerList.begin() + currentUser.userIndex);
        break;
    case VENDOR:
        vendorList.erase(vendorList.begin() + currentUser.userIndex);
        break;
    }

    // Save updated data
    saveUserIntoFile<Admin>(adminList, "admins.txt");
    saveUserIntoFile<Organizer>(organizerList, "organizers.txt");
    saveUserIntoFile<Vendor>(vendorList, "vendors.txt");
    loadUserFromFile<Admin>(adminList, "admins.txt");
    loadUserFromFile<Organizer>(organizerList, "organizers.txt");
    loadUserFromFile<Vendor>(vendorList, "vendors.txt");

    cout << "Account successfully deleted!" << endl;
    cout << "You will be logged out automatically." << endl;

    // Logout user
    logout(currentUser);

    pauseScreen();
    return true;
}

void deleteOwnService(CurrentUser &currentUser, vector<Vendor> &vendorList)
{
    if (currentUser.type != VENDOR)
    {
        cout << "Only vendors can delete services!" << endl;
        pauseScreen();
        return;
    }

    clearScreen();
    cout << "=== DELETE MY SERVICES ===" << endl;

    Vendor &vendor = vendorList[currentUser.userIndex];

    if (vendor.serviceHasProvide.empty())
    {
        cout << "You have no services to delete." << endl;
        pauseScreen();
        return;
    }

    // Display current services
    cout << "Your Current Services:" << endl;
    int serviceNum = 1;
    for (auto &service : vendor.serviceHasProvide)
    {
        cout << serviceNum << ". " << service.serviceName
             << " - RM" << fixed << setprecision(2) << service.price
             << " (Qty: " << service.quantity << ")" << endl;
        cout << "   Type: " << service.type << endl;
        cout << "   Status: " << (service.available ? "Available" : "Not Available") << endl;
        serviceNum++;
    }

    int choice;
    cout << "\nSelect service to delete (1-" << vendor.serviceHasProvide.size() << ") or 0 to cancel: ";
    cin >> choice;
    cin.ignore();

    if (choice == 0)
    {
        cout << "Service deletion cancelled." << endl;
        pauseScreen();
        return;
    }

    if (choice < 1 || choice > (int)vendor.serviceHasProvide.size())
    {
        cout << "Invalid selection!" << endl;
        pauseScreen();
        return;
    }

    // Show selected service details
    Service &selectedService = vendor.serviceHasProvide[choice - 1];
    cout << "\nYou selected to delete:" << endl;
    cout << "Service: " << selectedService.serviceName << endl;
    cout << "Type: " << selectedService.type << endl;
    cout << "Price: RM" << fixed << setprecision(2) << selectedService.price << endl;

    // Confirmation
    char confirm;
    cout << "\nAre you sure you want to delete this service? (y/n): ";
    cin >> confirm;
    cin.ignore();

    if (confirm == 'y' || confirm == 'Y')
    {
        vendor.serviceHasProvide.erase(vendor.serviceHasProvide.begin() + (choice - 1));
        vendor.totalServicesProvided = vendor.serviceHasProvide.size();

        saveUserIntoFile<Vendor>(vendorList, "vendors.txt");
        loadUserFromFile<Vendor>(vendorList, "vendors.txt");
        cout << "Service deleted successfully!" << endl;
    }
    else
    {
        cout << "Service deletion cancelled." << endl;
    }

    pauseScreen();
}

/*void cancelBookedService(CurrentUser &currentUser, vector<Organizer> &organizerList, vector<Vendor> &vendorList)
{
    if (currentUser.type != ORGANIZER)
    {
        cout << "Only organizers can cancel booked services!" << endl;
        pauseScreen();
        return;
    }

    clearScreen();
    cout << "=== CANCEL BOOKED SERVICES ===" << endl;

    Organizer &organizer = organizerList[currentUser.userIndex];

    if (organizer.bookedServices.empty())
    {
        cout << "You have no booked services to cancel." << endl;
        pauseScreen();
        return;
    }

    // Display booked services
    cout << "Your Booked Services:" << endl;
    vector<string> serviceNames;

    int displayNum = 1;
    for (auto &serviceId : organizer.bookedServices)
    {
        string serviceName = "Service ID " + to_string(serviceId);

        // Find service name from vendors
        for (auto &vendor : vendorList)
        {
            int currentId = 1;
            for (auto &service : vendor.serviceHasProvide)
            {
                if (currentId == serviceId)
                {
                    serviceName = service.serviceName + " (by " + vendor.baseInfo.name + ")";
                    break;
                }
                currentId++;
            }
        }

        serviceNames.push_back(serviceName);
        cout << displayNum << ". " << serviceName << endl;
        displayNum++;
    }

    int choice;
    cout << "\nSelect service to cancel (1-" << organizer.bookedServices.size() << ") or 0 to go back: ";
    cin >> choice;
    cin.ignore();

    if (choice == 0)
    {
        cout << "Service cancellation cancelled." << endl;
        pauseScreen();
        return;
    }

    if (choice < 1 || choice > (int)organizer.bookedServices.size())
    {
        cout << "Invalid selection!" << endl;
        pauseScreen();
        return;
    }

    cout << "\nYou selected to cancel: " << serviceNames[choice - 1] << endl;

    // Confirmation
    char confirm;
    cout << "Are you sure you want to cancel this booked service? (y/n): ";
    cin >> confirm;
    cin.ignore();

    if (confirm == 'y' || confirm == 'Y')
    {
        organizer.bookedServices.erase(organizer.bookedServices.begin() + (choice - 1));

        saveUserIntoFile(organizerList, "organizers.txt");
        cout << "Booked service cancelled successfully!" << endl;
    }
    else
    {
        cout << "Service cancellation cancelled." << endl;
    }

    pauseScreen();
}
*/
void UpdateWeddingMenu(CurrentUser &currentUser, vector<Vendor> &vendorList, vector<Organizer> &organizerList, vector<Admin> &adminList, vector<WeddingEvent> &events){
    int choice;
    do
    {
        clearScreen();
        cout << "1. Book a New Service for Wedding" << endl;
        cout << "2. Read All The Booked Service" << endl;
        cout << "3. Delete Booked Service" << endl;
        cout << "0. Back to Main Menu" << endl;
        cout << "==========================================" << endl;
        cout << "Enter your choice: ";
        cin >> choice;
        cin.ignore();

        switch (choice)
        {
        case 1:
            // Booking a new service
            bookServicesForWedding(currentUser, events, vendorList, organizerList);
            saveUserIntoFile<Vendor>(vendorList, "vendors.txt");
            saveUserIntoFile<Organizer>(organizerList, "organizers.txt");
            break;
        case 2:
            //Read all the service have be booked for this wedding
            break;
        case 3:
            //delete the booked service;
            break;
        case 0:
            return;
        default:
            cout << "Invalid choice! Please try again." << endl;
            pauseScreen();
            break;
        }
    } while (choice != 0);
}

void organizerMenu(CurrentUser &currentUser, vector<Vendor> &vendorList, vector<Organizer> &organizerList, vector<Admin> &adminList, vector<WeddingEvent> &events, vector<Participant> &participants)
{
    int choice;
    do
    {
        clearScreen();
        cout << "=== ORGANIZER DASHBOARD ===" << endl;
        cout << "Welcome, " << currentUser.userName << " (Wedding Organizer)" << endl;
        // Display wedding info
        Organizer &org = organizerList[currentUser.userIndex];
        cout << "==========================================" << endl;
        cout << "1. Book a New Wedding" << endl;
        cout << "2. View My All Wedding" << endl;
        cout << "3. Update My Current Wedding / Add new service for current wedding" << endl;
        cout << "4. Cancel Booked Wedding" << endl;
        cout << "5. Monitoring" << endl;
        cout << "6. Payment for Current Wedding" << endl;
        cout << "7. My Profile" << endl;
        cout << "0. Logout" << endl;
        cout << "==========================================" << endl;
        cout << "Enter your choice: ";
        cin >> choice;
        cin.ignore();

        switch (choice)
        {
        case 1:
            createNewWedding(currentUser,events,vendorList, organizerList);
            saveUserIntoFile<Organizer>(organizerList, "organizers.txt");
            break;
        case 2:
            viewAllWeddings(currentUser, events, vendorList);
            break;
        case 3:
            UpdateWeddingMenu(currentUser, vendorList, organizerList, adminList, events);
            break;
        case 4:
            manageMyWeddings(currentUser, events, vendorList, organizerList);
            saveUserIntoFile<Organizer>(organizerList, "organizers.txt");
            break;
        case 5:
            // Monitoring
            menu(participants);  // Add participants parameter - you'll need to pass it down
            break;
        case 6:
            // Payment
            break;
        case 7:
            displayUserProfile(currentUser, vendorList, organizerList, adminList);
            break;
        case 0:
            logout(currentUser);
            pauseScreen();
            break;
        default:
            cout << "Invalid choice! Please try again." << endl;
            pauseScreen();
            break;
        }
    } while (choice != 0 && currentUser.type != NONE);
}

void adminMenu(CurrentUser &currentUser, vector<Vendor> &vendorList, vector<Organizer> &organizerList, vector<Admin> &adminList)
{
    int choice;
    do
    {
        clearScreen();
        cout << "=== ADMIN DASHBOARD ===" << endl;
        cout << "Welcome, " << currentUser.userName << " (Administrator)" << endl;
        cout << "==========================================" << endl;
        cout << "USER MANAGEMENT:" << endl;
        cout << "1. View All Users" << endl;
        cout << "2. Register New User" << endl<< endl;

        cout << "Service MANAGEMENT:" << endl;
        cout << "3. View All Services" << endl;
        cout << "5. View Services by Vendor" << endl;
        cout << "7. View Services by Type" << endl << endl;

        cout << "OWN ACCOUNT MANAGEMENT:" << endl;
        cout << "8. View My Profile" << endl;
        cout << "9. Update My Profile" << endl;
        cout << "10. Delete My Account" << endl;
        cout << endl;
        cout << "0. Logout" << endl;
        cout << "==========================================" << endl;
        cout << "Enter your choice: ";
        cin >> choice;
        cin.ignore();

        switch (choice)
        {
        case 1:
            listAllUsers(vendorList, organizerList, adminList);
            break;
        case 2:
            userRegister(vendorList, organizerList, adminList);
            break;
        case 3:
            displayAllServices(vendorList);
            break;
        case 4:
            
            break;
        case 5:
            displayServicesByVendor(vendorList);
            break;
        case 6:
            
            break;
        case 7:
            displayServicesByType(vendorList);
            break;
        case 8:
            displayUserProfile(currentUser, vendorList, organizerList, adminList);
            break;
        case 9:
            updateUserProfile(currentUser, vendorList, organizerList, adminList);
            break;
        case 10:
            if (deleteOwnAccount(currentUser, vendorList, organizerList, adminList))
            {
                return; // Account deleted, exit menu
            }
            break;
        case 0:
            logout(currentUser);
            pauseScreen();
            break;
        default:
            cout << "Invalid choice! Please try again." << endl;
            pauseScreen();
            break;
        }
    } while (choice != 0 && currentUser.type != NONE);
}
// product
void vendorMenu(CurrentUser &currentUser, vector<Vendor> &vendorList, vector<Organizer> &organizerList, vector<Admin> &adminList)
{
    int choice;
    do
    {
        clearScreen();
        cout << "=== VENDOR DASHBOARD ===" << endl;
        cout << "Welcome, " << currentUser.userName << " (Service Vendor)" << endl;

        // Display vendor info
        Vendor &vendor = vendorList[currentUser.userIndex];
        cout << "Company: " << vendor.companyName << endl;
        cout << "Services: " << vendor.serviceHasProvide.size() << endl;
        cout << "==========================================" << endl;
        cout << "SERVICE MANAGEMENT:" << endl;
        cout << "1. Add New Service" << endl;
        cout << "2. View My Services" << endl;
        cout << "3. Update My Services" << endl;
        cout << "4. Delete My Services" << endl << endl;

        cout << "MARKET RESEARCH:" << endl;
        cout << "5. View All Services (Competition)" << endl;
        cout << "6. View Services by Type" << endl << endl;

        cout << "ACCOUNT MANAGEMENT:" << endl;
        cout << "7. View My Profile" << endl;
        cout << "8. Update My Profile" << endl;
        cout << "9. Delete My Account" << endl << endl;

        cout << "0. Logout" << endl;
        cout << "==========================================" << endl;
        cout << "Enter your choice: ";
        cin >> choice;
        cin.ignore();

        switch (choice)
        {
        case 1:
            addService(currentUser, vendorList);
            break;
        case 2:
            displayServicesByVendor(vendorList); // They can select themselves
            break;
        case 3:
            updateOwnService(currentUser, vendorList);
            break;
        case 4:
            deleteOwnService(currentUser, vendorList);
            break;
        case 5:
            displayAllServices(vendorList);
            break;
        case 6:
            displayServicesByType(vendorList);
            break;
        case 7:
            displayUserProfile(currentUser, vendorList, organizerList, adminList);
            break;
        case 8:
            updateUserProfile(currentUser, vendorList, organizerList, adminList);
            break;
        case 9:
            deleteOwnAccount( currentUser, vendorList, organizerList, adminList);
            break;
        case 0:
            logout(currentUser);
            break;
        default:
            cout << "Invalid choice! Please try again." << endl;
            pauseScreen();
            break;
        }
    } while (choice != 0 && currentUser.type != NONE);
}

void mainMenu(CurrentUser &currentUser, vector<Vendor> &vendorList, vector<Organizer> &organizerList, vector<Admin> &adminList, vector<WeddingEvent>& events, vector<Participant> &participants)
{
    int choice;

    do
    {
        clearScreen();
        cout << "======================================" << endl;
        cout << "    WEDDING MANAGEMENT SYSTEM" << endl;
        cout << "======================================" << endl;
        cout << "1. Login" << endl;
        cout << "2. Register New Account" << endl;
        cout << "3. Exit" << endl;
        cout << "======================================" << endl;
        cout << "Enter your choice: ";
        cin >> choice;
        cin.ignore();

        switch (choice)
        {
        case 1:
            if (login(vendorList, organizerList, adminList, currentUser))
            {
                // Route to appropriate menu based on user type
                switch (currentUser.type)
                {
                case ADMIN:
                    adminMenu(currentUser, vendorList, organizerList, adminList);
                    break;
                case ORGANIZER:
                    organizerMenu(currentUser, vendorList, organizerList, adminList,events, participants);
                    break;
                case VENDOR:
                    vendorMenu(currentUser, vendorList, organizerList, adminList);
                    break;
                default:
                    cout << "Unknown user type!" << endl;
                    pauseScreen();
                    break;
                }
            }
            break;
        case 2:
            userRegister(vendorList, organizerList, adminList);
            break;
        case 3:
            cout << "Thank you for using Wedding Management System!" << endl;
            cout << "Goodbye!" << endl;
            break;
        default:
            cout << "Invalid choice! Please try again." << endl;
            pauseScreen();
            break;
        }
    } while (choice != 3);
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

// void loadEventsFromFile(vector<WeddingEvent>& events, const string& filename) {
//     ifstream file(filename);
//     if (!file) return;

//     events.clear();
//     string line;
//     while (getline(file, line)) {
//         if (!line.empty()) {
//             try {
//                 events.push_back(WeddingEvent::fromFileString(line));
//             }
//             catch (...) {
//                 cout << "Error parsing event line: " << line << endl;
//             }
//         }
//     }
//     file.close();
// }

// void saveEventsToFile(const vector<WeddingEvent>& events, const string& filename) {
//     ofstream file(filename);
//     if (!file) {
//         cout << "Error opening events file for writing." << endl;
//         return;
//     }

//     for (const auto& event : events) {
//         file << event.toFileString() << endl;
//     }
//     file.close();
// }

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

    saveUserIntoFile(events, "events.txt");

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

    saveUserIntoFile(events, "events.txt");

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
            saveUserIntoFile(events, "events.txt");
            cout << "Wedding cancelled successfully." << endl;
        }
        else {
            cout << "Cancellation aborted." << endl;
        }
        pauseScreen();
        break;

    case 3:
        selectedEvent->status = "completed";
        saveUserIntoFile(events, "events.txt");
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
        cin.ignore();
        cout << "Invalid input. Please enter a number.\n\n";
        return -1;
    }

    cin.ignore();
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
        cin.ignore();
        //(numeric_limits<streamsize>::max(), '\n');
        cout << "Invalid input. Please enter a character.\n\n";
        return '\0'; // special value meaning "invalid"
    }

    cin.ignore(); // clean up buffer
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

void viewPaymentSummary(const CurrentUser& currentUser, const WeddingEvent& selectedEvent, const vector<Vendor>& vendorList) { // Changed to reference
    clearScreen();
    if (selectedEvent.status != "planning") { // Changed to '.'
        cout << "Payment summary is only available for weddings in the 'planning' state." << endl;
        pauseScreen();
        return;
    }

    cout << "=== PAYMENT SUMMARY (INVOICE) ===" << endl;
    cout << "Invoice for Wedding Event ID: " << selectedEvent.eventId << endl; // Changed to '.'
    cout << "Client: " << currentUser.userName << endl;
    cout << "Couple: " << selectedEvent.groomName << " & " << selectedEvent.brideName << endl; // Changed to '.'
    cout << "Wedding Date: " << selectedEvent.weddingDate << endl; // Changed to '.'
    cout << "---------------------------------------" << endl;
    cout << "Description                 Cost (RM)" << endl;
    cout << "---------------------------------------" << endl;

    double subtotal = 0.0;
    for (int serviceIdNum : selectedEvent.bookedServices) { // Changed to '.'
        bool found = false;
        for (const auto& vendor : vendorList) {
            for (const auto& service : vendor.serviceHasProvide) {
                if (service.serviceId == "S" + to_string(serviceIdNum)) {
                    cout << left << setw(30) << service.serviceName << right << setw(10) << fixed << setprecision(2) << service.price << endl;
                    subtotal += service.price;
                    found = true;
                    break;
                }
            }
            if (found) break;
        }
    }

    cout << "---------------------------------------" << endl;
    cout << left << setw(30) << "Subtotal" << right << setw(10) << fixed << setprecision(2) << subtotal << endl;

    double lateFine = 0.0;
    time_t now = time(0);
    tm nowTm;
    localtime_s(&nowTm, &now);

    tm weddingTm = { 0 };
    weddingTm.tm_year = stoi(selectedEvent.weddingDate.substr(0, 4)) - 1900; // Changed to '.'
    weddingTm.tm_mon = stoi(selectedEvent.weddingDate.substr(5, 2)) - 1; // Changed to '.'
    weddingTm.tm_mday = stoi(selectedEvent.weddingDate.substr(8, 2)); // Changed to '.'

    time_t weddingTime = mktime(&weddingTm);

    if (difftime(now, weddingTime) > 0) {
        lateFine = 50.0;
        cout << left << setw(30) << "Late Payment Fine" << right << setw(10) << fixed << setprecision(2) << lateFine << endl;
    }

    cout << left << setw(30) << "Grand Total" << right << setw(10) << fixed << setprecision(2) << (subtotal + lateFine) << endl;
    cout << "=======================================" << endl;

    if (lateFine > 0) {
        cout << "Note: A late payment fine of RM50 has been applied as the wedding date has passed." << endl;
    }

    pauseScreen();
}

void generateReport(const CurrentUser& currentUser, const WeddingEvent& selectedEvent, const vector<Vendor>& vendorList) { // Changed to reference
    clearScreen();
    if (selectedEvent.status != "completed") { // Changed to '.'
        cout << "Report can only be generated for weddings in the 'completed' state." << endl;
        pauseScreen();
        return;
    }

    vector<PaymentTransaction> transactions;
    saveUserIntoFile(transactions, "payment_history.txt");
    // ifstream file("payment_history.txt");
    // if (file.is_open()) {
    //     string line;
    //     while (getline(file, line)) {
    //         transactions.push_back(PaymentTransaction::fromFileString(line));
    //     }
    //     file.close();
    // }

    PaymentTransaction* transaction = nullptr;
    for (auto& pt : transactions) {
        if (pt.weddingId == selectedEvent.eventId) { // Changed to '.'
            transaction = &pt;
            break;
        }
    }

    if (!transaction) {
        cout << "No payment transaction found for this completed wedding." << endl;
        pauseScreen();
        return;
    }

    cout << "======================================" << endl;
    cout << "          WEDDING REPORT" << endl;
    cout << "======================================" << endl;
    cout << "Report Date: " << transaction->transactionDate << endl;
    cout << "Report Time: " << transaction->transactionTime << endl;
    cout << "--------------------------------------" << endl;
    cout << "Event ID: " << selectedEvent.eventId << endl; // Changed to '.'
    cout << "Couple: " << selectedEvent.groomName << " & " << selectedEvent.brideName << endl; // Changed to '.'
    cout << "Wedding Date: " << selectedEvent.weddingDate << endl; // Changed to '.'
    cout << "Wedding Venue: " << selectedEvent.weddingVenue << endl; // Changed to '.'
    cout << "Status: " << selectedEvent.status << endl; // Changed to '.'
    cout << "--------------------------------------" << endl;
    cout << "PAYMENT DETAILS" << endl;
    cout << "Transaction ID: " << transaction->transactionId << endl;
    cout << "Amount Paid: RM" << fixed << setprecision(2) << transaction->amount << endl;
    cout << "Payment Method: " << transaction->paymentMethod << endl;
    cout << "Payment Status: " << transaction->paymentStatus << endl;
    cout << "--------------------------------------" << endl;
    cout << "SERVICES RENDERED" << endl;
    for (int serviceIdNum : selectedEvent.bookedServices) { // Changed to '.'
        bool found = false;
        for (const auto& vendor : vendorList) {
            for (const auto& service : vendor.serviceHasProvide) {
                if (service.serviceId == "S" + to_string(serviceIdNum)) {
                    cout << "* " << service.serviceName << " (" << service.type << ") - RM" << fixed << setprecision(2) << service.price << endl;
                    found = true;
                    break;
                }
            }
            if (found) break;
        }
    }
    cout << "======================================" << endl;
    cout << "Note: This report confirms successful payment and completion of services for the event." << endl;
    pauseScreen();
}

void paymentAndReportingMenu(CurrentUser& currentUser, WeddingEvent* selectedEvent, vector<WeddingEvent>& events, const vector<Vendor>& vendorList) {
    int paymentChoice;
    do {
        clearScreen();
        cout << "=== PAYMENT & REPORTING MODULE ===" << endl;
        cout << "1. Make Payment" << endl;
        cout << "2. View Payment Summary" << endl;
        cout << "3. Generate Report" << endl;
        cout << "4. Return to Main Menu" << endl;
        cout << "Enter your choice: ";
        cin >> paymentChoice;
        cin.ignore();

        switch (paymentChoice) {
        case 1: {
            if (selectedEvent->totalCost <= 0) {
                cout << "No services booked yet. Nothing to pay." << endl;
                pauseScreen();
                break;
            }

            if (selectedEvent->status == "completed") {
                cout << "You are paid." << endl;
                pauseScreen();
                break;
            }

            string paymentMethod;
            int methodChoice;
            clearScreen();
            cout << "=== MAKE PAYMENT ===" << endl;
            cout << "Total amount to pay: RM" << fixed << setprecision(2) << selectedEvent->totalCost << endl;
            cout << "Select a payment method:" << endl;
            cout << "1. Credit/Debit Card" << endl;
            cout << "2. Online Banking" << endl;
            cout << "3. TNG" << endl;
            cout << "4. Cash" << endl;
            cout << "Enter your choice: ";
            cin >> methodChoice;
            cin.ignore();

            bool paymentSuccess = false;
            switch (methodChoice) {
            case 1: {
                paymentMethod = "Credit/Debit Card";
                string cardNumber, securityCode, expiryDate;
                cout << "Enter card number: ";
                getline(cin, cardNumber);
                cout << "Enter security code: ";
                getline(cin, securityCode);
                cout << "Enter expiry date (MM/YY): ";
                getline(cin, expiryDate);
                cout << "Payment processing..." << endl;
                paymentSuccess = true;
            } break;
            case 2: {
                paymentMethod = "Online Banking";
                string username, password;
                cout << "Enter bank username: ";
                getline(cin, username);
                cout << "Enter bank password: ";
                getline(cin, password);
                cout << "Payment processing..." << endl;
                paymentSuccess = true;
            } break;
            case 3: {
                paymentMethod = "TNG";
                string phoneNumber, password;
                cout << "Enter 11-digit Malaysian phone number: ";
                getline(cin, phoneNumber);
                cout << "Enter TNG password: ";
                getline(cin, password);
                cout << "Payment processing..." << endl;
                paymentSuccess = true;
            } break;
            case 4: {
                paymentMethod = "Cash";
                double cashAmount;
                cout << "Enter cash amount received: RM";
                cin >> cashAmount;
                cin.ignore();
                cout << "Payment processing..." << endl;
                paymentSuccess = true;
            } break;
            default:
                cout << "Invalid payment method!" << endl;
                pauseScreen();
                break;
            }

            if (paymentSuccess) {
                selectedEvent->status = "completed";
                saveUserIntoFile(events, "events.txt");

                time_t now = time(0);
                tm ltm;
                localtime_s(&ltm, &now);
                char dateStr[11];
                strftime(dateStr, sizeof(dateStr), "%Y-%m-%d", &ltm);
                char timeStr[9];
                strftime(timeStr, sizeof(timeStr), "%H:%M:%S", &ltm);

                clearScreen();
                cout << "======================================" << endl;
                cout << "          PAYMENT RECEIPT" << endl;
                cout << "======================================" << endl;
                cout << "User ID: " << currentUser.userId << endl;
                cout << "User Name: " << currentUser.userName << endl;
                cout << "Payment Date: " << dateStr << endl;
                cout << "Payment Time: " << timeStr << endl;
                cout << "Status: PAID" << endl;
                cout << "Payment Method: " << paymentMethod << endl;
                cout << "Amount Paid: RM" << fixed << setprecision(2) << selectedEvent->totalCost << endl;
                cout << "======================================" << endl;
                cout << "Payment successful!" << endl;

                ofstream paymentFile("payment_history.txt", ios::app);
                if (paymentFile) {
                    PaymentTransaction newTransaction;
                    size_t transactionCount = 0;
                    ifstream tempFile("payment_history.txt");
                    string line;
                    while (getline(tempFile, line)) {
                        transactionCount++;
                    }
                    tempFile.close();

                    newTransaction.transactionId = generateId("T", transactionCount + 1);
                    newTransaction.userId = currentUser.userId;
                    newTransaction.userName = currentUser.userName;
                    newTransaction.weddingId = selectedEvent->eventId;
                    newTransaction.amount = selectedEvent->totalCost;
                    newTransaction.paymentMethod = paymentMethod;
                    newTransaction.paymentStatus = "PAID";
                    newTransaction.transactionDate = dateStr;
                    newTransaction.transactionTime = timeStr;
                    paymentFile << newTransaction.toFileString() << endl;
                    paymentFile.close();
                }
                pauseScreen();
                return;
            }
            else {
                selectedEvent->status = "planning";
                saveUserIntoFile(events, "events.txt");
            }
        } break;
        case 2:
            viewPaymentSummary(currentUser, *selectedEvent, vendorList); // Changed call to use '*'
            break;
        case 3:
            generateReport(currentUser, *selectedEvent, vendorList); // Changed call to use '*'
            break;
        case 4:
            return;
        default:
            cout << "Invalid choice! Please try again." << endl;
            pauseScreen();
            break;
        }
    } while (true);
}

void viewPaymentHistory(CurrentUser& currentUser) {
    clearScreen();
    cout << "=== PAYMENT HISTORY ===" << endl;

    ifstream file("payment_history.txt");
    if (!file) {
        cout << "No Payment History." << endl;
        pauseScreen();
        return;
    }

    string line;
    bool foundHistory = false;
    while (getline(file, line)) {
        PaymentTransaction pt = PaymentTransaction::fromFileString(line);
        if (pt.userId == currentUser.userId) {
            cout << "--------------------------------------" << endl;
            cout << "Transaction ID: " << pt.transactionId << endl;
            cout << "Wedding ID: " << pt.weddingId << endl;
            cout << "Amount: RM" << fixed << setprecision(2) << pt.amount << endl;
            cout << "Payment Method: " << pt.paymentMethod << endl;
            cout << "Status: " << pt.paymentStatus << endl;
            cout << "Date: " << pt.transactionDate << endl;
            cout << "Time: " << pt.transactionTime << endl;
            cout << "--------------------------------------" << endl;
            foundHistory = true;
        }
    }
    file.close();

    if (!foundHistory) {
        cout << "No Payment History." << endl;
    }
    pauseScreen();
}

int main() {
    vector<Vendor> vendorList;
    vector<Organizer> organizerList;
    vector<Admin> adminList;
    vector<WeddingEvent> events;
    vector<Participant> participants;
    CurrentUser currentUser;

    // Load data from files
    loadUserFromFile<Admin>(adminList, "admins.txt");
    loadUserFromFile<Organizer>(organizerList, "organizers.txt");
    loadUserFromFile<Vendor>(vendorList, "vendors.txt");
    loadUserFromFile(events, "events.txt");
    addDefaultParticipants(participants);

    // Start main menu
    mainMenu(currentUser, vendorList, organizerList, adminList, events, participants);

    // Save data before exit
    saveUserIntoFile<Admin>(adminList, "admins.txt");
    saveUserIntoFile<Organizer>(organizerList, "organizers.txt");
    saveUserIntoFile<Vendor>(vendorList, "vendors.txt");
    saveUserIntoFile(events, "events.txt");

    return 0;
}