#include "main.h"

bool is_base64(unsigned char c) {
    return (isalnum(c) || (c == '+') || (c == '/'));
}

// Fonction pour encoder des données binaires en Base64
std::string base64_encode(const std::vector<unsigned char> &data) {
    const std::string base64_chars =
        "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";
    std::string encoded;
    int i = 0;
    int j = 0;
    unsigned char char_array_3[3];
    unsigned char char_array_4[4];

    for (const unsigned char &byte : data) {
        char_array_3[i++] = byte;
        if (i == 3) {
            char_array_4[0] = (char_array_3[0] & 0xfc) >> 2;
            char_array_4[1] = ((char_array_3[0] & 0x03) << 4) + ((char_array_3[1] & 0xf0) >> 4);
            char_array_4[2] = ((char_array_3[1] & 0x0f) << 2) + ((char_array_3[2] & 0xc0) >> 6);
            char_array_4[3] = char_array_3[2] & 0x3f;

            for (i = 0; i < 4; i++) {
                encoded += base64_chars[char_array_4[i]];
            }
            i = 0;
        }
    }

    if (i != 0) {
        for (j = i; j < 3; j++) {
            char_array_3[j] = 0;
        }

        char_array_4[0] = (char_array_3[0] & 0xfc) >> 2;
        char_array_4[1] = ((char_array_3[0] & 0x03) << 4) + ((char_array_3[1] & 0xf0) >> 4);
        char_array_4[2] = ((char_array_3[1] & 0x0f) << 2) + ((char_array_3[2] & 0xc0) >> 6);
        char_array_4[3] = char_array_3[2] & 0x3f;

        for (j = 0; j < i + 1; j++) {
            encoded += base64_chars[char_array_4[j]];
        }

        while (i++ < 3) {
            encoded += '=';
        }
    }

    return encoded;
}

// Fonction pour décoder des données Base64 en données binaires
std::vector<unsigned char> base64_decode(const std::string &encoded_string) {
    const std::string base64_chars =
        "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";
    std::vector<unsigned char> decoded;
    int in_len = encoded_string.size();
    int i = 0;
    int j = 0;
    int in_ = 0;
    unsigned char char_array_4[4];
    unsigned char char_array_3[3];

    while (in_len-- && (encoded_string[in_] != '=') &&
           (is_base64(encoded_string[in_]))) {
        char_array_4[i++] = encoded_string[in_];
        in_++;
        if (i == 4) {
            for (i = 0; i < 4; i++) {
                char_array_4[i] = base64_chars.find(char_array_4[i]);
            }

            char_array_3[0] = (char_array_4[0] << 2) + ((char_array_4[1] & 0x30) >> 4);
            char_array_3[1] = ((char_array_4[1] & 0xf) << 4) + ((char_array_4[2] & 0x3c) >> 2);
            char_array_3[2] = ((char_array_4[2] & 0x3) << 6) + char_array_4[3];

            for (i = 0; i < 3; i++) {
                decoded.push_back(char_array_3[i]);
            }
            i = 0;
        }
    }

    if (i) {
        for (j = i; j < 4; j++) {
            char_array_4[j] = 0;
        }

        for (j = 0; j < 4; j++) {
            char_array_4[j] = base64_chars.find(char_array_4[j]);
        }

        char_array_3[0] = (char_array_4[0] << 2) + ((char_array_4[1] & 0x30) >> 4);
        char_array_3[1] = ((char_array_4[1] & 0xf) << 4) + ((char_array_4[2] & 0x3c) >> 2);

        for (j = 0; j < i - 1; j++) {
            decoded.push_back(char_array_3[j]);
        }
    }

    return decoded;
}



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

    if (data.empty()) {
        cout << "No saved passwords found in the JSON file." << endl;
        return;
    }

    for (const json &entry : data)
    {
        string url = entry["url"];
        string username = entry["username"];
        std::string base64Password = entry["password"];
        std::vector<unsigned char> passwordData = base64_decode(base64Password);

        // Convertir les données binaires en une chaîne de caractères
        std::string password(passwordData.begin(), passwordData.end());
        Password newPassword(url, username, password);
        passwordList.push_back(newPassword);
    }

    cout << "Passwords loaded from passwords.json" << endl;
}
