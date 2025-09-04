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
#include <conio.h> // For _getch()
using namespace std;

// Forward declarations
struct WeddingEvent;
struct CurrentUser;
struct Vendor;
struct Organizer;
struct Admin;
struct PaymentTransaction;

// Utility functions declarations
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
void viewPaymentHistory(CurrentUser& currentUser);
void paymentAndReportingMenu(CurrentUser& currentUser, WeddingEvent* selectedEvent, vector<WeddingEvent>& events);
void viewPaymentSummary(const CurrentUser& currentUser, const WeddingEvent* selectedEvent, const vector<Vendor>& vendorList);
void generateReport(const CurrentUser& currentUser, const WeddingEvent* selectedEvent, const vector<Vendor>& vendorList);

// Enhanced validation functions
bool isValidEmail(const string& email);
bool isValidPhoneNumber(const string& phone);
bool isValidPassword(const string& password);
bool isValidName(const string& name);
bool isValidBudget(double budget);
bool isValidWeddingDate(const string& date);

// Service structure
struct Service
{
    string serviceId;
    string serviceName;
    string description;
    string type;
    double price;
    int quantity;
    bool available;

    string toFileString() const {
        return serviceId + "|" + serviceName + "|" + description + "|" + type + "|" +
            to_string(price) + "|" + to_string(quantity) + "|" + (available ? "1" : "0");
    }

    static Service fromFileString(string& str)
    {
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

    string toFileString() const {
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

    static WeddingEvent fromFileString(string& str) {
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

// Product structure
struct Product
{
    string productId;
    string productName;
    string description;
    string type;
    int quantity;
    double price;
    bool available;

    string toFileString() const
    {
        return productId + "|" + productName + "|" + description + "|" + type + "|" +
            to_string(quantity) + "|" + to_string(price) + "|" + (available ? "1" : "0");
    }

    static Product fromFileString(string& str)
    {
        Product p;
        stringstream ss(str);
        string temp;

        getline(ss, p.productId, '|');
        getline(ss, p.productName, '|');
        getline(ss, p.description, '|');
        getline(ss, p.type, '|');
        getline(ss, temp, '|');
        p.quantity = stoi(temp);
        getline(ss, temp, '|');
        p.price = stod(temp);
        getline(ss, temp);
        p.available = (temp == "1");

        return p;
    }
};

// BaseInfo structure
struct BaseInfo
{
    string name;
    string email;
    string phoneNum;
    string password;

    string toFileString() const
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

// Vendor structure
struct Vendor
{
    BaseInfo baseInfo;
    string vendorId;
    string companyName;
    string companyContactNum;
    string type;
    vector<Service> serviceHasProvide;
    vector<Product> productHasProvide;
    int totalServicesProvided;
    int totalProductProvided;

    string toFileString() const
    {
        string baseStr = baseInfo.toFileString();
        string vendorStr = vendorId + "|" + companyName + "|" + companyContactNum + "|" + type + "|" + to_string(totalServicesProvided) + "|" + to_string(totalProductProvided);

        string servicesStr;
        for (const auto& s : serviceHasProvide)
        {
            if (!servicesStr.empty())
                servicesStr += "##";
            servicesStr += s.toFileString();
        }

        string productsStr;
        for (const auto& p : productHasProvide)
        {
            if (!productsStr.empty())
                productsStr += "##";
            productsStr += p.toFileString();
        }

        return baseStr + "|" + vendorStr + "|" + servicesStr + "|" + productsStr;
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

        getline(ss, v.vendorId, '|');
        getline(ss, v.companyName, '|');
        getline(ss, v.companyContactNum, '|');
        getline(ss, v.type, '|');
        getline(ss, segment, '|');
        v.totalServicesProvided = stoi(segment);
        getline(ss, segment, '|');
        v.totalProductProvided = stoi(segment);

        getline(ss, segment, '|');
        if (!segment.empty())
        {
            stringstream sss(segment);
            string sItem;
            while (getline(sss, sItem, '#'))
            {
                if (!sItem.empty())
                {
                    v.serviceHasProvide.push_back(Service::fromFileString(sItem));
                }
            }
        }

        getline(ss, segment);
        if (!segment.empty())
        {
            stringstream pss(segment);
            string pItem;
            while (getline(pss, pItem, '#'))
            {
                if (!pItem.empty())
                {
                    v.productHasProvide.push_back(Product::fromFileString(pItem));
                }
            }
        }

        return v;
    }
};

// Organizer structure
struct Organizer
{
    BaseInfo baseInfo;
    string organizerId;
    string currentWeddingId;
    vector<int> bookedServices;

    string toFileString() const
    {
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

    static Organizer fromFileString(string& str)
    {
        Organizer o;
        stringstream ss(str);

        getline(ss, o.baseInfo.name, '|');
        getline(ss, o.baseInfo.email, '|');
        getline(ss, o.baseInfo.phoneNum, '|');
        getline(ss, o.baseInfo.password, '|');
        getline(ss, o.organizerId, '|');
        getline(ss, o.currentWeddingId, '|');

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

// Admin structure
struct Admin
{
    BaseInfo baseInfo;
    string adminId;

    string toFileString() const
    {
        return baseInfo.toFileString() + "|" + adminId;
    }

    static Admin fromFileString(string& str)
    {
        Admin a;
        stringstream ss(str);

        getline(ss, a.baseInfo.name, '|');
        getline(ss, a.baseInfo.email, '|');
        getline(ss, a.baseInfo.phoneNum, '|');
        getline(ss, a.baseInfo.password, '|');
        getline(ss, a.adminId);

        return a;
    }
};

enum UserType
{
    NONE,
    ADMIN,
    ORGANIZER,
    VENDOR
};

struct CurrentUser
{
    UserType type = NONE;
    int userIndex = -1;
    string userId = "";
    string userName = "";
    string currentWeddingId;
};

// Payment Transaction structure
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

    string toFileString() const {
        return transactionId + "|" + userId + "|" + userName + "|" + weddingId + "|" + to_string(amount) + "|" +
            paymentMethod + "|" + paymentStatus + "|" + transactionDate + "|" + transactionTime;
    }

    static PaymentTransaction fromFileString(string& str) {
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
        getline(ss, pt.transactionTime);

        return pt;
    }
};

// Utility Functions
string generateId(const string& prefix, size_t counter)
{
    stringstream ss;
    ss << prefix << 10000 + counter;
    return ss.str();
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

// Enhanced Validation Functions
bool isValidEmail(const string& email) {
    if (email.empty()) return false;

    size_t atPos = email.find('@');
    size_t dotPos = email.find('.', atPos);

    if (atPos == string::npos || dotPos == string::npos) {
        return false;
    }

    if (atPos == 0) return false;
    if (dotPos == atPos + 1) return false;
    if (dotPos == email.length() - 1) return false;
    if (email.find(' ') != string::npos) return false;
    if (email.find('@', atPos + 1) != string::npos) return false;

    return true;
}

bool isValidPhoneNumber(const string& phone) {
    if (phone.length() < 10 || phone.length() > 15) return false;

    for (char c : phone) {
        if (!isdigit(c) && c != '+' && c != '-' && c != ' ' && c != ')') {
            return false;
        }
    }

    int digitCount = 0;
    for (char c : phone) {
        if (isdigit(c)) digitCount++;
    }

    return digitCount >= 10 && digitCount <= 13;
}

bool isValidPassword(const string& password) {
    if (password.length() < 8) {
        cout << "Password must be at least 8 characters long!\n";
        return false;
    }

    bool hasUpper = false, hasLower = false, hasDigit = false;

    for (char c : password) {
        if (isupper(c)) hasUpper = true;
        if (islower(c)) hasLower = true;
        if (isdigit(c)) hasDigit = true;
    }

    if (!hasUpper) {
        cout << "Password must contain at least one uppercase letter!\n";
        return false;
    }
    if (!hasLower) {
        cout << "Password must contain at least one lowercase letter!\n";
        return false;
    }
    if (!hasDigit) {
        cout << "Password must contain at least one digit!\n";
        return false;
    }

    return true;
}

bool isValidName(const string& name) {
    if (name.empty() || name.length() < 2) {
        cout << "Name must be at least 2 characters long!\n";
        return false;
    }

    for (char c : name) {
        if (!isalpha(c) && c != ' ' && c != '\'' && c != '-') {
            cout << "Name can only contain letters, spaces, apostrophes, and hyphens!\n";
            return false;
        }
    }

    bool hasLetter = false;
    for (char c : name) {
        if (isalpha(c)) {
            hasLetter = true;
            break;
        }
    }

    if (!hasLetter) {
        cout << "Name must contain at least one letter!\n";
        return false;
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

void loadEventsFromFile(vector<WeddingEvent>& events, const string& filename) {
    ifstream file(filename);
    if (!file) return;

    events.clear();
    string line;
    while (getline(file, line)) {
        if (!line.empty()) {
            try {
                events.push_back(WeddingEvent::fromFileString(line));
            }
            catch (...) {
                cout << "Error parsing event line: " << line << endl;
            }
        }
    }
    file.close();
}

void saveEventsToFile(const vector<WeddingEvent>& events, const string& filename) {
    ofstream file(filename);
    if (!file) {
        cout << "Error opening events file for writing." << endl;
        return;
    }

    for (const auto& event : events) {
        file << event.toFileString() << endl;
    }
    file.close();
}

// Invitation Card Function
void generateInvitationCard(const CurrentUser& currentUser, const vector<WeddingEvent>& events,
    const vector<Organizer>& organizerList) {
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

// User Management Functions
void getBaseUserInfo(BaseInfo& baseInfo)
{
    do {
        cout << "Enter name: ";
        getline(cin, baseInfo.name);
    } while (!isValidName(baseInfo.name));

    do {
        cout << "Enter email: ";
        getline(cin, baseInfo.email);
        if (!isValidEmail(baseInfo.email)) {
            cout << "Invalid email format! Example: user@example.com\n";
        }
    } while (!isValidEmail(baseInfo.email));

    do {
        cout << "Enter phone: ";
        getline(cin, baseInfo.phoneNum);
        if (!isValidPhoneNumber(baseInfo.phoneNum)) {
            cout << "Invalid phone number! Must be 10-13 digits.\n";
        }
    } while (!isValidPhoneNumber(baseInfo.phoneNum));

    do {
        cout << "Enter password: ";
        getline(cin, baseInfo.password);
    } while (!isValidPassword(baseInfo.password));
}

void getVendorInfo(Vendor& vendor, vector<Vendor>& vendorList)
{
    getBaseUserInfo(vendor.baseInfo);
    vendor.vendorId = generateId("V", vendorList.size() + 1);

    cout << "Enter company name: ";
    getline(cin, vendor.companyName);

    cout << "Enter company contact number: ";
    getline(cin, vendor.companyContactNum);

    cout << "Enter vendor type (e.g., 'individual', 'company'): ";
    getline(cin, vendor.type);

    vendor.totalServicesProvided = 0;
    vendor.totalProductProvided = 0;
}

void getOrganizerInfo(Organizer& organizer, vector<Organizer>& organizerList)
{
    getBaseUserInfo(organizer.baseInfo);
    organizer.organizerId = generateId("O", organizerList.size() + 1);
    organizer.currentWeddingId = "";
}

bool getAdminInfo(Admin& admin, vector<Admin>& adminList)
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

template <typename T>
void saveUserIntoFile(const vector<T>& data, const string& fileName)
{
    ofstream write(fileName);

    if (!write)
    {
        cout << "Error opening file." << endl;
        return;
    }

    for (const auto& obj : data)
    {
        write << obj.toFileString() << endl;
    }
    write.close();
}

template <typename T>
void loadUserFromFile(vector<T>& data, const string& fileName)
{
    ifstream read(fileName);
    if (!read)
        return;

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

void loadAllData(vector<Vendor>& vendorList, vector<Organizer>& organizerList, vector<Admin>& adminList)
{
    loadUserFromFile(adminList, "admins.txt");
    loadUserFromFile(organizerList, "organizers.txt");
    loadUserFromFile(vendorList, "vendors.txt");
}

void saveAllData(vector<Vendor>& vendorList, vector<Organizer>& organizerList, vector<Admin>& adminList)
{
    saveUserIntoFile(adminList, "admins.txt");
    saveUserIntoFile(organizerList, "organizers.txt");
    saveUserIntoFile(vendorList, "vendors.txt");
}

bool login(vector<Vendor>& vendorList, vector<Organizer>& organizerList, vector<Admin>& adminList, CurrentUser& currentUser)
{
    clearScreen();
    cout << "=== LOGIN ===" << endl;

    string email, password;
    cout << "Enter email: ";
    getline(cin, email);

    cout << "Enter password: ";
    char ch;
    password = "";
    while ((ch = _getch()) != 13) { // 13 is Enter key
        if (ch == 8) { // 8 is Backspace key
            if (!password.empty()) {
                cout << "\b \b";
                password.pop_back();
            }
        }
        else {
            password += ch;
            cout << "*";
        }
    }
    cout << endl;

    for (size_t i = 0; i < adminList.size(); i++)
    {
        if (adminList[i].baseInfo.email == email && adminList[i].baseInfo.password == password)
        {
            currentUser.type = ADMIN;
            currentUser.userIndex = static_cast<int>(i);
            currentUser.userId = adminList[i].adminId;
            currentUser.userName = adminList[i].baseInfo.name;
            currentUser.currentWeddingId = "";
            cout << "Welcome Admin " << currentUser.userName << "!" << endl;
            pauseScreen();
            return true;
        }
    }

    for (size_t i = 0; i < organizerList.size(); i++)
    {
        if (organizerList[i].baseInfo.email == email && organizerList[i].baseInfo.password == password)
        {
            currentUser.type = ORGANIZER;
            currentUser.userIndex = static_cast<int>(i);
            currentUser.userId = organizerList[i].organizerId;
            currentUser.userName = organizerList[i].baseInfo.name;
            currentUser.currentWeddingId = organizerList[i].currentWeddingId;
            cout << "Welcome " << currentUser.userName << "!" << endl;
            pauseScreen();
            return true;
        }
    }

    for (size_t i = 0; i < vendorList.size(); i++)
    {
        if (vendorList[i].baseInfo.email == email && vendorList[i].baseInfo.password == password)
        {
            currentUser.type = VENDOR;
            currentUser.userIndex = static_cast<int>(i);
            currentUser.userId = vendorList[i].vendorId;
            currentUser.userName = vendorList[i].baseInfo.name;
            currentUser.currentWeddingId = "";
            cout << "Welcome " << currentUser.userName << "!" << endl;
            pauseScreen();
            return true;
        }
    }

    cout << "Invalid email or password!" << endl;
    pauseScreen();
    return false;
}

void logout(CurrentUser& currentUser)
{
    char confirmed;
    cout << "Are you sure you want to logout (Y/N): ";
    cin >> confirmed;
    cin.ignore();

    if (toupper(confirmed) == 'Y')
    {
        currentUser.type = NONE;
        currentUser.userIndex = -1;
        currentUser.userId = "";
        currentUser.userName = "";
        cout << "Logged out successfully!" << endl;
        return;
    }
    else
    {
        cout << "Logout cancelled." << endl;
        return;
    }
}

// Service and Product Management Functions
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

    cout << "Type (e.g., catering, photography, decoration, venue): ";
    getline(cin, newService.type);

    cout << "Price: RM";
    cin >> newService.price;

    cout << "Quantity available: ";
    cin >> newService.quantity;
    cin.ignore();

    newService.available = true;

    vendorList[currentUser.userIndex].serviceHasProvide.push_back(newService);
    vendorList[currentUser.userIndex].totalServicesProvided++;

    saveUserIntoFile(vendorList, "vendors.txt");
    cout << "Service added successfully!" << endl;
    pauseScreen();
}

void addProduct(CurrentUser& currentUser, vector<Vendor>& vendorList)
{
    if (currentUser.type != VENDOR)
        return;

    Product newProduct;
    newProduct.productId = generateId("P", vendorList[currentUser.userIndex].productHasProvide.size() + 1);
    cout << "=== ADD NEW PRODUCT ===" << endl;

    cout << "Product name: ";
    getline(cin, newProduct.productName);

    cout << "Description: ";
    getline(cin, newProduct.description);

    cout << "Type: ";
    getline(cin, newProduct.type);

    cout << "Price: RM";
    cin >> newProduct.price;

    cout << "Quantity available: ";
    cin >> newProduct.quantity;
    cin.ignore();

    newProduct.available = true;

    vendorList[currentUser.userIndex].productHasProvide.push_back(newProduct);
    vendorList[currentUser.userIndex].totalProductProvided++;

    saveUserIntoFile(vendorList, "vendors.txt");
    cout << "Product added successfully!" << endl;
    pauseScreen();
}

// Wedding Event Functions
void createNewWedding(CurrentUser& currentUser, vector<WeddingEvent>& events,
    vector<Vendor>& vendorList, vector<Organizer>& organizerList) {
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

    saveEventsToFile(events, "events.txt");
    saveUserIntoFile(organizerList, "organizers.txt");

    cout << "Wedding created successfully! Event ID: " << newEvent.eventId << endl;
    pauseScreen();
}

// Display Functions
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
        Admin& admin = adminList[static_cast<size_t>(currentUser.userIndex)];
        cout << "User Type: Administrator" << endl;
        cout << "Admin ID: " << admin.adminId << endl;
        cout << "Name: " << admin.baseInfo.name << endl;
        cout << "Email: " << admin.baseInfo.email << endl;
        cout << "Phone: " << admin.baseInfo.phoneNum << endl;
        break;
    }
    case ORGANIZER:
    {
        Organizer& org = organizerList[static_cast<size_t>(currentUser.userIndex)];
        cout << "User Type: Wedding Organizer" << endl;
        cout << "Organizer ID: " << org.organizerId << endl;
        cout << "Name: " << org.baseInfo.name << endl;
        cout << "Email: " << org.baseInfo.email << endl;
        cout << "Phone: " << org.baseInfo.phoneNum << endl;
        cout << "Current Wedding ID: " << org.currentWeddingId << endl;
        cout << "Booked Services: " << org.bookedServices.size() << endl;
        break;
    }
    case VENDOR:
    {
        Vendor& vendor = vendorList[static_cast<size_t>(currentUser.userIndex)];
        cout << "User Type: Service Vendor" << endl;
        cout << "Vendor ID: " << vendor.vendorId << endl;
        cout << "Name: " << vendor.baseInfo.name << endl;
        cout << "Email: " << vendor.baseInfo.email << endl;
        cout << "Phone: " << vendor.baseInfo.phoneNum << endl;
        cout << "Company: " << vendor.companyName << endl;
        cout << "Company Phone: " << vendor.companyContactNum << endl;
        cout << "Vendor Type: " << vendor.type << endl;
        cout << "Services Provided: " << vendor.serviceHasProvide.size() << endl;
        cout << "Products Provided: " << vendor.productHasProvide.size() << endl;
        break;
    }
    }
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
        if (getAdminInfo(admin, adminList))
        {
            adminList.push_back(admin);
            cout << "Admin registered successfully!" << endl;
        }
        break;
    }
    case 2:
    {
        Organizer organizer;
        getOrganizerInfo(organizer, organizerList);
        organizerList.push_back(organizer);
        cout << "Organizer registered successfully!" << endl;
        break;
    }
    case 3:
    {
        Vendor vendor;
        getVendorInfo(vendor, vendorList);
        vendorList.push_back(vendor);
        cout << "Vendor registered successfully!" << endl;
        break;
    }
    default:
        cout << "Invalid choice." << endl;
        break;
    }

    saveAllData(vendorList, organizerList, adminList);
    pauseScreen();
}

// Booking Functions
void bookServicesForWedding(CurrentUser& currentUser, vector<WeddingEvent>& events,
    vector<Vendor>& vendorList, vector<Organizer>& organizerList) {
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

    saveEventsToFile(events, "events.txt");
    saveUserIntoFile(vendorList, "vendors.txt");
    saveUserIntoFile(organizerList, "organizers.txt");

    cout << "Service booked successfully!" << endl;
    pauseScreen();
}

void viewAllWeddings(CurrentUser& currentUser, const vector<WeddingEvent>& events,
    const vector<Vendor>& vendorList) {
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

// New functions for View Payment Summary and Generate Report
void viewPaymentSummary(const CurrentUser& currentUser, const WeddingEvent* selectedEvent, const vector<Vendor>& vendorList) {
    clearScreen();
    if (selectedEvent->status != "planning") {
        cout << "Payment summary is only available for weddings in the 'planning' state." << endl;
        pauseScreen();
        return;
    }

    cout << "=== PAYMENT SUMMARY (INVOICE) ===" << endl;
    cout << "Invoice for Wedding Event ID: " << selectedEvent->eventId << endl;
    cout << "Client: " << currentUser.userName << endl;
    cout << "Couple: " << selectedEvent->groomName << " & " << selectedEvent->brideName << endl;
    cout << "Wedding Date: " << selectedEvent->weddingDate << endl;
    cout << "---------------------------------------" << endl;
    cout << "Description                    Cost (RM)" << endl;
    cout << "---------------------------------------" << endl;

    double subtotal = 0.0;
    for (int serviceIdNum : selectedEvent->bookedServices) {
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
    // Check for late payment
    time_t now = time(0);
    tm nowTm;
    localtime_s(&nowTm, &now);

    tm weddingTm = { 0 };
    weddingTm.tm_year = stoi(selectedEvent->weddingDate.substr(0, 4)) - 1900;
    weddingTm.tm_mon = stoi(selectedEvent->weddingDate.substr(5, 2)) - 1;
    weddingTm.tm_mday = stoi(selectedEvent->weddingDate.substr(8, 2));

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

void generateReport(const CurrentUser& currentUser, const WeddingEvent* selectedEvent, const vector<Vendor>& vendorList) {
    clearScreen();
    if (selectedEvent->status != "completed") {
        cout << "Report can only be generated for weddings in the 'completed' state." << endl;
        pauseScreen();
        return;
    }

    // Load payment history to find the transaction
    vector<PaymentTransaction> transactions;
    ifstream file("payment_history.txt");
    if (file.is_open()) {
        string line;
        while (getline(file, line)) {
            transactions.push_back(PaymentTransaction::fromFileString(line));
        }
        file.close();
    }

    PaymentTransaction* transaction = nullptr;
    for (auto& pt : transactions) {
        if (pt.weddingId == selectedEvent->eventId) {
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
    cout << "Event ID: " << selectedEvent->eventId << endl;
    cout << "Couple: " << selectedEvent->groomName << " & " << selectedEvent->brideName << endl;
    cout << "Wedding Date: " << selectedEvent->weddingDate << endl;
    cout << "Wedding Venue: " << selectedEvent->weddingVenue << endl;
    cout << "Status: " << selectedEvent->status << endl;
    cout << "--------------------------------------" << endl;
    cout << "PAYMENT DETAILS" << endl;
    cout << "Transaction ID: " << transaction->transactionId << endl;
    cout << "Amount Paid: RM" << fixed << setprecision(2) << transaction->amount << endl;
    cout << "Payment Method: " << transaction->paymentMethod << endl;
    cout << "Payment Status: " << transaction->paymentStatus << endl;
    cout << "--------------------------------------" << endl;
    cout << "SERVICES RENDERED" << endl;
    for (int serviceIdNum : selectedEvent->bookedServices) {
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
                saveEventsToFile(events, "events.txt");

                // Get current date and time
                time_t now = time(0);
                tm ltm;
                localtime_s(&ltm, &now);
                char dateStr[11];
                strftime(dateStr, sizeof(dateStr), "%Y-%m-%d", &ltm);
                char timeStr[9];
                strftime(timeStr, sizeof(timeStr), "%H:%M:%S", &ltm);

                // Generate receipt
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

                // Save transaction history
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
                return; // Exit payment menu after successful payment
            }
            else {
                selectedEvent->status = "planning";
                saveEventsToFile(events, "events.txt");
            }
        } break;
        case 2:
            viewPaymentSummary(currentUser, selectedEvent, vendorList);
            break;
        case 3:
            generateReport(currentUser, selectedEvent, vendorList);
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

void manageMyWeddings(CurrentUser& currentUser, vector<WeddingEvent>& events,
    vector<Vendor>& vendorList, vector<Organizer>& organizerList) {
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
    cout << "3. Going to pay" << endl;
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
        saveUserIntoFile(organizerList, "organizers.txt");
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
            saveEventsToFile(events, "events.txt");
            cout << "Wedding cancelled successfully." << endl;
        }
        else {
            cout << "Cancellation aborted." << endl;
        }
        pauseScreen();
        break;

    case 3:
        paymentAndReportingMenu(currentUser, selectedEvent, events, vendorList);
        break;

    case 4:
        generateInvitationCard(currentUser, events, organizerList);
        break;

    default:
        break;
    }
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

// Menu Functions
void organizerMenu(CurrentUser& currentUser, vector<Vendor>& vendorList,
    vector<Organizer>& organizerList, vector<Admin>& adminList,
    vector<WeddingEvent>& events) {
    int choice;
    do {
        clearScreen();
        cout << "=== ORGANIZER DASHBOARD ===" << endl;
        cout << "Welcome, " << currentUser.userName << " (Wedding Organizer)" << endl;

        string currentWeddingId;
        for (const auto& organizer : organizerList) {
            if (organizer.organizerId == currentUser.userId) {
                currentWeddingId = organizer.currentWeddingId;
                break;
            }
        }

        if (!currentWeddingId.empty()) {
            for (const auto& event : events) {
                if (event.eventId == currentWeddingId && event.status != "cancelled") {
                    cout << "Current Wedding: " << event.groomName << " & " << event.brideName << endl;
                    cout << "Date: " << event.weddingDate << " | Status: " << event.status << endl;
                    break;
                }
            }
        }

        cout << "==========================================" << endl;
        cout << "1. Create a New Wedding" << endl;
        cout << "2. Manage My Weddings" << endl;
        cout << "3. Book Services" << endl;
        cout << "4. My Profile" << endl;
        cout << "5. Payment History" << endl;
        cout << "0. Logout" << endl;
        cout << "==========================================" << endl;
        cout << "Enter your choice: ";
        cin >> choice;
        cin.ignore();

        switch (choice) {
        case 1:
            createNewWedding(currentUser, events, vendorList, organizerList);
            break;
        case 2:
            manageMyWeddings(currentUser, events, vendorList, organizerList);
            break;
        case 3:
            bookServicesForWedding(currentUser, events, vendorList, organizerList);
            break;
        case 4:
            displayUserProfile(currentUser, vendorList, organizerList, adminList);
            break;
        case 5:
            viewPaymentHistory(currentUser);
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

void adminMenu(CurrentUser& currentUser, vector<Vendor>& vendorList,
    vector<Organizer>& organizerList, vector<Admin>& adminList,
    vector<WeddingEvent>& events) {
    int choice;
    do {
        clearScreen();
        cout << "=== ADMIN DASHBOARD ===" << endl;
        cout << "Welcome, " << currentUser.userName << " (Administrator)" << endl;
        cout << "==========================================" << endl;
        cout << "1. Register New User" << endl;
        cout << "2. View All Weddings" << endl;
        cout << "3. View All Services" << endl;
        cout << "4. My Profile" << endl;
        cout << "0. Logout" << endl;
        cout << "==========================================" << endl;
        cout << "Enter your choice: ";
        cin >> choice;
        cin.ignore();

        switch (choice) {
        case 1:
            userRegister(vendorList, organizerList, adminList);
            break;
        case 2:
            viewAllWeddings(currentUser, events, vendorList);
            break;
        case 3:
            displayAllServices(vendorList);
            break;
        case 4:
            displayUserProfile(currentUser, vendorList, organizerList, adminList);
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

void vendorMenu(CurrentUser& currentUser, vector<Vendor>& vendorList,
    vector<Organizer>& organizerList, vector<Admin>& adminList)
{
    int choice;
    do
    {
        clearScreen();
        cout << "=== VENDOR DASHBOARD ===" << endl;
        cout << "Welcome, " << currentUser.userName << " (Service Vendor)" << endl;

        Vendor& vendor = vendorList[static_cast<size_t>(currentUser.userIndex)];
        cout << "Company: " << vendor.companyName << endl;
        cout << "Services: " << vendor.serviceHasProvide.size() << " | Products: " << vendor.productHasProvide.size() << endl;
        cout << "==========================================" << endl;

        cout << "1. Add New Service" << endl;
        cout << "2. Add New Product" << endl;
        cout << "3. My Profile" << endl;
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
            addProduct(currentUser, vendorList);
            break;
        case 3:
            displayUserProfile(currentUser, vendorList, organizerList, adminList);
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

void mainMenu(CurrentUser& currentUser, vector<Vendor>& vendorList,
    vector<Organizer>& organizerList, vector<Admin>& adminList, vector<WeddingEvent>& events)
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
                switch (currentUser.type)
                {
                case ADMIN:
                    adminMenu(currentUser, vendorList, organizerList, adminList, events);
                    break;
                case ORGANIZER:
                    organizerMenu(currentUser, vendorList, organizerList, adminList, events);
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

int main() {
    vector<Admin> adminList;
    vector<Organizer> organizerList;
    vector<Vendor> vendorList;
    vector<WeddingEvent> events;

    CurrentUser currentUser;

    loadAllData(vendorList, organizerList, adminList);
    loadEventsFromFile(events, "events.txt");

    mainMenu(currentUser, vendorList, organizerList, adminList, events);

    saveAllData(vendorList, organizerList, adminList);
    saveEventsToFile(events, "events.txt");

    return 0;
}
