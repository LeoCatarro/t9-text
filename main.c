//
//  Compile line: gcc -std=c99 -Wall hashtable_words.c hashtable_keys.c main.c -o main -lm
//

//
// Notes: Fix bugs with upperCase letters and word with " ' " or with " - " 
//

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <locale.h>
#include <wchar.h>
#include <math.h>

#include "fatal.h"
#include "hashtable_keys.h"
#include "hashtable_words.h"

#define BUFFER_LENGTH 233


int main()
{   
    //
    // Notes: 
    //  ->code for read a file to wchar_t array : https://www.ibm.com/docs/en/i/7.4?topic=lf-fwscanf-read-data-from-stream-using-wide-character
    //
 
    setlocale(LC_ALL, "");
    FILE *fp;
    wchar_t word[BUFFER_LENGTH];
    fp = fopen("dictionaries/portuguese-small.txt", "rb");

    HashTable KeysTable = InitializeKeysTable(10);
    HashTable WordsTable = InitializeWordsTable(50);

    InsertT9Keys(KeysTable);
    PrintHashKeysTable(KeysTable);

    while(fwscanf(fp,L"%ls", word) != EOF)
    {
        wchar_t *word_ = malloc(sizeof(word));
        wcscpy(word_, word);
        printf("Line Size: %ld\n", wcslen(word));
        printf("Line: %ls\n", word);
        unsigned int res = StringToIntAccordingT9Keys(word, KeysTable);
        printf("Res: %d\n", res);
        InsertWord(word_, res, WordsTable);
    }

    PrintHashWordsTable(WordsTable);
    fclose(fp);
    return 0;
}