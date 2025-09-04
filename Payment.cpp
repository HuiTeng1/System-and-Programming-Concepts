#include "Payment.h"
#include "Header.h" 
#include <iomanip>
#include <ctime>
#include <windows.h>

using namespace std;

// Payment Transaction structure implementations
string PaymentTransaction::toFileString() const {
    return transactionId + "|" + userId + "|" + userName + "|" + weddingId + "|" + to_string(amount) + "|" +
        paymentMethod + "|" + paymentStatus + "|" + transactionDate + "|" + transactionTime;
}

PaymentTransaction PaymentTransaction::fromFileString(string& str) {
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

// Function implementations
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
