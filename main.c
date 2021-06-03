#include "fatal.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <locale.h>
#include <wchar.h>
#include <math.h>

#include "hashtable_keys.h"
#include "hashtable_words.h"

#define BUFFER_LENGTH 233

struct ListNode{
    wchar_t Element;
    Position Next;
};

/*
int StringToIntAccordingT9Keys(wchar_t *word, HashTable KeysTable)
{
    int result = 0;

    for(int i=0; i<wcslen(word); i++)
        {
            //printf("Key To be finded: %lc\n", word[i]);
            int index = FindKey(word[i], KeysTable);
            //printf("%d\n", index);
            result += index*(pow(10, wcslen(word)-i-1));
        }
    return result;
}
*/


int main()
{   
    //
    // Notes: 
    //  ->code for read a file to wchar_t array : https://www.ibm.com/docs/en/i/7.4?topic=lf-fwscanf-read-data-from-stream-using-wide-character
    //
 

    setlocale(LC_ALL, "");
    //const wchar_t special_chars[13] = {L'á', L'à', L'â', L'ã',L'ç', L'é', L'ê', L'í', L'ó', L'ô', L'õ', L'ú'};
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
        int res = StringToIntAccordingT9Keys(word, KeysTable);
        printf("Res: %d\n", res);
        InsertWord(word_, res, WordsTable);
    }

    PrintHashWordsTable(WordsTable);
    fclose(fp);
    return 0;
}