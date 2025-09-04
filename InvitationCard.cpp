#include <iostream>
#include "UserModule.h"
using namespace std;

void generateClassicTemplate() {
       cout << "\n";
       cout << "╔════════════════════════════════════════════════════════════╗\n";
       cout << "║                                                            ║\n";
       cout << "║              ♥ WEDDING INVITATION ♥                       ║\n";
       cout << "║                                                            ║\n";
       cout << "║    Together with their families                           ║\n";
       cout << "║                                                            ║\n";
       cout << "║              " <<setw(20) << brideName << "                              ║\n";
       cout << "║                        &                                   ║\n";
       cout << "║              " <<setw(20) << groomName << "                              ║\n";
       cout << "║                                                            ║\n";
       cout << "║           request the pleasure of your company            ║\n";
       cout << "║              at their wedding ceremony                    ║\n";
       cout << "║                                                            ║\n";
       cout << "║    Date: " <<setw(25) << weddingDate << "                       ║\n";
       cout << "║    Time: " <<setw(25) << weddingTime << "                       ║\n";
       cout << "║    Venue: " <<setw(24) << venue << "                       ║\n";
        if (!additionalInfo.empty()) {
           cout << "║                                                            ║\n";
           cout << "║    " <<setw(30) << additionalInfo << "                          ║\n";
        }
       cout << "║                                                            ║\n";
       cout << "║              RSVP: Please confirm attendance              ║\n";
       cout << "║                                                            ║\n";
       cout << "╚════════════════════════════════════════════════════════════╝\n";
    }