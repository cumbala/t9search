#include <stdio.h>
#include <stdlib.h>

#define PB_SIZE 1024
#define BUF_SIZE 100

typedef struct TEntry {
    char Name[BUF_SIZE];
    int Number;
} Entry;

int main(int argc, char **argv) {
    Entry phonebook[PB_SIZE];

    for (int i = 0; i < argc; i++)
        printf("%s\n", argv[i]);

    fgets(phonebook[0].Name, BUF_SIZE, stdin);
    scanf("%d", &phonebook[0].Number);
    
    
    
    return 0;
}
