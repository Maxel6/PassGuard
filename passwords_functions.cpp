#include "main.h"

int PasswordManager::AddPassword(unsigned char *ciphertext, unsigned char *key, unsigned char *iv)
{
    string url;
    string username;
    string password_str;
    int ciphertext_len = 0;

    cout << "Enter URL: ";
    cin >> url;
    cout << "Enter username: ";
    cin >> username;
    cout << "Enter password: ";
    cin >> password_str;
    unsigned char *password = (unsigned char *)password_str.c_str();
    
    ciphertext_len = manager.encrypt(password, password_str.length(), key, iv ,ciphertext);
    printf("Ciphertext is:\n");
    BIO_dump_fp (stdout, (const char *)ciphertext, ciphertext_len);
    cout << ciphertext_len << endl;

    Password newPassword(url, username, string(reinterpret_cast<char*>(ciphertext)), ciphertext_len);

    cout << newPassword.getCipherLen() << endl;
    passwordList.push_back(newPassword);

    cout << "Password added!\n";
    cout << "Would you like to add another password? (y/n): ";
    char answer;
    cin >> answer;

    if (answer == 'y' || answer == 'Y')
    {
        //WIP -- Assertion failed: (it != m_value.object->end()), function operator[], file json.hpp,.
        manager.AddPassword(ciphertext, key, iv);
    }
    else
    {
        
        manager.SavePasswordsToJson();
        return ciphertext_len;
    }
    return 1;
}

void PasswordManager::ViewPasswords(unsigned char *ciphertext, int ciphertext_len, unsigned char *key, unsigned char *iv, unsigned char *decryptedtext)
{
    int decryptedtext_len = 0;
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
	        unsigned char* password = (unsigned char*) passwordList[i].getPassword().c_str(); 
            cout << "Site: " << passwordList[i].getUrl() << endl;
            cout << "Username: " << passwordList[i].getUsername() << endl;
            decryptedtext_len = manager.decrypt(password, passwordList[i].getCipherLen(), key, iv, decryptedtext);
            cout << "Password: " << decryptedtext << endl;
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
            PasswordManager::SavePasswordsToJson(); 
            return;
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

                //TODO: decrypt the old password
                if (passwordList[i].getPassword() == oldPassword)
                {
                    cout << "Enter the new password: ";
                    cin >> newPassword;
                    //TODO: encrypt the new password
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

