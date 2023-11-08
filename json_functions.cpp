#include "main.h"

void PasswordManager::SavePasswordsToJson()
{
    json data;

    for (const Password &password : passwordList)
    {
        json entry;
        entry["url"] = password.getUrl();
        entry["username"] = password.getUsername();
        std::vector<unsigned char> passwordData(password.getPassword().begin(), password.getPassword().end());
        std::string base64Password = base64_encode(passwordData);

        entry["password"] = base64Password;
        entry["ciphertext_len"] = password.getCipherLen();
        //TODO: save iv and key to json
        data.push_back(entry);
    }

    ofstream outputFile("passwords.json");
    outputFile << data.dump(4);
    outputFile.close();
}

void PasswordManager::LoadPasswordsFromJson()
{
    ifstream inputFile("passwords.json");
    if (!inputFile.is_open())
    {
        cout << "No saved passwords found." << endl;
        return;
    }

    json data;
    inputFile >> data;

    if (data.empty())
    {
        cout << "No saved passwords found in the JSON file." << endl;
        return;
    }

    for (const json &entry : data)
    {
        string url = entry["url"];
        string username = entry["username"];
        std::string base64Password = entry["password"];
        int ciphertext_len = entry["ciphertext_len"];
        string iv = entry["iv"];
        string key = entry["key"];

        //TODO: load iv and key to json
        std::vector<unsigned char> passwordData = base64_decode(base64Password);

        // Convertir les données binaires en une chaîne de caractères
        std::string password(passwordData.begin(), passwordData.end());
        Password newPassword(url, username, password, ciphertext_len);
        passwordList.push_back(newPassword);
    }

    cout << "Passwords loaded from passwords.json" << endl;
}
