#include <iostream>
#include <fstream>
#include <windows.h>
#include <vector>
#include <algorithm>
#include <sstream>

using namespace std;

string usersFileName = "Users.txt";
string contactsFileName = "Contacts.txt";
string tempContactsFileName = "Contacts_temp.txt";

struct User
{
    int id = 0;
    string login = "", password = "";
};

struct Contact
{
    int id = 0, userId = 0;
    string firstName = "", lastName = "", phoneNumber = "", email = "", address = "";
};

string loadLine()
{
    string input = "";
    getline(cin, input);
    return input;
}

char loadCharacter()
{
    string input = "";
    char character  = {0};

    while (true)
    {
        getline(cin, input);

        if (input.length() == 1)
        {
            character = input[0];
            break;
        }
        cout << "To nie jest pojedynczy znak. Wpisz ponownie." << endl;
    }
    return character;
}

int loadInteger()
{
    string input = "";
    int number = 0;

    while (true)
    {
        getline(cin, input);

        stringstream myStream(input);
        if (myStream >> number)
            break;
        cout << "To nie jest liczba. Wpisz ponownie. " << endl;
    }
    return number;
}

string convertIntToString(int number)
{
    ostringstream ss;
    ss << number;
    string str = ss.str();
    return str;
}

string changeFirstLetterToUpperCaseAndOthersToLowerCase(string text)
{
    if (!text.empty())
    {
        transform(text.begin(), text.end(), text.begin(), ::tolower);
        text[0] = toupper(text[0]);
    }
    return text;
}

User getUserData(string userDataSeparatedByVerticalBars)
{
    User user;
    string singleUserData = "";
    int singleUserDataNumber = 1;

    for (int characterPosition = 0; characterPosition < userDataSeparatedByVerticalBars.length(); characterPosition++)
    {
        if (userDataSeparatedByVerticalBars[characterPosition] != '|')
        {
            singleUserData += userDataSeparatedByVerticalBars[characterPosition];
        }
        else
        {
            switch(singleUserDataNumber)
            {
            case 1:
                user.id = atoi(singleUserData.c_str());
                break;
            case 2:
                user.login = singleUserData;
                break;
            case 3:
                user.password = singleUserData;
                break;
            }
            singleUserData = "";
            singleUserDataNumber++;
        }
    }
    return user;
}

void loadUsersFromFile(vector <User> &users)
{
    User user;
    string userDataSeparatedByVerticalBars = "";

    fstream textFile;
    textFile.open(usersFileName.c_str(), ios::in);

    if (textFile.good() == true)
    {
        while (getline(textFile, userDataSeparatedByVerticalBars))
        {
            user = getUserData(userDataSeparatedByVerticalBars);
            users.push_back(user);
        }

    }
    textFile.close();
}

bool isFileEmpty(fstream &textFile)
{
    textFile.seekg(0, ios::end);
    if (textFile.tellg() == 0)
        return true;
    else
        return false;
}

string convertUserDataToLineWithVerticalBars(User user)
{
    string userDataLine = "";

    userDataLine += convertIntToString(user.id)+ '|';
    userDataLine += user.login + '|';
    userDataLine += user.password + '|';

    return userDataLine;
}

void appendUserToFile(User user)
{
    fstream textFile;
    string userDataLine = "";
    textFile.open(usersFileName.c_str(), ios::app);

    if (textFile.good() == true)
    {
        userDataLine = convertUserDataToLineWithVerticalBars(user);

        if (isFileEmpty(textFile) == true)
        {
            textFile << userDataLine;
        }
        else
        {
            textFile << endl << userDataLine ;
        }
    }
    else
        cout << "Nie udalo sie otworzyc pliku " << usersFileName << " i zapisac w nim danych." << endl;
    textFile.close();
}

void saveAllUsersToFile(vector <User> &users)
{
    fstream textFile;
    string userDataLine = "";
    vector <User>::iterator itrEnd = --users.end();

    textFile.open(usersFileName.c_str(), ios::out);

    if (textFile.good() == true)
    {
        for (vector <User>::iterator itr = users.begin(); itr != users.end(); itr++)
        {
            userDataLine = convertUserDataToLineWithVerticalBars(*itr);

            if (itr == itrEnd)
            {
               textFile << userDataLine;
            }
            else
            {
                textFile << userDataLine << endl;
            }
            userDataLine = "";
        }
    }
    else
    {
        cout << "Nie mozna otworzyc pliku " << usersFileName << endl;
    }
    textFile.close();
}

bool doesLoginExist(vector <User> &users, string login)
{
    vector <User>::iterator itr = users.begin();
    while (itr != users.end())
    {
        if (itr -> login == login)
        {
            cout << endl << "Istnieje uzytkownik o takim loginie." << endl;
            return true;
        }
        else
            itr++;
    }
    return false;
}

int getNewUserId(vector <User> &users)
{
    if (users.empty() == true)
        return 1;
    else
        return users.back().id + 1;
}

User enterNewUserData(vector <User> &users)
{
    User user;

    user.id = getNewUserId(users);

    do
    {
        cout << endl << "Podaj login: ";
        user.login = loadLine();
    } while (doesLoginExist(users, user.login) == true);

    cout << "Podaj haslo: ";
    user.password = loadLine();

    return user;
}

void registerUser(vector <User> &users)
{
    User user = enterNewUserData(users);

    users.push_back(user);
    appendUserToFile(user);

    cout << endl << "Konto zalozono pomyslnie" << endl << endl;
    system("pause");
}

int loginUser(vector <User> &users)
{
    User user;
    string login = "", password = "";

    cout << endl << "Podaj login: ";
    login = loadLine();

    vector <User>::iterator itr = users.begin();
    while (itr != users.end())
    {
        if (itr -> login == login)
        {
            for (int attempts = 3; attempts > 0; attempts--)
            {
                cout << "Podaj haslo. Pozostalo prob: " << attempts << ": ";
                password = loadLine();

                if (itr -> password == password)
                {
                    cout << endl << "Zalogowales sie." << endl << endl;
                    system("pause");
                    return itr -> id;
                }
            }
            cout << "Wprowadzono 3 razy bledne haslo." << endl;
            system("pause");
            return 0;
        }
        itr++;
    }
    cout << "Nie ma uzytkownika z takim loginem" << endl << endl;
    system("pause");
    return 0;
}

void changePasswordOfLoggedInUser(vector <User> &users, int loggedInUserId)
{
    string newPassword = "";
    cout << "Podaj nowe haslo: ";
    newPassword = loadLine();

    for (vector <User>::iterator itr = users.begin(); itr != users.end(); itr++)
    {
        if (itr -> id == loggedInUserId)
        {
            itr -> password = newPassword;
            cout << "Haslo zostalo zmienione." << endl << endl;
            system("pause");
        }
    }
    saveAllUsersToFile(users);
}

string convertContactDataToLineWithVerticalBars(Contact contact)
{
    string contactDataLine = "";

    contactDataLine += convertIntToString(contact.id) + '|';
    contactDataLine += convertIntToString(contact.userId) + '|';
    contactDataLine += contact.firstName + '|';
    contactDataLine += contact.lastName + '|';
    contactDataLine += contact.phoneNumber + '|';
    contactDataLine += contact.email + '|';
    contactDataLine += contact.address + '|';

    return contactDataLine;
}

void displayContactData(Contact contact)
{
    cout << endl << "Id:                 " << contact.id << endl;
    cout << "Imie:               " << contact.firstName << endl;
    cout << "Nazwisko:           " << contact.lastName << endl;
    cout << "Numer telefonu:     " << contact.phoneNumber << endl;
    cout << "Email:              " << contact.email << endl;
    cout << "Adres:              " << contact.address << endl;
}

void appendContactToFile(Contact contact)
{
    string contactDataLine = "";
    fstream textFile;
    textFile.open(contactsFileName.c_str(), ios::out | ios::app);

    if (textFile.good() == true)
    {
        contactDataLine = convertContactDataToLineWithVerticalBars(contact);

        if (isFileEmpty(textFile) == true)
        {
            textFile << contactDataLine;
        }
        else
        {
            textFile << endl << contactDataLine ;
        }
    }
    else
    {
        cout << "Nie udalo sie otworzyc pliku i zapisac w nim danych." << endl;
    }
    textFile.close();
    system("pause");
}

Contact enterNewContactData(int loggedInUserId, int lastContactId)
{
    Contact contact;

    contact.id = ++lastContactId;
    contact.userId = loggedInUserId;

    cout << "Podaj imie: ";
    contact.firstName = loadLine();
    contact.firstName = changeFirstLetterToUpperCaseAndOthersToLowerCase(contact.firstName);

    cout << "Podaj nazwisko: ";
    contact.lastName = loadLine();
    contact.lastName = changeFirstLetterToUpperCaseAndOthersToLowerCase(contact.lastName);

    cout << "Podaj numer telefonu: ";
    contact.phoneNumber = loadLine();

    cout << "Podaj email: ";
    contact.email = loadLine();

    cout << "Podaj adres: ";
    contact.address = loadLine();

    return contact;
}

int addContact(vector <Contact> &contacts, int loggedInUserId, int lastContactId)
{
    Contact contact;

    system("cls");
    cout << " >>> DODAWANIE NOWEGO ADRESATA <<<" << endl << endl;
    contact = enterNewContactData(loggedInUserId, lastContactId);

    contacts.push_back(contact);
    appendContactToFile(contact);

    return ++lastContactId;
}

Contact getContactData(string contactDataSeparatedByVerticalBars)
{
    Contact contact;
    string singleContactData = "";
    int singleContactDataNumber = 1;

    for (int characterPosition = 0; characterPosition < contactDataSeparatedByVerticalBars.length(); characterPosition++)
    {
        if (contactDataSeparatedByVerticalBars[characterPosition] != '|')
        {
            singleContactData += contactDataSeparatedByVerticalBars[characterPosition];
        }
        else
        {
            switch(singleContactDataNumber)
            {
            case 1:
                contact.id = atoi(singleContactData.c_str());
                break;
            case 2:
                contact.userId = atoi(singleContactData.c_str());
                break;
            case 3:
                contact.firstName = singleContactData;
                break;
            case 4:
                contact.lastName = singleContactData;
                break;
            case 5:
                contact.phoneNumber = singleContactData;
                break;
            case 6:
                contact.email = singleContactData;
                break;
            case 7:
                contact.address = singleContactData;
                break;
            }
            singleContactData = "";
            singleContactDataNumber++;
        }
    }
    return contact;
}

string getNumber(string text, int characterPosition)
{
    string number = "";
    while(isdigit(text[characterPosition]) == true)
    {
        number += text[characterPosition];
        characterPosition ++;
    }
    return number;
}

int convertStringToInt(string number)
{
    int numberInt;
    istringstream iss(number);
    iss >> numberInt;

    return numberInt;
}

int getUserIdFromDataSeparatedByVerticalBars(string oneContactDataSeparatedByVerticalBars)
{
    int startOfUserIdPosition = oneContactDataSeparatedByVerticalBars.find_first_of('|') + 1;
    int userId = convertStringToInt(getNumber(oneContactDataSeparatedByVerticalBars, startOfUserIdPosition));

    return userId;
}

int getContactIdFromDataSeparatedByVerticalBars(string oneContactDataSeparatedByVerticalBars)
{
    int startOfContactIdPosition = 0;
    int contactId = convertStringToInt(getNumber(oneContactDataSeparatedByVerticalBars, startOfContactIdPosition));
    return contactId;
}

int loadContactsOfLoggedInUserFromFile(vector <Contact> &contacts, int loggedInUserId)
{
    Contact contact;
    int lastContactId = 0;
    string oneContactDataSeparatedByVerticalBars = "";
    string lastContactDataInFile = "";
    fstream textFile;
    textFile.open(contactsFileName.c_str(), ios::in);

    if (textFile.good() == true)
    {
        while (getline(textFile, oneContactDataSeparatedByVerticalBars))
        {
            if(loggedInUserId == getUserIdFromDataSeparatedByVerticalBars(oneContactDataSeparatedByVerticalBars))
            {
                contact = getContactData(oneContactDataSeparatedByVerticalBars);
                contacts.push_back(contact);
            }
        }
        lastContactDataInFile = oneContactDataSeparatedByVerticalBars;
    }
    else
        cout << "Nie udalo sie otworzyc pliku i wczytac danych." << endl;

    textFile.close();

    if (lastContactDataInFile != "")
    {
        lastContactId = getContactIdFromDataSeparatedByVerticalBars(lastContactDataInFile);
        return lastContactId;
    }
    else
        return 0;
}

void displayNumberOfSearchedContacts(int numberOfContacts)
{
    if (numberOfContacts == 0)
        cout << endl << "W ksiazce adresowej nie ma adresatow z tymi danymi." << endl;
    else
        cout << endl << "Ilosc adresatow w ksiazce adresowej wynosi: " << numberOfContacts << endl << endl;
}

void searchContactsByName(vector <Contact> &contacts)
{
    string searchedContactName = "";
    int numberOfContacts = 0;

    system("cls");
    if (!contacts.empty())
    {
        cout << ">>> WYSZUKIWANIE ADRESATOW O IMIENIU <<<" << endl << endl;

        cout << "Wyszukaj adresatow o imieniu: ";
        searchedContactName = loadLine();
        searchedContactName = changeFirstLetterToUpperCaseAndOthersToLowerCase(searchedContactName);

        for (vector <Contact>::iterator  itr = contacts.begin(); itr != contacts.end(); itr++)
        {
            if (itr -> firstName == searchedContactName)
            {
                displayContactData(*itr);
                numberOfContacts++;
            }
        }
        displayNumberOfSearchedContacts(numberOfContacts);
    }
    else
    {
        cout << endl << "Ksiazka adresowa jest pusta" << endl << endl;
    }
    cout << endl;
    system("pause");
}

void searchContactsBySurname(vector <Contact> &contacts)
{
    string searchedContactSurname;
    int numberOfContacts = 0;

    system("cls");
    if (!contacts.empty())
    {
        cout << ">>> WYSZUKIWANIE ADRESATOW O NAZWISKU <<<" << endl << endl;

        cout << "Wyszukaj adresatow o nazwisku: ";
        searchedContactSurname = loadLine();
        searchedContactSurname = changeFirstLetterToUpperCaseAndOthersToLowerCase(searchedContactSurname);

        for (vector <Contact>::iterator itr = contacts.begin(); itr != contacts.end(); itr++)
        {
            if (itr -> lastName == searchedContactSurname)
            {
                displayContactData(*itr);
                numberOfContacts++;
            }
        }
         displayNumberOfSearchedContacts(numberOfContacts);
    }
    else
    {
        cout << endl << "Ksiazka adresowa jest pusta." << endl << endl;
    }
    cout << endl;
    system("pause");
}

void displayAllContacts(vector <Contact> &contacts)
{
    system("cls");
    if (!contacts.empty())
    {
        cout << "             >>> ADRESACI <<<" << endl;
        cout << "-----------------------------------------------" << endl;
        for (vector <Contact> :: iterator itr = contacts.begin(); itr != contacts.end(); itr++)
        {
            displayContactData(*itr);
        }
        cout << endl;
    }
    else
    {
        cout << endl << "Ksiazka adresowa jest pusta." << endl << endl;
    }
    system("pause");
}

void deleteFile(string fileNameWithExtension)
{
    if (remove(fileNameWithExtension.c_str()) == 0) {}
    else
        cout << "Nie udalo sie usunac pliku " << fileNameWithExtension << endl;
}

void changeFileName(string oldName, string newName)
{
    if (rename(oldName.c_str(), newName.c_str()) == 0) {}
    else
        cout << "Nazwa pliku nie zostala zmieniona." << oldName << endl;
}

void deleteSelectedLineInFile(int numberDeletedLine)
{
    fstream readTextFile, temporaryTextFile;
    string loadedLine = "";
    int numberLoadedLine = 1;

    readTextFile.open(contactsFileName.c_str(), ios::in);
    temporaryTextFile.open(tempContactsFileName.c_str(), ios::out | ios::app);

    if (readTextFile.good() == true && numberDeletedLine != 0)
    {
        while (getline(readTextFile, loadedLine))
        {
            // There are so many cases because we want to achieve such an effect,
            // so that there is no empty line at the end of the file
            if (numberLoadedLine == numberDeletedLine) {}
            else if (numberLoadedLine == 1 && numberLoadedLine != numberDeletedLine)
                temporaryTextFile << loadedLine;
            else if (numberLoadedLine == 2 && numberDeletedLine == 1)
                temporaryTextFile << loadedLine;
            else if (numberLoadedLine > 2 && numberDeletedLine == 1)
                temporaryTextFile << endl << loadedLine;
            else if (numberLoadedLine > 1 && numberDeletedLine != 1)
                temporaryTextFile << endl << loadedLine;
            numberLoadedLine++;
        }
        readTextFile.close();
        temporaryTextFile.close();

        deleteFile(contactsFileName);
        changeFileName(tempContactsFileName, contactsFileName);
    }
}

void editSelectedLineInFile(int numberEditedLine, string lineWithContactDataSeparatedByVerticalBars)
{
    fstream readTextFile, temporaryTextFile;
    string loadedLine = "";
    int numberLoadedLine = 1;

    readTextFile.open(contactsFileName.c_str(), ios::in);
    temporaryTextFile.open(tempContactsFileName.c_str(), ios::out | ios::app);

    if (readTextFile.good() == true)
    {
        while (getline(readTextFile, loadedLine))
        {
            if (numberLoadedLine == numberEditedLine)
            {
                if (numberLoadedLine == 1)
                    temporaryTextFile << lineWithContactDataSeparatedByVerticalBars;
                else if (numberLoadedLine > 1)
                    temporaryTextFile << endl << lineWithContactDataSeparatedByVerticalBars;
            }
            else
            {
                if (numberLoadedLine == 1)
                    temporaryTextFile << loadedLine;
                else if (numberLoadedLine > 1)
                    temporaryTextFile << endl << loadedLine;
            }
            numberLoadedLine++;
        }
        readTextFile.close();
        temporaryTextFile.close();

        deleteFile(contactsFileName);
        changeFileName(tempContactsFileName, contactsFileName);
    }
}

int returnLineNumberSearchedContact(int contactId)
{
    bool doesContactExist = false;
    int lineNumberInTextFile = 1;
    string oneContactDataSeparatedByVerticalBars = "";
    fstream textFile;
    textFile.open(contactsFileName.c_str(), ios::in);

    if (textFile.good() == true && contactId != 0)
    {
        while(getline(textFile, oneContactDataSeparatedByVerticalBars))
        {
            if(contactId == getContactIdFromDataSeparatedByVerticalBars(oneContactDataSeparatedByVerticalBars))
            {
                doesContactExist = true;
                textFile.close();
                return lineNumberInTextFile;
            }
            else
                lineNumberInTextFile++;
        }
        if (doesContactExist = false)
        {
            textFile.close();
            return 0;
        }
    }
    return 0;
}

int enterSelectedContactId()
{
    int selectedContactId = 0;
    cout << "Podaj numer ID Adresata: ";
    selectedContactId  = loadInteger();
    return selectedContactId;
}

int getLastContactIdFromFile()
{
    int lastContactId = 0;
    string oneContactDataSeparatedByVerticalBars = "";
    string lastContactDataInFile = "";
    fstream textFile;
    textFile.open(contactsFileName.c_str(), ios::in);

    if (textFile.good() == true)
    {
        while (getline(textFile, oneContactDataSeparatedByVerticalBars)) {}
            lastContactDataInFile = oneContactDataSeparatedByVerticalBars;
            textFile.close();
    }
    else
        cout << "Nie udalo sie otworzyc pliku i wczytac danych." << endl;

    if (lastContactDataInFile != "")
    {
        lastContactId = getContactIdFromDataSeparatedByVerticalBars(lastContactDataInFile);
    }
    return lastContactId;
}

int getLastContactIdAfterDeletingSelectedContact(int deletedContactId, int lastContactId)
{
    if (deletedContactId == lastContactId)
        return getLastContactIdFromFile();
    else
        return lastContactId;
}

int deleteContact(vector <Contact> &contacts)
{
    int deletedContactId = 0;
    int numberDeletedLine = 0;

    system("cls");
    cout << ">>> USUWANIE WYBRANEGO ADRESATA <<<" << endl << endl;
    deletedContactId = enterSelectedContactId();

    char character;
    bool doesContactExist = false;

    for (vector <Contact>::iterator itr = contacts.begin(); itr != contacts.end(); itr++)
    {
        if (itr -> id == deletedContactId)
        {
            doesContactExist = true;
            cout << endl << "Potwierdz naciskajac klawisz 't': ";
            character = loadCharacter();
            if (character == 't')
            {
                numberDeletedLine = returnLineNumberSearchedContact(deletedContactId);
                deleteSelectedLineInFile(numberDeletedLine);
                contacts.erase(itr);
                cout << endl << endl << "Szukany adresat zostal USUNIETY" << endl << endl;
                system("pause");
                return deletedContactId;
            }
            else
            {
                cout << endl << endl <<  "Wybrany adresat NIE zostal usuniety" << endl << endl;
                system("pause");
                return 0;
            }
        }
    }
    if (doesContactExist == false)
    {
        cout << endl << "Nie ma takiego adresata w ksiazce adresowej" << endl << endl;
        system("pause");
    }
    return 0;
}

void updateDataOfSelectedContact(Contact contact, int editedContactId)
{
    int numberEditedLine = 0;
    string lineWithContactData = "";

    numberEditedLine = returnLineNumberSearchedContact(editedContactId);
    lineWithContactData = convertContactDataToLineWithVerticalBars(contact);
    editSelectedLineInFile(numberEditedLine, lineWithContactData);

    cout << endl << "Dane zostaly zaktualizowane." << endl << endl;
}

char chooseOptionFromEditMenu()
{
    char choice;

    cout << endl << "   >>> MENU  EDYCJA <<<" << endl;
    cout << "---------------------------" << endl;
    cout << "Ktore dane zaktualizowac: " << endl;
    cout << "1 - Imie" << endl;
    cout << "2 - Nazwisko" << endl;
    cout << "3 - Numer telefonu" << endl;
    cout << "4 - Email" << endl;
    cout << "5 - Adres" << endl;
    cout << "6 - Powrot " << endl;
    cout << endl << "Twoj wybor: ";
    choice = loadCharacter();

    return choice;
}

void editContact(vector <Contact> &contacts)
{
    system("cls");
    Contact contact;
    int editedContactId = 0;
    int numberEditedLine = 0;
    string lineWithContactData = "";

    cout << ">>> EDYCJA WYBRANEGO ADRESATA <<<" << endl << endl;
    editedContactId = enterSelectedContactId();

    char choice;
    bool doesContactExist = false;

    for (int i = 0; i < contacts.size(); i++)
    {
        if (contacts[i].id == editedContactId)
        {
            doesContactExist = true;
            choice = chooseOptionFromEditMenu();

            switch (choice)
            {
            case '1':
                cout << "Podaj nowe imie: ";
                contacts[i].firstName = loadLine();
                contacts[i].firstName = changeFirstLetterToUpperCaseAndOthersToLowerCase(contacts[i].firstName);
                updateDataOfSelectedContact(contacts[i], editedContactId);
                break;
            case '2':
                cout << "Podaj nowe nazwisko: ";
                contacts[i].lastName = loadLine();
                contacts[i].lastName = changeFirstLetterToUpperCaseAndOthersToLowerCase(contacts[i].lastName);
                updateDataOfSelectedContact(contacts[i], editedContactId);
                break;
            case '3':
                cout << "Podaj nowy numer telefonu: ";
                contacts[i].phoneNumber = loadLine();
                updateDataOfSelectedContact(contacts[i], editedContactId);
                break;
            case '4':
                cout << "Podaj nowy email: ";
                contacts[i].email = loadLine();
                updateDataOfSelectedContact(contacts[i], editedContactId);
                break;
            case '5':
                cout << "Podaj nowy adres zamieszkania: ";
                contacts[i].address = loadLine();
                updateDataOfSelectedContact(contacts[i], editedContactId);
                break;
            case '6':
                cout << endl << "Powrot do menu uzytkownika" << endl << endl;
                break;
            default:
                cout << endl << "Nie ma takiej opcji w menu! Powrot do menu uzytkownika." << endl << endl;
                break;
            }
        }
    }
    if (doesContactExist == false)
    {
        cout << endl << "Nie ma takiego adresata." << endl << endl;
    }
    system("pause");
}

char chooseOptionFromMainMenu()
{
    char choice;

    system("cls");
    cout << "    >>> MENU  GLOWNE <<<" << endl;
    cout << "---------------------------" << endl;
    cout << "1. Rejestracja" << endl;
    cout << "2. Logowanie" << endl;
    cout << "9. Koniec programu" << endl;
    cout << "---------------------------" << endl;
    cout << "Twoj wybor: ";
    choice = loadCharacter();

    return choice;
}

char chooseOptionFromUserMenu()
{
    char choice;

    system("cls");
    cout << " >>> MENU UZYTKOWNIKA <<<" << endl;
    cout << "---------------------------" << endl;
    cout << "1. Dodaj adresata" << endl;
    cout << "2. Wyszukaj po imieniu" << endl;
    cout << "3. Wyszukaj po nazwisku" << endl;
    cout << "4. Wyswietl adresatow" << endl;
    cout << "5. Usun adresata" << endl;
    cout << "6. Edytuj adresata" << endl;
    cout << "---------------------------" << endl;
    cout << "7. Zmien haslo" << endl;
    cout << "8. Wyloguj sie" << endl;
    cout << "---------------------------" << endl;
    cout << "Twoj wybor: ";
    choice = loadCharacter();

    return choice;
}

int main()
{
    vector <User> users;
    vector <Contact> contacts;

    int loggedInUserId = 0;
    int lastContactId = 0;
    int deletedContactId = 0;
    char choice;

    loadUsersFromFile(users);

    while (true)
    {
        if (loggedInUserId == 0)
        {
            choice = chooseOptionFromMainMenu();

            switch (choice)
            {
            case '1':
                registerUser(users);
                break;
            case '2':
                loggedInUserId = loginUser(users);
                break;
            case '9':
                exit(0);
                break;
            default:
                cout << endl << "Nie ma takiej opcji w menu." << endl << endl;
                system("pause");
                break;
            }
        }
        else
        {

            if (contacts.empty() == true)
                lastContactId = loadContactsOfLoggedInUserFromFile(contacts, loggedInUserId);

            choice = chooseOptionFromUserMenu();

            switch (choice)
            {
            case '1':
                lastContactId = addContact(contacts, loggedInUserId, lastContactId);
                break;
            case '2':
                searchContactsByName(contacts);
                break;
            case '3':
                searchContactsBySurname(contacts);
                break;
            case '4':
                displayAllContacts(contacts);
                break;
            case '5':
                deletedContactId = deleteContact(contacts);
                lastContactId = getLastContactIdAfterDeletingSelectedContact(deletedContactId, lastContactId);
                break;
            case '6':
                editContact(contacts);
                break;
            case '7':
                changePasswordOfLoggedInUser(users, loggedInUserId);
                break;
            case '8':
                loggedInUserId = 0;
                contacts.clear();
                break;
            }
        }
    }
return 0;
}
