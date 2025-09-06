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
    std::string transactionId;
    std::string userId;
    std::string userName;
    std::string weddingId;
    double amount;
    std::string paymentMethod;
    std::string paymentStatus;
    std::string transactionDate;
    std::string transactionTime;

    std::string toFileString() const;
    static PaymentTransaction fromFileString(std::string& str);
};

// Function declarations
void viewPaymentSummary(const CurrentUser& currentUser, const WeddingEvent& selectedEvent, const std::vector<Vendor>& vendorList);
void processPayment(CurrentUser& currentUser, WeddingEvent* selectedEvent, std::vector<PaymentTransaction>& transactions);
void paymentAndReportingMenu(CurrentUser& currentUser, WeddingEvent* selectedEvent, std::vector<WeddingEvent>& events, std::vector<Vendor>& vendorList, std::vector<PaymentTransaction>& transactions);
void viewPaymentHistory(CurrentUser& currentUser);
void savePaymentToFile(const std::vector<PaymentTransaction>& transactions, const std::string& filename);
