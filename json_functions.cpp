#include "main.h"

vector<unsigned char> PasswordManager::loadFromJson(const string& filename) {
    ifstream inputFile(filename);
    if (!inputFile.is_open()) {
        cout << "Could not open file: " << filename << endl;
        return vector<unsigned char>();
    }

    json encrypted_json;
    inputFile >> encrypted_json;

    // Extraire la chaîne Base64 de la structure JSON
    string base64_encoded = encrypted_json["data"];

    // Décoder la chaîne Base64 en vecteur d'octets
    vector<unsigned char> decrypted_data = base64_decode(base64_encoded);

    // Déchiffrer les données avec AES-256-CBC

    return decrypted_data;
}



// void PasswordManager::SavePasswordsToJson(unsigned char key[32 ], unsigned char iv[16]) {
//     json data;

//     // Ajouter chaque mot de passe à la structure JSON
  
//     for (const Password &password : passwordList) {
//         json entry;
//         entry["url"] = password.getUrl();
//         entry["username"] = password.getUsername();
//         entry["password"] = password.getPassword();
//         data.push_back(entry);
//     }
//     // Convertir la structure JSON en une chaîne JSON
//     string json_content = data.dump();

//     // Chiffrer la chaîne JSON
//     vector<unsigned char> encrypted_data = manager.encryptData(vector<unsigned char>(json_content.begin(), json_content.end()), key, iv);

//     // Convertir le vecteur d'octets en une chaîne encodée en Base64
//     string encoded_json = base64_encode(encrypted_data);

//     // Sauvegarder la chaîne JSON encodée en Base64 dans un fichier
//     ofstream outputFile("password.json");
//     outputFile << encoded_json;
//     outputFile.close();
//     cout << "saved" << endl;
// }

void PasswordManager::SavePasswordsToJson(unsigned char key[32], unsigned char iv[16])
{
    json data;

    // Ajouter chaque mot de passe à la structure JSON
    for (const Password &password : passwordList) {
        json entry;
        entry["url"] = password.getUrl();
        entry["username"] = password.getUsername();
        entry["password"] = password.getPassword();
        data.push_back(entry);
    }

    string json_content = data.dump();

    vector<unsigned char> encrypted_data = manager.encryptData(
        vector<unsigned char>(json_content.begin(), json_content.end()), key, iv
    );

    string base64_encoded = base64_encode(encrypted_data);

    json encrypted_json;
    encrypted_json["data"] = base64_encoded;

    ofstream outputFile("password.json");
    outputFile << encrypted_json.dump();
    outputFile.close();

}
