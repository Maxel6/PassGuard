#include <gtk/gtk.h>
#include "main.h"
#include <set>
#include <pango/pango.h>

typedef struct
{
    GtkBuilder *builder;
    GtkGrid *add_grid;
    GtkListBox *listbox;
    GtkLabel *function_name;
    GtkWidget *fenetre_principale;
    GtkButton *addButton;
    GtkButton *modifyButton;
    GtkButton *deleteButton;
    GtkEntry *last_entry;

} SwitchViewSaveParams;

struct PasswordDialogData
{
    GtkWidget *dialog;
    GtkWidget *entry;
    std::string result;
};

vector<Password> passwordList;
PasswordManager manager;
unsigned char iv[16] = "012345678901234";
void switch_to_listbox(GtkButton *button, SwitchViewSaveParams *params);
int isJsonFileEmpty(const std::string &filename);
void add_password(GtkButton *button, SwitchViewSaveParams *params);
void update_listbox(GtkListBox *listbox);
void switch_to_add(GtkButton *button, SwitchViewSaveParams *params);
void switch_to_modify(GtkButton *button, SwitchViewSaveParams *params);
std::string show_password_dialog();
void on_ok_button_clicked(GtkButton *button, PasswordDialogData *data);
void switch_to_delete(GtkButton *button, SwitchViewSaveParams *params);
void modify_password(GtkButton *button, SwitchViewSaveParams *params);
void delete_password(GtkButton *button, SwitchViewSaveParams *params);

int main(int argc, char *argv[])
{
    GtkWidget *fenetre_principale = NULL;
    GtkBuilder *builder = NULL;
    GError *error = NULL;
    gchar *filename = NULL;
    GtkButton *pwd_view_button = NULL;
    GtkButton *Add_Save_button = NULL;
    GtkButton *pwd_mod_button = NULL;
    GtkGrid *add_grid = NULL;
    GtkListBox *listbox = NULL;
    GtkButton *pwd_add_button = NULL;
    GtkLabel *function_name = NULL;
    GtkButton *modify_button = NULL;
    GtkButton *delete_button = NULL;
    GtkButton *pwd_del_button = NULL;
    static gulong add_button_handler_id = 0;
    static gulong modify_button_handler_id = 0;
    static gulong delete_button_handler_id = 0;
    // Créer une instance du gestionnaire de mots de passe
    string s = show_password_dialog();
    gtk_init(&argc, &argv);

    builder = gtk_builder_new();

    filename = g_build_filename("2.glade", NULL);

    gtk_builder_add_from_file(builder, filename, &error);
    g_free(filename);

    if (error)
    {
        gint code = error->code;
        g_printerr("%s\n", error->message);
        g_error_free(error);
        return code;
    }

    std::vector<unsigned char> ciphertext = manager.loadFromJson("password.json");

    // Créer un vecteur pour stocker le texte déchiffré
    if (ciphertext.size() != 0)
    {
        cout << ciphertext.data() << endl;
        std::vector<unsigned char> plaintext(ciphertext.size());
        manager.decrypt(ciphertext.data(), ciphertext.size(), key, iv, plaintext.data());
    }

    function_name = GTK_LABEL(gtk_builder_get_object(builder, "funct_name_label"));
    fenetre_principale = GTK_WIDGET(gtk_builder_get_object(builder, "MainWindow"));
    pwd_add_button = GTK_BUTTON(gtk_builder_get_object(builder, "Pwd_Add_Button"));
    pwd_view_button = GTK_BUTTON(gtk_builder_get_object(builder, "Pwd_View_Button"));
    Add_Save_button = GTK_BUTTON(gtk_builder_get_object(builder, "Add_Save_button"));
    pwd_del_button = GTK_BUTTON(gtk_builder_get_object(builder, "Pwd_Del_Button"));
    pwd_mod_button = GTK_BUTTON(gtk_builder_get_object(builder, "Pwd_Ch_Button"));
    add_grid = GTK_GRID(gtk_builder_get_object(builder, "Add_grid"));
    listbox = GTK_LIST_BOX(gtk_builder_get_object(builder, "listbox1"));
    GtkEntry *url_entry = GTK_ENTRY(gtk_builder_get_object(builder, "Url_in"));
    GtkEntry *login_entry = GTK_ENTRY(gtk_builder_get_object(builder, "Login_in"));
    GtkEntry *password_entry = GTK_ENTRY(gtk_builder_get_object(builder, "Passord_in"));
    modify_button = GTK_BUTTON(gtk_builder_get_object(builder, "Modify_Button"));
    delete_button = GTK_BUTTON(gtk_builder_get_object(builder, "Del_Button"));

    ////////////////////////////////////////////////////////////////

    // Récupérer les entrées et le bouton

    // Attacher les widgets à la grid

    // Permettre à tous les widgets d'occuper l'espace disponible en largeur
    gtk_widget_set_hexpand(GTK_WIDGET(url_entry), TRUE);
    gtk_widget_set_hexpand(GTK_WIDGET(login_entry), TRUE);
    gtk_widget_set_hexpand(GTK_WIDGET(password_entry), TRUE);
    gtk_widget_set_hexpand(GTK_WIDGET(Add_Save_button), TRUE);

    PangoFontDescription *font_desc = pango_font_description_from_string("Arial 17");
    gtk_widget_override_font(GTK_WIDGET(function_name), font_desc);
    pango_font_description_free(font_desc);

    // Ajuster la marge autour du label
    int margin_top = 10;
    int margin_bttm = 10;
    gtk_widget_set_margin_top(GTK_WIDGET(function_name), margin_top);
    gtk_widget_set_margin_bottom(GTK_WIDGET(function_name), margin_bttm);

    // window size
    gtk_window_set_default_size(GTK_WINDOW(fenetre_principale), 800, 600);

    ////////////////////////////////////////////////////////////////

    if (pwd_view_button == NULL || Add_Save_button == NULL || add_grid == NULL || listbox == NULL)
    {
        g_printerr("Certains widgets n'ont pas été trouvés.\n");
        return -1;
    }

    // std::string password = show_password_dialog();

    SwitchViewSaveParams params = {builder, add_grid, listbox, function_name, fenetre_principale, Add_Save_button, modify_button, delete_button, password_entry};
    g_signal_connect(G_OBJECT(pwd_view_button), "clicked", G_CALLBACK(switch_to_listbox), &params);
    g_signal_connect(G_OBJECT(pwd_add_button), "clicked", G_CALLBACK(switch_to_add), &params);
    g_signal_connect(G_OBJECT(pwd_mod_button), "clicked", G_CALLBACK(switch_to_modify), &params);
    g_signal_connect(G_OBJECT(pwd_del_button), "clicked", G_CALLBACK(switch_to_delete), &params);


    add_button_handler_id = g_signal_connect(G_OBJECT(Add_Save_button), "clicked", G_CALLBACK(add_password), &params);
    modify_button_handler_id = g_signal_connect(G_OBJECT(modify_button), "clicked", G_CALLBACK(modify_password), &params);
    delete_button_handler_id = g_signal_connect(G_OBJECT(delete_button), "clicked", G_CALLBACK(delete_password), &params);




    // Connecter le signal "destroy" de la fenêtre principale à gtk_main_quit
    g_signal_connect(G_OBJECT(fenetre_principale), "destroy", G_CALLBACK(gtk_main_quit), NULL);

    // Afficher la fenêtre principale
    gtk_widget_show_all(fenetre_principale);

    // Démarrer la boucle principale GTK
    gtk_main();
    return 0;
}

void switch_to_add(GtkButton *button, SwitchViewSaveParams *params)
{
    GtkLabel *url_label = NULL;
    GtkLabel *login_label = NULL;
    GtkLabel *pwd_label = NULL;

    url_label = GTK_LABEL(gtk_builder_get_object(params->builder, "url_label"));
    login_label = GTK_LABEL(gtk_builder_get_object(params->builder, "login_label"));
    pwd_label = GTK_LABEL(gtk_builder_get_object(params->builder, "pwd_label"));

    gtk_label_set_text(url_label, "Url:");
    gtk_label_set_text(login_label, "Login:");
    gtk_label_set_text(pwd_label, "Password:");
    gtk_label_set_text(params->function_name, "Add a Password");
    // Masquer la grid
    gtk_widget_hide(GTK_WIDGET(params->listbox));

    // Afficher la listbox
    gtk_widget_show(GTK_WIDGET(params->add_grid));

    gtk_widget_show(GTK_WIDGET(params->addButton));
    gtk_widget_show(GTK_WIDGET(params->last_entry));
    gtk_widget_show(GTK_WIDGET(pwd_label));
    



    // Afficher le bouton "Modifier"
    gtk_widget_hide(GTK_WIDGET(params->modifyButton));
    gtk_widget_hide(GTK_WIDGET(params->deleteButton));

}
void switch_to_delete(GtkButton *button, SwitchViewSaveParams *params)
{
    
    GtkLabel *url_label = NULL;
    GtkLabel *login_label = NULL;
    GtkLabel *pwd_label = NULL;


    url_label = GTK_LABEL(gtk_builder_get_object(params->builder, "url_label"));
    login_label = GTK_LABEL(gtk_builder_get_object(params->builder, "login_label"));
    pwd_label = GTK_LABEL(gtk_builder_get_object(params->builder, "pwd_label"));
    GtkEntry *newPassword_entry = GTK_ENTRY(gtk_builder_get_object(params->builder, "Passord_in"));


    gtk_label_set_text(url_label, "Url:");
    gtk_label_set_text(login_label, "Password:");
    gtk_label_set_text(params->function_name, "Delete a Password");


    gtk_widget_hide(GTK_WIDGET(params->listbox));
    gtk_widget_hide(GTK_WIDGET(params->modifyButton));
    gtk_widget_hide(GTK_WIDGET(params->addButton));
    gtk_widget_set_visible(GTK_WIDGET(newPassword_entry), FALSE);
    gtk_widget_set_visible(GTK_WIDGET(pwd_label), FALSE);

    gtk_widget_show(GTK_WIDGET(params->add_grid));
    gtk_widget_show(GTK_WIDGET(params->deleteButton));



}

void switch_to_modify(GtkButton *button, SwitchViewSaveParams *params)
{
    
    GtkLabel *url_label = NULL;
    GtkLabel *login_label = NULL;
    GtkLabel *pwd_label = NULL;

    url_label = GTK_LABEL(gtk_builder_get_object(params->builder, "url_label"));
    login_label = GTK_LABEL(gtk_builder_get_object(params->builder, "login_label"));
    pwd_label = GTK_LABEL(gtk_builder_get_object(params->builder, "pwd_label"));

    gtk_label_set_text(url_label, "Url:");
    gtk_label_set_text(login_label, "Old Password:");
    gtk_label_set_text(pwd_label, "New Password:");
    gtk_label_set_text(params->function_name, "Modify a Password");


    // Masquer la grid
    gtk_widget_hide(GTK_WIDGET(params->listbox));

    // Afficher la listbox
    gtk_widget_show(GTK_WIDGET(params->add_grid));
    gtk_widget_hide(GTK_WIDGET(params->addButton));
    gtk_widget_hide(GTK_WIDGET(params->deleteButton));

    // Afficher le bouton "Modifier"
    gtk_widget_show(GTK_WIDGET(params->modifyButton));
    gtk_widget_show(GTK_WIDGET(params->last_entry));
    gtk_widget_show(GTK_WIDGET(pwd_label));



}

void switch_to_listbox(GtkButton *button, SwitchViewSaveParams *params)
{
    // Masquer la grid
    gtk_label_set_text(params->function_name, "Passwords List");

    gtk_widget_hide(GTK_WIDGET(params->add_grid));

    // Afficher la listbox
    gtk_widget_show(GTK_WIDGET(params->listbox));

    // Mettre à jour la listbox avec les données actuelles
    update_listbox(params->listbox);

}
void modify_password(GtkButton *button, SwitchViewSaveParams *params)
{
    GtkEntry *url_entry = GTK_ENTRY(gtk_builder_get_object(params->builder, "Url_in"));
    GtkEntry *oldPassword_entry = GTK_ENTRY(gtk_builder_get_object(params->builder, "Login_in"));
    GtkEntry *newPassword_entry = GTK_ENTRY(gtk_builder_get_object(params->builder, "Passord_in"));

    const gchar *button_label = gtk_button_get_label(button);
    const gchar *url = gtk_entry_get_text(url_entry);
    const gchar *oldPassword = gtk_entry_get_text(oldPassword_entry);
    const gchar *newPassword = gtk_entry_get_text(newPassword_entry);

    if (passwordList.empty())
    {
        GtkWidget *dialog = gtk_message_dialog_new(GTK_WINDOW(params->fenetre_principale),
                                                   GTK_DIALOG_MODAL,
                                                   GTK_MESSAGE_ERROR,
                                                   GTK_BUTTONS_OK,
                                                   "No password saved");
        gtk_dialog_run(GTK_DIALOG(dialog));
        gtk_widget_destroy(dialog);

        return; // Ne pas ajouter le mot de passe si un champ est vide
    }

    if (strlen(url) == 0 || strlen(oldPassword) == 0 || strlen(newPassword) == 0)
    {
        // Afficher un message à l'utilisateur
        GtkWidget *dialog = gtk_message_dialog_new(GTK_WINDOW(params->fenetre_principale),
                                                   GTK_DIALOG_MODAL,
                                                   GTK_MESSAGE_ERROR,
                                                   GTK_BUTTONS_OK,
                                                   "Veuillez remplir tous les champs.");
        gtk_dialog_run(GTK_DIALOG(dialog));
        gtk_widget_destroy(dialog);

        return; // Ne pas ajouter le mot de passe si un champ est vide
    }
    for (size_t i = 0; i < passwordList.size(); ++i)
    {
        if (passwordList[i].getUrl() == url)
        {
            if (passwordList[i].getPassword() == oldPassword)
            {

                passwordList[i].setPassword(newPassword);

                manager.SavePasswordsToJson(key, iv);
                return;
            }
            else
            {
                GtkWidget *dialog = gtk_message_dialog_new(GTK_WINDOW(params->fenetre_principale),
                                                           GTK_DIALOG_MODAL,
                                                           GTK_MESSAGE_ERROR,
                                                           GTK_BUTTONS_OK,
                                                           "Icorrect password");
                gtk_dialog_run(GTK_DIALOG(dialog));
                gtk_widget_destroy(dialog);
                return;
            }
        }
        cout << "No password found for site: " << url << endl;
    }
}

void add_password(GtkButton *button, SwitchViewSaveParams *params)
{
    // Récupérer les valeurs des champs
    GtkEntry *url_entry = GTK_ENTRY(gtk_builder_get_object(params->builder, "Url_in"));
    GtkEntry *login_entry = GTK_ENTRY(gtk_builder_get_object(params->builder, "Login_in"));
    GtkEntry *password_entry = GTK_ENTRY(gtk_builder_get_object(params->builder, "Passord_in"));

    const gchar *button_label = gtk_button_get_label(button);
    const gchar *url = gtk_entry_get_text(url_entry);
    const gchar *login = gtk_entry_get_text(login_entry);
    const gchar *password = gtk_entry_get_text(password_entry);

    if (strlen(url) == 0 || strlen(login) == 0 || strlen(password) == 0)
    {
        // Afficher un message à l'utilisateur
        GtkWidget *dialog = gtk_message_dialog_new(GTK_WINDOW(params->fenetre_principale),
                                                   GTK_DIALOG_MODAL,
                                                   GTK_MESSAGE_ERROR,
                                                   GTK_BUTTONS_OK,
                                                   "Veuillez remplir tous les champs.");
        gtk_dialog_run(GTK_DIALOG(dialog));
        gtk_widget_destroy(dialog);

        return; // Ne pas ajouter le mot de passe si un champ est vide
    }
    // Créer un nouvel objet Password et l'ajouter à la liste
    Password newPassword(url, login, password);
    passwordList.push_back(newPassword);

    // Imprimer la liste (à des fins de test)
    for (const auto &pwd : passwordList)
    {
        std::cout << "URL: " << pwd.getUrl() << " | Username: " << pwd.getUsername() << " | Password: " << pwd.getPassword() << std::endl;
    }
    // Vider les champs
    gtk_entry_set_text(url_entry, "");
    gtk_entry_set_text(login_entry, "");
    gtk_entry_set_text(password_entry, "");

    manager.SavePasswordsToJson(key, iv);
}

void update_listbox(GtkListBox *listbox)
{
    // Supprimer les éléments actuels de la liste box
    gtk_container_foreach(
        GTK_CONTAINER(listbox), [](GtkWidget *child, gpointer)
        { gtk_widget_destroy(child); },
        nullptr);

    // Utiliser un ensemble pour éviter d'ajouter plusieurs fois le même élément
    std::set<std::string> addedItems;

    // Ajouter les nouveaux éléments à la liste box
    for (const auto &password : passwordList)
    {
        // Utiliser une chaîne composée de l'URL, du nom d'utilisateur et du mot de passe comme identifiant unique
        std::string identifier = password.getUrl() + password.getUsername() + password.getPassword();

        // Vérifier si cet identifiant a déjà été ajouté
        if (addedItems.find(identifier) == addedItems.end())
        {
            // Création d'une ligne de liste pour chaque mot de passe
            GtkWidget *row = gtk_list_box_row_new();
            // Utilisez le getter pour obtenir la valeur de url_
            std::string label_text = password.getUrl() + " " + password.getUsername() + " " + password.getPassword();
            GtkWidget *label = gtk_label_new(label_text.c_str());
            gtk_container_add(GTK_CONTAINER(row), label);

            // Ajout de la ligne à la liste box
            gtk_container_add(GTK_CONTAINER(listbox), row);

            // Ajouter l'identifiant à l'ensemble
            addedItems.insert(identifier);
        }
    }

    // Afficher tous les éléments de la liste box
    gtk_widget_show_all(GTK_WIDGET(listbox));
}
void on_ok_button_clicked(GtkButton *button, PasswordDialogData *data)
{
    const gchar *password = gtk_entry_get_text(GTK_ENTRY(data->entry));
    data->result = password;
    gtk_widget_destroy(data->dialog); // Fermer la fenêtre
}
std::string show_password_dialog()
{
    // Initialiser GTK
    gtk_init(NULL, NULL);

    // Créer la fenêtre de dialogue
    GtkWidget *dialog = gtk_dialog_new_with_buttons("Enter Password",
                                                    NULL,
                                                    GTK_DIALOG_MODAL,
                                                    "_login",
                                                    GTK_RESPONSE_OK,
                                                    NULL);

    // Récupérer la zone de contenu de la fenêtre

    // Ajouter une boîte d'entrée de texte à la zone de contenu
    GtkWidget *entry = gtk_entry_new();
    gtk_entry_set_visibility(GTK_ENTRY(entry), FALSE); // Cacher les caractères du mot de passe
    gtk_container_add(GTK_CONTAINER(gtk_dialog_get_content_area(GTK_DIALOG(dialog))), entry);

    // Ajouter un bouton "OK" à la fenêtre de dialogue
    GtkWidget *ok_button = gtk_dialog_get_widget_for_response(GTK_DIALOG(dialog), GTK_RESPONSE_OK);

    // Connecter la fonction de rappel au bouton "OK"
    PasswordDialogData data = {dialog, entry, ""};
    g_signal_connect(G_OBJECT(ok_button), "clicked", G_CALLBACK(on_ok_button_clicked), &data);

    // Afficher tous les widgets
    gtk_widget_show_all(dialog);

    // Exécuter la boucle principale GTK (attendre que l'utilisateur entre le mot de passe)
    gtk_main();

    // Retourner le mot de passe entré par l'utilisateur
    return data.result;
}


void delete_password(GtkButton *button, SwitchViewSaveParams *params)
{
    GtkEntry *url_entry = GTK_ENTRY(gtk_builder_get_object(params->builder, "Url_in"));
    GtkEntry *oldPassword_entry = GTK_ENTRY(gtk_builder_get_object(params->builder, "Login_in"));

    const gchar *button_label = gtk_button_get_label(button);
    const gchar *url = gtk_entry_get_text(url_entry);
    const gchar *Password = gtk_entry_get_text(oldPassword_entry);
    cout << " ici " << endl;
    if (passwordList.empty())
    {
        GtkWidget *dialog = gtk_message_dialog_new(GTK_WINDOW(params->fenetre_principale),
                                                   GTK_DIALOG_MODAL,
                                                   GTK_MESSAGE_ERROR,
                                                   GTK_BUTTONS_OK,
                                                   "No password saved");
        gtk_dialog_run(GTK_DIALOG(dialog));
        gtk_widget_destroy(dialog);

        return; // Ne pas ajouter le mot de passe si un champ est vide
    }

    if (strlen(url) == 0 || strlen(Password) == 0 )
    {
        // Afficher un message à l'utilisateur
        GtkWidget *dialog = gtk_message_dialog_new(GTK_WINDOW(params->fenetre_principale),
                                                   GTK_DIALOG_MODAL,
                                                   GTK_MESSAGE_ERROR,
                                                   GTK_BUTTONS_OK,
                                                   "Veuillez remplir tous les champs.");
        gtk_dialog_run(GTK_DIALOG(dialog));
        gtk_widget_destroy(dialog);

        return;
    }
    for (size_t i = 0; i < passwordList.size(); ++i)
    {
        if (passwordList[i].getUrl() == url)
        {
            passwordList.erase(passwordList.begin() + i);
            manager.SavePasswordsToJson(key, iv);
        }
        else
        {
            GtkWidget *dialog = gtk_message_dialog_new(GTK_WINDOW(params->fenetre_principale),
                                                        GTK_DIALOG_MODAL,
                                                        GTK_MESSAGE_ERROR,
                                                        GTK_BUTTONS_OK,
                                                        "Icorrect URL or password");
            gtk_dialog_run(GTK_DIALOG(dialog));
            gtk_widget_destroy(dialog);
            return;
        }
    }
    gtk_entry_set_text(url_entry, "");
    gtk_entry_set_text(oldPassword_entry, "");
    cout<< "deleted" << endl;
}