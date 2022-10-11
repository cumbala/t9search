/// 
/// IZP - t9search.c
///
/// Done by Konstantin Romanets (xroman18)
///
/// VUT FIT 2022
///

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define PB_SIZE 1024
#define BUF_SIZE 100

/// Will print error message and terminate program
/// @param msg Message to print
void RaiseError(char *msg) {
    printf("ERROR: %s\n", msg);
    exit(-1); 
}

/// Phonebook entry
/// @param Name Contact name
/// @param Number Contact's phone number
typedef struct TEntry {
    char Name[BUF_SIZE];
    long Number;
} Entry;

/// Fills provided name and number to @p ent
/// @param ent Phonebook Entry
/// @param name Entry name
/// @param number Entry number
/// @return -1 If input is invalid, ent remains unchanged
int Entry_Fill(Entry *ent, char *name, long number) {
    if (ent == NULL || strcmp(name, "") == 0 || number == 0) return -1;

    /// Clean-up trailing characters
    /// @code 
    /// '\r\n' on Windows
    /// '\n' on Unix
    name[strcspn(name, "\r\n")] = 0;

    ent->Number = number;
    strlcpy(ent->Name, name, BUF_SIZE);
    
    return 0;
}

/// Main entry point
int main(int argc, char **argv) {
    /// Main array
    Entry phonebook[PB_SIZE];
    /// Actual array length
    int len = 0;
    /// Buffer size for @p getline()
    //size_t buf_size = BUF_SIZE;
    
    /// Temp buffers
    char t_name[BUF_SIZE], t_num[BUF_SIZE];
    
    /// Read stdin line by line, until we get an empty line
//    while (getline(&t_name, &buf_size, stdin) > 0) {
//        getline(&t_num, &buf_size, stdin);
    while (fgets(t_name, sizeof(t_name), stdin) != NULL) {
        fgets(t_num, sizeof(t_num), stdin);
        /// Since we are reading to a string buffer, we need to convert number to int type
        ///
        /// @remarks Yes, @p atoi() is unsafe, but whatever
        Entry_Fill(&phonebook[len], t_name, atol(t_num));
        len++;
    }

    /// Check arguments
    if (argc == 1) {
        for (int i = 0; i < len; i++) {
            printf("%s, %ld\n", phonebook[i].Name, phonebook[i].Number);
        }
    } else {
        printf("Searching for: %d\n", atoi(argv[1]));    
    }
    

    
    return 0;
}
