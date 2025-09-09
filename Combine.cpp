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
#include <regex>
using namespace std;

struct Service;
struct BaseInfo;
struct Vendor;
struct Organizer;
struct Admin;
struct CurrentUser;
struct WeddingEvent;
struct Attendance;
struct Participant;
struct PaymentTransaction;
struct InvitationCard;

// Function declarations
template <typename T>
void saveDataIntoFile(vector<T>& data, string fileName);

template <typename T>
void loadDataFromFile(vector<T>& data, string fileName);
void loadingTime();
string generateId(const string& prefix, int counter);
void clearScreen();
void pauseScreen();
void getBaseUserInfo(BaseInfo& baseInfo, vector<Admin>& adminList, vector<Organizer>& organizerList, vector<Vendor>& vendorList);
void getVendorInfo(Vendor& vendor, vector<Admin>& adminList, vector<Organizer>& organizerList, vector<Vendor>& vendorList);
void getOrganizerInfo(Organizer& organizer, vector<Admin>& adminList, vector<Organizer>& organizerList, vector<Vendor>& vendorList);
bool getAdminInfo(Admin& admin, vector<Admin>& adminList, vector<Organizer>& organizerList, vector<Vendor>& vendorList);
bool login(vector<Vendor>& vendorList, vector<Organizer>& organizerList, vector<Admin>& adminList, CurrentUser& currentUser);
void logout(CurrentUser& currentUser);
void addService(CurrentUser& currentUser, vector<Vendor>& vendorList);
void displayUserProfile(CurrentUser& currentUser, vector<Vendor>& vendorList, vector<Organizer>& organizerList, vector<Admin>& adminList);
void displayAllUser(vector<Vendor>& vendorList, vector<Organizer>& organizerList, vector<Admin>& adminList);
void userRegister(vector<Vendor>& vendorList, vector<Organizer>& organizerList, vector<Admin>& adminList);
void updateBaseInfo(BaseInfo& baseInfo, vector<Admin>& adminList, vector<Organizer>& organizerList, vector<Vendor>& vendorList);
void updateAdminInfo(Admin& admin, vector<Admin>& adminList, vector<Organizer>& organizerList, vector<Vendor>& vendorList);
void updateVendorInfo(Vendor& vendor, vector<Admin>& adminList, vector<Organizer>& organizerList, vector<Vendor>& vendorList);
void updateUserProfile(CurrentUser& currentUser, vector<Vendor>& vendorList, vector<Organizer>& organizerList, vector<Admin>& adminList);
void updateOwnService(CurrentUser& currentUser, vector<Vendor>& vendorList);
void displayAllServices(vector<Vendor>& vendorList);
void displayServicesByVendor(vector<Vendor>& vendorList);
void displayServicesByType(vector<Vendor>& vendorList);
void displayBookedServices(CurrentUser& currentUser, vector<Organizer>& organizerList, vector<Vendor>& vendorList);
bool deleteOwnAccount(CurrentUser& currentUser, vector<Vendor>& vendorList, vector<Organizer>& organizerList, vector<Admin>& adminList);
void deleteOwnService(CurrentUser& currentUser, vector<Vendor>& vendorList);
void generateInvitationCardMenu(CurrentUser& currentUser, vector<WeddingEvent>& events, vector<Organizer>& organizerList);
void generateCustomInvitation(vector<WeddingEvent>& events, WeddingEvent& wedding, string contactNumber, CurrentUser& currentUser);
void invitationTemplate(vector<WeddingEvent>& events, WeddingEvent& wedding, CurrentUser& currentUser);
string template1(WeddingEvent& wedding, string& contactNumber);
string template2(WeddingEvent& wedding, string& contactNumber);
string template3(WeddingEvent& wedding, string& contactNumber);
string template4(WeddingEvent& wedding, string& contactNumber);
string template5(WeddingEvent& wedding, string& contactNumber);
void showInvitationCards(CurrentUser& currentUser, vector<InvitationCard>& existingCards);
void deleteInvitationCard(CurrentUser& currentUser, vector<InvitationCard>& existingCards);
// Event related functions
void createNewWedding(CurrentUser& currentUser, vector<WeddingEvent>& events, vector<Vendor>& vendorList, vector<Organizer>& organizerList);
void bookServicesForWedding(CurrentUser& currentUser, vector<WeddingEvent>& events, vector<Vendor>& vendorList, vector<Organizer>& organizerList);
void viewAllWeddings(CurrentUser& currentUser, const vector<WeddingEvent>& events, const vector<Vendor>& vendorList);
void viewAllMyWeddings(CurrentUser& currentUser, const vector<WeddingEvent>& events, const vector<Vendor>& vendorList);
void viewWeddingsByOrganizer(vector<WeddingEvent>& events, vector<Organizer>& organizerList);
void viewWeddingByStatus(vector<WeddingEvent>& events);
void manageMyWeddings(CurrentUser& currentUser, vector<WeddingEvent>& events, vector<Vendor>& vendorList, vector<Organizer>& organizerList);
void generateInvitationCard(CurrentUser& currentUser, const vector<WeddingEvent>& events, const vector<Organizer>& organizerList);
bool isValidDate(const string& date);
bool isValidWeddingDate(const string& date);
bool isDateAvailable(const string& date, const vector<WeddingEvent>& events, const string& venue);
bool isValidBudget(double budget);
double calculateServiceCost(const vector<int>& serviceIds, const vector<Vendor>& vendorList);
void setWeddingToCurrent(CurrentUser& currentUser, vector<Organizer>& organizerList, vector<WeddingEvent>& events);
void cancelWedding(string currentWeddingId, vector<WeddingEvent>& events, vector<Vendor>& vendorList, vector<Participant>& participants);

// Monitoring functions
void menu(vector<Participant>& participants);
bool addDefaultParticipants(vector<Participant>& participants);
void addParticipantsMenu(vector<Participant>& participants, WeddingEvent currentEvent);
void searchParticipantsMenu(vector<Participant>& participants);
void markAttendance(vector<Participant>& participants, string weddingId);
void updateParticipants(vector<Participant>& participants);
void deleteParticipants(vector<Participant>& participants);
void showAllParticipants(vector<Participant> participants, WeddingEvent currentEvent);
void deleteAllParticipants(vector<Participant>& participants, string eventId);
// Payment functions
void paymentAndReportingMenu(CurrentUser& currentUser, vector<WeddingEvent>& events, vector<Vendor>& vendorList);
void viewPaymentSummary(CurrentUser& currentUser, WeddingEvent& selectedEvent, vector<Vendor>& vendorList);
bool processPayment(CurrentUser& currentUser, vector<WeddingEvent>& events, WeddingEvent& selectedEvent);
void receipt(CurrentUser& currentUser, WeddingEvent& selectedEvent, string paymentMethod);
void generateReport(CurrentUser& currentUser, WeddingEvent& selectedEvent, vector<Vendor>& vendorList);
void viewPaymentHistory(CurrentUser& currentUser);
void generateAllPaidReports(vector<WeddingEvent>& events, vector<Vendor>& vendorList, vector<PaymentTransaction>& transactions);

// Menu functions
void UpdateWeddingMenu(CurrentUser& currentUser, vector<Vendor>& vendorList, vector<Organizer>& organizerList, vector<Admin>& adminList, vector<WeddingEvent>& events);
void organizerMenu(CurrentUser& currentUser, vector<Vendor>& vendorList, vector<Organizer>& organizerList, vector<Admin>& adminList, vector<WeddingEvent>& events, vector<Participant>& participants);
void adminMenu(CurrentUser& currentUser, vector<Vendor>& vendorList, vector<Organizer>& organizerList, vector<Admin>& adminList, vector<WeddingEvent>& events, vector<Participant>& participants, vector<PaymentTransaction>& transactions);
void vendorMenu(CurrentUser& currentUser, vector<Vendor>& vendorList, vector<Organizer>& organizerList, vector<Admin>& adminList);
void mainMenu(CurrentUser& currentUser, vector<Vendor>& vendorList, vector<Organizer>& organizerList, vector<Admin>& adminList, vector<WeddingEvent>& events, vector<Participant>& participants, vector<PaymentTransaction>& transactions);

// Add these function declarations
void readAllBookedServices(CurrentUser& currentUser, vector<WeddingEvent>& events, vector<Vendor>& vendorList, vector<Organizer>& organizerList);
void deleteBookedService(CurrentUser& currentUser, vector<WeddingEvent>& events, vector<Vendor>& vendorList, vector<Organizer>& organizerList);
void updateFile(ofstream& outFile, vector<Participant>& participants);
void participantMenu(vector<Participant>& participants, WeddingEvent currentEvent);

struct Service {
    string serviceId;
    string serviceName;
    string description;
    string type;
    double price;
    int quantity;
    bool available;
    string toFileString() {
        return serviceId + "|" + serviceName + "|" + description + "|" + type + "|" +
            to_string(price) + "|" + to_string(quantity) + "|" + (available ? "1" : "0");
    }

    static Service fromFileString(string& str) {
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

    static BaseInfo fromFileString(string& str)
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
        for (auto& s : serviceHasProvide)
        {
            if (!servicesStr.empty())
                servicesStr += "##";
            servicesStr += s.toFileString();
        }

        return baseStr + "|" + vendorStr + "|" + servicesStr;
    }

    static Vendor fromFileString(string& str)
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

    string toFileString()
    {

        return baseInfo.toFileString() + "|" + organizerId + "|" + currentWeddingId + "|";
    }

    static Organizer fromFileString(string& str)
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

    static Admin fromFileString(string& str)
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
enum UserType {
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
    vector<string> bookedServices;

    // Method to serialize to file string
    string toFileString() {
        string servicesStr;
        for (int i = 0; i < bookedServices.size(); ++i) {
            servicesStr += bookedServices[i];
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

        getline(ss, temp, '|'); event.budget = stod(temp);
        getline(ss, temp, '|'); event.totalCost = stod(temp);
        getline(ss, event.status, '|');

        string servicesStr;
        getline(ss, servicesStr);
        event.bookedServices.clear();
        if (!servicesStr.empty()) {
            stringstream ssServices(servicesStr);
            string serviceIdStr;
            while (getline(ssServices, serviceIdStr, ',')) {
                if (!serviceIdStr.empty()) {
                    event.bookedServices.push_back(serviceIdStr);
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
    string eventId;
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

struct InvitationCard {
    string userId;
    string weddingId;
    string cardContent;

    string toFileString() const {
        return userId + "%" + weddingId + "%" + cardContent + "$";
    }

    static InvitationCard fromFileString(const string& str) {
        InvitationCard card;
        stringstream ss(str);
        getline(ss, card.userId, '%');
        getline(ss, card.weddingId, '%');
        getline(ss, card.cardContent, '$');
        return card;
    }
};

template <typename T>
void saveDataIntoFile(vector<T>& data, string fileName) {
    ofstream write(fileName);

    if (!write)
    {
        cout << "Error opening file." << endl;
        return;
    }
    // 'auto' means the compiler will automatically figure out the type of this variable based on its value
    for (auto& obj : data)
    {
        write << obj.toFileString() << endl;
    }
    write.close();
}

template <typename T>
void loadDataFromFile(vector<T>& data, string fileName)
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

void loadInvitationCard(vector<InvitationCard>& existingCards) {
    existingCards.clear();

    ifstream file("invitation_cards.txt");
    if (!file.is_open()) {
        return;
    }

    string line;
    while (getline(file, line)) {
        if (!line.empty()) {
            InvitationCard card = InvitationCard::fromFileString(line);
            existingCards.push_back(card);
        }
    }

    file.close();
}

bool isValidEmail(const string& email) {
    // Simple regex pattern for email validation
    const regex pattern(R"([a-zA-Z0-9._%+-]+@[a-zA-Z0-9.-]+\.[a-zA-Z]{2,})");
    return regex_match(email, pattern);
}

bool isValidMalaysianPhone(const string& phone) {
    // Malaysian phone number regex pattern
    // Matches: +60xxxxxxxxx, 60xxxxxxxxx, 0xxxxxxxxx, or xxxxxxxxx
    // Mobile: 01x-xxxx-xxxx (10-11 digits)
    // Landline: 0x-xxxx-xxxx (9-10 digits)
    const regex pattern(R"(^(\+?60|0)?[1-9][0-9]{7,9}$)");

    return regex_match(phone, pattern);
}

bool isEmailExist(string& email, vector<Admin>& adminList, vector<Organizer>& organizerList, vector<Vendor>& vendorList) {
    // Check in admin list
    for (auto& admin : adminList) {
        if (admin.baseInfo.email == email) {
            return true;
        }
    }
    // Check in organizer list
    for (auto& organizer : organizerList) {
        if (organizer.baseInfo.email == email) {
            return true;
        }
    }
    // Check in vendor list
    for (auto& vendor : vendorList) {
        if (vendor.baseInfo.email == email) {
            return true;
        }
    }
    return false; // Email not found in any list
}

string generateId(const string& prefix, int counter) {
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

void getBaseUserInfo(BaseInfo& baseInfo, vector<Admin>& adminList, vector<Organizer>& organizerList, vector<Vendor>& vendorList)
{

    cout << "Enter name: ";
    getline(cin, baseInfo.name);

    do {
        cout << "Enter email: ";
        getline(cin, baseInfo.email);
        if (baseInfo.email == "0") {
            return;
        }
        if (!isValidEmail(baseInfo.email)) {
            cout << "Invalid email format. Please try again. Enter '0' to exit. " << endl;
            continue;
        }
        if (isEmailExist(baseInfo.email, adminList, organizerList, vendorList)) {
            cout << "Email already exists. Please try again. Enter '0' to exit. " << endl;
            continue;;
        }
        break; // Valid email
    } while (true);

    do {
        cout << "Enter phone: ";
        getline(cin, baseInfo.phoneNum);
        if (baseInfo.phoneNum == "0") {
            return;
        }
        if (!isValidMalaysianPhone(baseInfo.phoneNum)) {
            cout << "Invalid Malaysian phone number. Please try again. Enter '0' to exit. " << endl;
        }
        break;
    } while (true);

    cout << "Enter password: ";
    getline(cin, baseInfo.password);
}

void getVendorInfo(Vendor& vendor, vector<Admin>& adminList, vector<Organizer>& organizerList, vector<Vendor>& vendorList)
{
    // Get base info
    getBaseUserInfo(vendor.baseInfo, adminList, organizerList, vendorList);

    // Get vendor-specific info
    vendor.vendorId = generateId("V", vendorList.size() + 1);

    cout << "Enter company name: ";
    getline(cin, vendor.companyName);

    cout << "Enter company contact number: ";
    getline(cin, vendor.companyContactNum);

    cout << "Enter vendor type (e.g., 'individual', 'company'): ";
    getline(cin, vendor.type);
};

void getOrganizerInfo(Organizer& organizer, vector<Admin>& adminList, vector<Organizer>& organizerList, vector<Vendor>& vendorList)
{
    string input;

    // Get base info
    getBaseUserInfo(organizer.baseInfo, adminList, organizerList, vendorList);

    organizer.organizerId = generateId("O", organizerList.size() + 1);
}

bool getAdminInfo(Admin& admin, vector<Admin>& adminList, vector<Organizer>& organizerList, vector<Vendor>& vendorList)
{
    string secretPassword = "Secret";
    string inputPassword;
    getBaseUserInfo(admin.baseInfo, adminList, organizerList, vendorList);

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

bool login(vector<Vendor>& vendorList, vector<Organizer>& organizerList, vector<Admin>& adminList, CurrentUser& currentUser) {
    clearScreen();
    cout << "=== LOGIN ===" << endl;

    string email, password;
    cout << "Enter email: ";
    getline(cin, email);

    cout << "Enter password: ";
    getline(cin, password);

    // Check admin
    for (int i = 0; i < adminList.size(); i++)
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
    for (int i = 0; i < organizerList.size(); i++)
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
    for (int i = 0; i < vendorList.size(); i++) {
        if (vendorList[i].baseInfo.email == email && vendorList[i].baseInfo.password == password) {
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

void logout(CurrentUser& currentUser)
{
    char confirmed;
    cout << "Are you sure you want to logout (Y/N)";
    cin >> confirmed;
    if (toupper(confirmed) == 'Y')
    {
        currentUser.type = UserType::NONE;
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

void addService(CurrentUser& currentUser, vector<Vendor>& vendorList)
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

    int categoryChoice;
    while (true) {
        cout << "\nChoose Service Category:" << endl;
        cout << "1. Catering" << endl;
        cout << "2. Photography" << endl;
        cout << "3. Decoration" << endl;
        cout << "4. Music / Entertainment" << endl;
        cout << "5. Venue" << endl;
        cout << "Enter your choice (1-5): ";
        cin >> categoryChoice;

        if (cin.fail()) {
            cin.clear();
            cin.ignore();
            cout << "Invalid input. Please enter a number between 1 and 5.\n";
            continue;
        }

        switch (categoryChoice)
        {
        case 1: newService.type = "Catering"; break;
        case 2: newService.type = "Photography"; break;
        case 3: newService.type = "Decoration"; break;
        case 4: newService.type = "Music / Entertainment"; break;
        case 5: newService.type = "Venue"; break;
        default:
            cout << "Invalid choice. Please try again.\n";
            continue;
        }
        break; // valid choice → exit loop
    }

    cout << "Price: RM";
    cin >> newService.price;

    cout << "Quantity available: ";
    cin >> newService.quantity;
    cin.ignore();

    newService.available = true;

    // Add to current vendor's services
    vendorList[currentUser.userIndex].serviceHasProvide.push_back(newService);
    vendorList[currentUser.userIndex].totalServicesProvided++;

    saveDataIntoFile<Vendor>(vendorList, "vendors.txt");
    cout << "Service added successfully!" << endl;
    pauseScreen();
}

void displayUserProfile(CurrentUser& currentUser, vector<Vendor>& vendorList, vector<Organizer>& organizerList, vector<Admin>& adminList)
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
        Admin& admin = adminList[currentUser.userIndex];
        cout << "User Type: Administrator" << endl;
        cout << "Admin ID: " << admin.adminId << endl;
        cout << "Name: " << admin.baseInfo.name << endl;
        cout << "Email: " << admin.baseInfo.email << endl;
        cout << "Phone: " << admin.baseInfo.phoneNum << endl;
        break;
    }
    case ORGANIZER:
    {
        Organizer& org = organizerList[currentUser.userIndex];
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
        break;
    }
    case VENDOR:
    {
        Vendor& vendor = vendorList[currentUser.userIndex];
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

void displayAllUser(vector<Vendor>& vendorList, vector<Organizer>& organizerList, vector<Admin>& adminList)
{
    clearScreen();
    cout << "=== ALL USERS LIST ===" << endl;

    cout << "\n--- ADMINISTRATORS ---" << endl;
    for (Admin& admin : adminList)
    {
        cout << "Admin ID: " << admin.adminId << " | Name: " << admin.baseInfo.name
            << " | Email: " << admin.baseInfo.email << endl;
    }

    cout << "\n--- ORGANIZERS ---" << endl;
    for (Organizer& org : organizerList)
    {
        cout << "Organizer ID: " << org.organizerId << " | Name: " << org.baseInfo.name
            << " | Email: " << org.baseInfo.email << endl;
    }

    cout << "\n--- VENDORS ---" << endl;
    for (Vendor& vendor : vendorList)
    {
        cout << "Vendor ID: " << vendor.vendorId << " | Name: " << vendor.baseInfo.name
            << " | Email: " << vendor.baseInfo.email
            << " | Company: " << vendor.companyName << endl;
    }

    pauseScreen();
}

void userRegister(vector<Vendor>& vendorList, vector<Organizer>& organizerList, vector<Admin>& adminList)
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
        if (getAdminInfo(admin, adminList, organizerList, vendorList))
        {
            adminList.push_back(admin);
        }
        break;
    }
    case 2:
    {
        Organizer organizer;
        getOrganizerInfo(organizer, adminList, organizerList, vendorList);
        organizerList.push_back(organizer);
        break;
    }
    case 3:
    {
        Vendor vendor;
        getVendorInfo(vendor, adminList, organizerList, vendorList);
        vendorList.push_back(vendor);
        break;
    }
    default:
        cout << "Invalid choice." << endl;
        break;
    }

    saveDataIntoFile<Admin>(adminList, "admins.txt");
    saveDataIntoFile<Organizer>(organizerList, "organizers.txt");
    saveDataIntoFile<Vendor>(vendorList, "vendors.txt");
}

void updateBaseInfo(BaseInfo& baseInfo, vector<Admin>& adminList, vector<Organizer>& organizerList, vector<Vendor>& vendorList)
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
        do {
            cout << "Enter email: ";
            getline(cin, baseInfo.email);
            if (baseInfo.email == "0") {
                return;
            }
            if (!isValidEmail(baseInfo.email)) {
                cout << "Invalid email format. Please try again. Enter '0' to exit. " << endl;
            }
            if (isEmailExist(baseInfo.email, adminList, organizerList, vendorList)) {
                cout << "Email already exists. Please try again. Enter '0' to exit. " << endl;
            }
            break;
        } while (true);
        cout << "Email updated successfully!" << endl;
        break;
    case 3:
        do {
            cout << "Enter phone: ";
            getline(cin, baseInfo.phoneNum);
            if (baseInfo.phoneNum == "0") {
                return;
            }
            if (!isValidMalaysianPhone(baseInfo.phoneNum)) {
                cout << "Invalid Malaysian phone number. Please try again. Enter '0' to exit. " << endl;
            }
            break;
        } while (true);
        cout << "Phone updated successfully!" << endl;
        break;
    case 4:
        cout << "Enter new password: ";
        getline(cin, baseInfo.password);
        cout << "Password updated successfully!" << endl;
        break;
    case 5:
        getBaseUserInfo(baseInfo, adminList, organizerList, vendorList);
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

void updateAdminInfo(Admin& admin, vector<Admin>& adminList, vector<Organizer>& organizerList, vector<Vendor>& vendorList)
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
        updateBaseInfo(admin.baseInfo, adminList, organizerList, vendorList);
        break;
    case 0:
        return;
    default:
        cout << "Invalid choice!" << endl;
        pauseScreen();
        break;
    }
}

void updateVendorInfo(Vendor& vendor, vector<Admin>& adminList, vector<Organizer>& organizerList, vector<Vendor>& vendorList)
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
        updateBaseInfo(vendor.baseInfo, adminList, organizerList, vendorList);
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
        updateBaseInfo(vendor.baseInfo, adminList, organizerList, vendorList);
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

void updateUserProfile(CurrentUser& currentUser, vector<Vendor>& vendorList, vector<Organizer>& organizerList, vector<Admin>& adminList)
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
        updateAdminInfo(adminList[currentUser.userIndex], adminList, organizerList, vendorList);
        updated = true;
        break;
    }
    case ORGANIZER:
    {
        updateBaseInfo(organizerList[currentUser.userIndex].baseInfo, adminList, organizerList, vendorList);
        updated = true;
        break;
    }
    case VENDOR:
    {
        updateVendorInfo(vendorList[currentUser.userIndex], adminList, organizerList, vendorList);
        updated = true;
        break;
    }
    }

    if (updated)
    {
        // Save the updated data to files
        saveDataIntoFile<Admin>(adminList, "admins.txt");
        saveDataIntoFile<Organizer>(organizerList, "organizers.txt");
        saveDataIntoFile<Vendor>(vendorList, "vendors.txt");
    }
}

void updateOwnService(CurrentUser& currentUser, vector<Vendor>& vendorList)
{
    if (currentUser.type != VENDOR)
    {
        cout << "Only vendors can update services!" << endl;
        pauseScreen();
        return;
    }

    clearScreen();
    cout << "=== UPDATE SERVICES ===" << endl;

    Vendor& vendor = vendorList[currentUser.userIndex];

    if (vendor.serviceHasProvide.empty())
    {
        cout << "No services available to update!" << endl;
        pauseScreen();
        return;
    }

    cout << "\nYour Services:" << endl;
    int index = 1;
    for (auto& service : vendor.serviceHasProvide)
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

    if (serviceIndex < 1 || serviceIndex >(int)vendor.serviceHasProvide.size())
    {
        cout << "Invalid selection!" << endl;
        pauseScreen();
        return;
    }

    Service& service = vendor.serviceHasProvide[serviceIndex - 1];

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

    saveDataIntoFile<Vendor>(vendorList, "vendors.txt");
    loadDataFromFile<Vendor>(vendorList, "vendors.txt");
    pauseScreen();
}

void displayAllServices(vector<Vendor>& vendorList)
{
    bool hasServices = false;

    clearScreen();
    cout << "=== ALL AVAILABLE SERVICES ===" << endl;

    for (auto& vendor : vendorList)
    {
        if (!vendor.serviceHasProvide.empty())
        {
            int serviceNum = 1;
            hasServices = true;
            cout << "\n--- Services by " << vendor.baseInfo.name << " (" << vendor.companyName << ") ---" << endl;
            cout << "Vendor ID: " << vendor.vendorId << " | Contact: " << vendor.baseInfo.phoneNum << endl;

            for (auto& service : vendor.serviceHasProvide)
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

void displayServicesByVendor(vector<Vendor>& vendorList)
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
    for (auto& vendor : vendorList)
    {
        cout << vendorNum << ". " << vendor.baseInfo.name << " (" << vendor.companyName << ")" << endl;
        cout << "   Vendor ID: " << vendor.vendorId << " | Services: " << vendor.serviceHasProvide.size() << endl;
        vendorNum++;
    }

    cout << "\nSelect vendor to view services (1-" << vendorList.size() << "): ";
    cin >> choice;
    cin.ignore();

    if (choice < 1 || choice >(int)vendorList.size())
    {
        cout << "Invalid selection!" << endl;
        pauseScreen();
        return;
    }

    Vendor& selectedVendor = vendorList[choice - 1];

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
        for (auto& service : selectedVendor.serviceHasProvide)
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

void displayServicesByType(vector<Vendor>& vendorList)
{
    clearScreen();
    cout << "=== SERVICES BY TYPE ===" << endl;

    // First, collect all unique service types
    vector<string> serviceTypes;
    for (auto& vendor : vendorList)
    {
        for (auto& service : vendor.serviceHasProvide)
        {
            bool typeExists = false;
            for (auto& existingType : serviceTypes)
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
    for (auto& type : serviceTypes)
    {
        cout << typeNum << ". " << type << endl;
        typeNum++;
    }

    int choice;
    cout << "\nSelect service type (1-" << serviceTypes.size() << "): ";
    cin >> choice;
    cin.ignore();

    if (choice < 1 || choice >(int)serviceTypes.size())
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
    for (auto& vendor : vendorList)
    {
        for (auto& service : vendor.serviceHasProvide)
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

bool deleteOwnAccount(CurrentUser& currentUser, vector<Vendor>& vendorList, vector<Organizer>& organizerList, vector<Admin>& adminList)
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
    saveDataIntoFile<Admin>(adminList, "admins.txt");
    saveDataIntoFile<Organizer>(organizerList, "organizers.txt");
    saveDataIntoFile<Vendor>(vendorList, "vendors.txt");
    loadDataFromFile<Admin>(adminList, "admins.txt");
    loadDataFromFile<Organizer>(organizerList, "organizers.txt");
    loadDataFromFile<Vendor>(vendorList, "vendors.txt");

    cout << "Account successfully deleted!" << endl;
    cout << "You will be logged out automatically." << endl;

    // Logout user
    logout(currentUser);

    pauseScreen();
    return true;
}

void deleteOwnService(CurrentUser& currentUser, vector<Vendor>& vendorList)
{
    if (currentUser.type != VENDOR)
    {
        cout << "Only vendors can delete services!" << endl;
        pauseScreen();
        return;
    }

    clearScreen();
    cout << "=== DELETE MY SERVICES ===" << endl;

    Vendor& vendor = vendorList[currentUser.userIndex];

    if (vendor.serviceHasProvide.empty())
    {
        cout << "You have no services to delete." << endl;
        pauseScreen();
        return;
    }

    // Display current services
    cout << "Your Current Services:" << endl;
    int serviceNum = 1;
    for (auto& service : vendor.serviceHasProvide)
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

    if (choice < 1 || choice >(int)vendor.serviceHasProvide.size())
    {
        cout << "Invalid selection!" << endl;
        pauseScreen();
        return;
    }

    // Show selected service details
    Service& selectedService = vendor.serviceHasProvide[choice - 1];
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

        saveDataIntoFile<Vendor>(vendorList, "vendors.txt");
        loadDataFromFile<Vendor>(vendorList, "vendors.txt");
        cout << "Service deleted successfully!" << endl;
    }
    else
    {
        cout << "Service deletion cancelled." << endl;
    }

    pauseScreen();
}

void cancelBookedService(CurrentUser& currentUser, vector<Organizer>& organizerList, vector<Vendor>& vendorList, vector<WeddingEvent>& events)
{
    int cancelChoice;
    char confirm;
    if (currentUser.type != ORGANIZER)
    {
        cout << "Only organizers can cancel booked services!" << endl;
        pauseScreen();
        return;
    }

    clearScreen();
    cout << "=== CANCEL BOOKED SERVICES ===" << endl;
    Organizer& organizer = organizerList[currentUser.userIndex];

    for (int i = 0; i < events.size(); i++) {
        if (events[i].eventId == currentUser.currentWeddingId) {
            if (events[i].bookedServices.empty()) {
                cout << "You have no booked services to cancel." << endl;
                pauseScreen();
                return;
            }
            break;
        }
        readAllBookedServices(currentUser, events, vendorList, organizerList);
        cout << "\nSelect service to cancel (1-" << events[i].bookedServices.size() << ") or 0 to go back: ";
        cin >> cancelChoice;
        cin.ignore();
        if (cancelChoice == 0)
        {
            cout << "Service cancellation cancelled." << endl;
            pauseScreen();
            return;
        }
        if (cancelChoice < 1 || cancelChoice >(int)events[i].bookedServices.size()) {
            cout << "Invalid selection!" << endl;
            pauseScreen();
            return;
        }
        for (int j = 0; j < vendorList.size(); j++) {
            for (int k = 0; k < vendorList[j].serviceHasProvide.size(); k++) {
                if (vendorList[j].serviceHasProvide[k].serviceId == events[i].bookedServices[cancelChoice - 1]) {
                    cout << "Service: " << vendorList[j].serviceHasProvide[k].serviceName << endl;
                    cout << "Description: " << vendorList[j].serviceHasProvide[k].description << endl;
                    cout << "Type: " << vendorList[j].serviceHasProvide[k].type << endl;
                    cout << "Price: RM" << fixed << setprecision(2) << vendorList[j].serviceHasProvide[k].price << endl;
                    break;
                }
            }
        }
        cout << "Are you sure you want to cancel this booked service? (y/n): ";
        cin >> confirm;
        cin.ignore();
        events[i].bookedServices.erase(events[i].bookedServices.begin() + (cancelChoice - 1));
        events[i].totalCost -= events[i].totalCost;

        for (int j = 0; j < vendorList.size(); j++) {
            for (int k = 0; k < vendorList[j].serviceHasProvide.size(); k++) {
                if (vendorList[j].serviceHasProvide[k].serviceId == events[i].bookedServices[cancelChoice - 1]) {
                    vendorList[j].serviceHasProvide[k].quantity += 1;
                    break;
                }
            }
        }
    }
    pauseScreen();
}

void readAllBookedServices(CurrentUser& currentUser, vector<WeddingEvent>& events, vector<Vendor>& vendorList, vector<Organizer>& organizerList) {
    if (currentUser.type == VENDOR) {
        cout << "Vendor cannot view booked services!" << endl;
        pauseScreen();
        return;
    }

    // Get current wedding ID from organizer
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

    // Find the current wedding event
    const WeddingEvent* currentEvent = nullptr;
    for (const auto& event : events) {
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
    cout << "=== BOOKED SERVICES FOR CURRENT WEDDING ===" << endl;
    cout << "Event: " << currentEvent->groomName << " & " << currentEvent->brideName << endl;
    cout << "Wedding Date: " << currentEvent->weddingDate << endl;
    cout << "Wedding Venue: " << currentEvent->weddingVenue << endl;
    cout << "Total Budget: RM" << fixed << setprecision(2) << currentEvent->budget << endl;
    cout << "Current Total Cost: RM" << fixed << setprecision(2) << currentEvent->totalCost << endl;
    cout << "Remaining Budget: RM" << fixed << setprecision(2) << (currentEvent->budget - currentEvent->totalCost) << endl;
    cout << string(80, '=') << endl;

    if (currentEvent->bookedServices.empty()) {
        cout << "No services booked for this wedding yet." << endl;
    }
    else {
        cout << "Total Booked Services: " << currentEvent->bookedServices.size() << endl;
        cout << string(80, '-') << endl;

        int serviceCount = 1;
        for (const string& bookedId : currentEvent->bookedServices) {
            bool serviceFound = false;

            // Search through all vendors and their services
            for (const auto& vendor : vendorList) {
                for (const auto& service : vendor.serviceHasProvide) {
                    if (service.serviceId == bookedId) {
                        cout << serviceCount << ". Service: " << service.serviceName << endl;
                        cout << "   Vendor: " << vendor.baseInfo.name << " (" << vendor.companyName << ")" << endl;
                        cout << "   Description: " << service.description << endl;
                        cout << "   Type: " << service.type << endl;
                        cout << "   Price: RM" << fixed << setprecision(2) << service.price << endl;
                        cout << "   Vendor Contact: " << vendor.baseInfo.phoneNum << endl;
                        cout << "   Vendor Email: " << vendor.baseInfo.email << endl;
                        cout << "   Service Status: " << (service.available ? "Available" : "Fully Booked") << endl;
                        cout << "   " << string(70, '-') << endl;
                        serviceFound = true;
                        break;
                    }
                }
                if (serviceFound) break;
            }

            if (!serviceFound) {
                cout << serviceCount << ". Service ID " << bookedId << " (Service details not found)" << endl;
                cout << "   " << string(70, '-') << endl;
            }

            serviceCount++;
        }

        cout << "\nSUMMARY:" << endl;
        cout << "Total Services Booked: " << currentEvent->bookedServices.size() << endl;
        cout << "Total Amount: RM" << fixed << setprecision(2) << currentEvent->totalCost << endl;
        cout << "Budget Utilization: " << fixed << setprecision(1)
            << (currentEvent->totalCost / currentEvent->budget * 100) << "%" << endl;
    }

    pauseScreen();
}

// Function to delete a booked service from current wedding
void deleteBookedService(CurrentUser& currentUser, vector<WeddingEvent>& events, vector<Vendor>& vendorList, vector<Organizer>& organizerList) {
    if (currentUser.type != ORGANIZER) {
        cout << "Only organizers can delete booked services!" << endl;
        pauseScreen();
        return;
    }

    // Get current wedding ID from organizer
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

    // Find the current wedding event
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
    if (currentEvent->status == "completed") {
        cout << "Cannot delete services from a completed wedding!" << endl;
        pauseScreen();
        return;
    }

    clearScreen();
    cout << "=== DELETE BOOKED SERVICE ===" << endl;
    cout << "Event: " << currentEvent->groomName << " & " << currentEvent->brideName << endl;
    cout << "Wedding Date: " << currentEvent->weddingDate << endl;
    cout << string(60, '=') << endl;

    if (currentEvent->bookedServices.empty()) {
        cout << "No services booked for this wedding." << endl;
        pauseScreen();
        return;
    }

    // Display booked services with selection numbers
    cout << "Currently Booked Services:" << endl;
    cout << string(60, '-') << endl;

    vector<string> validServiceIds; // Store only valid service IDs for selection
    int displayIndex = 1;

    for (const string& bookedId : currentEvent->bookedServices) {
        bool serviceFound = false;

        // Search through all vendors and their services
        for (const auto& vendor : vendorList) {
            for (const auto& service : vendor.serviceHasProvide) {
                if (service.serviceId == bookedId) {
                    cout << displayIndex << ". " << service.serviceName << endl;
                    cout << "   Vendor: " << vendor.baseInfo.name << " (" << vendor.companyName << ")" << endl;
                    cout << "   Type: " << service.type << endl;
                    cout << "   Price: RM" << fixed << setprecision(2) << service.price << endl;
                    cout << "   " << string(50, '-') << endl;

                    validServiceIds.push_back(bookedId);
                    displayIndex++;
                    serviceFound = true;
                    break;
                }
            }
            if (serviceFound) break;
        }

        if (!serviceFound) {
            cout << displayIndex << ". Service ID " << bookedId << " (Details not found - maybe deleted by vendor)" << endl;
            cout << "   Price: Unknown" << endl;
            cout << "   " << string(50, '-') << endl;

            validServiceIds.push_back(bookedId); // Still allow deletion of invalid services
            displayIndex++;
        }
    }

    if (validServiceIds.empty()) {
        cout << "No valid services to delete." << endl;
        pauseScreen();
        return;
    }

    int choice;
    cout << "\nSelect service to delete (1-" << validServiceIds.size() << ") or 0 to cancel: ";
    cin >> choice;

    // Input validation
    if (cin.fail()) {
        cin.clear();
        cin.ignore(10000, '\n');
        cout << "Invalid input! Please enter a number." << endl;
        pauseScreen();
        return;
    }
    cin.ignore();

    if (choice == 0) {
        cout << "Service deletion cancelled." << endl;
        pauseScreen();
        return;
    }

    if (choice < 1 || choice > static_cast<int>(validServiceIds.size())) {
        cout << "Invalid selection!" << endl;
        pauseScreen();
        return;
    }

    string selectedServiceId = validServiceIds[choice - 1];

    // Find service details and the actual index in bookedServices
    string serviceName = "Unknown Service";
    double servicePrice = 0.0;
    Vendor* serviceVendor = nullptr;
    Service* selectedService = nullptr;
    int actualIndex = -1;

    // Find the actual index in bookedServices vector
    for (int i = 0; i < currentEvent->bookedServices.size(); i++) {
        if (currentEvent->bookedServices[i] == selectedServiceId) {
            actualIndex = static_cast<int>(i);
            break;
        }
    }

    if (actualIndex == -1) {
        cout << "Error: Service not found in booked services!" << endl;
        pauseScreen();
        return;
    }

    // Find service details from vendor list
    for (auto& vendor : vendorList) {
        for (auto& service : vendor.serviceHasProvide) {
            if (service.serviceId == selectedServiceId) {
                serviceName = service.serviceName;
                servicePrice = service.price;
                serviceVendor = &vendor;
                selectedService = &service;
                break;
            }
        }
        if (selectedService) break;
    }

    // Confirmation
    cout << "\nYou selected to delete:" << endl;
    cout << "Service: " << serviceName << endl;
    if (serviceVendor) {
        cout << "Vendor: " << serviceVendor->baseInfo.name << endl;
    }
    cout << "Price: RM" << fixed << setprecision(2) << servicePrice << endl;

    if (servicePrice > 0) {
        cout << "\nThis will reduce your total cost from RM" << fixed << setprecision(2) << currentEvent->totalCost
            << " to RM" << fixed << setprecision(2) << (currentEvent->totalCost - servicePrice) << endl;
    }

    char confirm;
    cout << "\nAre you sure you want to delete this booked service? (y/n): ";
    cin >> confirm;
    cin.ignore();

    if (confirm != 'y' && confirm != 'Y') {
        cout << "Service deletion cancelled." << endl;
        pauseScreen();
        return;
    }

    // Remove service from booked services
    currentEvent->bookedServices.erase(currentEvent->bookedServices.begin() + actualIndex);

    // Update total cost only if service price is known
    if (servicePrice > 0) {
        currentEvent->totalCost -= servicePrice;
        // Ensure total cost doesn't go negative
        if (currentEvent->totalCost < 0) {
            currentEvent->totalCost = 0;
        }
    }

    // Update service availability
    if (selectedService) {
        selectedService->quantity++;
        selectedService->available = true;
    }

    // Save updated data
    saveDataIntoFile(events, "events.txt");
    saveDataIntoFile<Vendor>(vendorList, "vendors.txt");

    cout << "\nService '" << serviceName << "' deleted successfully!" << endl;
    cout << "New total cost: RM" << fixed << setprecision(2) << currentEvent->totalCost << endl;

    if (currentEvent->budget > 0) {
        cout << "Remaining budget: RM" << fixed << setprecision(2) << (currentEvent->budget - currentEvent->totalCost) << endl;
    }

    pauseScreen();
}

void setWeddingToCurrent(CurrentUser& currentUser, vector<Organizer>& organizerList, vector<WeddingEvent>& events) {
    if (currentUser.type != ORGANIZER) {
        cout << "Only organizers can manage weddings!" << endl;
        pauseScreen();
        return;
    }

    vector<WeddingEvent> myEvents;
    for (auto& event : events) {
        if (event.organizerId == currentUser.userId && event.status != "planning") {
            myEvents.push_back(event);
        }
    }

    clearScreen();
    cout << "=== SET CURRENT WEDDING ===" << endl;
    cout << "Your Weddings:" << endl;

    if (myEvents.empty()) {
        cout << "You have no avalaible weddings to set as current wedding!" << endl;
        pauseScreen();
        return;
    }

    for (int i = 0; i < myEvents.size(); i++) {
        cout << i + 1 << ". " << myEvents[i].groomName << " & " << myEvents[i].brideName << endl
            << " | Date: " << myEvents[i].weddingDate << endl
            << " | Status: " << myEvents[i].status << endl;
    }

    int choice;
    cout << "\nSelect wedding to set as current (1-" << myEvents.size() << ") or 0 to cancel: ";
    cin >> choice;
    cin.ignore();

    if (choice == 0) {
        cout << "Set current wedding cancelled." << endl;
        pauseScreen();
        return;
    }

    if (choice < 1 || choice >myEvents.size()) {
        cout << "Invalid selection!" << endl;
        pauseScreen();
        return;
    }

    // Set the selected wedding as current
    currentUser.currentWeddingId = myEvents[choice - 1].eventId;
    currentUser.currentWeddingId = currentUser.currentWeddingId;

    saveDataIntoFile<Organizer>(organizerList, "organizers.txt");
    cout << "Current wedding set to: " << myEvents[choice - 1].groomName << " & " << myEvents[choice - 1].brideName << endl;
    cout << "Wedding ID: " << myEvents[choice - 1].eventId << endl;
    pauseScreen();

}

void cancelWedding(string currentWeddingId, vector<WeddingEvent>& events, vector<Vendor>& vendorList, vector<Participant>& participants) {
    WeddingEvent selectedEvent;
    clearScreen();
    cout << "=== CANCEL WEDDING ===" << endl;
    for (int i = 0; i < events.size(); i++) {
        if (events[i].eventId == currentWeddingId) {
            if (events[i].status == "cancelled") {
                cout << "This wedding is already cancelled." << endl;
                pauseScreen();
                return;
            }
            if (events[i].status == "completed") {
                cout << "Cannot cancel a completed wedding." << endl;
                pauseScreen();
                return;
            }
            // Proceed to cancel the wedding
            selectedEvent = events[i];
            break;
        }
    }
    cout << "If you cancel this wedding, all booked services will be released and made available for other weddings." << endl;
    cout << "You are about to cancel the following wedding:" << endl;
    cout << "Wedding Details:" << endl;
    cout << "Wedding ID: " << selectedEvent.eventId << endl;
    cout << "Event: " << selectedEvent.groomName << " & " << selectedEvent.brideName << endl;
    cout << "Date: " << selectedEvent.weddingDate << endl;
    cout << "Are you sure you want to cancel this wedding? (y/n): ";
    char confirm;
    cin >> confirm;
    cin.ignore();

    if (confirm == 'y' || confirm == 'Y') {
        // Restore all booked services
        for (string& serviceId : selectedEvent.bookedServices) {
            for (auto& vendor : vendorList) {
                for (auto& service : vendor.serviceHasProvide) {
                    if (service.serviceId == serviceId) {
                        service.quantity++;
                        service.available = true;
                        break; // found the service, no need to keep searching
                    }
                }
            }
        }

        // Mark wedding as cancelled
        selectedEvent.status = "cancelled";

        // Clear booked services & reset cost
        selectedEvent.bookedServices.clear();
        selectedEvent.totalCost = 0;

        // Save updates
        saveDataIntoFile(events, "events.txt");
        saveDataIntoFile(vendorList, "vendors.txt");

        deleteAllParticipants(participants, currentWeddingId);

        cout << "Wedding cancelled successfully. All booked services were released." << endl;
    }
    else {
        cout << "Cancellation aborted." << endl;
    }
    pauseScreen();
}

void UpdateWeddingMenu(CurrentUser& currentUser, vector<Vendor>& vendorList, vector<Organizer>& organizerList, vector<Admin>& adminList, vector<WeddingEvent>& events) {
    int choice;
    do
    {
        clearScreen();
        cout << "=== SERVICES MANAGEMENT===" << endl;
        cout << "1. Book a New Service for Wedding" << endl;
        cout << "2. Read All The Booked Service" << endl;
        cout << "3. Cancel Booked Service" << endl << endl;

        cout << "=== WEDDING MANAGEMENT ===" << endl;
        cout << "4. Mark as Completed" << endl;
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
            saveDataIntoFile<Vendor>(vendorList, "vendors.txt");
            saveDataIntoFile<Organizer>(organizerList, "organizers.txt");
            break;
        case 2:
            readAllBookedServices(currentUser, events, vendorList, organizerList);
            break;
        case 3:
            cancelBookedService(currentUser, organizerList, vendorList, events);
            break;
        case 4:
            for (int i = 0; i < events.size(); i++) {
                if (events[i].eventId == currentUser.currentWeddingId) {
                    events[i].status = "completed";
                }
            }
            saveDataIntoFile(events, "events.txt");
            cout << "Wedding marked as completed." << endl;
            pauseScreen();
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

void MyProfileMenu(CurrentUser& currentUser, vector<Vendor>& vendorList, vector<Organizer>& organizerList, vector<Admin>& adminList) {
    int choice;
    do
    {
        clearScreen();
        cout << "=== MY PROFILE ===" << endl;
        cout << "1. View My Profile" << endl;
        cout << "2. Update My Profile" << endl;
        cout << "3. Delete My Account" << endl;
        cout << "0. Back to Main Menu" << endl;
        cout << "==========================================" << endl;
        cout << "Enter your choice: ";
        cin >> choice;
        cin.ignore();

        switch (choice)
        {
        case 1:
            displayUserProfile(currentUser, vendorList, organizerList, adminList);
            break;
        case 2:
            updateUserProfile(currentUser, vendorList, organizerList, adminList);
            pauseScreen();
            break;
        case 3:
            if (deleteOwnAccount(currentUser, vendorList, organizerList, adminList)) {
                return;
            }
        case 0:
            pauseScreen();
            return;
        default:
            cout << "Invalid choice! Please try again." << endl;
            pauseScreen();
            break;
        }
    } while (choice != 0);
}

void organizerMenu(CurrentUser& currentUser, vector<Vendor>& vendorList, vector<Organizer>& organizerList, vector<Admin>& adminList, vector<WeddingEvent>& events, vector<Participant>& participants)
{
    int choice;
    WeddingEvent* currentEvent = nullptr;

    do
    {
        clearScreen();
        cout << "=== ORGANIZER DASHBOARD ===" << endl;
        cout << "Welcome, " << currentUser.userName << " (Wedding Organizer)" << endl;
        // Display wedding info
        Organizer& org = organizerList[currentUser.userIndex];
        cout << "==========================================" << endl;
        cout << "1. Book a New Wedding" << endl;
        cout << "2. View My All Wedding" << endl;
        cout << "3. Update My Current Wedding Detail" << endl;
        cout << "4. Set as Current Wedding" << endl;
        cout << "5. Cancel Wedding" << endl;
        cout << "6. Generate Invitation Card" << endl;
        cout << "7. Monitoring" << endl;
        cout << "8. Payment for Current Wedding" << endl;
        cout << "9. My Profile" << endl;
        cout << "0. Logout" << endl;
        cout << "==========================================" << endl;
        cout << "Enter your choice: ";
        cin >> choice;
        cin.ignore();

        switch (choice)
        {
        case 1:
            createNewWedding(currentUser, events, vendorList, organizerList);
            saveDataIntoFile<Organizer>(organizerList, "organizers.txt");
            break;
        case 2:
            viewAllMyWeddings(currentUser, events, vendorList);
            break;
        case 3:
            UpdateWeddingMenu(currentUser, vendorList, organizerList, adminList, events);
            break;
        case 4:
            setWeddingToCurrent(currentUser, organizerList, events);
            saveDataIntoFile<Organizer>(organizerList, "organizers.txt");
            break;
        case 5:
            cancelWedding(currentUser.currentWeddingId, events, vendorList, participants);
            break;
        case 6:
            generateInvitationCardMenu(currentUser, events, organizerList);
            break;
        case 7:
            for (auto& event : events) {
                if (event.eventId == currentUser.currentWeddingId) {
                    currentEvent = &event;
                    break;
                }
            }

            if (!currentEvent || currentEvent->status == "cancelled") {
                cout << "Wedding not found or cancelled!" << endl;
                pauseScreen();
                return;
            }

            participantMenu(participants, *currentEvent);
            break;
        case 8:
            // Payment
            paymentAndReportingMenu(currentUser, events, vendorList);
            break;
        case 9:
            MyProfileMenu(currentUser, vendorList, organizerList, adminList);
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

void adminMenu(CurrentUser& currentUser, vector<Vendor>& vendorList, vector<Organizer>& organizerList, vector<Admin>& adminList, vector<WeddingEvent>& events, vector<Participant>& participants, vector<PaymentTransaction>& transactions)
{
    int choice;
    WeddingEvent currentEvent;
    do
    {
        clearScreen();
        cout << "=== ADMIN DASHBOARD ===" << endl;
        cout << "Welcome, " << currentUser.userName << " (Administrator)" << endl;
        cout << "==========================================" << endl;
        cout << "USER MANAGEMENT:" << endl;
        cout << "1. View All Users" << endl;
        cout << "2. Register New User" << endl << endl;

        cout << "SERVICE MANAGEMENT:" << endl;
        cout << "3. View All Services" << endl;
        cout << "4. View Services by Vendor" << endl;
        cout << "5. View Services by Type" << endl << endl;

        cout << "EVENT MANAGEMENT:" << endl;
        cout << "6. View All Weddings" << endl;
        cout << "7. View Weddings by Organizer" << endl;
        cout << "8. View Weddings by Status" << endl << endl;

        cout << "MONITORING MANAGEMENT" << endl;
        cout << "9. View Participant by Wedding" << endl << endl;

        cout << "Reporting Management" << endl;
        cout << "10. Generate Reports" << endl << endl;

        cout << "OWN ACCOUNT MANAGEMENT:" << endl;
        cout << "11. My Profile" << endl;

        cout << endl;
        cout << "0. Logout" << endl;
        cout << "==========================================" << endl;
        cout << "Enter your choice: ";
        cin >> choice;
        cin.ignore();

        switch (choice)
        {
        case 1:
            displayAllUser(vendorList, organizerList, adminList);
            break;
        case 2:
            userRegister(vendorList, organizerList, adminList);
            break;
        case 3:
            displayAllServices(vendorList);
            break;
        case 4:
            displayServicesByVendor(vendorList);
            break;
        case 5:
            displayServicesByType(vendorList);
            break;
        case 6:
            viewAllWeddings(currentUser, events, vendorList);
            break;
        case 7:
            viewWeddingsByOrganizer(events, organizerList);
            break;
        case 8:
            viewWeddingByStatus(events);
            break;
        case 9:
            for (auto& event : events) {
                if (event.eventId == currentUser.currentWeddingId) {
                    currentEvent = event;
                    break;
                }
            }

            if (currentEvent.eventId.empty() || currentEvent.status == "cancelled") {
                cout << "Wedding not found or cancelled!" << endl;
                pauseScreen();
                return;
            }
            showAllParticipants(participants, currentEvent);
            break;
        case 10:
            generateAllPaidReports(events, vendorList, transactions);
            break;
        case 11:
            MyProfileMenu(currentUser, vendorList, organizerList, adminList);
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
void vendorMenu(CurrentUser& currentUser, vector<Vendor>& vendorList, vector<Organizer>& organizerList, vector<Admin>& adminList)
{
    int choice;
    do
    {
        clearScreen();
        cout << "=== VENDOR DASHBOARD ===" << endl;
        cout << "Welcome, " << currentUser.userName << " (Service Vendor)" << endl;

        // Display vendor info
        Vendor& vendor = vendorList[currentUser.userIndex];
        cout << "Company: " << vendor.companyName << endl;
        cout << "Services: " << vendor.serviceHasProvide.size() << endl;
        cout << "==========================================" << endl;
        cout << "SERVICE MANAGEMENT:" << endl;
        cout << "1. Add New Service" << endl;
        cout << "2. View My Services" << endl;
        cout << "3. Update My Services" << endl;
        cout << "4. Delete My Services" << endl << endl;

        cout << "MARKET RESEARCH:" << endl;
        cout << "5. View All Services " << endl;
        cout << "6. View Services by Type" << endl << endl;

        cout << "ACCOUNT MANAGEMENT:" << endl;
        cout << "7. My Profile" << endl;

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
            MyProfileMenu(currentUser, vendorList, organizerList, adminList);
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

void mainMenu(CurrentUser& currentUser, vector<Vendor>& vendorList, vector<Organizer>& organizerList, vector<Admin>& adminList, vector<WeddingEvent>& events, vector<Participant>& participants, vector<PaymentTransaction>& transactions)
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
                    adminMenu(currentUser, vendorList, organizerList, adminList, events, participants, transactions);
                    break;
                case ORGANIZER:
                    organizerMenu(currentUser, vendorList, organizerList, adminList, events, participants);
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

// Event
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

void createNewWedding(CurrentUser& currentUser, vector<WeddingEvent>& events, vector<Vendor>& vendorList, vector<Organizer>& organizerList) {
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

    saveDataIntoFile(events, "events.txt");

    cout << "Wedding created successfully! Event ID: " << newEvent.eventId << endl;
    pauseScreen();
}

void bookServicesForWedding(CurrentUser& currentUser, vector<WeddingEvent>& events, vector<Vendor>& vendorList, vector<Organizer>& organizerList) {
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

    vector<pair<int, int>> availableServices;
    int optionNum = 1;

    for (int i = 0; i < vendorList.size(); i++) {
        const auto& vendor = vendorList[i];
        for (int j = 0; j < vendor.serviceHasProvide.size(); j++) {
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

    auto& selected = availableServices[static_cast<int>(serviceChoice - 1)];
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

    currentEvent->bookedServices.push_back(service.serviceId);
    currentEvent->totalCost += service.price;

    saveDataIntoFile(events, "events.txt");

    cout << "Service booked successfully!" << endl;
    pauseScreen();
}

void viewAllWeddings(CurrentUser& currentUser, const vector<WeddingEvent>& events, const vector<Vendor>& vendorList) {
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

void viewAllMyWeddings(CurrentUser& currentUser, const vector<WeddingEvent>& events, const vector<Vendor>& vendorList) {
    clearScreen();
    cout << "=== ALL WEDDING EVENTS ===" << endl;

    if (events.empty()) {
        cout << "No wedding events found." << endl;
        pauseScreen();
        return;
    }

    for (const auto& event : events) {
        if (event.status == "cancelled") continue;
        if (event.organizerId == currentUser.userId) {

            cout << "Event ID: " << event.eventId << endl;
            cout << "Couple: " << event.groomName << " & " << event.brideName << endl;
            cout << "Date: " << event.weddingDate << endl;
            cout << "Theme: " << event.weddingTheme << endl;
            cout << "Budget: RM" << fixed << setprecision(2) << event.budget << endl;
            cout << "Total Cost: RM" << fixed << setprecision(2) << event.totalCost << endl;
            cout << "Status: " << event.status << endl;
            cout << string(50, '=') << endl;
        }
    }

    pauseScreen();
}

void viewWeddingsByOrganizer(vector<WeddingEvent>& events, vector<Organizer>& organizerList) {
    clearScreen();
    cout << "=== VIEW WEDDINGS BY ORGANIZER ===" << endl;

    if (organizerList.empty()) {
        cout << "No organizers found." << endl;
        pauseScreen();
        return;
    }

    cout << "Available Organizers:" << endl;
    for (int i = 0; i < organizerList.size(); i++) {
        cout << (i + 1) << ". " << organizerList[i].baseInfo.name << endl;
    }

    int choice;
    cout << "Select organizer (0 to cancel): ";
    cin >> choice;
    cin.ignore();

    if (choice < 1 || choice > organizerList.size()) {
        cout << "Invalid selection!" << endl;
        pauseScreen();
        return;
    }

    string& selectedOrganizerId = organizerList[choice - 1].organizerId;

    clearScreen();
    cout << "=== WEDDINGS BY " << organizerList[choice - 1].baseInfo.name << " ===" << endl;

    bool found = false;
    for (auto& event : events) {
        if (event.organizerId == selectedOrganizerId && event.status != "cancelled") {
            cout << "Event ID: " << event.eventId << endl;
            cout << "Couple: " << event.groomName << " & " << event.brideName << endl;
            cout << "Date: " << event.weddingDate << endl;
            cout << "Theme: " << event.weddingTheme << endl;
            cout << "Budget: RM" << fixed << setprecision(2) << event.budget << endl;
            cout << "Total Cost: RM" << fixed << setprecision(2) << event.totalCost << endl;
            cout << "Status: " << event.status << endl;
            cout << string(50, '=') << endl;
            found = true;
        }
    }

    if (!found) {
        cout << "No weddings found for this organizer." << endl;
    }

    pauseScreen();
}

void viewWeddingByStatus(vector<WeddingEvent>& events) {
    clearScreen();
    cout << "=== VIEW WEDDINGS BY STATUS ===" << endl;

    cout << "Available Statuses:" << endl;
    cout << "1. planning" << endl;
    cout << "2. completed" << endl;
    cout << "3. cancelled" << endl;
    cout << "4. Piad" << endl;

    int choice;
    cout << "Select status (0 to cancel): ";
    cin >> choice;
    cin.ignore();

    string status;
    switch (choice) {
    case 1: status = "planning"; break;
    case 2: status = "completed"; break;
    case 3: status = "cancelled"; break;
    case 4: status = "Paid"; break;
    case 0: return;
    default:
        cout << "Invalid selection!" << endl;
        pauseScreen();
        return;
    }

    clearScreen();
    cout << "=== WEDDINGS WITH STATUS: " << status << " ===" << endl;

    bool found = false;
    for (auto& event : events) {
        if (event.status == status) {
            cout << "Event ID: " << event.eventId << endl;
            cout << "Couple: " << event.groomName << " & " << event.brideName << endl;
            cout << "Date: " << event.weddingDate << endl;
            cout << "Theme: " << event.weddingTheme << endl;
            cout << "Budget: RM" << fixed << setprecision(2) << event.budget << endl;
            cout << "Total Cost: RM" << fixed << setprecision(2) << event.totalCost << endl;
            cout << string(50, '=') << endl;
            found = true;
        }
    }

    if (!found) {
        cout << "No weddings found with this status." << endl;
    }

    pauseScreen();
}

void showInvitationCards(CurrentUser& currentUser) {
    vector<InvitationCard> existingCards;
    vector<InvitationCard> userCards;

    clearScreen();
    loadInvitationCard(existingCards);

    // Filter cards by current user's ID and wedding ID
    for (auto& card : existingCards) {
        if (card.userId == currentUser.userId && card.weddingId == currentUser.currentWeddingId) {
            userCards.push_back(card);
        }
    }

    if (userCards.empty()) {
        cout << "\n+-------------------------------------+" << endl;
        cout << "|      NO INVITATION CARDS FOUND     |" << endl;
        cout << "+-------------------------------------+" << endl;
        cout << "You haven't created any invitation cards for this wedding yet." << endl;
        pauseScreen();
        return;
    }

    cout << "\n+-------------------------------------------------+" << endl;
    cout << "|           YOUR SAVED INVITATION CARDS          |" << endl;
    cout << "+-------------------------------------------------+" << endl;
    cout << "Found " << userCards.size() << " invitation card(s) for this wedding:" << endl;

    for (size_t i = 0; i < userCards.size(); i++) {
        cout << "\n+===================================================+" << endl;
        cout << "|           Invitation Card #" << (i + 1) << "                   |" << endl;
        cout << "+===================================================+" << endl;
        cout << userCards[i].cardContent << endl;
        cout << "+===================================================+" << endl;

        // Ask if user wants to continue viewing more cards (if multiple exist)
        if (i < userCards.size() - 1) {
            cout << "\nPress Enter to view next card or 'q' to quit viewing: ";
            char continueChoice;
            cin.ignore(); // Clear input buffer
            continueChoice = cin.get();
            if (tolower(continueChoice) == 'q') {
                break;
            }
        }
    }
    pauseScreen();
}

void deleteInvitationCard(CurrentUser& currentUser) {
    vector<InvitationCard> existingCards;
    vector<InvitationCard> userCards;

    clearScreen();
    loadInvitationCard(existingCards);

    // Filter cards by current user's ID and wedding ID
    for (auto& card : existingCards) {
        if (card.userId == currentUser.userId && card.weddingId == currentUser.currentWeddingId) {
            userCards.push_back(card);
        }
    }

    if (userCards.empty()) {
        cout << "\n+-------------------------------------+" << endl;
        cout << "|         NO CARDS TO DELETE          |" << endl;
        cout << "+-------------------------------------+" << endl;
        pauseScreen();
        return;
    }

    cout << "\n+-------------------------------------------------+" << endl;
    cout << "|            DELETE INVITATION CARD              |" << endl;
    cout << "+-------------------------------------------------+" << endl;
    cout << "Select card to delete:" << endl;
    cout << "=================================================" << endl;

    for (size_t i = 0; i < userCards.size(); i++) {
        cout << "\n" << (i + 1) << ". Card #" << (i + 1) << endl;
        cout << "   +- Preview ------------------------------+" << endl;
        // Show a preview of the card (first few lines)
        string preview = userCards[i].cardContent.substr(0, 100);
        cout << "   | " << preview << "..." << endl;
        cout << "   +----------------------------------------+" << endl;
    }
    cout << "\n0. <- Cancel" << endl;
    cout << "=================================================" << endl;
    cout << "Enter choice: ";

    int deleteChoice;
    cin >> deleteChoice;

    if (deleteChoice == 0) {
        cout << "\nDeletion cancelled." << endl;
        pauseScreen();
        return;
    }

    if (deleteChoice < 1 || deleteChoice > static_cast<int>(userCards.size())) {
        cout << "\nInvalid choice!" << endl;
        pauseScreen();
        return;
    }

    // Confirm deletion
    cout << "\nAre you sure you want to delete this card? (y/n): ";
    char confirm;
    cin >> confirm;

    if (toupper(confirm) == 'Y') {
        InvitationCard cardToDelete = userCards[deleteChoice - 1];

        // Remove from main vector
        auto it = remove_if(existingCards.begin(), existingCards.end(),
            [&cardToDelete, &currentUser](const InvitationCard& card) {
                return card.userId == cardToDelete.userId &&
                    card.weddingId == cardToDelete.weddingId &&
                    card.cardContent == cardToDelete.cardContent;
            });

        existingCards.erase(it, existingCards.end());

        // Save updated data
        saveDataIntoFile<InvitationCard>(existingCards, "invitation_cards.txt");

        cout << "\nInvitation card deleted successfully!" << endl;
    }
    else {
        cout << "\nDeletion cancelled." << endl;
    }
    pauseScreen();
}

void generateInvitationCardMenu(CurrentUser& currentUser, vector<WeddingEvent>& events, vector<Organizer>& organizerList) {
    int choice;
    string contactNumber;
    if (currentUser.type != ORGANIZER) {
        cout << "Only organizers can generate invitation cards!" << endl;
        pauseScreen();
        return;
    }

    string currentWeddingId;
    for (auto& organizer : organizerList) {
        if (organizer.organizerId == currentUser.userId) {
            currentWeddingId = organizer.currentWeddingId;
            contactNumber = organizer.baseInfo.phoneNum;
            break;
        }
    }

    if (currentWeddingId.empty()) {
        cout << "No active wedding! Please create a wedding first." << endl;
        cin.ignore();
        pauseScreen();
        return;
    }

    WeddingEvent wedding;
    for (auto& event : events) {
        if (event.eventId == currentWeddingId && event.status == "planning") {
            wedding = event;
            break;
        }
    }

    if (wedding.eventId.empty()) {
        cout << "Wedding not found or not in planning status!" << endl;
        pauseScreen();
        return;
    }

    do {
        clearScreen();
        cout << "\n=== GENERATE INVITATION CARD ===" << endl;
        cout << "Bride: " << wedding.brideName << " | Groom: " << wedding.groomName << endl;
        cout << "=====================================" << endl;
        cout << "1. View Template Samples" << endl;
        cout << "2. Generate Your Own Invitation Card" << endl;
        cout << "3. View Saved Invitation Cards" << endl;
        cout << "4. Delete Saved Invitation Cards" << endl;
        cout << "0. Back to Main Menu" << endl;
        cout << "Enter your choice: ";
        cin >> choice;

        switch (choice) {
        case 1:
            invitationTemplate(events, wedding, currentUser);
            break;

        case 2:
            generateCustomInvitation(events, wedding, contactNumber, currentUser);
            break;
        case 3:
            showInvitationCards(currentUser);
            break;
        case 4:
            deleteInvitationCard(currentUser);
            break;
        case 0:
            cout << "Returning to main menu..." << endl;
            break;

        default:
            cout << "Invalid choice! Please try again." << endl;
            pauseScreen();
            break;
        }
    } while (choice != 0);
}

void generateCustomInvitation(vector<WeddingEvent>& events, WeddingEvent& wedding, string contactNumber, CurrentUser& currentUser) {
    int templateChoice;
    char saveChoice;
    char generateAnother;
    InvitationCard card;
    vector<InvitationCard> existingCards;

    do {
        clearScreen();
        loadInvitationCard(existingCards);

        cout << "\n=== GENERATE YOUR INVITATION CARD ===" << endl;
        cout << "Current Wedding Details:" << endl;
        cout << "Bride: " << wedding.brideName << endl;
        cout << "Groom: " << wedding.groomName << endl;
        cout << "Date: " << wedding.weddingDate << endl;
        cout << "Venue: " << wedding.weddingVenue << endl;
        cout << "Contact: " << contactNumber << endl << endl;
        cout << "=================================" << endl;
        cout << "Select your preferred template:" << endl;
        cout << "1. Classic Elegant" << endl;
        cout << "2. Modern Minimalist" << endl;
        cout << "3. Romantic with Hearts" << endl;
        cout << "4. Formal Traditional" << endl;
        cout << "5. Fun & Casual" << endl;
        cout << "0. Back to Menu" << endl;
        cout << "Enter your choice: ";
        cin >> templateChoice;

        if (templateChoice == 0) {
            cout << "Generation cancelled." << endl;
            return;
        }

        if (templateChoice < 1 || templateChoice > 5) {
            cout << "Invalid template choice!" << endl;
            pauseScreen();
            continue;  // Fixed: Single semicolon
        }

        cout << "\n=== YOUR WEDDING INVITATION ===" << endl;

        switch (templateChoice) {
        case 1:
            cout << "\n--- Your Classic Elegant Invitation ---" << endl;  // Added newline and title
            card.cardContent = template1(wedding, contactNumber);
            cout << card.cardContent << endl;
            break;
        case 2:
            cout << "\n--- Your Modern Minimalist Invitation ---" << endl;
            card.cardContent = template2(wedding, contactNumber);
            cout << card.cardContent << endl;
            break;
        case 3:
            cout << "\n--- Your Romantic Hearts Invitation ---" << endl;
            card.cardContent = template3(wedding, contactNumber);
            cout << card.cardContent << endl;
            break;
        case 4:
            cout << "\n--- Your Formal Traditional Invitation ---" << endl;
            card.cardContent = template4(wedding, contactNumber);
            cout << card.cardContent << endl;
            break;
        case 5:
            cout << "\n--- Your Fun & Casual Invitation ---" << endl;
            card.cardContent = template5(wedding, contactNumber);
            cout << card.cardContent << endl;
            break;
        }

        cout << "\nDo you want to save this invitation card? (y/n): ";
        cin >> saveChoice;

        if (toupper(saveChoice) == 'Y') {
            card.userId = currentUser.userId;
            card.weddingId = currentUser.currentWeddingId;
            existingCards.push_back(card);
            saveDataIntoFile<InvitationCard>(existingCards, "invitation_cards.txt");
            cout << "Invitation card saved successfully!" << endl;
        }
        else {
            cout << "Invitation card not saved." << endl;
        }

        cout << "\nWould you like to generate another template? (y/n): ";
        cin >> generateAnother;

    } while (toupper(generateAnother) == 'Y');
}

void invitationTemplate(vector<WeddingEvent>& events, WeddingEvent& wedding, CurrentUser& currentUser) {
    int templateChoice;

    do {
        cout << "\n=== INVITATION TEMPLATES ===" << endl;
        cout << "Choose a template to preview:" << endl;
        cout << "1. Classic Elegant" << endl;
        cout << "2. Modern Minimalist" << endl;
        cout << "3. Romantic with Hearts" << endl;
        cout << "4. Formal Traditional" << endl;
        cout << "5. Fun & Casual" << endl;
        cout << "0. Back to Previous Menu" << endl;
        cout << "Enter your choice: ";
        cin >> templateChoice;

        // Create sample data for preview
        WeddingEvent sampleWedding;
        sampleWedding.brideName = "Sarah Johnson";
        sampleWedding.groomName = "Michael Davis";
        sampleWedding.weddingDate = "June 15, 2024";

        sampleWedding.weddingVenue = "Rose Garden Chapel";
        string contactNumber = "(555) 123-4567";

        cout << "\n=== TEMPLATE PREVIEW ===";

        switch (templateChoice) {
        case 1:
            cout << "\n--- Classic Elegant Template ---";
            cout << template1(sampleWedding, contactNumber);
            pauseScreen();
            break;
        case 2:
            cout << "\n--- Modern Minimalist Template ---";
            cout << template2(sampleWedding, contactNumber);
            pauseScreen();
            break;
        case 3:
            cout << "\n--- Romantic with Hearts Template ---";
            cout << template3(sampleWedding, contactNumber);
            pauseScreen();
            break;
        case 4:
            cout << "\n--- Formal Traditional Template ---";
            cout << template4(sampleWedding, contactNumber);
            pauseScreen();
            break;
        case 5:
            cout << "\n--- Fun & Casual Template ---";
            cout << template5(sampleWedding, contactNumber);
            pauseScreen();
            break;
        case 0:
            pauseScreen();
            return;
        default:
            cout << "Invalid choice! Please try again." << endl;
            pauseScreen();
            continue;
        }

        if (templateChoice >= 1 && templateChoice <= 5) {
            cout << "\nThis is just a sample preview." << endl;
            cout << "Your actual invitation will use your wedding details." << endl;
            pauseScreen();
        }

    } while (templateChoice != 0);
}

string template1(WeddingEvent& wedding, string& contactNumber) {
    stringstream ss;
    ss << "\n";
    ss << "********************************************************\n";
    ss << "*                                                      *\n";
    ss << "*              WEDDING INVITATION                      *\n";
    ss << "*                                                      *\n";
    ss << "*        " << wedding.brideName << " & " << wedding.groomName << "\n";
    ss << "*                                                      *\n";
    ss << "*    Together with their families request the          *\n";
    ss << "*    pleasure of your company at their wedding         *\n";
    ss << "*                                                      *\n";
    ss << "*    Date: " << wedding.weddingDate << "\n";
    ss << "*    Venue: " << wedding.weddingVenue << "\n";
    ss << "*                                                      *\n";
    ss << "*    RSVP by calling: " << contactNumber << "\n";
    ss << "*                                                      *\n";
    ss << "********************************************************\n";
    return ss.str();
}

string template2(WeddingEvent& wedding, string& contactNumber) {
    stringstream ss;
    ss << "\n";
    ss << "========================================\n";
    ss << "           SAVE THE DATE\n";
    ss << "========================================\n";
    ss << "\n";
    ss << wedding.brideName << " + " << wedding.groomName << "\n";
    ss << "\n";
    ss << "are getting married!\n";
    ss << "\n";
    ss << "WHEN: " << wedding.weddingDate << "\n";
    ss << "WHERE: " << wedding.weddingVenue << "\n";
    ss << "\n";
    ss << "Join us for a celebration of love\n";
    ss << "\n";
    ss << "Contact: " << contactNumber << "\n";
    ss << "========================================\n";
    return ss.str();
}

string template3(WeddingEvent& wedding, string& contactNumber) {
    stringstream ss;
    ss << "\n";
    ss << "<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3\n";
    ss << "<3                                                <3\n";
    ss << "<3           Two Hearts Becoming One              <3\n";
    ss << "<3                                                <3\n";
    ss << "<3               " << wedding.brideName << "\n";
    ss << "<3                    &                           <3\n";
    ss << "<3               " << wedding.groomName << "\n";
    ss << "<3                                                <3\n";
    ss << "<3        invite you to share in their           <3\n";
    ss << "<3             wedding celebration               <3\n";
    ss << "<3                                                <3\n";
    ss << "<3  On: " << wedding.weddingDate << "\n";
    ss << "<3  Venue: " << wedding.weddingVenue << "\n";
    ss << "<3                                                <3\n";
    ss << "<3  Please RSVP: " << contactNumber << "\n";
    ss << "<3                                                <3\n";
    ss << "<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3\n";
    return ss.str();
}

string template4(WeddingEvent& wedding, string& contactNumber) {
    stringstream ss;
    ss << "\n";
    ss << "+======================================================+\n";
    ss << "|                                                      |\n";
    ss << "|              WEDDING ANNOUNCEMENT                    |\n";
    ss << "|                                                      |\n";
    ss << "|        Mr. " << wedding.groomName << "\n";
    ss << "|                      and                             |\n";
    ss << "|        Ms. " << wedding.brideName << "\n";
    ss << "|                                                      |\n";
    ss << "|    request the honor of your presence at their      |\n";
    ss << "|                  wedding ceremony                   |\n";
    ss << "|                                                      |\n";
    ss << "|    Date: " << wedding.weddingDate << "\n";
    ss << "|    Location: " << wedding.weddingVenue << "\n";
    ss << "|                                                      |\n";
    ss << "|    Kindly respond by contacting: " << contactNumber << "\n";
    ss << "|                                                      |\n";
    ss << "+======================================================+\n";
    return ss.str();
}

string template5(WeddingEvent& wedding, string& contactNumber) {
    stringstream ss;
    ss << "\n";
    ss << "~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~\n";
    ss << "                                                   \n";
    ss << "        ** WEDDING PARTY INVITATION **           \n";
    ss << "                                                   \n";
    ss << "    " << wedding.brideName << " & " << wedding.groomName << " are tying the knot!\n";
    ss << "                                                   \n";
    ss << "    Come party with us as we say 'I DO!'          \n";
    ss << "                                                   \n";
    ss << "    DATE: " << wedding.weddingDate << "\n";
    ss << "    TIME: Coming Soon!\n";
    ss << "    PLACE: " << wedding.weddingVenue << "\n";
    ss << "                                                   \n";
    ss << "    There will be good food, great music,         \n";
    ss << "    and lots of dancing!                          \n";
    ss << "                                                   \n";
    ss << "    RSVP: " << contactNumber << "\n";
    ss << "                                                   \n";
    ss << "    Can't wait to celebrate with you! <3          \n";
    ss << "                                                   \n";
    ss << "~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~\n";
    return ss.str();
}

string returnTime(Participant& p)
{
    if (p.attendance.checkInTime == 0) {
        return "N/A";
    }

    char buf[20];
    tm timeinfo{};

    localtime_s(&timeinfo, &p.attendance.checkInTime);

    strftime(buf, sizeof(buf), "%Y-%m-%d %H:%M:%S", &timeinfo);
    return string(buf);
}

bool checkIsExist(vector<Participant> participant, string name, string eventId)
{
    for (auto& p : participant)
    {
        if (p.eventId == eventId) {
            if (p.name == name)
            {
                return true; // true represent is exist
            }
        }

    }
    return false; // false represent not exist
}
//Monitoring
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
void updateFile(ofstream& outFile, vector<Participant>& participants)
{
    for (auto& p : participants)
    {
        outFile << p.eventId << "|"
            << p.name << "|"
            << (p.isVip ? 1 : 0) << "|"
            << (p.attendance.isPresent ? 1 : 0) << "|"
            << p.attendance.checkInTime
            << "\n";
    }
}

time_t parseDate(const string& dateStr) {
    tm t = {};
    istringstream ss(dateStr);
    ss >> get_time(&t, "%Y-%m-%d"); // parse YYYY-MM-DD
    if (ss.fail()) {
        throw runtime_error("Invalid date format: " + dateStr);
    }
    // Convert to time_t
    return mktime(&t);
}

// Strip time (keep only date part)
time_t toDateOnly(time_t t) {
    tm temp;
    if (localtime_s(&temp, &t) != 0) {
        // Handle error - return original time or some error value
        return t; // or return -1 or throw exception depending on your error handling strategy
    }
    temp.tm_hour = 0;
    temp.tm_min = 0;
    temp.tm_sec = 0;
    return mktime(&temp);
}

// [Monitoring][R][E]
void printParticipant(Participant p, WeddingEvent currentEvent)
{
    time_t now = time(nullptr); // current time
    time_t today = toDateOnly(now);

    time_t weddingDay = parseDate(currentEvent.weddingDate); // convert from string
    weddingDay = toDateOnly(weddingDay);

    // Name
    cout << "| " << setw(30) << left << p.name;

    // VIP
    cout << "| " << setw(20) << left << (p.isVip ? "Yes" : "No");

    // Present
    if (p.attendance.isPresent) {
        cout << "| " << setw(30) << left << (p.attendance.isPresent ? "Yes" : "No");
    }
    else {
        if (today < weddingDay)
        {
            cout << "| " << setw(30) << left << "Haven't reach the date";
        }
        else
        {
            cout << "| " << setw(30) << left << (p.attendance.isPresent ? "Yes" : "No");
        }
    }

    if (p.attendance.isPresent) {
        cout << "| " << setw(26) << left << returnTime(p);
    }
    else {
        // Check-in Time
        if (today < weddingDay)
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
    }

    cout << "|\n";
}

void deleteAllParticipants(vector<Participant>& participants, string eventId) {
    bool anyRemoved = false;
    for (int i = 0; i < participants.size(); i++)
    {
        if (participants[i].eventId == eventId)
        {
            participants.erase(participants.begin() + i);
            anyRemoved = true;
        }
    }
    if (anyRemoved) {
        ofstream outFile("participants.txt");
        if (!outFile)
        {
            cerr << "Cant open file. Location: addParticipantsToList.\n";
            return;
        }
        updateFile(outFile, participants);
        outFile.close();
    }
}

void deleteParticipants(vector<Participant>& participants, WeddingEvent currentEvent)
{
    // enter the name, if searched then show the information, then ask to edit what
    string name, input;
    int selection = 0;
    bool found = false;
    Participant participant;

    if (participants.empty())
    {
        cout << "There are no participants in the file.\n\n";
        pauseScreen();
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

        for (auto& p : participants)
        {
            if (p.eventId == currentEvent.eventId && name == p.name)
            {
                printParticipant(p, currentEvent);
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
            pauseScreen();
            return;
        }
        else
        {
            cout << string(115, '-') << "\n";

            ofstream outFile("participants.txt");

            if (!outFile)
            {
                cerr << "Cant open file. Location: deleteParticipants.\n";
                cout << "Sorry you cant delete now, There some error here.\n\n";
                pauseScreen();
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
            pauseScreen();
        }
        break;
    }
}
//[Monitoring][E]
void editParticipantsName(vector<Participant>& participants, Participant participant, string weddingId)
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

        if (!checkIsExist(participants, newname, weddingId))
        {
            // Find and update the participant in the vector
            for (auto& p : participants)
            {
                if (p.name == participant.name && p.eventId == weddingId)
                {
                    p.name = newname;

                    // Open file only when we need to update
                    ofstream outFile("participants.txt");
                    if (!outFile)
                    {
                        cerr << "Can't open file for writing.\n";
                        return;
                    }

                    updateFile(outFile, participants);
                    outFile.close();

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
void editParticipantsVip(vector<Participant>& participants, Participant participant, string weddingId)
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
            for (auto& p : participants)
            {
                if (p.name == participant.name && p.eventId == weddingId)
                {
                    p.isVip = !p.isVip;

                    // Open file only when we need to update
                    ofstream outFile("participants.txt");
                    if (!outFile)
                    {
                        cerr << "Can't open file for writing.\n";
                        return;
                    }

                    updateFile(outFile, participants);
                    outFile.close();

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

bool askForTime(Participant participant, tm& t, string& weddingDateStr)
{
    while (true)
    {
        std::string input;
        cout << "Current Edit Participant: " << participant.name
            << "\nCheck-in Time: " << returnTime(participant)
            << "\nPlease enter check-in time. (format: HH:MM:SS) (0: exit) \n> ";

        getline(cin, input);

        if (cin.fail())
        {
            cin.clear();
            cout << "Please enter valid input.\n\n";
            continue;
        }

        if (input == "0")
        {
            cout << "Participant check-in time retain.\n\n";
            return false;
        }

        // Parse wedding date string (YYYY-MM-DD)
        tm weddingDate = {};
        istringstream ws(weddingDateStr);
        ws >> get_time(&weddingDate, "%Y-%m-%d");
        if (ws.fail())
        {
            cerr << "Wedding date string invalid! (" << weddingDateStr << ")\n";
            return false;
        }

        // Parse input time (HH:MM:SS)
        tm timeOnly = {};
        istringstream ss(input);
        ss >> get_time(&timeOnly, "%H:%M:%S");
        if (ss.fail())
        {
            cerr << "Invalid time format! Enter again.\n\n";
            continue;
        }

        // Combine date + time
        t = weddingDate;
        t.tm_hour = timeOnly.tm_hour;
        t.tm_min = timeOnly.tm_min;
        t.tm_sec = timeOnly.tm_sec;

        // normalize
        mktime(&t);

        return true;
    }
}

//[Monitoring][E]
void editParticipantsPresent(vector<Participant>& participants, Participant participant, WeddingEvent currentEvent)
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
                for (auto& p : participants)
                {
                    if (p.name == participant.name && p.eventId == currentEvent.eventId)
                    {
                        p.attendance.checkInTime = -1;
                        p.attendance.isPresent = !p.attendance.isPresent;

                        // Open file only when we need to update
                        ofstream outFile("participants.txt");
                        if (!outFile)
                        {
                            cerr << "Can't open file for writing.\n";
                            return;
                        }

                        updateFile(outFile, participants);
                        outFile.close();

                        cout << "Participant check-in time clear successfully.\n";
                        cout << "Participant present status changes successfully from '" << (!p.attendance.isPresent ? "Yes" : "No") << "' to '" << (p.attendance.isPresent ? "Yes" : "No") << "'.\n\n";
                        break;
                    }
                }
            }
            else
            { // no -> yes, need to insert the time
                if (askForTime(participant, t, currentEvent.weddingDate))
                {
                    // Find and update the participant in the vector
                    for (auto& p : participants)
                    {
                        if (p.name == participant.name && p.eventId == currentEvent.eventId)
                        {
                            p.attendance.checkInTime = mktime(&t);
                            p.attendance.isPresent = !p.attendance.isPresent;

                            // Open file only when we need to update
                            ofstream outFile("participants.txt");
                            if (!outFile)
                            {
                                cerr << "Can't open file for writing.\n";
                                return;
                            }

                            updateFile(outFile, participants);
                            outFile.close();

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
void editParticipantsCheckInTime(vector<Participant>& participants, Participant participant, WeddingEvent currentEvent)
{
    tm t = {};

    if (askForTime(participant, t, currentEvent.weddingDate))
    {
        // Find and update the participant in the vector
        for (auto& p : participants)
        {
            if (p.name == participant.name && p.eventId == currentEvent.eventId)
            {
                p.attendance.checkInTime = mktime(&t);
                p.attendance.isPresent = true;

                // Open file only when we need to update
                ofstream outFile("participants.txt");
                if (!outFile)
                {
                    cerr << "Can't open file for writing.\n";
                    return;
                }

                updateFile(outFile, participants);
                outFile.close();

                cout << "Participant check-in time changes successfully from '" << returnTime(participant) << "' to '" << returnTime(p) << "'.\n";
                cout << "Participant present status set as \"Yes\" successfully.\n\n";
                break;
            }
        }
    }
}

//[Monitoring][E]
void updateParticipants(vector<Participant>& participants, WeddingEvent currentEvent)
{
    // enter the name, if searched then show the information, then ask to edit what
    string name, input;
    int selection = 0;
    bool found = false;
    Participant participant;

    if (participants.empty())
    {
        cout << "There are no participants in the file.\n\n";
        pauseScreen();
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

        for (auto& p : participants)
        {
            if (p.eventId == currentEvent.eventId && name == p.name)
            {
                printParticipant(p, currentEvent);
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
            pauseScreen();
            return;
        }
        else
        {
            cout << string(115, '-') << "\n";

            cout << "What the information you going to edit.\n1. Name\n2. Vip (yes or not)\n3. Present (yes or not)\n4. Check-In Time\n5. Exit\n\n";
            selection = returnInt();

            switch (selection)
            {
            case 1:
                cout << "Edit Name selected.\n\n";
                editParticipantsName(participants, participant, currentEvent.eventId);
                break;
            case 2:
                cout << "Edit Vip Status selected.\n\n";
                editParticipantsVip(participants, participant, currentEvent.eventId);
                break;
            case 3:
                cout << "Edit Present Status selected.\n\n";
                editParticipantsPresent(participants, participant, currentEvent);
                break;
            case 4:
                cout << "Edit Check-in Time selected.\n\n";
                editParticipantsCheckInTime(participants, participant, currentEvent);
                break;
            case 5:
                cout << "Exit Monitoring Module selected.\n\n";
                pauseScreen();
                return;
            default:
                cout << "Invalid selection. Try again.\n\n";
                pauseScreen();
                break;
            }
            pauseScreen();
        }
        break;
    }
}

// [Monitoring][C][Mark][D] check user enter participants list
bool checkParticipantsList(stringstream& buffer)
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
void readBuffer(stringstream& buffer)
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
void markAttendance(vector<Participant>& participants, string weddingId)
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
        pauseScreen();
        return;
    }

    // get the buffer
    if (!checkParticipantsList(buffer))
    {
        cout << "Please insert \" done \" on the end of list.\n Back to previous page...\n";
        pauseScreen();
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
                    for (auto& p : participants)
                    {
                        if (p.eventId == weddingId && name == p.name)
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
                pauseScreen();
                updateFile(outFile, participants);
            }
            break;
        }
        else if (selection == 'N')
        {
            cout << "Attendance of these name will not be marked.\n";
            pauseScreen();
            break;
        }
        else
        {
            cout << "Please choose the option given.\n";
            pauseScreen();
        }
    }
    outFile.close();
}

// [Search][R]
void searchParticipantOneByOne(vector<Participant> participants, WeddingEvent currentEvent)
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

        for (auto& p : participants)
        {
            if (p.eventId != currentEvent.eventId)
                continue;

            string compare = p.name;
            transform(compare.begin(), compare.end(), compare.begin(), ::tolower);

            if (compare.find(name) != string::npos)
            {
                printParticipant(p, currentEvent);
                found++;
            }
        }

        if (found < 1)
        {

            cout << "There are no participants names " << name << " in the file.\n\n";
            pauseScreen();
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
void showAllParticipants(vector<Participant> participants, WeddingEvent currentEvent)
{

    string input;
    bool found = false;

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

    while (!participants.empty())
    {
        Participant p = participants.back();
        participants.pop_back();

        if (p.eventId == currentEvent.eventId)
        {
            printParticipant(p, currentEvent);
            found = true;
        }
    }

    if (!found) {
        cout << "| " << setw(111) << left << "No participants found for this wedding." << "|\n";
    }

    cout << string(115, '-') << "\n";
    cout << "Press Enter to continue .... )\n";
    do
    {
        getline(cin, input);
    } while (!input.empty());
}

// [Monitoring][R] search participants menu
void searchParticipantsMenu(vector<Participant>& participants, WeddingEvent currentEvent)
{
    int selection;

    while (true)
    {
        clearScreen();
        cout << "===Search Participants Menu===" << endl;
        cout << "1. Show all the participants\n2. Search Participant by name\n0. Exit\n\n";
        selection = returnInt();

        if (selection == -1)
        {
            continue;
        }

        switch (selection)
        {
        case 1:
            showAllParticipants(participants, currentEvent);
            break;
        case 2:
            searchParticipantOneByOne(participants, currentEvent);
            break;
        case 0:
            cout << "Exit successful\nBack To Monitoring Menu\n\n";
            pauseScreen();
            return;
        default:
            cout << "Please choose only the option given.\n";
            pauseScreen();
            break;
        }
    }
}

// [Monitoring][C] add participants to list
void addParticipantsToList(vector<Participant>& participants, stringstream& buffer, string weddingId)
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
                    if (!checkIsExist(participants, name, weddingId))
                    {
                        Participant p;
                        p.eventId = weddingId;
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
                ofstream outFile("participants.txt");
                if (!outFile)
                {
                    cerr << "Cant open file. Location: addParticipantsToList.\n";
                    return;
                }
                updateFile(outFile, participants);
                cout << "All the name was successfully added.\n\n";
                outFile.close();
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

void addParticipantsByList(vector<Participant>& participants, string weddingId)
{
    string name;
    stringstream buffer;
    char selection;

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
            bool needFileUpdate = false;

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
                    if (!checkIsExist(participants, name, weddingId))
                    {
                        Participant p;
                        p.eventId = weddingId;
                        p.name = name;
                        p.isVip = false;
                        p.attendance.isPresent = false;
                        p.attendance.checkInTime = -1;

                        participants.push_back(p);
                        cout << p.name << " was successfully added.\n\n";
                        needFileUpdate = true;
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
                    addParticipantsToList(participants, buffer, weddingId);
                    return; // Exit after handling "Add All"
                }
                else if (selection == 'R')
                {
                    cout << "The else will not add to the list\n";
                    buffer.str("");
                    buffer.clear();
                    break;
                }
            }

            // Only update file if any participants were added
            if (needFileUpdate)
            {
                ofstream outFile("participants.txt");
                if (!outFile)
                {
                    cerr << "Cant open file. Location: addParticipantsByList.\n";
                    return;
                }
                updateFile(outFile, participants);
                outFile.close();
            }
            break;
        }
        else if (selection == 'N')
        {
            addParticipantsToList(participants, buffer, weddingId);
            break;
        }
        else
        {
            cout << "Please enter the option given.\n";
        }
    }
}

void addParticipantOneByOne(vector<Participant>& participants, string weddingId)
{
    string name;
    char selection;
    int isVip = 0;
    Participant p;

    cout << "Enter the participants name you going to add.\n> ";
    getline(cin, name);

    if (cin.fail())
    {
        cout << "Please enter number or character only.\n\n";
        return;
    }

    if (checkIsExist(participants, name, weddingId))
    {
        cout << "Error: A participant with the name \"" << name << "\" already exists.\n\n";
        return;
    }

    // Ask if VIP
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

    // Confirm addition
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

    // Only open file when we need to write
    if (selection == 'Y')
    {
        ofstream outFile("participants.txt");
        if (!outFile)
        {
            cerr << "Cant open file. Location: addParticipantOneByOne.\n";
            return;
        }

        p.eventId = weddingId;
        p.name = name;
        p.isVip = isVip;
        p.attendance.isPresent = false;
        p.attendance.checkInTime = -1;

        participants.push_back(p);
        updateFile(outFile, participants);
        cout << name << " was successfully added.\n\n";
        outFile.close();
    }
    else if (selection == 'N')
    {
        cout << name << " will not add to the list.\n";
        // No need to open file or update anything
    }
    else
    {
        cout << "There was some error occur\n\n";
    }
}

void addParticipantsMenu(vector<Participant>& participants, WeddingEvent currentEvent)
{
    int selection;

    while (true)
    {
        clearScreen();
        cout << "===Add Participants Menu===" << endl;
        cout << "1. Add Participants by list \n2. Add Participant one by one\n0. Exit\n\n";
        selection = returnInt();

        if (selection == -1)
        {
            continue;
        }

        switch (selection)
        {
        case 1:
            addParticipantsByList(participants, currentEvent.eventId);
            break;
        case 2:
            addParticipantOneByOne(participants, currentEvent.eventId);
            break;
        case 0:
            cout << "Exit successful\n\n";
            return;
        default:
            cout << "Please choose only the option given.\n";
            break;
        }
    }
}

bool addDefaultParticipants(vector<Participant>& participants)
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
        string eventId, name, vipStr, presentStr, timeStr;

        getline(ss, eventId, '|');
        getline(ss, name, '|');
        getline(ss, vipStr, '|');
        getline(ss, presentStr, '|');
        getline(ss, timeStr, '|');

        Participant p;
        p.eventId = eventId;
        p.name = name;
        p.isVip = (stoi(vipStr) == 1);
        p.attendance.isPresent = (stoi(presentStr) == 1);
        p.attendance.checkInTime = stoll(timeStr);

        participants.push_back(p);
    }
    inFile.close();
    return true;
}

// [Monitoring][R] display attendance statistics
void displayPresentAmount(vector<Participant> participants, WeddingEvent currentEvent)
{
    string input;
    int totalParticipants = 0;
    int presentParticipants = 0;
    double attendancePercentage = 0.0;

    if (participants.empty())
    {
        cout << "There are no participants in the file.\n\n";
        return;
    }

    // Count participants for current wedding
    for (const auto& p : participants)
    {
        if (p.eventId == currentEvent.eventId)
        {
            totalParticipants++;
            if (p.attendance.isPresent)
            {
                presentParticipants++;
            }
        }
    }

    if (totalParticipants == 0)
    {
        cout << "No participants found for this wedding.\n\n";
        return;
    }

    // Calculate attendance percentage
    attendancePercentage = (static_cast<double>(presentParticipants) / totalParticipants) * 100.0;

    // Display statistics
    cout << string(60, '=') << "\n";
    cout << "         ATTENDANCE STATISTICS REPORT\n";
    cout << string(60, '=') << "\n";
    cout << "Wedding Event: " << currentEvent.eventId << "\n";
    cout << "Wedding Date:  " << currentEvent.weddingDate << "\n";
    cout << string(60, '-') << "\n";
    cout << "Present Participants:  " << presentParticipants << "\n";
    cout << "Total Participants:    " << totalParticipants << "\n";
    cout << "Attendance Ratio:      " << presentParticipants << "/" << totalParticipants << "\n";
    cout << "Attendance Percentage: " << fixed << setprecision(1) << attendancePercentage << "%\n";
    cout << string(60, '=') << "\n";

    pauseScreen();
}

void participantMenu(vector<Participant>& participants, WeddingEvent currentEvent)
{
    int selection = 0;
    time_t now = time(nullptr); // current time
    time_t today = toDateOnly(now);

    time_t weddingDay = parseDate(currentEvent.weddingDate); // convert from string
    weddingDay = toDateOnly(weddingDay);        // keep only date

    while (true)
    {
        clearScreen();
        cout << "=== PARTICIPANT MONITORING MENU ===\n";
        cout << "1. Add Participants\n2. Search Participants\n3. Mark Attendance\n4. Edit Participants Information\n5. Delete Participants\n0. Exit Monitoring Module\n\n";
        selection = returnInt();

        if (selection == -1)
        {
            continue;
        }

        switch (selection)
        {
        case 1:
            if (today > weddingDay) {
                cout << "Wedding already ended. You are not allowed to add participants.\n";
            }
            else {
                cout << "Add Participants selected.\n\n";
                addParticipantsMenu(participants, currentEvent);
            }
            break;
        case 2:
            cout << "Search Participants selected.\n\n";
            searchParticipantsMenu(participants, currentEvent);
            break;
        case 3:
            if (today > weddingDay) {
                cout << "Wedding already ended. You are not allowed to mark attendance.\n";
                pauseScreen();
            }
            else if (today < weddingDay) {
                cout << "Wedding has not started yet. You are not allowed to mark attendance.\n";
                pauseScreen();
            }
            else {
                cout << "Mark Attendance selected.\n\n";
                markAttendance(participants, currentEvent.eventId);
            }
            break;
        case 4:
            cout << "Update Participants details selected.\n\n";
            updateParticipants(participants, currentEvent);
            break;
        case 5:
            cout << "Delete Monitoring Module selected.\n\n";
            deleteParticipants(participants, currentEvent);
            break;
        case 6:
            if (today < weddingDay) {
                cout << "Wedding has not started yet. You are not allowed to view attendance.\n";
            }
            else {
                cout << "View Attendance Statistics selected.\n\n";
                displayPresentAmount(participants, currentEvent);
            }
            break;
        case 0:
            cout << "Exit Monitoring Module selected.\n\n";
            pauseScreen();
            return;
        default:
            cout << "Invalid selection. Try again.\n\n";
            pauseScreen();
            break;
        }
    }
}
//Payment
void viewPaymentSummary(CurrentUser& currentUser, WeddingEvent& selectedEvent, vector<Vendor>& vendorList) { // Changed to reference
    clearScreen();
    if (selectedEvent.status != "planning") {
        cout << "Payment summary is only available for weddings in the 'complete' state." << endl;
        pauseScreen();
        return;
    }

    if (selectedEvent.bookedServices.empty()) {
        cout << "No services have been booked for this wedding." << endl;
        pauseScreen();
        return;
    }

    cout << "=== PAYMENT SUMMARY (INVOICE) ===" << endl;
    cout << "Invoice for Wedding Event ID: " << selectedEvent.eventId << endl;
    cout << "Client: " << currentUser.userName << endl;
    cout << "Couple: " << selectedEvent.groomName << " & " << selectedEvent.brideName << endl;
    cout << "Wedding Date: " << selectedEvent.weddingDate << endl;
    cout << "---------------------------------------" << endl;
    cout << "Description                 Cost (RM)" << endl;
    cout << "---------------------------------------" << endl;

    double subtotal = 0.0;
    for (string serviceIdNum : selectedEvent.bookedServices) {
        string searchId = serviceIdNum;
        cout << "Looking for service: " << searchId << endl;

        bool found = false;
        for (const auto& vendor : vendorList) {
            cout << "Checking vendor: " << vendor.vendorId << endl;
            for (const auto& service : vendor.serviceHasProvide) {
                cout << "Service ID: '" << service.serviceId << "' vs '" << searchId << "'" << endl;

                if (service.serviceId == searchId) {
                    cout << left << setw(30) << service.serviceName << right << setw(10) << fixed << setprecision(2) << service.price << endl;
                    subtotal += service.price;
                    found = true;
                    break;
                }
            }
            if (found) break;
        }

        if (!found) {
            cout << "Service " << searchId << " NOT FOUND!" << endl;
        }
    }

    cout << "---------------------------------------" << endl;
    cout << left << setw(30) << "Subtotal" << right << setw(10) << fixed << setprecision(2) << subtotal << endl;

    double lateFine = 0.0;
    time_t now = time(0);
    tm nowTm;
    localtime_s(&nowTm, &now);

    tm weddingTm = { 0 };
    weddingTm.tm_year = stoi(selectedEvent.weddingDate.substr(0, 4)) - 1900;
    weddingTm.tm_mon = stoi(selectedEvent.weddingDate.substr(5, 2)) - 1;
    weddingTm.tm_mday = stoi(selectedEvent.weddingDate.substr(8, 2));

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

void generateReport(CurrentUser& currentUser, WeddingEvent& selectedEvent, vector<Vendor>& vendorList) { // Changed to reference
    clearScreen();
    if (selectedEvent.status != "Paid") {
        cout << "Report can only be generated after weddings is 'Paid'." << endl;
        pauseScreen();
        return;
    }

    vector<PaymentTransaction> transactions;
    saveDataIntoFile(transactions, "payment_history.txt");

    PaymentTransaction* transaction = nullptr;
    for (auto& pt : transactions) {
        if (pt.weddingId == selectedEvent.eventId) {
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
    cout << "Event ID: " << selectedEvent.eventId << endl;
    cout << "Couple: " << selectedEvent.groomName << " & " << selectedEvent.brideName << endl;
    cout << "Wedding Date: " << selectedEvent.weddingDate << endl;
    cout << "Wedding Venue: " << selectedEvent.weddingVenue << endl;
    cout << "Status: " << selectedEvent.status << endl;
    cout << "--------------------------------------" << endl;
    cout << "PAYMENT DETAILS" << endl;
    cout << "Transaction ID: " << transaction->transactionId << endl;
    cout << "Amount Paid: RM" << fixed << setprecision(2) << transaction->amount << endl;
    cout << "Payment Method: " << transaction->paymentMethod << endl;
    cout << "Payment Status: " << transaction->paymentStatus << endl;
    cout << "--------------------------------------" << endl;
    cout << "SERVICES RENDERED" << endl;
    for (string serviceIdNum : selectedEvent.bookedServices) {
        bool found = false;
        for (const auto& vendor : vendorList) {
            for (const auto& service : vendor.serviceHasProvide) {
                if (service.serviceId == serviceIdNum) {
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

void generateAllPaidReports(vector<WeddingEvent>& events, vector<Vendor>& vendorList, vector<PaymentTransaction>& transactions) {
    clearScreen();
    cout << "======================================" << endl;
    cout << "        ALL PAID WEDDINGS" << endl;
    cout << "======================================" << endl;

    for (auto& event : events) {
        if (event.status == "Paid") {
            // 找对应交易
            PaymentTransaction* transaction = nullptr;
            for (auto& pt : transactions) {
                if (pt.weddingId == event.eventId) {
                    transaction = &pt;
                    break;
                }
            }

            if (!transaction) continue; // 没有交易就跳过

            cout << "--------------------------------------" << endl;
            cout << "Event ID: " << event.eventId << endl;
            cout << "Couple: " << event.groomName << " & " << event.brideName << endl;
            cout << "Wedding Date: " << event.weddingDate << endl;
            cout << "Venue: " << event.weddingVenue << endl;
            cout << "--------------------------------------" << endl;
            cout << "Transaction ID: " << transaction->transactionId << endl;
            cout << "Amount Paid: RM" << fixed << setprecision(2) << transaction->amount << endl;
            cout << "Payment Method: " << transaction->paymentMethod << endl;
            cout << "Payment Status: " << transaction->paymentStatus << endl;
            cout << "--------------------------------------" << endl;
            cout << "Services:" << endl;
            for (string serviceIdNum : event.bookedServices) {
                for (const auto& vendor : vendorList) {
                    for (const auto& service : vendor.serviceHasProvide) {
                        if (service.serviceId == serviceIdNum) {
                            cout << "* " << service.serviceName << " (" << service.type
                                << ") - RM" << fixed << setprecision(2) << service.price << endl;
                        }
                    }
                }
            }
            cout << "======================================" << endl;
        }
    }
    pauseScreen();
}

bool processPayment(CurrentUser& currentUser, vector<WeddingEvent>& events, WeddingEvent& selectedEvent) {
    int PaymentChoice;
    string paymentMethod;
    int methodChoice;

    clearScreen();
    if (selectedEvent.totalCost <= 0) {
        cout << "No services booked yet. Nothing to pay." << endl;
        pauseScreen();
        return false;
    }
    if (selectedEvent.status == "Paid") {
        cout << "You are paid." << endl;
        pauseScreen();
        return false;
    }
    cout << "=== MAKE PAYMENT ===" << endl;
    cout << "Total amount to pay: RM" << fixed << setprecision(2) << selectedEvent.totalCost << endl;
    cout << "Select a payment method:" << endl;
    cout << "1. Credit/Debit Card" << endl;
    cout << "2. Online Banking" << endl;
    cout << "3. TNG" << endl;
    cout << "4. Cash" << endl;
    cout << "Enter your choice: ";
    cin >> methodChoice;
    cin.ignore();

    switch (methodChoice) {
    case 1: {
        paymentMethod = "Credit/Debit Card";
        string cardNumber, securityCode, expiryDate;
        bool cardValid = false;

        while (!cardValid) {
            cout << "Enter 16-digit card number: ";
            getline(cin, cardNumber);

            cout << "Enter 3-digit security code: ";
            getline(cin, securityCode);

            cout << "Enter expiry date (MM/YY): ";
            getline(cin, expiryDate);

            bool isCardNumberValid = cardNumber.length() == 16 && all_of(cardNumber.begin(), cardNumber.end(), ::isdigit);
            bool isSecurityCodeValid = securityCode.length() == 3 && all_of(securityCode.begin(), securityCode.end(), ::isdigit);

            // Expiry date validation: Must be a future date from the current date (September 2025)
            bool isExpiryDateValid = false;
            if (regex_match(expiryDate, regex("\\d{2}/\\d{2}"))) {
                int expMonth = stoi(expiryDate.substr(0, 2));
                int expYear = stoi(expiryDate.substr(3, 2));
                // Check if month is between 1-12 and year is valid
                if (expMonth >= 1 && expMonth <= 12 && expYear >= 25) {
                    isExpiryDateValid = true;
                }
            }

            if (isCardNumberValid && isSecurityCodeValid && isExpiryDateValid) {
                cardValid = true;
            }
            else {
                cout << "Invalid card details. Please ensure card number is 16 digits, security code is 3 digits, and the expiry date is valid (MM/YY)." << endl;
            }
        }
        loadingTime();
        break;
    }
    case 2: {
        paymentMethod = "Online Banking";
        string username, password;
        cout << "Enter bank username: ";
        getline(cin, username);
        cout << "Enter bank password: ";
        getline(cin, password);
        cout << "Payment processing..." << endl;
        loadingTime();
    } break;
    case 3: {
        paymentMethod = "TNG";
        string phoneNumber;
        bool phoneValid = false;
        while (!phoneValid) {
            cout << "Enter 11-digit Malaysian phone number: ";
            getline(cin, phoneNumber);

            if (phoneNumber.length() == 11 && all_of(phoneNumber.begin(), phoneNumber.end(), ::isdigit)) {
                phoneValid = true;
            }
            else {
                cout << "Invalid phone number. Please enter a valid 11-digit number." << endl;
            }
        }
        loadingTime();
        break;
    }
    case 4: {
        paymentMethod = "Cash";
        double cashAmount;
        cout << "Enter cash amount received: RM";
        cin >> cashAmount;
        cin.ignore();
        cout << "Payment processing..." << endl;
        loadingTime();
    } break;
    default:
        cout << "Invalid payment method!" << endl;
        pauseScreen();
        loadingTime();
        break;
    }
    return true;
}

void receipt(CurrentUser& currentUser, WeddingEvent& selectedEvent, string paymentMethod) {
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
    cout << "Amount Paid: RM " << fixed << setprecision(2) << selectedEvent.totalCost << endl;
    cout << "======================================" << endl;
    cout << "Payment successful!" << endl;

    ofstream paymentFile("payment_history.txt", ios::app);
    if (paymentFile) {
        PaymentTransaction newTransaction;
        int transactionCount = 0;
        ifstream tempFile("payment_history.txt");
        string line;
        while (getline(tempFile, line)) {
            transactionCount++;
        }
        tempFile.close();

        newTransaction.transactionId = generateId("T", transactionCount + 1);
        newTransaction.userId = currentUser.userId;
        newTransaction.userName = currentUser.userName;
        newTransaction.weddingId = selectedEvent.eventId;
        newTransaction.amount = selectedEvent.totalCost;
        newTransaction.paymentMethod = paymentMethod;
        newTransaction.paymentStatus = "PAID";
        newTransaction.transactionDate = dateStr;
        newTransaction.transactionTime = timeStr;
        paymentFile << newTransaction.toFileString() << endl;
        paymentFile.close();
    }
    pauseScreen();
}

void paymentAndReportingMenu(CurrentUser& currentUser, vector<WeddingEvent>& events, vector<Vendor>& vendorList) {
    WeddingEvent selectedEvent;
    string paymentMethod;
    //find out the current wedding
    for (int i = 0; i < events.size(); i++) {
        if (events[i].eventId == currentUser.currentWeddingId) {
            selectedEvent = events[i];
            break;
        }
    }
    if (currentUser.currentWeddingId.empty() || selectedEvent.eventId.empty()) {
        cout << "No active wedding found!" << endl;
        pauseScreen();
        return;
    }

    int paymentChoice;
    do {
        clearScreen();
        cout << "=== PAYMENT & REPORTING MODULE ===" << endl;
        cout << "1. Make Payment" << endl;
        cout << "2. View Payment Summary" << endl;
        cout << "3. Generate Report" << endl;
        cout << "4. View Payment History" << endl;
        cout << "0. Return to Main Menu" << endl;
        cout << "Enter your choice: ";
        cin >> paymentChoice;
        cin.ignore();

        switch (paymentChoice) {
        case 1: {
            if (processPayment(currentUser, events, selectedEvent)) {
                receipt(currentUser, selectedEvent, paymentMethod);
                selectedEvent.status = "Paid";
                saveDataIntoFile(events, "events.txt");
            }
            break;
        }
        case 2:
            viewPaymentSummary(currentUser, selectedEvent, vendorList); // Changed call to use '*'
            break;
        case 3:
            generateReport(currentUser, selectedEvent, vendorList); // Changed call to use '*'
            break;
        case 4:
            viewPaymentHistory(currentUser);
            break;
        case 0:
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
    if (currentUser.type != ORGANIZER) {
        cout << "Access denied. Only organizers can view payment history." << endl;
        pauseScreen();
        return;
    }
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
            cout << "Amount: RM " << fixed << setprecision(2) << pt.amount << endl;
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

void loadingTime() {
    cout << "Loading...";

    for (int i = 0; i < 3; i++)
    {
        cout << ".";
        Sleep(500); // Pauses execution for 500 milliseconds (0.5 seconds)
    }
    cout << endl;
}

int main() {
    vector<Vendor> vendorList;
    vector<Organizer> organizerList;
    vector<Admin> adminList;
    vector<WeddingEvent> events;
    vector<Participant> participants;
    vector<PaymentTransaction> transactions;
    CurrentUser currentUser;

    // Load data from files
    loadDataFromFile<Admin>(adminList, "admins.txt");
    loadDataFromFile<Organizer>(organizerList, "organizers.txt");
    loadDataFromFile<Vendor>(vendorList, "vendors.txt");
    loadDataFromFile<PaymentTransaction>(transactions, "payment_history.txt");
    loadDataFromFile<WeddingEvent>(events, "events.txt");
    addDefaultParticipants(participants);



    // Start main menu
    mainMenu(currentUser, vendorList, organizerList, adminList, events, participants, transactions);

    // Save data before exit
    saveDataIntoFile<Admin>(adminList, "admins.txt");
    saveDataIntoFile<Organizer>(organizerList, "organizers.txt");
    saveDataIntoFile<Vendor>(vendorList, "vendors.txt");
    saveDataIntoFile(events, "events.txt");

    return 0;
}
