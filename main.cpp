#include <iostream>
#include <vector>
#include <fstream>
#include "nlohmann/json.hpp"
#include "main.h"



PasswordManager manager;

int PasswordManager::menu(unsigned char *ciphertext, unsigned char *key, unsigned char *iv, unsigned char *decryptedtext)
{

    int ciphertext_len;
    int choice;
    while (true)
    {
        cout << "Menu:"
             << "\n1. Add a password"
             << "\n2. View passwords"
             << "\n3. Delete a password"
             << "\n4. Modify a password"
             << "\n5. Help"
             << "\n0. Quit"
             << "\nChoose an option: ";


        if (cin >> choice && choice >= 0 && choice <= 5)
        {

            switch (choice)
            {
            case 1:
                cout << "Option 1: Add a password\n";
                ciphertext_len = manager.AddPassword(ciphertext, key, iv);
                break;
            case 2:
                cout << "Option 2: View passwords\n";
                manager.ViewPasswords(ciphertext, ciphertext_len, key, iv, decryptedtext);
                break;

            case 3:
                cout << "Option 3: Delete a password\n";
                manager.DeletePassword();
                break;

            case 4:
                cout << "Option 4: Modify a password\n";
                manager.ChangePassword();
                break;

            case 5:
                cout << "Option 5: Help\n";
                // Insert code to display help here
                break;

            case 0:
                cout << "Goodbye!\n";

                return 0;

            default:
                cout << "Invalid option. Please choose a valid option.\n";
                break;
            }
        }
        else
        {
            cout << "Invalid option. Please choose a valid option.\n";
        }
        return 1;
    }
}




// int main()
// {
//     OpenSSL_add_all_algorithms();
//     ERR_load_crypto_strings();
//     //initialisation de la clé
//     unsigned char key[32] ;
//     RAND_bytes(key, sizeof(key));
//     //initialisation de l'iv
//     unsigned char iv[16];
//     RAND_bytes(iv, sizeof(iv));
    
//     unsigned char ciphertext[128];
//     unsigned char decryptedtext[128];
    
//     int decryptedtext_len, ciphertext_len;
//     unsigned char *plaintext =
//         (unsigned char *)"C'est mon bois";

//     ciphertext_len = manager.encrypt (plaintext, strlen ((char *)plaintext), key, iv,
//                               ciphertext);
     
//     printf("Ciphertext is:\n");
//     BIO_dump_fp (stdout, (const char *)ciphertext, ciphertext_len);

//     decryptedtext_len = manager.decrypt(ciphertext, ciphertext_len, key, iv,
//                                 decryptedtext);

//     decryptedtext[decryptedtext_len] = '\0';

//     /* Show the decrypted text */
//     printf("Decrypted text is:\n");
//     printf("%s\n", decryptedtext);
//     // manager.LoadPasswordsFromJson();
//     // while(manager.menu())
//     // {}
//     // manager.SavePasswordsToJson();

//     return 0;
// }
int main (void) {

    OpenSSL_add_all_algorithms();
    ERR_load_crypto_strings();
    //initialisation de la clé
    unsigned char key[32] ;
    RAND_bytes(key, sizeof(key));
    //initialisation de l'iv
    unsigned char iv[16];
    RAND_bytes(iv, sizeof(iv));
    
    unsigned char ciphertext[128];
    unsigned char decryptedtext[128];
    
    manager.LoadPasswordsFromJson();
    while(manager.menu(ciphertext, key, iv, decryptedtext))
    {}
    manager.SavePasswordsToJson();

}
