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
    int quantity;
    double price;
    bool available;
    string toFileString() const {
        return productName + "|||" + description + "|||" + type + "|||" + 
               to_string(quantity) + "|||" + to_string(price) + "|||" + (available ? "1" : "0");
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
        getline(ss, temp); p.available = (temp == "1");

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
    int totalProductProvided;
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

        return baseStr + "|||" + vendorStr + "|||" + to_string(totalServicesProvided) + "|||" + to_string(totalProductProvided) +
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
    
    string toFileString() {
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

    static Organizer fromFileString(const string &str) {
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
    return true;
}

template <typename T>
void saveUserIntoFile(vector<T> data,  string fileName){
    ofstream write(fileName);

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

void addService(CurrentUser &currentUser, vector<Vendor> &vendorList) {
    if (currentUser.type != VENDOR) return;
    
    Service newService;
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
    
    saveUserIntoFile(vendorList, "vendors.txt");
    cout << "Service added successfully!" << endl;
    cin.get();
    pauseScreen();
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
    for(Admin &admin : adminList) {
        cout << "Admin ID: " << admin.adminId << " | Name: " << admin.baseInfo.name 
             << " | Email: " << admin.baseInfo.email << endl;
    }
    
    cout << "\n--- ORGANIZERS ---" << endl;
    for(Organizer &org : organizerList) {
        cout << "Organizer ID: " << org.organizerId << " | Name: " << org.baseInfo.name 
             << " | Email: " << org.baseInfo.email 
             << " | Wedding: " << org.groomName << " & " << org.brideName << endl;
    }
    
    cout << "\n--- VENDORS ---" << endl;
    for(Vendor &vendor : vendorList) {
        cout << "Vendor ID: " << vendor.vendorId << " | Name: " << vendor.baseInfo.name 
             << " | Email: " << vendor.baseInfo.email 
             << " | Company: " << vendor.companyName << endl;
    }
    
    pauseScreen();
}

void userRegister(vector<Vendor> &vendorList,vector<Organizer> &organizerList,vector<Admin> &adminList){
    int choice;
    clearScreen();
    cout << "=== USER REGISTER ===" << endl;
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
        }
        break;
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
    
    saveAllData(vendorList,organizerList,adminList);
}

void addProduct(CurrentUser &currentUser, vector<Vendor> &vendorList) {
    if (currentUser.type != VENDOR) return;
    
    Product newProduct;
    cout << "=== ADD NEW PRODUCT ===" << endl;
    
    cout << "Product name: ";
    getline(cin, newProduct.productName);
    
    cout << "Description: ";
    getline(cin, newProduct.description);
    
    cout << "Type (e.g., catering, photography, decoration): ";
    getline(cin, newProduct.type);
    
    cout << "Price: RM";
    cin >> newProduct.price;
    
    cout << "Quantity available: ";
    cin >> newProduct.quantity;
    cin.ignore();
    
    newProduct.available = true;
    
    // Add to current vendor's services
    vendorList[currentUser.userIndex].productHasProvide.push_back(newProduct);
    vendorList[currentUser.userIndex].totalProductProvided++;
    
    saveUserIntoFile(vendorList, "vendors.txt");
    cout << "Product added successfully!" << endl;
    cin.get();
    pauseScreen();
} 

void updateBaseInfo(BaseInfo &baseInfo) {
    int choice;
    cout << "\n=== UPDATE BASE INFORMATION ===" << endl;
    cout << "Current Information:" << endl;
    cout << "1. Name" << endl;
    cout << "2. Email" << endl;
    cout << "3. Phone"<< endl;
    cout << "4. Password" << endl;
    cout << "5. Update All Base Info" << endl;
    cout << "0. Back to main menu" << endl;
    cout << "Choose what to update: ";
    cin >> choice;
    cin.ignore();
    
    switch(choice) {
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

void updateAdminInfo(Admin &admin) {
    int choice;
    cout << "\n=== UPDATE ADMIN INFORMATION ===" << endl;
    cout << "Current Information:" << endl;
    cout << "Admin ID: " << admin.adminId << " (Cannot be changed)" << endl;
    cout << "1. Update Base Information (Name, Email, Phone, Password)" << endl;
    cout << "0. Back to main menu" << endl;
    cout << "Choose what to update: ";
    cin >> choice;
    cin.ignore();
    
    switch(choice) {
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

void updateOrganizerInfo(Organizer &organizer) {
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
    
    switch(choice) {
        case 1:
            updateBaseInfo(organizer.baseInfo);
            break;
        case 2:
            cout << "Enter new groom name: ";
            getline(cin, organizer.groomName);
            cout << "Groom name updated successfully!" << endl;
            pauseScreen();
            break;
        case 3:
            cout << "Enter new bride name: ";
            getline(cin, organizer.brideName);
            cout << "Bride name updated successfully!" << endl;
            pauseScreen();
            break;
        case 4:
            cout << "Enter new wedding date (leave empty to clear): ";
            getline(cin, organizer.weddingDate);
            cout << "Wedding date updated successfully!" << endl;
            pauseScreen();
            break;
        case 5:
            cout << "Enter new wedding venue (leave empty to clear): ";
            getline(cin, organizer.weddingVenue);
            cout << "Wedding venue updated successfully!" << endl;
            pauseScreen();
            break;
        case 6:
            cout << "Enter new budget (RM): ";
            cin >> organizer.budget;
            cin.ignore();
            cout << "Budget updated successfully!" << endl;
            pauseScreen();
            break;
        case 7:
            cout << "Enter new wedding theme (leave empty to clear): ";
            getline(cin, organizer.weddingTheme);
            cout << "Wedding theme updated successfully!" << endl;
            pauseScreen();
            break;
        case 8:
            cout << "Enter new wedding stage (e.g., planning, booked, completed): ";
            getline(cin, organizer.weddingStage);
            cout << "Wedding stage updated successfully!" << endl;
            pauseScreen();
            break;
        case 9:
            updateBaseInfo(organizer.baseInfo);
            cout << "Enter new groom name: ";
            getline(cin, organizer.groomName);
            cout << "Enter new bride name: ";
            getline(cin, organizer.brideName);
            cout << "Wedding date (leave empty if not set): ";
            getline(cin, organizer.weddingDate);
            cout << "Wedding venue (leave empty if not set): ";
            getline(cin, organizer.weddingVenue);
            cout << "Wedding theme (leave empty if not set): ";
            getline(cin, organizer.weddingTheme);
            cout << "Budget (RM): ";
            cin >> organizer.budget;
            cin.ignore();
            cout << "Wedding stage: ";
            getline(cin, organizer.weddingStage);
            cout << "All organizer information updated successfully!" << endl;
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

void updateVendorInfo(Vendor &vendor) {
    int choice;
    
    cout << "\n=== UPDATE VENDOR INFORMATION ===" << endl;
    cout << "Current Information:" << endl;
    cout << "Vendor ID: " << vendor.vendorId << " (Cannot be changed)" << endl;
    cout << "1. Update Base Information (Name, Email, Phone, Password)" << endl;
    cout << "2. Company Name" << endl;
    cout << "3. Company Contact"  << endl;
    cout << "4. Vendor Type" << endl;
    cout << "5. Update All Vendor Info" << endl;
    cout << "0. Back to main menu" << endl;
    cout << "Choose what to update: ";
    cin >> choice;
    cin.ignore();
    
    switch(choice) {
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

void updateUserProfile(CurrentUser &currentUser, vector<Vendor> &vendorList, vector<Organizer> &organizerList, vector<Admin> &adminList) {
    clearScreen();
    
    if(currentUser.type == NONE) {
        cout << "No user logged in!" << endl;
        pauseScreen();
        return;
    }
    
    cout << "=== UPDATE PROFILE ===" << endl;
    cout << "Logged in as: " << currentUser.userName << " (" << currentUser.userId << ")" << endl;
    
    bool updated = false;
    
    switch(currentUser.type) {
        case ADMIN: {
            updateAdminInfo(adminList[currentUser.userIndex]);
            updated = true;
            break;
        }
        case ORGANIZER: {
            updateOrganizerInfo(organizerList[currentUser.userIndex]);
            updated = true;
            break;
        }
        case VENDOR: {
            updateVendorInfo(vendorList[currentUser.userIndex]);
            updated = true;
            break;
        }
    }
    
    if(updated) {
        // Save the updated data to files
        saveAllData(vendorList, organizerList, adminList);
        cout << "\nProfile changes saved to file!" << endl;
    }
}

void updateService(CurrentUser &currentUser, vector<Vendor> &vendorList) {
    if(currentUser.type != VENDOR) {
        cout << "Only vendors can update services!" << endl;
        pauseScreen();
        return;
    }
    
    clearScreen();
    cout << "=== UPDATE SERVICES ===" << endl;
    
    Vendor &vendor = vendorList[currentUser.userIndex];
    
    if(vendor.serviceHasProvide.empty()) {
        cout << "No services available to update!" << endl;
        pauseScreen();
        return;
    }
    
    cout << "\nYour Services:" << endl;
    int index = 1;
    for(const auto &service : vendor.serviceHasProvide) {
        cout << index << ". " << service.serviceName 
             << " - RM" << service.price 
             << " (Qty: " << service.quantity << ")" << endl;
        index++;
    }
    
    int serviceIndex;
    cout << "Select service to update (1-" << vendor.serviceHasProvide.size() << "): ";
    cin >> serviceIndex;
    cin.ignore();
    
    if(serviceIndex < 1 || serviceIndex > (int)vendor.serviceHasProvide.size()) {
        cout << "Invalid selection!" << endl;
        pauseScreen();
        return;
    }
    
    Service &service = vendor.serviceHasProvide[serviceIndex-1];
    
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
    
    switch(updateChoice) {
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
    
    saveUserIntoFile(vendorList, "vendors.txt");
    pauseScreen();
}

void updateProduct(CurrentUser &currentUser, vector<Vendor> &vendorList) {
    if(currentUser.type != VENDOR) {
        cout << "Only vendors can update products!" << endl;
        pauseScreen();
        return;
    }
    
    clearScreen();
    cout << "=== UPDATE PRODUCTS ===" << endl;
    
    Vendor &vendor = vendorList[currentUser.userIndex];
    
    if(vendor.productHasProvide.empty()) {
        cout << "No products available to update!" << endl;
        pauseScreen();
        return;
    }
    
    cout << "\nYour Products:" << endl;
    int index = 1;
    for(const auto &product : vendor.productHasProvide) {
        cout << index << ". " << product.productName 
             << " - RM" << product.price 
             << " (Qty: " << product.quantity << ")" << endl;
        index++;
    }
    
    int productIndex;
    cout << "Select product to update (1-" << vendor.productHasProvide.size() << "): ";
    cin >> productIndex;
    cin.ignore();
    
    if(productIndex < 1 || productIndex > (int)vendor.productHasProvide.size()) {
        cout << "Invalid selection!" << endl;
        pauseScreen();
        return;
    }
    
    Product &product = vendor.productHasProvide[productIndex-1];
    
    int updateChoice;
    cout << "\nCurrent Product Information:" << endl;
    cout << "1. Product Name: " << product.productName << endl;
    cout << "2. Description: " << product.description << endl;
    cout << "3. Type: " << product.type << endl;
    cout << "4. Price: RM" << product.price << endl;
    cout << "5. Quantity: " << product.quantity << endl;
    cout << "6. Availability: " << (product.available ? "Available" : "Not Available") << endl;
    cout << "7. Update All Product Info" << endl;
    cout << "0. Back" << endl;
    cout << "Choose what to update: ";
    cin >> updateChoice;
    cin.ignore();
    
    switch(updateChoice) {
        case 1:
            cout << "Enter new product name: ";
            getline(cin, product.productName);
            cout << "Product name updated successfully!" << endl;
            break;
        case 2:
            cout << "Enter new description: ";
            getline(cin, product.description);
            cout << "Description updated successfully!" << endl;
            break;
        case 3:
            cout << "Enter new type: ";
            getline(cin, product.type);
            cout << "Type updated successfully!" << endl;
            break;
        case 4:
            cout << "Enter new price: RM";
            cin >> product.price;
            cin.ignore();
            cout << "Price updated successfully!" << endl;
            break;
        case 5:
            cout << "Enter new quantity: ";
            cin >> product.quantity;
            cin.ignore();
            cout << "Quantity updated successfully!" << endl;
            break;
        case 6:
            product.available = !product.available;
            cout << "Availability changed to: " << (product.available ? "Available" : "Not Available") << endl;
            break;
        case 7:
            cout << "Enter new product name: ";
            getline(cin, product.productName);
            cout << "Enter new description: ";
            getline(cin, product.description);
            cout << "Enter new type: ";
            getline(cin, product.type);
            cout << "Enter new price: RM";
            cin >> product.price;
            cout << "Enter new quantity: ";
            cin >> product.quantity;
            cin.ignore();
            char availChoice;
            cout << "Is product available? (y/n): ";
            cin >> availChoice;
            product.available = (availChoice == 'y' || availChoice == 'Y');
            cout << "All product information updated successfully!" << endl;
            break;
        case 0:
            return;
        default:
            cout << "Invalid choice!" << endl;
            pauseScreen();
            return;
    }
    
    saveUserIntoFile(vendorList, "vendors.txt");
    pauseScreen();
}

void updateServiceOrProduct(CurrentUser &currentUser, vector<Vendor> &vendorList) {
    if(currentUser.type != VENDOR) {
        cout << "Only vendors can update services/products!" << endl;
        pauseScreen();
        return;
    }
    
    clearScreen();
    cout << "=== UPDATE SERVICES/PRODUCTS ===" << endl;
    
    int choice;
    cout << "1. Update Services" << endl;
    cout << "2. Update Products" << endl;
    cout << "0. Back" << endl;
    cout << "Choose: ";
    cin >> choice;
    cin.ignore();
    
    switch(choice) {
        case 1:
            updateService(currentUser, vendorList);
            break;
        case 2:
            updateProduct(currentUser, vendorList);
            break;
        case 0:
            return;
        default:
            cout << "Invalid choice!" << endl;
            pauseScreen();
            break;
    }
}

void displayAllServices(vector<Vendor> &vendorList) {
    bool hasServices = false;

    clearScreen();
    cout << "=== ALL AVAILABLE SERVICES ===" << endl;
    
    for(auto &vendor : vendorList) {
        if(!vendor.serviceHasProvide.empty()) {
            int serviceNum = 1;
            hasServices = true;
            cout << "\n--- Services by " << vendor.baseInfo.name << " (" << vendor.companyName << ") ---" << endl;
            cout << "Vendor ID: " << vendor.vendorId << " | Contact: " << vendor.baseInfo.phoneNum << endl;
            

            for(auto &service : vendor.serviceHasProvide) {
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

    if(!hasServices) {
        cout << "No services available at the moment." << endl;
    }
    
    pauseScreen();
}

void displayAllProducts(vector<Vendor> &vendorList) {
    bool hasProducts = false;
    clearScreen();
    cout << "=== ALL AVAILABLE PRODUCTS ===" << endl;
    
    for(auto &vendor : vendorList) {
        if(!vendor.productHasProvide.empty()) {
            int productNum = 1;
            hasProducts = true;
            cout << "\n--- Products by " << vendor.baseInfo.name << " (" << vendor.companyName << ") ---" << endl;
            cout << "Vendor ID: " << vendor.vendorId << " | Contact: " << vendor.baseInfo.phoneNum << endl;
            
            for(const auto &product : vendor.productHasProvide) {
                cout << productNum << ". Product: " << product.productName << endl;
                cout << "   Description: " << product.description << endl;
                cout << "   Type: " << product.type << endl;
                cout << "   Price: RM" << fixed << setprecision(2) << product.price << endl;
                cout << "   Quantity Available: " << product.quantity << endl;
                cout << "   Status: " << (product.available ? "Available" : "Not Available") << endl;
                cout << "   " << string(50, '-') << endl;
                productNum++;
            }
        }
    }
    
    if(!hasProducts) {
        cout << "No products available at the moment." << endl;
    }
    
    pauseScreen();
}

void displayServicesByVendor(vector<Vendor> &vendorList) {
    int vendorNum = 1;
    int choice;

    clearScreen();
    cout << "=== SERVICES BY VENDOR ===" << endl;
    
    if(vendorList.empty()) {
        cout << "No vendors registered." << endl;
        pauseScreen();
        return;
    }
    
    // Display all vendors
    cout << "Available Vendors:" << endl;
    for(auto &vendor : vendorList) {
        cout << vendorNum << ". " << vendor.baseInfo.name << " (" << vendor.companyName << ")" << endl;
        cout << "   Vendor ID: " << vendor.vendorId << " | Services: " << vendor.serviceHasProvide.size() << endl;
        vendorNum++;
    }
    
    cout << "\nSelect vendor to view services (1-" << vendorList.size() << "): ";
    cin >> choice;
    cin.ignore();
    
    if(choice < 1 || choice > (int)vendorList.size()) {
        cout << "Invalid selection!" << endl;
        pauseScreen();
        return;
    }
    
    const Vendor &selectedVendor = vendorList[choice-1];
    
    clearScreen();
    cout << "=== SERVICES BY " << selectedVendor.baseInfo.name << " ===" << endl;
    cout << "Company: " << selectedVendor.companyName << endl;
    cout << "Contact: " << selectedVendor.baseInfo.phoneNum << " | Email: " << selectedVendor.baseInfo.email << endl;
    cout << string(60, '=') << endl;
    
    if(selectedVendor.serviceHasProvide.empty()) {
        cout << "This vendor has no services available." << endl;
    } else {
        int serviceNum = 1;
        for(const auto &service : selectedVendor.serviceHasProvide) {
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

void displayProductsByVendor(vector<Vendor> &vendorList) {
    int vendorNum = 1;
    int choice;

    clearScreen();
    cout << "=== PRODUCTS BY VENDOR ===" << endl;
    
    if(vendorList.empty()) {
        cout << "No vendors registered." << endl;
        pauseScreen();
        return;
    }
    
    // Display all vendors
    cout << "Available Vendors:" << endl;
    for(const auto &vendor : vendorList) {
        cout << vendorNum << ". " << vendor.baseInfo.name << " (" << vendor.companyName << ")" << endl;
        cout << "   Vendor ID: " << vendor.vendorId << " | Products: " << vendor.productHasProvide.size() << endl;
        vendorNum++;
    }

    cout << "\nSelect vendor to view products (1-" << vendorList.size() << "): ";
    cin >> choice;
    cin.ignore();
    
    if(choice < 1 || choice > (int)vendorList.size()) {
        cout << "Invalid selection!" << endl;
        pauseScreen();
        return;
    }
    
    const Vendor &selectedVendor = vendorList[choice-1];
    
    clearScreen();
    cout << "=== PRODUCTS BY " << selectedVendor.baseInfo.name << " ===" << endl;
    cout << "Company: " << selectedVendor.companyName << endl;
    cout << "Contact: " << selectedVendor.baseInfo.phoneNum << " | Email: " << selectedVendor.baseInfo.email << endl;
    cout << string(60, '=') << endl;
    
    if(selectedVendor.productHasProvide.empty()) {
        cout << "This vendor has no products available." << endl;
    } else {
        int productNum = 1;
        for(const auto &product : selectedVendor.productHasProvide) {
            cout << productNum << ". " << product.productName << endl;
            cout << "   Description: " << product.description << endl;
            cout << "   Type: " << product.type << endl;
            cout << "   Price: RM" << fixed << setprecision(2) << product.price << endl;
            cout << "   Quantity: " << product.quantity << endl;
            cout << "   Status: " << (product.available ? "Available" : "Not Available") << endl;
            cout << "   " << string(50, '-') << endl;
            productNum++;
        }
    }
    
    pauseScreen();
}

void displayServicesByType(vector<Vendor> &vendorList) {
    clearScreen();
    cout << "=== SERVICES BY TYPE ===" << endl;
    
    // First, collect all unique service types
    vector<string> serviceTypes;
    for(const auto &vendor : vendorList) {
        for(const auto &service : vendor.serviceHasProvide) {
            bool typeExists = false;
            for(const auto &existingType : serviceTypes) {
                if(existingType == service.type) {
                    typeExists = true;
                    break;
                }
            }
            if(!typeExists && !service.type.empty()) {
                serviceTypes.push_back(service.type);
            }
        }
    }
    
    if(serviceTypes.empty()) {
        cout << "No service types available." << endl;
        pauseScreen();
        return;
    }
    
    // Display available types
    cout << "Available Service Types:" << endl;
    int typeNum = 1;
    for(const auto &type : serviceTypes) {
        cout << typeNum << ". " << type << endl;
        typeNum++;
    }
    
    int choice;
    cout << "\nSelect service type (1-" << serviceTypes.size() << "): ";
    cin >> choice;
    cin.ignore();
    
    if(choice < 1 || choice > (int)serviceTypes.size()) {
        cout << "Invalid selection!" << endl;
        pauseScreen();
        return;
    }
    
    string selectedType = serviceTypes[choice-1];
    
    clearScreen();
    cout << "=== " << selectedType << " SERVICES ===" << endl;
    cout << string(50, '=') << endl;
    
    bool found = false;
    for(const auto &vendor : vendorList) {
        for(const auto &service : vendor.serviceHasProvide) {
            if(service.type == selectedType) {
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
    
    if(!found) {
        cout << "No services found for this type." << endl;
    }
    
    pauseScreen();
}

// Function to read organizer's booked services
void displayBookedServices(CurrentUser &currentUser, vector<Organizer> &organizerList, vector<Vendor> &vendorList) {
    if(currentUser.type != ORGANIZER) {
        cout << "Only organizers can view booked services!" << endl;
        pauseScreen();
        return;
    }
    
    clearScreen();
    cout << "=== MY BOOKED SERVICES ===" << endl;
    
    const Organizer &organizer = organizerList[currentUser.userIndex];
    
    cout << "Wedding: " << organizer.groomName << " & " << organizer.brideName << endl;
    cout << "Date: " << (organizer.weddingDate.empty() ? "Not set" : organizer.weddingDate) << endl;
    cout << "Venue: " << (organizer.weddingVenue.empty() ? "Not set" : organizer.weddingVenue) << endl;
    cout << "Budget: RM" << fixed << setprecision(2) << organizer.budget << endl;
    cout << string(60, '=') << endl;
    
    if(organizer.bookedServices.empty()) {
        cout << "No services booked yet." << endl;
    } else {
        cout << "Total Booked Services: " << organizer.bookedServices.size() << endl;
        cout << string(50, '-') << endl;
        
        double totalCost = 0.0;
        int serviceCount = 1;
        
        for(auto &serviceId : organizer.bookedServices) {
            // Find the service in vendors list
            bool serviceFound = false;
            for(auto &vendor : vendorList) {
                int currentServiceId = 1;
                for(auto &service : vendor.serviceHasProvide) {
                    if(currentServiceId == serviceId) {
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
                if(serviceFound) break;
            }
            
            if(!serviceFound) {
                cout << serviceCount << ". Service ID " << serviceId << " (Service not found)" << endl;
            }
            
            cout << "   " << string(45, '-') << endl;
            serviceCount++;
        }
        
        cout << "\nTotal Cost: RM" << fixed << setprecision(2) << totalCost << endl;
        cout << "Remaining Budget: RM" << fixed << setprecision(2) << (organizer.budget - totalCost) << endl;
    }
    
    pauseScreen();
}

// User Management
int main (){
    vector<Admin> adminList;
    vector<Organizer> organizerList;
    vector<Vendor> vendorList;

    //Variable 
    CurrentUser currentUser;

    userRegister(vendorList,organizerList,adminList);
}

