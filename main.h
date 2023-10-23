#ifndef MAIN_H
#define MAIN_H

#include <gtk/gtk.h>
#include <nlohmann/json.hpp>
#include <openssl/evp.h>
#include <openssl/rand.h>
#include <fstream>
#include <string>
#include <iostream>
#include <vector>
#include <string>


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

    void setUsername(const string &username) { username_ = username; }
    void setPassword(const string &password) { password_ = password; }

private:
    string url_;
    string username_;
    string password_;
};

class PasswordManager {
public:
    void SavePasswordsToJson();
    void LoadPasswordsFromJson();
    int AddPassword();
    int menu();
    void DeletePassword();
    void ViewPasswords();
    void ChangePassword();
private:
    std::vector<Password> passwordList;
};

extern vector<Password> passwordList;
extern PasswordManager manager;

#endif // MAIN_H
