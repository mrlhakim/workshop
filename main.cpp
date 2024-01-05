#include <iostream>
#include <string>
#include <conio.h>
#include <mysql.h> //libmysql
#include <iomanip> 
#include <random>
#include <ctime>
#include <cstdlib>
#include <string>
#include <cmath>
#include <vector>
#include <cstdio>
#include <random>
using namespace std;

void login();
void menu();
void menuAsset();
void disAsset();
void newAsset();
void disAdmin();
void menuSetting();
void newAdmin();
void menuInvois();
void disInvois();
void newInvois();
void upAsset();
void exitMenu();
void exitAsset();

vector<string> Brand;

int result;
MYSQL* conn;
MYSQL_ROW row;
MYSQL_RES* res;

string generateRandomString(int length) {
    const char characters[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz";
    const int charactersLength = sizeof(characters) - 1;

    string randomString;

    for (int i = 0; i < length; i++) {
        randomString += characters[rand() % charactersLength];
    }

    return randomString;
}

string generateRandomUpper(int length) {
    const char characters[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
    const int charactersLength = sizeof(characters) - 1;

    string randomString;

    for (int i = 0; i < length; i++) {
        randomString += characters[rand() % charactersLength];
    }

    return randomString;
}

string generateRandomNumber(int length) {
    const char characters[] = "0123456789";
    const int charactersLength = sizeof(characters) - 1;

    string randomNumber;

    for (int i = 0; i < length; i++) {
        randomNumber += characters[rand() % charactersLength];
    }

    return randomNumber;
}

class db
{
public:
    static void ConnectionFunction()
    {
        conn = mysql_init(0);
        if (conn) {
            cout << "Database Connected" << endl;
            conn = mysql_real_connect(conn, "localhost", "root", "", "workshop", 3306, NULL, 0);
        }
        else {
            cout << "Failed To Connect!" << endl;
        }

        if (conn) {
            cout << "Database Connected To MySql \n\n" << endl;
        }
        else {
            cout << "Failed To Connect!" << endl;
        }

    }
};

int main() {
    system("cls");
    system("title My Project");
    db::ConnectionFunction();
    login();

    return 0;
}

void login() {
    system("cls");
    string inputUser;
    string inputPass;
    string sql;
    string result;

    cout << "----- Login ----- \n";

    cout << "Enter your username: ";
    cin >> inputUser;

    cout << "Enter your password: ";
    char ch;

    while ((ch = _getch()) != 13) {
        inputPass += ch;
        cout << '*';
    }

    sql = "SELECT username, password FROM acc_main WHERE username = '" + inputUser + "' AND password = '" + inputPass + "'";
    const char* sqlc = sql.c_str();
    result = mysql_query(conn, sqlc);

    if (result != "") {
        res = mysql_store_result(conn);
        if (res != nullptr) {
            if (mysql_num_rows(res) == 1) {
                cout << "Login successful! \n\n" << endl;
                menu();
                // Proceed to the next step after successful login
            }
            else {
                cout << "Oops, your username or password is invalid!" << endl;
                login(); // Recursive call to login again
            }
        }
    }
    else {
        cout << "Query execution problem: " << mysql_error(conn) << endl;
    }
}

void menu() {
    system("cls");
    int x;
    cout << "----- Main Menu -----\n\n";
    cout << "1. Asset\n\n";
    cout << "2. Invois\n\n";
    cout << "3. setting\n\n";
    cout << "4. Logout\n\n";

    cout << "Choose your number: ";
    cin >> x;
    cout << "\n\n";

    switch (x) {
        case 1:
            menuAsset();
            break;
        case 2:
            menuInvois();
            break;
        case 3: 
            menuSetting();
            break;
        case 4:
            login();
            break;
        default:
            cout << "Please choose between 1 - 4. Press Enter To Continue...";
            system("cls");
            menu();
            break;
    }
}

void exitMenu() {
    string input;
    cout << "\n\n";
    cout << "Press x to menu asset: ";
    cin >> input;

    if (input == "x") {
        menu();
    }
    else {
        menu();
    }
}

void exitAsset() {
    string input;
    cout << "\n\n";
    cout << "Press x to menu asset: ";
    cin >> input;

    if (input == "x") {
        menuAsset();
    }
    else {
        menu();
    }
}

//asset
void menuAsset() {
    system("cls");
    int u;
    cout << "----- Asset page -----\n\n";
    cout << "1. Display Devices\n\n";
    cout << "2. Add New Devices\n\n";
    cout << "3. Update Devices\n\n";
    cout << "4. Main Menu\n\n";

    cout << "Choose your number: ";
    cin >> u;
    cout << "\n\n";

    switch (u) {
    case 1:
        disAsset();
        break;
    case 2:
        newAsset();
        break;
    case 3:
        upAsset();
        break;
    case 4:
        menu();
        break;
    default:
        cout << "Please choose between 1 - 4. Press Enter To Continue...";
        system("cls");
        menu();
        break;
    }
}

void disAsset() {
    system("cls");
    int y = 1;
    string sql;
    string result;

    sql = "SELECT a.model, a.serial, a.color, a.year, b.brand, c.category, d.fName, e.type, a.logDate, a.checkIn FROM asset_main a INNER JOIN asset_brand b ON a.brandID = b.id INNER JOIN asset_category c ON a.categoryID = c.id INNER JOIN acc_main d ON a.userID = d.id INNER JOIN asset_status e ON a.status = e.id WHERE e.status = 1 ORDER BY a.checkin DESC";
    const char* sqlc = sql.c_str();
    result = mysql_query(conn, sqlc);

    if (result != "") {
        cout << endl;
        cout << "Recent Activity" << endl;
        cout << "===============" << endl;
        cout << left << setw(5) << "#" << left << setw(30) << "Model" << left << setw(20) << "\tSerial" << left << setw(15) << "\tCategory" << left << setw(15) << "Color" << left << setw(10) << "Year" << left << setw(20) << "Staff" << left << setw(15) << "Status" << left << setw(30) << "Date" << endl;
        cout << "---------------------------------------------------------------------------------------------------------------------------------------------------------------" << endl;

        res = mysql_store_result(conn);
        while (row = mysql_fetch_row(res)) {
            string model = row[0];
            string brand = row[4];
            string serial = row[1];
            string category = row[5];
            string color = row[2];
            string year = row[3];
            string fname = row[6];
            string status = row[7];
            if (status.find("Check Out") != string::npos) {
                status = "Available";
            }

            string logdate = row[8];
            string checkin = row[9];
            string newDate = (checkin.find("0000-00-00 00:00:00") != string::npos) ? logdate : checkin;

            cout << left << setw(5) << + y << left << setw(30) << brand + " " + model << left << setw(20) << "\t" + serial << left << setw(15) << "\t" + category << left << setw(15) << color << left << setw(10) << year << left << setw(20) << fname << left << setw(15) << status << left << setw(30) << newDate << endl;
            cout << "---------------------------------------------------------------------------------------------------------------------------------------------------------------" << endl;
            cout << endl;
            y++;
        }
    }
    exitAsset();
}

void newAsset() {
    system("cls");
    string brand, model, serial, category, color, year, sql, sql2, sql3, sql4, result, result3, result4, close;
    int numColumns = 5;
    int counter = 0;

    cout << "\n----- Add New Devices -----\n\n";

    sql2 = "SELECT id,category FROM asset_category WHERE status = 1";
    const char* sqls = sql2.c_str();
    result = mysql_query(conn, sqls);

    if (result != "") {
        res = mysql_store_result(conn);
        while (row = mysql_fetch_row(res)) {
            string categoryID = row[0];
            string cat = row[1];

            if (counter != 0 && counter % numColumns == 0) {
                cout << endl;
            }

            cout << left << setw(5) << "|" + categoryID + "." << left << setw(10) << cat;
            counter++;
        }
        cout << endl;
    }

    cout << "\n";

    cout << "Category: ";
    cin.ignore(1, '\n');
    getline(cin, category);

    cout << "\n";

    sql = "SELECT id,brand FROM asset_brand WHERE status = 1";
    const char* sqlc = sql.c_str();
    result = mysql_query(conn, sqlc);

    if (result != "") {
        res = mysql_store_result(conn);
        while (row = mysql_fetch_row(res)) {
            string brandId = row[0];
            string brands = row[1];

            if (counter != 0 && counter % numColumns == 0) {
                cout << endl;
            }

            cout << left << setw(5) << "|" + brandId + "." << left << setw(10) << brands;
            counter++;
        }
        cout << endl;
    }

    cout << "\n";

    cout << "Brand: ";
    getline(cin, brand);

    cout << "Model: ";
    getline(cin, model);

    cout << "Serial No.: ";
    getline(cin, serial);

    cout << "Color: ";
    getline(cin, color);

    cout << "Year: ";
    getline(cin, year);

    cout << "\n";

    srand(static_cast<unsigned int>(time(nullptr)));
    string ranHash = generateRandomNumber(6);

    sql4 = "SELECT hash FROM asset_main WHERE hash = '" + ranHash + "'";
    const char* sqlv = sql4.c_str();
    result4 = mysql_query(conn, sqlv);

    if (result4 != "") { // Check if the query was executed successfully
        res = mysql_store_result(conn);
        if (res != nullptr) {
            if (mysql_num_rows(res) == 1) {
                sql3 = "INSERT INTO asset_main SET hash = '" + ranHash + "', model = '" + model + "', serial = '" + serial + "', brandID = '" + brand + "', categoryID = '" + category + "', color = '" + color + "', userID = '20', assign = NULL, year = '" + year + "', status = '3', logdate = NOW(), checkIn = 'EMPTY'";
                const char* sqlE = sql3.c_str();
                result3 = mysql_query(conn, sqlE);

                if (result3 != "") {
                    sql = "SELECT brand FROM asset_brand WHERE id = '" + brand + "'";
                    const char* sqlH = sql.c_str();
                    result = mysql_query(conn, sqlH);

                    if (result != "") {
                        res = mysql_store_result(conn);
                        while (row = mysql_fetch_row(res)) {
                            string brandId = row[0];

                            cout << "Brand: " << brandId << endl;
                            cout << "Model: " << model << endl;
                            cout << "Serial No.: " << serial << endl;
                            cout << "Category: " << category << endl;
                            cout << "Color: " << color << endl;
                            cout << "Year: " << year << endl;
                            cout << "\n" << endl;
                        }
                    }
                    cout << "Press x to exit: ";
                    getline(cin, close);
                    if (close == "x") {
                        menu();
                    }
                }
                else {
                    cout << "Query Execution Problem!" << mysql_errno(conn) << endl;
                }
            }
            else {
                sql3 = "INSERT INTO asset_main SET hash = '" + ranHash + "', model = '" + model + "', serial = '" + serial + "', brandID = '" + brand + "', categoryID = '" + category + "', color = '" + color + "', userID = '20', assign = NULL, year = '" + year + "', status = '1', logdate = NOW(), checkIn = 'EMPTY'";
                const char* sqlE = sql3.c_str();
                result3 = mysql_query(conn, sqlE);

                if (result3 != "") {
                    sql = "SELECT brand FROM asset_brand WHERE id = '" + brand + "'";
                    const char* sqlH = sql.c_str();
                    result = mysql_query(conn, sqlH);

                    if (result != "") {
                        res = mysql_store_result(conn);
                        while (row = mysql_fetch_row(res)) {
                            string brandId = row[0];

                            cout << "Brand: " << brandId << endl;
                            cout << "Model: " << model << endl;
                            cout << "Serial No.: " << serial << endl;
                            cout << "Category: " << category << endl;
                            cout << "Color: " << color << endl;
                            cout << "Year: " << year << endl;
                            cout << "\n" << endl;
                        }
                    }
                    cout << "Press x to exit: ";
                    getline(cin, close);
                    if (close == "x") {
                        menu();
                    }
                }
                else {
                    cout << "Query Execution Problem!" << mysql_errno(conn) << endl;
                }
            }
        }
    }
    else {
        cout << "Query execution problem: " << mysql_error(conn) << endl;
    }
}

void upAsset() {
    system("cls");
    int y = 1;
    string input1;
    string sql, sql2, sql3, sql4, sql5, sql6, sql7;
    string result, result2, result3, result4, result5, result6, result7;

    sql = "SELECT a.model, a.serial, a.color, a.year, b.brand, c.category, d.fName, e.type, a.hash FROM asset_main a INNER JOIN asset_brand b ON a.brandID = b.id INNER JOIN asset_category c ON a.categoryID = c.id INNER JOIN acc_main d ON a.userID = d.id INNER JOIN asset_status e ON a.status = e.id WHERE e.status = 1";
    const char* sqlc = sql.c_str();
    result = mysql_query(conn, sqlc);

    if (result != "") {
        cout << left << setw(5) << "#" << left << setw(15) << "UniqueID" << left << setw(30) << "Model" << left << setw(20) << "\tSerial" << left << setw(15) << "\tCategory" << left << setw(15) << "Color" << left << setw(15) << "Year" << left << setw(15) << "Staff" << left << setw(15) << "Status" << endl;

        res = mysql_store_result(conn);
        while (row = mysql_fetch_row(res)) {
            string model = row[0];
            string brand = row[4];
            string serial = row[1];
            string category = row[5];
            string color = row[2];
            string year = row[3];
            string fname = row[6];
            string status = row[7];
            string hasha = row[8];
            if (status.find("Check Out") != string::npos) {
                status = "Available";
            }
            cout << left << setw(5) << y << left << setw(15) << hasha << left << setw(30) << brand + " " + model << left << setw(20) << "\t" + serial << left << setw(15) << "\t" + category << left << setw(15) << color << left << setw(15) << year << left << setw(20) << fname << left << setw(15) << status << endl;
            y++;
        }

        sql3 = "SELECT hash FROM asset_main";
        const char* sqlb = sql3.c_str();
        result3 = mysql_query(conn, sqlb);

        if (result3 != "") {
            cout << "\n\n";
            cout << "Enter UniqueID: ";
            cin >> input1;
            res = mysql_store_result(conn);
            while (row = mysql_fetch_row(res)) {
                string unik = row[0];
                if (input1 == unik) {
                    system("cls");
                    int inChose = 0;
                    sql2 = "SELECT a.model, a.serial, a.color, a.year, b.brand, c.category, d.fName, e.type, a.hash FROM asset_main a INNER JOIN asset_brand b ON a.brandID = b.id INNER JOIN asset_category c ON a.categoryID = c.id INNER JOIN acc_main d ON a.userID = d.id INNER JOIN asset_status e ON a.status = e.id WHERE e.status = 1 AND a.hash = '" + input1 + "'";
                    const char* sqlv = sql2.c_str();
                    result2 = mysql_query(conn, sqlv);

                    if (result2 != "") {
                        res = mysql_store_result(conn);
                        while (row = mysql_fetch_row(res)) {
                            string data1 = row[0];
                            string data2 = row[1];
                            string data3 = row[2];
                            string data4 = row[3];
                            string data5 = row[4];
                            string data6 = row[5];
                            string data7 = row[6];
                            string data8 = row[7];
                            string data9 = row[8];
                            if (data8.find("Check Out") != string::npos) {
                                data8 = "Available";
                            }
                            cout << "----- Device ID #" + data9 + " -----" << endl;
                            cout << "Brand: " << data5 << endl;
                            cout << "Model: " << data1 << endl;
                            cout << "Serial: " << data2 << endl;
                            cout << "Category: " << data6 << endl;
                            cout << "Color: " << data3 << endl;
                            cout << "Year: " << data4 << endl;
                            cout << "Staff: " << data7 << endl;
                            cout << "Status: " << data8 << endl;
                        }
                        cout << endl;
                        cout << "===============================" << endl;
                        cout << "1. Edit Staff" << endl;
                        cout << "2. Edit Status" << endl;
                        cout << "3. Exit" << endl;
                        cout << endl;
                        cout << "Select option: ";
                        cin >> inChose;

                        if (inChose == 1) {
                            cout << "Tak buat lagi hihi!!";
                        }
                        else if(inChose == 2) {
                            string inStatus;
                            sql4 = "SELECT id, type FROM asset_status WHERE status = 1";
                            const char* sqln = sql4.c_str();
                            result4 = mysql_query(conn, sqln);
                            if (result4 != "") {
                                cout << endl;
                                cout << "===============================" << endl;
                                cout << left << setw(5) << "#" << left << setw(15) << "Status" << endl;

                                res = mysql_store_result(conn);
                                while (row = mysql_fetch_row(res)) {
                                    string id = row[0];
                                    string type = row[1];
                                    cout << left << setw(5) << id << left << setw(15) << type << endl;
                                }
                                mysql_free_result(res);

                                cout << endl;
                                cout << "Select status: ";
                                cin >> inStatus;

                                if (inStatus == "2" || inStatus == "4") {
                                    sql5 = "SELECT id, fname, dept FROM `acc_main` WHERE status = 1 AND auth = 2;";
                                    const char* sqlm = sql5.c_str();
                                    result5 = mysql_query(conn, sqlm);

                                    if (result5 != "") {
                                        res = mysql_store_result(conn);

                                        cout << "===============================" << endl;
                                        cout << left << setw(5) << "#" << left << setw(25) << "Staff Name" << left << setw(15) << "Department" << endl;\
                                        
                                        while (row = mysql_fetch_row(res)) {
                                            string id = row[0];
                                            string name = row[1];
                                            string dept = row[2];
                                            cout << left << setw(5) << id + "." << left << setw(25) << name << left << setw(15) << dept << endl;
                                        }
                                       
                                        cout << endl;
                                        string inUser;
                                        cout << "Enter userID: ";
                                        cin >> inUser;

                                        mysql_data_seek(res, 0);

                                        while ((row = mysql_fetch_row(res)) != NULL) {
                                            string id = row[0];
                                            string name = row[1];

                                            if (inUser == id) {
                                                sql7 = "UPDATE asset_main SET status = '" + inStatus + "', userID = '" + id + "', checkIn = NOW() WHERE hash = '" + input1 + "'";
                                                const char* sqlh = sql7.c_str();
                                                result7 = mysql_query(conn, sqlh);

                                                if (result7 != "") {
                                                    if (inStatus == "2") {
                                                        cout << "Check in success!" << endl;
                                                        cout << "Staff name: " + name << endl;
                                                        exitAsset();
                                                    }
                                                    if (inStatus == "4") {
                                                        cout << "Request success!" << endl;
                                                        cout << "Staff name: " + name << endl;
                                                        exitAsset();
                                                    }
                                                }
                                            }
                                        }
                                        mysql_free_result(res);
                                    }
                                }
                                else if (inStatus == "3" || inStatus == "5" || inStatus == "6") {
                                    sql6 = "UPDATE asset_main SET status = '" + inStatus + "', checkIn = NOW() WHERE hash = '" + input1 + "'";
                                    const char* sqlk = sql6.c_str();
                                    result6 = mysql_query(conn, sqlk);

                                    if (result6 != "") {
                                        if (inStatus == "2") {
                                            cout << "Check in success!" << endl;
                                            exitAsset();
                                        }
                                        else if (inStatus == "3") {
                                            cout << "Check Out success!" << endl;
                                            exitAsset();
                                        }
                                        else if (inStatus == "5") {
                                            cout << "Broken / Lost Updated!" << endl;
                                            exitAsset();
                                        }
                                        else if (inStatus == "6") {
                                            cout << "Delete Successfully!" << endl;
                                            exitAsset();
                                        }
                                    }
                                    else {
                                        cout << "Query 6 Problem!" << endl;
                                    }
                                }
                                else {
                                    cout << inStatus + " Data Not Found!" << endl;
                                    exitAsset();
                                }
                            }
                        }
                        else if (inChose == 3){
                            menuAsset();
                        }
                    }
                    else {
                        cout << "Query 2 Problem!" << mysql_errno(conn) << endl;
                    }
                }
                else {
                    system("cls");
                    cout << "Salah tu!";
                }
            }
        }
        else {
            cout << "Query 3 Problem!" << mysql_errno(conn) << endl;
        }
    }
    else {
        cout << "Query 1 Problem!" << mysql_errno(conn) << endl;
    }
}


//admin
void menuSetting() {
    system("cls");
    int u;
    cout << "----- Setting page -----\n\n";
    cout << "1. Display admin and user\n\n";
    cout << "2. Add New Admin or User\n\n";
    cout << "3. Update Admin\n\n";
    cout << "4. Main Menu\n\n";

    cout << "Choose your number: ";
    cin >> u;
    cout << "\n\n";

    switch (u) {
    case 1:
        disAdmin();
        break;
    case 2:
        newAdmin();
        break;
    case 3:
        //updateSetting();
        break;
    case 4:
        menu();
        break;
    default:
        cout << "Please choose between 1 - 4. Press Enter To Continue...";
        system("cls");
        menu();
        break;
    }
}

void disAdmin() {
    system("cls");
    int d = 1;

    int y = 1;
    string sql, sql2, result;
    string close;

    //display admin

    sql = "SELECT username, fname, nName, dept, email, phone FROM `acc_main` WHERE status = 1 AND auth = 1;";
    const char* sqlc = sql.c_str();
    result = mysql_query(conn, sqlc);

    if (result != "") {
        cout << "----- Admin ----- \n\n";
        cout << left << setw(5) << "#" << left << setw(10) << "Username" << left << setw(30) << "Fullname" << left << setw(15) << "Nickname" << left << setw(20) << "Department" << left << setw(20) << "Email" << left << setw(15) << "Phone" << endl;

        res = mysql_store_result(conn);
        while (row = mysql_fetch_row(res)) {
            string username = row[0];
            string fname = row[1];
            string nName = row[2];
            string dept = row[3];
            string email = row[4];
            string phone = row[5];
            cout << left << setw(5) << d << left << setw(10) << username << left << setw(30) << fname << left << setw(15) << nName << left << setw(20) << dept << left << setw(20) << email << left << setw(15) << phone << endl;
            d++;
        }
        cout << "\n\n";
    }
    else {
        cout << "Query Execution Problem!" << mysql_errno(conn) << endl;
    }

    //display user

    sql2 = "SELECT fname, nName, dept, email, phone FROM `acc_main` WHERE status = 1 AND auth = 2;";
    const char* sqlc2 = sql2.c_str();
    result = mysql_query(conn, sqlc2);

    if (result != "") {
        cout << "----- User ----- \n\n";
        cout << left << setw(5) << "#" << left << setw(20) << "Fullname" << left << setw(20) << "Nickname" << left << setw(20) << "Department" << left << setw(20) << "Email" << left << setw(15) << "Phone" << endl;

        res = mysql_store_result(conn);
        while (row = mysql_fetch_row(res)) {
            string fname = row[0];
            string nName = row[1];
            string dept = row[2];
            string email = row[3];
            string phone = row[4];
            cout << left << setw(5) << y << left << setw(20) << fname << left << setw(20) << nName << left << setw(20) << dept << left << setw(20) << email << left << setw(15) << phone << endl;
            y++;
        }
    }
    else {
        cout << "Query Execution Problem!" << mysql_errno(conn) << endl;
    }
    exitMenu();
}

void newAdmin() {
    system("cls");
    string input1, input2, input3, input4, input5, input6, username1, username2, ranPass2, close;
    string sql, sql2;
    string result;
    cout << "\n----- New admin or user -----\n";

    cout << "Fullname: ";
    cin.ignore(1, '\n');
    getline(cin, input1);

    cout << "Nickname: ";
    getline(cin, input2);

    cout << "Email: ";
    getline(cin, input3);

    cout << "Phone No.: ";
    getline(cin, input4);

    cout << "Department: ";
    getline(cin, input5);

    cout << "Select authenticator:\n";
    cout << "1. Admin\n"; 
    cout << "2. Staff\n";
    cout << "Authenticator: ";
    getline(cin, input6);

    cout << "\n";

    for (int i = 0; i < 11; i++) {
        if (i > 0) {
            username1 = to_string(i) + "_" + input2;
        }
        else {
            username1 = input2;
        }
        sql = "SELECT username FROM acc_main WHERE username = '" + username1 + "'";
        const char* sqlc = sql.c_str();
        result = mysql_query(conn, sqlc);

        res = mysql_store_result(conn);
        if (res -> row_count == 0) {
            break;
        }
    }

    srand(static_cast<unsigned int>(time(nullptr)));
    string randomString = generateRandomString(3);
    string randomNumber = generateRandomNumber(3);
    string ranPass = randomString + "" + randomNumber;

    if (input6 == "1") {
        username2 = username1;
        ranPass2 = ranPass;
    }
    else if (input6 == "2") {
        username2 = "";
        ranPass2 = "";
    }
  
    sql2 = "INSERT INTO acc_main SET username = '" + username2 + "', fname = '" + input1 + "', nName = '" + input2 + "', email = '" + input3 + "', password = '" + ranPass2 + "', dept = '" + input5 + "', auth = '" + input6 + "', status = '1', logdate = NOW(), phone = '" + input4 + "'";
    const char* sqls = sql2.c_str();
    result = mysql_query(conn, sqls);

    if (result != "") {
        if (input6 == "1") {
            cout << "Username and Temporary password " << endl;
            cout << "Username: " << username2 << endl;
            cout << "Password: " << ranPass2 << endl;
            cout << "\n" << endl;\
            cout << "Press x to exit: ";
            getline(cin, close);
            if (close == "x") {
                menu();
            }
        }
        else if (input6 == "2") {
            disAdmin();
        }
    }
    else {
        cout << "Query Execution Problem!" << mysql_errno(conn) << endl;
    }
}

//Invois
void menuInvois() {
    system("cls");
    int u;
    cout << "----- Setting page -----\n\n";
    cout << "1. Display Invois\n\n";
    cout << "2. Add New Invois\n\n";
    cout << "3. Update Devices\n\n";
    cout << "4. Main Menu\n\n";

    cout << "Choose your number: ";
    cin >> u;
    cout << "\n\n";

    switch (u) {
    case 1:
        disInvois();
        break;
    case 2:
        newInvois();
        break;
    case 3:
        //updateSetting();
        break;
    case 4:
        menu();
        break;
    default:
        cout << "Please choose between 1 - 4. Press Enter To Continue...";
        system("cls");
        menu();
        break;
    }
}

void disInvois() {
    system("cls");
}

void newInvois() {
    system("cls");
    int input3;
    double input4, tprice = 0.0;
    string input1, input2, close, y, item;
    string sql, sql2;
    string result;

    do {
        cout << "\n----- New Invois -----\n";

        cout << "Brand: ";
        cin >> input1;

        cout << "Model: ";
        cin >> input2;

        cout << "Quantity: ";
        cin >> input3;

        cout << "Price: ";
        cin >> input4;

        cout << "----------\n";

        cout << "More Order ? (y/n)";
        cin >> y;

        tprice += input3 * input4;

    } while (y == "y");
        Brand.push_back(input1);
        cout << "Values in the array:" << endl;
        for (const string& str : Brand) {
            std::cout << str << " ";
        }
}