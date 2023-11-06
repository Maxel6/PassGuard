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
    password = manager.EncryptPassword(password);
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
            string ClearPassword = manager.DecryptPassword(passwordList[i].getPassword());
            cout << "Password: " << ClearPassword << endl;
            cout << "-------------------" << endl;
        }
    }
}

void PasswordManager::DeletePassword()
{
    
    string url;
    if (passwordList.empty())
    {
        cout << "No passwords stored." << endl;
        return;
    }

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

    if (passwordList.empty())
    {
        cout << "No passwords stored." << endl;
        return;
    }
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

string PasswordManager::EncryptPassword(const string &password) {
    // Génération de la clé et de l'IV (vecteur d'initialisation)

    // Initialisation du contexte de chiffrement
    EVP_CIPHER_CTX *ctx = EVP_CIPHER_CTX_new();
    EVP_EncryptInit_ex(ctx, EVP_aes_256_cbc(), NULL, key, iv);

    // Chiffrement du mot de passe
    const unsigned char *input = (const unsigned char *)password.c_str();
    unsigned char encryptedData[256];  // Taille suffisante pour stocker les données chiffrées
    int encryptedLength = 0;
    EVP_EncryptUpdate(ctx, encryptedData, &encryptedLength, input, password.length());

    int finalLength = 0;
    EVP_EncryptFinal_ex(ctx, encryptedData + encryptedLength, &finalLength);
    encryptedLength += finalLength;

    // Conversion des données chiffrées en Base64
    BIO *bio, *b64;
    BUF_MEM *bufferPtr;

    b64 = BIO_new(BIO_f_base64());
    bio = BIO_new(BIO_s_mem());
    bio = BIO_push(b64, bio);

    BIO_write(bio, encryptedData, encryptedLength);
    BIO_flush(bio);
    BIO_get_mem_ptr(bio, &bufferPtr);

    string encryptedPassword(bufferPtr->data, bufferPtr->length);

    // Nettoyage
    EVP_CIPHER_CTX_free(ctx);
    BIO_free_all(bio);

    return encryptedPassword;
}

string PasswordManager::DecryptPassword(const string &encryptedPassword) {
    // Conversion de la chaîne Base64 en données binaires
    BIO *bio, *b64;
    b64 = BIO_new(BIO_f_base64());
    BIO_write(b64, encryptedPassword.c_str(), encryptedPassword.size());
    BIO *bio_mem = BIO_new(BIO_s_mem());
    bio = BIO_push(b64, bio_mem);
    std::vector<unsigned char> encryptedData(encryptedPassword.size(), 0);

    int decryptedLength = 0;
    decryptedLength = BIO_read(bio, &encryptedData[0], encryptedPassword.size());
    BIO_free_all(bio);

    // Initialisation du contexte de déchiffrement
    EVP_CIPHER_CTX *ctx = EVP_CIPHER_CTX_new();
    EVP_DecryptInit_ex(ctx, EVP_aes_256_cbc(), NULL, key, iv);

    // Déchiffrement des données
    unsigned char decryptedData[512];
    int decryptedDataLength = 0;

    if (EVP_DecryptUpdate(ctx, reinterpret_cast<unsigned char*>(&decryptedData[0]), &decryptedDataLength, encryptedData.data(), decryptedLength) != 1) {
        // Gestion de l'erreur
        return "Decryption Error 1";
    }



    int finalLength = 0;
    if (EVP_DecryptFinal_ex(ctx, reinterpret_cast<unsigned char*>(&decryptedData[0]) + decryptedDataLength, &finalLength) != 1) {
        // Gestion de l'erreur finale
        return "Decryption Error 2";
    }


    decryptedDataLength += finalLength;

    // Nettoyage
    EVP_CIPHER_CTX_free(ctx);

    // Conversion des données déchiffrées en une chaîne
    string decryptedPassword(reinterpret_cast<const char *>(&decryptedData[0]), decryptedDataLength);

    return decryptedPassword;
}
