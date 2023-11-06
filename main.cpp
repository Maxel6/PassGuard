#include <iostream>
#include <vector>
#include <fstream>
#include "nlohmann/json.hpp"
#include "main.h"

unsigned char key[32];
unsigned char iv[16];

PasswordManager manager;

int PasswordManager::menu()
{

    int choice;
    while (true)
    {
        cout << "Menu:"
             << "\n1. Add a password"
             << "\n2. View passwords"
             << "\n3. Delete a password"
             << "\n4. Modify a password"
             << "\n5. Help"
             << "\n0. Quit"
             << "\nChoose an option: ";


        if (cin >> choice && choice >= 0 && choice <= 5)
        {

            switch (choice)
            {
            case 1:
                cout << "Option 1: Add a password\n";
                manager.AddPassword();
                break;
            case 2:
                cout << "Option 2: View passwords\n";
                manager.ViewPasswords();
                break;

            case 3:
                cout << "Option 3: Delete a password\n";
                manager.DeletePassword();
                break;

            case 4:
                cout << "Option 4: Modify a password\n";
                manager.ChangePassword();
                break;

            case 5:
                cout << "Option 5: Help\n";
                // Insert code to display help here
                break;

            case 0:
                cout << "Goodbye!\n";

                return 0;

            default:
                cout << "Invalid option. Please choose a valid option.\n";
                break;
            }
        }
        else
        {
            cout << "Invalid option. Please choose a valid option.\n";
        }
        return 1;
    }
}




int main()
{
    OpenSSL_add_all_algorithms();
    ERR_load_crypto_strings();

    RAND_bytes(key, sizeof(key));
    RAND_bytes(iv, sizeof(iv));
    
    manager.LoadPasswordsFromJson();
    while(manager.menu())
    {}
    manager.SavePasswordsToJson();

    return 0;
}
