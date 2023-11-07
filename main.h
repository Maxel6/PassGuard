#ifndef MAIN_H
#define MAIN_H

#include <gtk/gtk.h>
#include <nlohmann/json.hpp>
#include <openssl/rand.h>
#include <openssl/evp.h>
#include <openssl/bio.h>
#include <openssl/buffer.h>
#include <openssl/err.h>
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
    string EncryptPassword(const string &password);
    int encrypt(unsigned char *plaintext, int plaintext_len, unsigned char *key, unsigned char *iv, unsigned char *ciphertext);
    int decrypt(unsigned char *ciphertext, int ciphertext_len, unsigned char *key, unsigned char *iv, unsigned char *plaintext);
    string DecryptPassword(const string &encryptedPassword);
    void SavePasswordsToJson();
    void LoadPasswordsFromJson();
    int AddPassword(unsigned char *ciphertext, unsigned char *key, unsigned char *iv);
    int menu(unsigned char *ciphertext, unsigned char *key, unsigned char *iv, unsigned char *decryptedtext);
    void DeletePassword();
    void ViewPasswords(unsigned char *ciphertext, int ciphertext_len, unsigned char *key, unsigned char *iv, unsigned char *decryptedtext);
    void ChangePassword();
private:
    std::vector<Password> passwordList;
};

std::vector<unsigned char> base64_decode(const std::string &encoded_string);
std::string base64_encode(const std::vector<unsigned char> &data);
bool is_base64(unsigned char c);

extern vector<Password> passwordList;
extern PasswordManager manager;

#endif // MAIN_H
