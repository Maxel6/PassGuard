#include "main.h"


void PasswordManager::SavePasswordsToJson()
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


void PasswordManager::LoadPasswordsFromJson()
{
    ifstream inputFile("passwords.json");
    if (!inputFile.is_open())
    {
        cout << "No saved passwords found." << endl;
        return; // Quitte la fonction si le fichier n'existe pas
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


