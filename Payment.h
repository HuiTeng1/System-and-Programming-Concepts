#pragma once

#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <sstream>

#include "UserModule.h"
#include "EventModule.h"

// Struct definitions
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

    string toFileString() const;
    static PaymentTransaction fromFileString(string& str);
};

// Function declarations
void viewPaymentSummary(const CurrentUser& currentUser, const WeddingEvent& selectedEvent, const vector<Vendor>& vendorList);
void processPayment(CurrentUser& currentUser, WeddingEvent* selectedEvent, vector<PaymentTransaction>& transactions);
void paymentAndReportingMenu(CurrentUser& currentUser, WeddingEvent* selectedEvent, vector<WeddingEvent>& events, vector<Vendor>& vendorList, vector<PaymentTransaction>& transactions);
void viewPaymentHistory(CurrentUser& currentUser);
// void savePaymentToFile(const vector<PaymentTransaction>& transactions, const string& filename);
