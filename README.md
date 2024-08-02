### Description
  The CLI program to work with SQL database via command prompt.
### Current function
  - sql_add_note(). Add user data in table
  - sql_remove_note(). Remove note (Primary key (ID) should be known)
  - sql_display_notes() Print all notes in table
### Compiling
# OpenSUSE
```
sudo zypper install sqlite3 sqlite3-devel
g++ main.cpp -o app -lsqlite3
```

### Possible future
  Light-weight GUI and CLI database browser with SQL commands support.
