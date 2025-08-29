#include <iomanip>
#include <iostream>
#include <vector>
#include <cstdlib>
#include <string>
#include <fstream>
#include <sstream>
using namespace std;

//All method
void userRegister();
void clearScreen();
//All Structure
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

        // 拼接所有字段，用"|||"连接
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
                if (!sItem.empty() && sItem != "#") { // 避免空元素
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


/*Organizer org;
Vendor v;

cout << "Enter Organizer info:\n";
getBaseInfo(org.baseInfo);

cout << "Enter Vendor info:\n";
getBaseInfo(v.baseInfo);
*/

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

int main (){
    vector<Admin> adminList;
    vector<Organizer> organizerList;
    vector<Vendor> vendorList;

    //File
    ofstream write;
    ifstream read;
}
void clearScreen(){
    system("cls");
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

void getVendorInfo(Vendor &vendor){
    // Get base info
    getBaseUserInfo(vendor.baseInfo);

    // Get vendor-specific info
    cout << "Enter vendor ID: ";
    getline(cin, vendor.vendorId);

    cout << "Enter company name: ";
    getline(cin, vendor.companyName);

    cout << "Enter company contact number: ";
    getline(cin, vendor.companyContactNum);

    cout << "Enter vendor type (e.g., 'individual', 'company'): ";
    getline(cin, vendor.type);
};

void getOrganizerInfo(Organizer &organizer){
    string input;

    // Get base info
    getBaseUserInfo(organizer.baseInfo);

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
   
void getAdminInfo(Admin &admin){
    getBaseUserInfo(admin.baseInfo);


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
        getAdminInfo(admin);
        adminList.push_back(admin);
        break;
    }case 2:{
        Organizer organizer;
        getOrganizerInfo(organizer);
        organizerList.push_back(organizer);
        break;
    }case 3:{
        Vendor vendor;
        getVendorInfo(vendor);
        vendorList.push_back(vendor);
        break;
    }default:
    cout << "Invalid choice." << endl;
        break;
    }

}

template <typename T>
void saveUserIntoFile(vector<T> data, string &fileName, ofstream &write){
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

void monitoring(){
    /*1. add the visitor 
    -> Please enter the visitor name: 
    -> show comfirmation message, if not loop back 
    -> 
    */ 
}