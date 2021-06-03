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
#include <wctype.h>

#include "fatal.h"
#include "hashtable_keys.h"
#include "hashtable_words.h"

#define BUFFER_LENGTH 233

wchar_t *CleanWordProcess(wchar_t* word)
{
    wchar_t *tmpWord = (wchar_t*)malloc(sizeof(wchar_t*)*wcslen(word));

    //Clean word process
    for(int i=0 ; i<wcslen(word); i++)
    {
        //Remove '-' and '\''(apostrophe) from the word
        if(word[i] != '-' || word[i] != '\'')
        {
            tmpWord[i]= towlower(word[i]);
        }   
    }
    wchar_t *cleanWord = (wchar_t*)malloc(sizeof(wchar_t*)*wcslen(tmpWord));
    wcscpy(cleanWord, tmpWord);
    return cleanWord;
}


int main()
{   
    //
    // Notes: 
    //  ->code for read a file to wchar_t array : https://www.ibm.com/docs/en/i/7.4?topic=lf-fwscanf-read-data-from-stream-using-wide-character
    //
    
    wchar_t buffer[BUFFER_LENGTH];
    setlocale(LC_ALL, "");
    FILE *fp;
    fp = fopen("dictionaries/portuguese-small.txt", "rb");

    HashTable KeysTable = InitializeKeysTable(10);
    HashTable WordsTable = InitializeWordsTable(100);

    InsertT9Keys(KeysTable);
    PrintHashKeysTable(KeysTable);

    while(fwscanf(fp,L"%ls", buffer) != EOF)
    {
        wchar_t *tmpWord = (wchar_t*)malloc(sizeof(wchar_t*)*BUFFER_LENGTH);
        wcscpy(tmpWord, buffer);
        printf("Line Size: %ld\n", wcslen(tmpWord));
        printf("Line: %ls\n", tmpWord);
        wchar_t *cleanWord = (wchar_t*)malloc(sizeof(wchar_t*)*wcslen(tmpWord));
        cleanWord = CleanWordProcess(tmpWord);
        printf("cleanWord Size: %ld\n", wcslen(cleanWord));
        printf("Cleaned Word: %ls\n", cleanWord);
        unsigned int res = StringToIntAccordingT9Keys(cleanWord, KeysTable);
        printf("Res: %d\n", res);
        InsertWord(tmpWord, res, WordsTable);
        printf("\n\n");
    }

    PrintHashWordsTable(WordsTable);
    fclose(fp);
    return 0;
}