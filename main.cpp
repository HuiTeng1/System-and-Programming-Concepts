#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <fstream>
#include <iomanip>
#include <algorithm>
#include <limits>
using namespace std;

struct Service{
    string serviceName;
    string description;
    string type; ///not sure -- like for planning , food 
    double price;
    int quantity;
    bool available;
    string toFileString() const {
        return serviceName + "|||" + description + "|||" + type + "|||" + 
               to_string(price) + "|||" + to_string(quantity) + "|||" + (available ? "1" : "0");
    }
    
    static Service fromFileString(const string& str) {
        Service s;
        stringstream ss(str);
        string temp;
        
        getline(ss, s.serviceName, '|'); ss.ignore(2);
        getline(ss, s.description, '|'); ss.ignore(2);
        getline(ss, s.type, '|'); ss.ignore(2);
        getline(ss, temp, '|'); s.price = stod(temp); ss.ignore(2);
        getline(ss, temp, '|'); s.quantity = stoi(temp); ss.ignore(2);
        getline(ss, temp); s.available = (temp == "1");
        
        return s;
    }
};

struct Product{
    string productName;
    string description;
    string type; // not sure
    string type;
    int quantity;
    double price;
    string toFileString() const {
        return productName + "|||" + description + "|||" + type + "|||" + 
               to_string(quantity) + "|||" + to_string(price);
    }
    
    static Product fromFileString(const string& str) {
        Product p;
        stringstream ss(str);
        string temp;
        
        getline(ss, p.productName, '|'); ss.ignore(2);
        getline(ss, p.description, '|'); ss.ignore(2);
        getline(ss, p.type, '|'); ss.ignore(2);
        getline(ss, temp, '|'); p.quantity = stoi(temp); ss.ignore(2);
        getline(ss, temp); p.price = stod(temp);
        
        return p;
    }
};

struct BaseInfo{
    string name;
    string email;
    string phoneNum;
    string password;
    string toFileString() const {
        return name + "|||" + email + "|||" + phoneNum + "|||" + password;
    }
    
    static BaseInfo fromFileString(const string& str) {
        BaseInfo b;
        stringstream ss(str);
        
        getline(ss, b.name, '|'); ss.ignore(2);
        getline(ss, b.email, '|'); ss.ignore(2);
        getline(ss, b.phoneNum, '|'); ss.ignore(2);
        getline(ss, b.password);
        
        return b;
    }
};
struct Vendor{
    BaseInfo baseInfo;
    string vendorId;
    string companyName;
    string companyContactNum;
    string type; // not sure will use or not, "individual", "company", "partnership" or which type service can provide
    vector<Service> serviceHasProvide;
    vector<Product> productHasProvide;
    int totalServicesProvided;
    string toFileString() const {
        // baseInfo
        string baseStr = baseInfo.toFileString();
        //Vendor detail
        string vendorStr =  vendorId + "|||" + companyName + "|||" + companyContactNum + "|||" + type + "|||" + to_string(totalServicesProvided);
         // service
        string servicesStr;
        for (const auto& s : serviceHasProvide) {
            if (!servicesStr.empty()) servicesStr += "##";
            servicesStr += s.toFileString();
        }

        // Product
        string productsStr;
        for (const auto& p : productHasProvide) {
            if (!productsStr.empty()) productsStr += "##";
            productsStr += p.toFileString();
        }

        return baseStr + "|||" + vendorStr + "|||" + to_string(totalServicesProvided) + "|||" +
               servicesStr + "|||" + productsStr;
    }
    
    static Vendor fromFileString(const string& str) {
        Vendor v;
        stringstream ss(str);
        string segment;

        // 1. baseInfo
        getline(ss, segment, '|');
        v.baseInfo = BaseInfo::fromFileString(segment);

        // 2. vendorStr
        getline(ss, segment, '|');
        stringstream vss(segment);
        getline(vss, v.vendorId, '|');
        getline(vss, v.companyName, '|');
        getline(vss, v.companyContactNum, '|');
        getline(vss, v.type, '|');
        string totalStr;
        getline(vss, totalStr, '|');
        v.totalServicesProvided = stoi(totalStr);

        // 3. services
        getline(ss, segment, '|');
        if (!segment.empty()) {
            stringstream sss(segment);
            string sItem;
            while (getline(sss, sItem, '#')) {
                if (!sItem.empty() && sItem != "#") { 
                    v.serviceHasProvide.push_back(Service::fromFileString(sItem));
                }
            }
        }

        // 4. products
        getline(ss, segment, '|');
        if (!segment.empty()) {
            stringstream pss(segment);
            string pItem;
            while (getline(pss, pItem, '#')) {
                if (!pItem.empty() && pItem != "#") {
                    v.productHasProvide.push_back(Product::fromFileString(pItem));
                }
            }
        }

        return v;
    }
};
struct Organizer{
    BaseInfo baseInfo;
    string organizerId;
    string groomName;
    string brideName;
    string weddingDate;
    string weddingVenue;
    double budget;
    string weddingTheme;
    vector<int> bookedServices;
    string weddingStage;
    string toFileString() const {
        return baseInfo.toFileString() + "|||" + organizerId + "|||" + groomName + "|||" + 
               brideName + "|||" + weddingDate + "|||" + weddingVenue + "|||" + 
               to_string(budget) + "|||" + weddingTheme + "|||" + weddingStage;
    }
    
    string toFileString() const {
        // bookedServices
        string bookedServicesStr;
        for (size_t i = 0; i < bookedServices.size(); ++i) {
            bookedServicesStr += to_string(bookedServices[i]);
            if (i < bookedServices.size() - 1) {
                bookedServicesStr += ",";
            }
        }

        return baseInfo.toFileString() + "|||" +
               organizerId + "|||" +
               groomName + "|||" +
               brideName + "|||" +
               weddingDate + "|||" +
               weddingVenue + "|||" +
               to_string(budget) + "|||" +
               weddingTheme + "|||" +
               weddingStage + "|||" +
               bookedServicesStr;
    }

    static Organizer fromFileString(const string& str) {
        Organizer o;
        stringstream ss(str);
        string segment;

        // 1. baseInfo
        getline(ss, segment, '|');
        o.baseInfo = BaseInfo::fromFileString(segment);

        //Organizer details
        getline(ss, segment, '|');
        stringstream vss(segment);
        getline(vss, o.organizerId, '|');
        getline(vss, o.groomName, '|');
        getline(vss, o.brideName, '|');
        getline(vss, o.weddingDate, '|');
        getline(vss, o.weddingVenue, '|');

        string temp;
        getline(vss, temp, '|');
        o.budget = stod(temp);

        getline(vss, o.weddingTheme, '|');
        getline(vss, o.weddingStage, '|');

        // bookedServices
        getline(ss, segment); 
        o.bookedServices.clear();
        if (!segment.empty()) {
            stringstream ssServices(segment);
            string serviceStr;
            while (getline(ssServices, serviceStr, ',')) {
                if (!serviceStr.empty()) {
                    o.bookedServices.push_back(stoi(serviceStr));
                }
            }
        }

        return o;
    }
};
struct Admin{
    BaseInfo baseInfo;
    string adminId;
    string toFileString() const {
        return baseInfo.toFileString() + "|||" + adminId;
    }
    
    static Admin fromFileString(const string& str) {
        Admin a;
        stringstream ss(str);
        string baseStr, temp;
        
        // Extract base info part
        for(int i = 0; i < 4; i++) {
            getline(ss, temp, '|');
            if(i > 0) baseStr += "|||";
            baseStr += temp;
            if(i < 3) ss.ignore(2);
        }
        a.baseInfo = BaseInfo::fromFileString(baseStr);
        
        ss.ignore(2);
        getline(ss, a.adminId);
        
        return a;
    }

};
// Current User Management
enum UserType { NONE, ADMIN, ORGANIZER, VENDOR };
struct CurrentUser {
    UserType type = NONE;
    int userIndex = -1;  // Index in the respective vector
    string userId = "";
    string userName = "";
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

// User Management
int main (){
    vector<Admin> adminList;
    vector<Organizer> organizerList;
    vector<Vendor> vendorList;

    //Variable 
    CurrentUser currentUser;
}

string generateId(const string& prefix, int counter) {
    return prefix + to_string(1000 + counter);
}

void clearScreen(){
    system("cls");
}

void pauseScreen() {
    cout << "\nPress Enter to continue...";
    cin.get();
}

void getBaseUserInfo(BaseInfo &baseInfo){

    cout << "Enter name: ";
    getline(cin, baseInfo.name);
    
    cout << "Enter email: ";
    getline(cin, baseInfo.email);
    
    cout << "Enter phone: ";
    getline(cin, baseInfo.phoneNum);
    
    cout << "Enter password: ";
    getline(cin, baseInfo.password);
}

void getVendorInfo(Vendor &vendor, vector<Vendor> &vendorList){
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

void getOrganizerInfo(Organizer &organizer,vector<Organizer> &organizerList){
    string input;

    // Get base info
    getBaseUserInfo(organizer.baseInfo);

    organizer.organizerId = generateId("O", organizerList.size() + 1);

    cout << "Enter groom's name: ";
    getline(cin, organizer.groomName);

    cout << "Enter bride's name: ";
    getline(cin, organizer.brideName);

    // Optional fields: ask user if they want to set or leave empty
    cout << "Wedding date (leave empty if not set): ";
    getline(cin, input);
    organizer.weddingDate = input;

    cout << "Wedding venue (leave empty if not set): ";
    getline(cin, input);
    organizer.weddingVenue = input;

    cout << "Wedding theme (leave empty if not set): ";
    getline(cin, input);
    organizer.weddingTheme = input;

    cout << "Budget (enter 0 if not set): ";
    getline(cin, input);
    try {
        organizer.budget = stod(input);
    } catch (...) {
        organizer.budget = 0.0;
    }

    organizer.weddingStage = "planning";
}
   
bool getAdminInfo(Admin &admin, vector<Admin> &adminList){
    string secretPassword = "Secret";
    string inputPassword;
    getBaseUserInfo(admin.baseInfo);

    admin.adminId = generateId("A", adminList.size() + 1);

    cout << "Enter Secret Password to Register Admin: ";
    getline(cin, inputPassword);

    if (inputPassword != secretPassword) {
        cout << "Incorrect secret password! Registration failed." << endl;
        pauseScreen();
        return false;
    }
}

void userRegister(vector<Admin> &adminList, vector<Organizer> &organizerList, vector<Vendor> &vendorList){
    int choice;
    clearScreen();
    cout << "=== USER REGISTER ===";
    cout << "1. Admin (System Administrator)" << endl;
    cout << "2. Organizer (Wedding Planner - Individual/Couple)" << endl;
    cout << "3. Supplier (Business/Service Provider)" << endl;
    cout << "Enter choice: ";
    cin >> choice;
    cin.ignore();

    switch(choice){
    case 1:{
        Admin admin;
        if(getAdminInfo(admin, adminList)){
            adminList.push_back(admin);
            break;
        }
    }case 2:{
        Organizer organizer;
        getOrganizerInfo(organizer, organizerList);
        organizerList.push_back(organizer);
        break;
    }case 3:{
        Vendor vendor;
        getVendorInfo(vendor, vendorList);
        vendorList.push_back(vendor);
        break;
    }default:
    cout << "Invalid choice." << endl;
        break;
    }

}

template <typename T>
void saveUserIntoFile(vector<T> data,  string fileName){
    ofstream write
    write.open(fileName);

    if(!write){
        cout << "Error opening file." << endl;
        return;
    }
    // 'auto' means the compiler will automatically figure out the type of this variable based on its value
    for(auto &obj : data){
        write << obj.toFileString() << endl;
    }
    write.close();

}

template <typename T>
void loadUserFromFile(vector<T>& data, string fileName) {
    ifstream read(fileName);
    if (!read) return; // File doesn't exist yet
    
    data.clear();
    string line;
    while (getline(read, line)) {
        if (!line.empty()) {
            try {
                data.push_back(T::fromFileString(line));
            } catch (...) {
                cout << "Error parsing line in " << fileName << endl;
            }
        }
    }
    read.close();
}

void loadAllData(vector<Vendor> &vendorList,vector<Organizer> &organizerList,vector<Admin> &adminList) {
    loadUserFromFile(adminList, "admins.txt");
    loadUserFromFile(organizerList, "organizers.txt");
    loadUserFromFile(vendorList, "vendors.txt");
}

bool login(vector<Vendor> &vendorList,vector<Organizer> &organizerList,vector<Admin> &adminList, CurrentUser &currentUser){
    clearScreen();
    cout << "=== LOGIN ===" << endl;
    
    string email, password;
    cout << "Enter email: ";
    getline(cin, email);
    
    cout << "Enter password: ";
    getline(cin, password);
    
    // Check admin
    for (size_t i = 0; i < adminList.size(); i++) {
        if (adminList[i].baseInfo.email == email && adminList[i].baseInfo.password == password) {
            currentUser.type = ADMIN;
            currentUser.userIndex = i;
            currentUser.userId = adminList[i].adminId;
            currentUser.userName = adminList[i].baseInfo.name;
            cout << "Welcome Admin " << currentUser.userName << "!" << endl;
            cin.get();
            return true;
        }
    }
    
    // Check organizer
    for (size_t i = 0; i < organizerList.size(); i++) {
        if (organizerList[i].baseInfo.email == email && organizerList[i].baseInfo.password == password) {
            currentUser.type = ORGANIZER;
            currentUser.userIndex = i;
            currentUser.userId = organizerList[i].organizerId;
            currentUser.userName = organizerList[i].baseInfo.name;
            cout << "Welcome " << currentUser.userName << "!" << endl;
            cin.get();
            return true;
        }
    }
    
    // Check vendor
    for (size_t i = 0; i < vendorList.size(); i++) {
        if (vendorList[i].baseInfo.email == email && vendorList[i].baseInfo.password == password) {
            currentUser.type = VENDOR;
            currentUser.userIndex = i;
            currentUser.userId = vendorList[i].vendorId;
            currentUser.userName = vendorList[i].baseInfo.name;
            cout << "Welcome " << currentUser.userName << "!" << endl;
            cin.get();
            return true;
        }
    }
    
    cout << "Invalid email or password!" << endl;
    cin.get();
    return false;
}

void logout(CurrentUser &currentUser) {
    currentUser.type = NONE;
    currentUser.userIndex = -1;
    currentUser.userId = "";
    currentUser.userName = "";
}

void addService(CurrentUser &currentUser, vector<Vendor> &vendorList,vector<Organizer> &organizerList,vector<Admin> &adminList) {
    if (currentUser.type != VENDOR) return;
    
    Service newService;
    cout << "=== ADD NEW SERVICE ===" << endl;
    
    cout << "Service name: ";
    getline(cin, newService.serviceName);
    
    cout << "Description: ";
    getline(cin, newService.description);
    
    cout << "Type (e.g., catering, photography, decoration): ";
    getline(cin, newService.type);
    
    cout << "Price: $";
    cin >> newService.price;
    
    cout << "Quantity available: ";
    cin >> newService.quantity;
    cin.ignore();
    
    newService.available = true;
    
    // Add to current vendor's services
    vendorList[currentUser.userIndex].serviceHasProvide.push_back(newService);
    vendorList[currentUser.userIndex].totalServicesProvided++;
    
    saveUserIntoFile(vendorList, "vendors.txt");
    cout << "Service added successfully!" << endl;
    cin.get();
}

void saveAllData(vector<Vendor> &vendorList,vector<Organizer> &organizerList,vector<Admin> &adminList) {
    saveUserIntoFile(adminList, "admins.txt");
    saveUserIntoFile(organizerList, "organizers.txt");
    saveUserIntoFile(vendorList, "vendors.txt");
}

void displayUserProfile(CurrentUser &currentUser, vector<Vendor> &vendorList,vector<Organizer> &organizerList,vector<Admin> &adminList) {
    clearScreen();
    cout << "=== MY PROFILE ===" << endl;
    
    if(currentUser.type == NONE) {
        cout << "No user logged in!" << endl;
        pauseScreen();
        return;
    }
    
    switch(currentUser.type) {
        case ADMIN: {
            Admin& admin = adminList[currentUser.userIndex];
            cout << "User Type: Administrator" << endl;
            cout << "Admin ID: " << admin.adminId << endl;
            cout << "Name: " << admin.baseInfo.name << endl;
            cout << "Email: " << admin.baseInfo.email << endl;
            cout << "Phone: " << admin.baseInfo.phoneNum << endl;
            break;
        }
        case ORGANIZER: {
            Organizer& org = organizerList[currentUser.userIndex];
            cout << "User Type: Wedding Organizer" << endl;
            cout << "Organizer ID: " << org.organizerId << endl;
            cout << "Name: " << org.baseInfo.name << endl;
            cout << "Email: " << org.baseInfo.email << endl;
            cout << "Phone: " << org.baseInfo.phoneNum << endl;
            cout << "Groom: " << org.groomName << endl;
            cout << "Bride: " << org.brideName << endl;
            cout << "Wedding Date: " << (org.weddingDate.empty() ? "Not set" : org.weddingDate) << endl;
            cout << "Venue: " << (org.weddingVenue.empty() ? "Not set" : org.weddingVenue) << endl;
            cout << "Theme: " << (org.weddingTheme.empty() ? "Not set" : org.weddingTheme) << endl;
            cout << "Budget: RM" << fixed << setprecision(2) << org.budget << endl;
            cout << "Wedding Stage: " << org.weddingStage << endl;
            cout << "Booked Services: " << org.bookedServices.size() << endl;
            break;
        }
        case VENDOR: {
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
            cout << "Products Provided: " << vendor.productHasProvide.size() << endl;
            break;
        }
    }
    pauseScreen();
}

void listAllUsers(vector<Vendor> &vendorList,vector<Organizer> &organizerList,vector<Admin> &adminList) {
    clearScreen();
    cout << "=== ALL USERS LIST ===" << endl;
    
    cout << "\n--- ADMINISTRATORS ---" << endl;
    for(auto& admin : adminList) {
        cout << "Admin ID: " << admin.adminId << " | Name: " << admin.baseInfo.name 
             << " | Email: " << admin.baseInfo.email << endl;
    }
    
    cout << "\n--- ORGANIZERS ---" << endl;
    for(const auto& org : organizerList) {
        cout << "Organizer ID: " << org.organizerId << " | Name: " << org.baseInfo.name 
             << " | Email: " << org.baseInfo.email 
             << " | Wedding: " << org.groomName << " & " << org.brideName << endl;
    }
    
    cout << "\n--- VENDORS ---" << endl;
    for(const auto& vendor : vendorList) {
        cout << "Vendor ID: " << vendor.vendorId << " | Name: " << vendor.baseInfo.name 
             << " | Email: " << vendor.baseInfo.email 
             << " | Company: " << vendor.companyName << endl;
    }
    
    pauseScreen();
}


//Event
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





