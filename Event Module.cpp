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
using namespace std;

// Forward declarations using 'struct' for consistency
struct WeddingEvent;
struct CurrentUser;
struct Vendor;
struct Organizer;
struct Admin;

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

// Corrected `Service::toFileString` to be a const member function
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

// Corrected `WeddingEvent::toFileString` to be a const member function
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

// Corrected `Product::toFileString` to be a const member function
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

// Corrected `BaseInfo::toFileString` to be a const member function
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

// Corrected `Vendor::toFileString` to be a const member function
// Corrected parsing in `Vendor::fromFileString`
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

        // Corrected parsing logic for services
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
        // Corrected parsing logic for products
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

// Corrected `Organizer::toFileString` to be a const member function
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

// Corrected `Admin::toFileString` to be a const member function
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

// Utility Functions
string generateId(const string& prefix, size_t counter)
{
    // Use stringstream to handle the conversion safely and avoid arithmetic overflow warnings
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

bool isValidDate(const string& date) {
    if (date.length() != 10) return false;
    if (date[4] != '-' || date[7] != '-') return false;

    for (int i = 0; i < 10; i++) {
        if (i == 4 || i == 7) continue;
        if (!isdigit(date[i])) return false;
    }

    // Corrected C4996 warning by using localtime_s
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
        // Corrected C2662 and E1086 errors by marking toFileString as const
        file << event.toFileString() << endl;
    }
    file.close();
}

// User Management Functions
void getBaseUserInfo(BaseInfo& baseInfo)
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
    getline(cin, password);

    // Check admin
    for (size_t i = 0; i < adminList.size(); i++)
    {
        if (adminList[i].baseInfo.email == email && adminList[i].baseInfo.password == password)
        {
            currentUser.type = ADMIN;
            currentUser.userIndex = static_cast<int>(i); // Corrected C4267 conversion
            currentUser.userId = adminList[i].adminId;
            currentUser.userName = adminList[i].baseInfo.name;
            currentUser.currentWeddingId = "";
            cout << "Welcome Admin " << currentUser.userName << "!" << endl;
            pauseScreen();
            return true;
        }
    }

    // Check organizer
    for (size_t i = 0; i < organizerList.size(); i++)
    {
        if (organizerList[i].baseInfo.email == email && organizerList[i].baseInfo.password == password)
        {
            currentUser.type = ORGANIZER;
            currentUser.userIndex = static_cast<int>(i); // Corrected C4267 conversion
            currentUser.userId = organizerList[i].organizerId;
            currentUser.userName = organizerList[i].baseInfo.name;
            currentUser.currentWeddingId = organizerList[i].currentWeddingId;
            cout << "Welcome " << currentUser.userName << "!" << endl;
            pauseScreen();
            return true;
        }
    }

    // Check vendor
    for (size_t i = 0; i < vendorList.size(); i++)
    {
        if (vendorList[i].baseInfo.email == email && vendorList[i].baseInfo.password == password)
        {
            currentUser.type = VENDOR;
            currentUser.userIndex = static_cast<int>(i); // Corrected C4267 conversion
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
    cout << "Are you sure you want to logout (Y/N): ";
    cin >> confirmed;
    cin.ignore();  // Added to consume newline

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
    newEvent.weddingVenue = "";  // Initialize venue

    cout << "Groom's Name: ";
    getline(cin, newEvent.groomName);

    cout << "Bride's Name: ";
    getline(cin, newEvent.brideName);

    bool validDate = false;
    while (!validDate) {
        cout << "Wedding Date (YYYY-MM-DD): ";
        getline(cin, newEvent.weddingDate);

        if (!isValidDate(newEvent.weddingDate)) {
            cout << "Invalid date! Please use YYYY-MM-DD format and ensure it's a future date." << endl;
        }
        else {
            validDate = true;
        }
    }

    cout << "Wedding Theme: ";
    getline(cin, newEvent.weddingTheme);

    bool validBudget = false;
    while (!validBudget) {
        cout << "Budget (RM): ";
        string budgetStr;
        getline(cin, budgetStr);

        try {
            newEvent.budget = stod(budgetStr);
            if (newEvent.budget <= 0) {
                cout << "Budget must be greater than 0!" << endl;
            }
            else {
                validBudget = true;
            }
        }
        catch (...) {
            cout << "Invalid budget amount!" << endl;
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
        Admin& admin = adminList[static_cast<size_t>(currentUser.userIndex)]; // Corrected C4267 conversion
        cout << "User Type: Administrator" << endl;
        cout << "Admin ID: " << admin.adminId << endl;
        cout << "Name: " << admin.baseInfo.name << endl;
        cout << "Email: " << admin.baseInfo.email << endl;
        cout << "Phone: " << admin.baseInfo.phoneNum << endl;
        break;
    }
    case ORGANIZER:
    {
        Organizer& org = organizerList[static_cast<size_t>(currentUser.userIndex)]; // Corrected C4267 conversion
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
        Vendor& vendor = vendorList[static_cast<size_t>(currentUser.userIndex)]; // Corrected C4267 conversion
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

// Menu Functions
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
    case 5: serviceType = ""; break; // Show all
    default:
        cout << "Invalid choice!" << endl;
        pauseScreen();
        return;
    }

    clearScreen();
    cout << "=== AVAILABLE SERVICES ===" << endl;

    vector<pair<size_t, size_t>> availableServices; // vendor index, service index
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

    if (serviceChoice < 1 || serviceChoice > static_cast<int>(availableServices.size())) { // Corrected C4267 conversion
        cout << "Invalid selection!" << endl;
        pauseScreen();
        return;
    }

    auto& selected = availableServices[static_cast<size_t>(serviceChoice - 1)]; // Corrected C4267 conversion
    Service& service = vendorList[selected.first].serviceHasProvide[selected.second];
    Vendor& vendor = vendorList[selected.first];

    // Check budget
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

    if (choice < 1 || choice > static_cast<int>(myEvents.size())) { // Corrected C4267 conversion
        cout << "Invalid selection!" << endl;
        pauseScreen();
        return;
    }

    WeddingEvent* selectedEvent = myEvents[static_cast<size_t>(choice - 1)]; // Corrected C4267 conversion

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
        selectedEvent->status = "completed";
        saveEventsToFile(events, "events.txt");
        cout << "Wedding marked as completed." << endl;
        pauseScreen();
        break;

    default:
        break;
    }
}

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

        Vendor& vendor = vendorList[static_cast<size_t>(currentUser.userIndex)]; // Corrected C4267 conversion
        cout << "Company: " << vendor.companyName << endl;
        cout << "Services: " << vendor.serviceHasProvide.size() << " | Products: " << vendor.productHasProvide.size() << endl;
        cout << "==========================================" << endl;

        cout << "1. Add New Service" << endl;
        cout << "2. Add New Product" << endl;
        cout << "3. View All Services" << endl;
        cout << "4. My Profile" << endl;
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
