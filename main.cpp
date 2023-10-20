#include <iostream>
#include <vector>
#include <fstream>
#include "nlohmann/json.hpp"

using namespace std;
using json = nlohmann::json;

class Password
{
public:
    Password(const string &url, const string &username, const string &password)
        : url_(url), username_(username), password_(password) {}

    const string &getUrl() const { return url_; }
    const string &getUsername() const { return username_; }
    const string &getPassword() const { return password_; }

private:
    string url_;
    string username_;
    string password_;
};

vector<Password> passwordList;

void SavePasswordsToJson()
{
    json data;

    for (const Password &password : passwordList)
    {
        json entry;
        entry["url"] = password.getUrl();
        entry["username"] = password.getUsername();
        entry["password"] = password.getPassword();
        data.push_back(entry);
    }

    ofstream outputFile("passwords.json");
    outputFile << data.dump(4); // Le paramètre 4 permet une mise en forme lisible.
    outputFile.close();
}

void LoadPasswordsFromJson()
{
    ifstream inputFile("passwords.json");
    if (!inputFile.is_open())
    {
        cout << "No saved passwords found." << endl;
        return;
    }

    json data;
    inputFile >> data;

    for (const json &entry : data)
    {
        string url = entry["url"];
        string username = entry["username"];
        string password = entry["password"];
        Password newPassword(url, username, password);
        passwordList.push_back(newPassword);
    }

    cout << "Passwords loaded from passwords.json." << endl;
}

int AddPassword()
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
        AddPassword();
    }
    else
    {
        SavePasswordsToJson(); // Sauvegarde les mots de passe dans un fichier JSON
        return 0;
    }
    return 1;
}

int menu()
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
                AddPassword();
                break;
            case 2:
                cout << "Option 2: View passwords\n";
                // Insert code to view passwords here
                break;

            case 3:
                cout << "Option 3: Delete a password\n";
                // Insert code to delete a password here$
                break;

            case 4:
                cout << "Option 4: Modify a password\n";
                // Insert code to modify a password here
                break;

            case 5:
                cout << "Option 5: Help\n";
                // Insert code to display help here
                break;

            case 0:
                cout << "Goodbye!\n";

                return 1;

            default:
                cout << "Invalid option. Please choose a valid option.\n";
                break;
            }
        }
        else{
            cout << "Invalid option. Please choose a valid option.\n";
        }
        return 0;
    }
}

int main()
{
    LoadPasswordsFromJson();
    menu();

    return 0;
}
