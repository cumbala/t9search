/// @code
/// t9search.c
///
/// Author: Konstantin Romanets, xroman18@stud.fit.vut.cz
/// Date:   10.10.2022
/// Desc:   Implementation of T9 contacts search algorithm for IZP course
///         without recursion
///
/// VUT FIT 2022
/// @endcode

#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>

/// Global string buffer size
#define BUF_SIZE 100

/// To keep track of search
static int found = -1;

/// Keyboard, lol
char KB[10][5] = {
    "+",
    "",
    "abc",
    "def",
    "ghi",
    "jkl",
    "mno",
    "pqrs",
    "tuv",
    "wxyz"
};

/// Converts normal string format to lowercase
/// @param str String to convert
/// @remarks
/// Using unsigned char here removes clang-tidy warning about
/// "implementation-dependent" char to int conversion, but...
/// wtf, there are symbols BEYOND 0???
void ToLower(unsigned char *str) {
    for(unsigned char *p = str; *p; ++p)
        *p = *p >= 'A' && *p <= 'Z' ? *p | 0x60 : *p;
}

/// A proper way to search for a match
/// @param search A name in contacts to search for (case-sensitive!)
/// @param query Numbers from keypad (can only be numbers)
void CheckNormal(char *search, char *query) {
    found = -1;
    /// We check by converting the search string into a sequence of numbers
    /// and then checking if the query string is a substring of the search string

    int len = (int) strlen(search);
    char numbers[len + 1];
    numbers[len] = '\0';
    
    /// Convert search into a sequence of numbers
    for (int i = 0; i < len; i++) {
        for (int j = 0; j < 10; j++) {
            /// Check if there is any odd symbols in the search and replace them with space
            if (strchr(" .,:;!?-*&^%$#@()=_{}[]'\"<>/|\\", search[i]) != NULL) {
                numbers[i] = ' ';
                break;
            }
            /// But the letters should be replaced with numbers from keyboard
            if (strchr(KB[j], search[i]) != NULL) {
                numbers[i] = j + '0';
                break;
            }
        }
    }

    /// Compare query to the sequence of numbers
    if (strstr(numbers, query) != NULL) {
        found = 0;
    }
}

void CheckNumber(char *number, char *query) {
    /// here found = -1 
    /// replace + in number
    char t_num[(int)strlen(number) + 1];
    strcpy(t_num, number);
    t_num[strcspn(t_num, "+")] = '0';
    if (strstr(t_num, query) != NULL) {
        found = 0;
    }
}

/// Entry point
int main(int argc, char **argv) {
    /// Buffers
    char t_name[BUF_SIZE], t_num[BUF_SIZE];

    /// Counter for "not found" message
    int found_count = 0;

    /// Check if the argv[1] is a number
    /// if not - print error to STDERR
    if (argc > 1)
        for (int i = 0; i < (int)strlen(argv[1]); i++) {
            if (argv[1][i] < '0' || argv[1][i] > '9') {
                fprintf(stderr, "Error: Invalid query input\n");
                return 1;
            }
        }

    /// Check if there other arguments in argv and exit if there are
    if (argc > 2) {
        fprintf(stderr, "Error: Too many arguments\n");
        return 1;
    }
    
    /// Read name in t_name unless newline or EOF
    while (fgets(t_name, sizeof(t_name), stdin) != NULL) {
        /// Read number in t_num
        fgets(t_num, sizeof(t_num), stdin);

        /// Clean up line end characters
        t_name[strcspn(t_name, "\r\n")] = 0;
        t_num[strcspn(t_num, "\r\n")] = 0;

        if (argc == 1) {
            /// No args - print the formatted contact
            printf("%s, %s\n", t_name, t_num);
        } else {
            /// Convert name to lowercase for search
            char l_name[BUF_SIZE];
            strcpy(l_name, t_name);
            ToLower((unsigned char *)l_name);
            
            CheckNormal(l_name, argv[1]);
            CheckNumber(t_num, argv[1]);
            if (found == 0) {
                found_count++;
                printf("%s, %s\n", t_name, t_num);
            }

        }
    }

    /// If we are searching for a contact
    /// and haven't found anything
    if (argc != 1 && found_count == 0)
        printf("Not found\n");

    return 0;
}