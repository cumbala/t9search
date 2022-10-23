/// @code
/// t9search_levenshtein.c
///
/// Author: Konstantin Romanets, xroman18@stud.fit.vut.cz
/// Date:   10.10.2022
/// Desc:   Implementation of T9 contacts search algorithm for IZP course
///         without recursion with Levenshtein distance fuzzy search
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
/// @c -l argument
static int levenshtein = -1;

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

int Min(int count, ...) {
    va_list args;
    int min = 9999999;

    va_start(args, count);
    
    for (int i = 0; i < count; i++) {
        int val = va_arg(args, int);
        if (val < min) {
            min = val;
        }
    }

    va_end(args);
    
    return min;
}

/// Compute Levenshtein distance between two strings
/// @param a First string
/// @param b Second string
/// @return A difference distance between two strings
int LevenshteinDistance(char *a, char *b) {
    int m = (int)strlen(a), n = (int)strlen(b), cost, distance;
    int d[(m + 1) * (n + 1)];
    for (int k = 0; k <= m; k++)
        d[k * (n + 1)] = k;

    for (int l = 0; l <= n; l++)
        d[l] = l;

    for (int i = 1; i <= m; i++)
        for (int j = 1; j <= n; j++) {
            cost = (a[i - 1] == b[j - 1]) ? 0 : 1;
            d[i * (n + 1) + j] = Min(3, d[(i - 1) * (n + 1) + j] + 1, d[i * (n + 1) + j - 1] + 1, d[(i - 1) * (n + 1) + j - 1] + cost);
        }

    distance = d[m * (n + 1) + n];

    return distance;
}

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
            /// Check if there is any symbols in the search and replace them with space
            if (strchr(" .,:;!?-*&^%$#@()+=_{}[]'\"<>/|\\", search[i]) != NULL) {
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
    if (strstr(numbers, query) != NULL || (levenshtein != -1 && LevenshteinDistance(numbers, query) <= levenshtein)) {
        found = 0;
    }
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
        /// If it's not a number - skip this contact\n
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
            /// Check if -l argument is set
            if (strcmp(argv[1], "-l") == 0) {
                /// Check if -l argument is set and has a value
                levenshtein = (int)strtol(argv[2], (char **)NULL, 10);
            }
            
            /// Convert name to lowercase for search
            char l_name[BUF_SIZE];
            strcpy(l_name, t_name);
            ToLower((unsigned char *)l_name);
            
            CheckNormal(l_name, argv[levenshtein == -1 ? 1 : 3]);
            if (found == 0 || strstr(t_num, argv[levenshtein == -1 ? 1 : 3]) != NULL) {
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