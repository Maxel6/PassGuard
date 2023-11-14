#include "main.h"

void handleErrors(void)
{
    ERR_print_errors_fp(stderr);
    abort();
}
std::vector<unsigned char> PasswordManager::encryptData(const std::vector<unsigned char> &data,
                                                        unsigned char *key, unsigned char *iv)
{
    int data_len = data.size();

    std::vector<unsigned char> ciphertext(data_len + EVP_MAX_BLOCK_LENGTH);
    
    int ciphertext_len = encrypt(const_cast<unsigned char*>(data.data()), 
                                    data_len, key, iv, ciphertext.data());

    ciphertext.resize(ciphertext_len);

    return ciphertext;
}

int PasswordManager::encrypt(unsigned char *plaintext, int plaintext_len, 
            unsigned char *key, unsigned char *iv, unsigned char *ciphertext)
{
    EVP_CIPHER_CTX *ctx;

    int len;

    int ciphertext_len;

    /* Create and initialise the context */
    if(!(ctx = EVP_CIPHER_CTX_new()))
        handleErrors();

    if(1 != EVP_EncryptInit_ex(ctx, EVP_aes_256_cbc(), NULL, key, iv))
        handleErrors();

    if(1 != EVP_EncryptUpdate(ctx, ciphertext, &len, plaintext, plaintext_len))
        handleErrors();

    ciphertext_len = len;

    if(1 != EVP_EncryptFinal_ex(ctx, ciphertext + len, &len))
        handleErrors();
    ciphertext_len += len;

    EVP_CIPHER_CTX_free(ctx);

    return ciphertext_len;
}

int PasswordManager::decrypt(unsigned char *ciphertext, int ciphertext_len, unsigned char *key, unsigned char *iv, unsigned char *plaintext)
{
    EVP_CIPHER_CTX *ctx;

    int len = 0;

    int plaintext_len = 0;
    cout << "decrypt" << endl;
    /* Create and initialise the context */
    if(!(ctx = EVP_CIPHER_CTX_new()))
        handleErrors();

    /*
     * Initialise the decryption operation. IMPORTANT - ensure you use a key
     * and IV size appropriate for your cipher
     * In this example we are using 256 bit AES (i.e. a 256 bit key). The
     * IV size for *most* modes is the same as the block size. For AES this
     * is 128 bits
     */
    if(1 != EVP_DecryptInit_ex(ctx, EVP_aes_256_cbc(), NULL, key, iv))
        handleErrors();

    /*
     * Provide the message to be decrypted, and obtain the plaintext output.
     * EVP_DecryptUpdate can be called multiple times if necessary.
     */
    if(1 != EVP_DecryptUpdate(ctx, plaintext, &len, ciphertext, ciphertext_len))
        handleErrors();
    plaintext_len = len;

    /*
     * Finalise the decryption. Further plaintext bytes may be written at
     * this stage.
     */
    if(1 != EVP_DecryptFinal_ex(ctx, plaintext + len, &len))
        handleErrors();
    plaintext_len += len;

    /* Clean up */
    EVP_CIPHER_CTX_free(ctx);


    try {
        json jsonArray = json::parse(plaintext);

        // Création de la liste de mots de passe

        // Parcours du tableau JSON et ajout des éléments à la liste
        for (const auto& entry : jsonArray) {
            std::string url = entry["url"];
            std::string username = entry["username"];
            std::string password = entry["password"];
            passwordList.emplace_back(url, username, password);
        }
   } catch (const json::exception& e) {
        std::cerr << "Erreur JSON : " << e.what() << std::endl;
    }

    return plaintext_len;
}