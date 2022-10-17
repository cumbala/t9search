/// @code
/// t9search.c
///
/// Author: Konstantin Romanets, xroman18@stud.fit.vut.cz
/// Date:   10.10.2022
/// Desc:   Implementation of T9 search contacts algorithm for IZP course
///
/// VUT FIT 2022
/// @endcode

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/// Global string buffer size
#define BUF_SIZE 100

/// To keep track of contacts
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

/// Prints @c msg to @c stderr and exits with @c EXIT_FAILURE
/// @param err_type Type of error (just a string for clarity)
/// @param msg Error message
void Error(char *err_type, char *msg) {
    fprintf(stderr, "ERROR (%s): %s\n", err_type, msg);
    exit(EXIT_FAILURE);
}

/// Converts normal string format to lowercase
/// @param str String to convert
/// @remarks
/// Using unsigned char here removes clang-tidy warning about
/// "implementation-dependent" char to int conversion, but...
/// wtf, there are symbols BEYOND 0???
void ToLower(unsigned char *str) {
    for(unsigned char *p = str; *p; ++p) *p = *p >= 'A' && *p <= 'Z' ? *p | 0x60 : *p;
}

/// Recursive function to iterate over all variants of input numbers on a keypad
/// If a match is found, sets global @c found flag to 0
/// @param search A name in contacts to search for (case-sensitive!)
/// @param number Input numbers on keypad in array form (easier to iterate over)
/// @param current_digit Current digit that is being checked (recursive limiter)
/// @param output Recursive output collector - a modifiable string
/// @param len Max length of @c output
void Check(char* search, int number[], int current_digit, char output[], int len) {
    if (current_digit == len) {
        /// Output here is a proper combination
        /// Here we pop out of this recursion hell
        if (strstr(search, output) != NULL) {
            found = 0;
        }

        return;
    }

    /// Iterate over all possible letters for current digit
    for (int i = 0; i < (int)strlen(KB[number[current_digit]]); i++) {
        output[current_digit] = KB[number[current_digit]][i];
        /// Go to next digit
        Check(search, number, current_digit + 1, output, len);
        /// If we found a match, we don't need to check any more
        if (number[current_digit] == 0 || number[current_digit] == 1) return;
    }
}

/// Caller function for @c Check
/// Resets @c found flag
/// @param search A name in contacts to search for (case-sensitive!)
/// @param number Input numbers on keypad in array form (easier to iterate over)
/// @param len Length of @c number
void CheckRecursive(char *search, int number[], int len) {
    /// Reset flag
    found = -1;
    /// Allocate memory
    char result[len + 1];
    result[len] = '\0';
    /// Call the Spaghetti monster
    Check(search, number, 0, result, len);
}

/// Entry point
int main(int argc, char **argv) {
    /// Buffers
    char t_name[BUF_SIZE], t_num[BUF_SIZE];

    /// Counter for "not found" message
    int found_count = 0;

    /// Read name in t_name unless newline or EOF
    while (fgets(t_name, sizeof(t_name), stdin) != NULL) {
        /// Read number in t_num
        fgets(t_num, sizeof(t_num), stdin);

        /// Clean up line end characters
        t_name[strcspn(t_name, "\r\n")] = 0;
        t_num[strcspn(t_num, "\r\n")] = 0;

        /// Check if t_num is actually a number after cleaning
        /// line breaks from it
        /// If it's not a number - skip this contact
        ///
        /// For now, we will only allow
        /// any ASCII character in the name...
        for (int i = 0; i < (int)strlen(t_num); i++) {
            if (t_num[i] < '0' || t_num[i] > '9') {
                continue;
            }
        }

        if (argc == 1) {
            /// No args - print the formatted contact
            printf("%s, %s\n", t_name, t_num);
        } else {
            /// Convert string search argument to int array
            int len = (int)strlen(argv[1]);
            int number[len];

            for (int i = 0; i < len; i++)
                number[i] = argv[1][i] - '0';

            /// Convert name to lowercase for search
            char l_name[BUF_SIZE];
            strcpy(l_name, t_name);
            ToLower((unsigned char *)l_name);

            /// Do the magic
            CheckRecursive(l_name, number, len);
            if (found == 0 || strstr(t_num, argv[1]) != NULL) {
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