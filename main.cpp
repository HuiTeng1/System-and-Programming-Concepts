#include <iomanip>
#include <iostream>
#include <vector>
using namespace std;

//All method

//All Structure
struct Service{
    string serviceName;
    string description;
    string type; ///not sure -- like for planning , food 
    double price;
    int quantity;
    bool available;
};

struct Product{
    string productName;
    string description;
    string type; // not sure
    string type;
    int quantity;
    double price;
};
struct Vendor{
    string vendorId;
    string name; // use companyName or vendor he own name?
    string contactNum;
    string type; // not sure will use or not
    vector<Service> serviceHasProvide;
    vector<Product> productHasProvide;
};

struct Organization{
    string name;
    string contactNum;
    string email;
    string ic;
    string address
}

int main(){

    
    return 0;
}

void userManagement(){

}

void monitoring(){
    /*1. add the visitor 
    -> Please enter the visitor name: 
    -> show comfirmation message, if not loop back 
    -> 
    */ 
}