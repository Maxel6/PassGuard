#ifndef MAIN_H
#define MAIN_H

#include <gtk/gtk.h>
#include <nlohmann/json.hpp>
#include <openssl/evp.h>
#include <openssl/rand.h>
#include <fstream>
#include <string>
#include <iostream>


class Password {
public:
    Password(const std::string& url, const std::string& username, const std::string& password);
    const std::string& getUrl() const;
    const std::string& getUsername() const;
    const std::string& getPassword() const;
};

class PasswordManager {
public:
    void SavePasswordsToJson();
    void LoadPasswordsFromJson();
    int AddPassword();
private:
    std::vector<Password> passwordList;
};

#endif // MAIN_H
