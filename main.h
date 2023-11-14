#ifndef MAIN_H
#define MAIN_H

#include <glib.h>
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
#include <set>


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

    //TODO: add key and iv
    void setUsername(const string &username) { username_ = username; }
    void setPassword(const string &password) { password_ = password; }

private:
    string url_;
    string username_;
    string password_;


};

class PasswordManager
{
public:
    std::vector<unsigned char> loadFromJson(const std::string& filename);
    void saveToJson(const std::string &filename, const unsigned char *ciphertext, int ciphertext_len);
    int encrypt(unsigned char *plaintext, int plaintext_len, unsigned char *key, unsigned char *iv, unsigned char *ciphertext);
    int decrypt(unsigned char *ciphertext, int ciphertext_len, unsigned char *key, unsigned char *iv, unsigned char *plaintext);
    vector<unsigned char> encryptData(const std::vector<unsigned char> &data, unsigned char *key, unsigned char *iv);
    void SavePasswordsToJson(unsigned char key[32], unsigned char iv[16]);

    int AddPassword();
    int menu();
    void DeletePassword();
    void ViewPasswords();
    void ChangePassword();
private:
    
};

std::vector<unsigned char> base64_decode(const std::string &encoded_string);
std::string base64_encode(const std::vector<unsigned char> &data);
bool is_base64(unsigned char c);
int isFirstLogin();
void addPadding(std::vector<unsigned char> &data);
void removePadding(std::vector<unsigned char> &data);

extern PasswordManager manager;
extern std::vector<Password> passwordList;

#endif // MAIN_H
