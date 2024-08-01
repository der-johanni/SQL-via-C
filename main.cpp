#include <sqlite3.h>
#include <iostream>
#include <string>
#include <vector>
#include <iomanip>
#include <string.h>

static int display_time_flag = 0;
std::vector<int> column_widths;

int call_back(void*, int, char**, char**);

int sql_add_note        (sqlite3*);
int sql_remove_note     (sqlite3*);
int sql_display_notes   (sqlite3*);

int main(){
    sqlite3 *db;
    int res_msg = sqlite3_open("test.db", &db);
    if (res_msg != SQLITE_OK){
        std::cout << "Something went wrong" << std::endl;
        sqlite3_close(db);
        return 1;
    }

    std::cout << "Test database. Choose what you want to do." << std::endl;
    std::cout << std::endl;
    while (1){
        std::string user_command;
        std::cout << "1. Add note into table    " << std::endl;
        std::cout << "2. Remove note from table " << std::endl;
        std::cout << "3. Browse data in table   " << std::endl;
        std::cout << "4. Exit" << std::endl;

        std::cout << "Command: ";
        std::cin >> user_command;
        int res;
        if (user_command == "1"){
            res = sql_add_note(db);
        } else if (user_command == "2"){
            res = sql_remove_note(db);
        } else if (user_command == "3"){
            res = sql_display_notes(db);
        } else if (user_command == "4"){
            break;
        } else std::cout << "Unknown command." << std::endl;
        if (res != SQLITE_OK) break;
    }

    sqlite3_close(db);
    return 0;
}

int callback(void *notUsed, int column_size, char **columns, char **column_names) {
    if (display_time_flag == 0) {
        column_widths.resize(column_size, 0);
        for (int i = 0; i < column_size; i++) {
            column_widths[i] = std::max(column_widths[i], (int)strlen(column_names[i]));
        }
    }
    for (int i = 0; i < column_size; i++) {
        column_widths[i] = std::max(column_widths[i], (int)strlen(columns[i]));
    }

    if (display_time_flag == 0) {
        for (int i = 0; i < column_size; i++) {
            std::cout << std::setw(column_widths[i] + 2) << column_names[i];
        }
        std::cout << std::endl;
        for (int i = 0; i < column_size; i++) {
            std::cout << std::setw(column_widths[i] + 2) << std::setfill('-') << "";
        }
        std::cout << std::setfill(' ') << std::endl;
        display_time_flag = 1;
    }

    for (int i = 0; i < column_size; i++) {
        std::cout << std::setw(column_widths[i] + 2) << columns[i];
    }
    std::cout << std::endl;
    return 0;
}
int sql_add_note        (sqlite3 *db){
    char *err_msg = 0;
    display_time_flag = 0;
    std::string name;
    std::string age;
    std::cout << "Enter the name: ";
    std::cin >> name;
    std::cout << "Enter the age: ";
    std::cin >> age;
    std::string sql_command = "INSERT INTO people (name, age) VALUES ('";
    sql_command += name;
    sql_command += "', ";
    sql_command += age;
    sql_command += ')';

    const char *c_sql_command = sql_command.c_str();
    int res_msg = sqlite3_exec(db, c_sql_command, 0, 0, &err_msg);
    if (res_msg != SQLITE_OK){
        std::cerr << "SQLite error: " << err_msg << std::endl;
        sqlite3_free(err_msg);
        return SQLITE_ERROR;
    }

    std::cout << "Successfully inserted." << std::endl;
    return SQLITE_OK;
}
int sql_display_notes   (sqlite3 *db){
    char *err_msg = 0;
    display_time_flag = 0;
    std::string sql_command = "SELECT * FROM people";
    const char *c_sql_command = sql_command.c_str();
    int res_msg = sqlite3_exec(db, c_sql_command, callback, 0, &err_msg);
    if (res_msg != SQLITE_OK){
        std::cout << "SQLite error: " << res_msg;
        sqlite3_free(err_msg);
        sqlite3_close(db);
        return SQLITE_ERROR;
    }
    return SQLITE_OK;
}
int sql_remove_note     (sqlite3 *db){
    char *err_msg = 0;
    display_time_flag = 0;
    std::string parameter;
    std::cout << "Enter ID you want to delete: ";
    std::cin >> parameter;

    std::string sql_command = "DELETE FROM people WHERE id='";
    sql_command += parameter;
    sql_command += "'";

    const char *c_sql_command = sql_command.c_str();

    int res_msg = sqlite3_exec(db, c_sql_command, 0, 0, &err_msg);
    if (res_msg != SQLITE_OK){
        std::cout << "SQL error: " << err_msg << std::endl;
        sqlite3_free(err_msg);
        sqlite3_close(db);
        return SQLITE_ERROR;
    }
    return SQLITE_OK;
}