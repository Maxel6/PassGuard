#include <iostream>
#include <vector>
#include <fstream>
#include "nlohmann/json.hpp"
#include "main.h"

int PasswordManager::AddPassword()
{
    string url;
    string username;
    string password;

    cout << "Enter URL: ";
    cin >> url;
    cout << "Enter username: ";
    cin >> username;
    cout << "Enter password: ";
    cin >> password;

    Password newPassword(url, username, password);
    passwordList.push_back(newPassword);

    cout << "Password added!\n";
    cout << "Would you like to add another password? (y/n): ";
    char answer;
    cin >> answer;

    if (answer == 'y' || answer == 'Y')
    {
        manager.AddPassword();
    }
    else
    {
        manager.SavePasswordsToJson();
        return 0;
    }
    return 1;
}

void PasswordManager::ViewPasswords()
{
    if (passwordList.empty())
    {
        cout << "No passwords stored." << endl;
    }
    else
    {
        cout << '\n'
             << "Stored passwords:" << '\n'
             << endl;
        for (size_t i = 0; i < passwordList.size(); ++i)
        {
            cout << "Site: " << passwordList[i].getUrl() << endl;
            cout << "Username: " << passwordList[i].getUsername() << endl;
            cout << "Password: " << passwordList[i].getPassword() << endl;
            cout << "-------------------" << endl;
        }
    }
}

void PasswordManager::DeletePassword()
{
    string url;
    cout << "Enter the URL of the site to delete the password: ";
    cin >> url;

    for (size_t i = 0; i < passwordList.size(); ++i)
    {
        if (passwordList[i].getUrl() == url)
        {
            cout << "Deleted password for site: " << url << endl;
            passwordList.erase(passwordList.begin() + i);
            PasswordManager::SavePasswordsToJson(); // Mettre à jour le fichier JSON après la suppression
            return;                                 // Sortir de la boucle dès que le mot de passe est trouvé et supprimé
        }
    }
    cout << "No password found for site: " << url << endl;
}

void PasswordManager::ChangePassword()
{
    string url;
    cout << "Enter the URL of the site to change the password: ";
    cin >> url;

    for (size_t i = 0; i < passwordList.size(); ++i)
    {
        if (passwordList[i].getUrl() == url)
        {
            string oldPassword;
            string newPassword;
            int attempts = 0;

            for (; attempts < 3; ++attempts)
            {
                cout << "Enter the old password: ";
                cin >> oldPassword;

                if (passwordList[i].getPassword() == oldPassword)
                {
                    cout << "Enter the new password: ";
                    cin >> newPassword;

                    passwordList[i].setPassword(newPassword);

                    cout << "Password changed for site: " << url << endl;
                    PasswordManager::SavePasswordsToJson();
                    return;
                }
                else{
                    cout << "Incorrect password!" << endl;
                }
            }
            return;
        }
    }
    cout << "No password found for site: " << url << endl;
}

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

PasswordManager manager;

int main()
{

    manager.LoadPasswordsFromJson();
    while(manager.menu())
    {}
    manager.SavePasswordsToJson();

    return 0;
}
