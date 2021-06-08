//
//  Compile line: gcc -std=c99 -Wall hashtable_words.c hashtable_keys.c main.c -o main -lm
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
        if('-' == word[i] || '\'' == word[i])
        {
            //printf("Finded '-' in word\n");
            break;  
        }
        else
        {
            tmpWord[i]= towlower(word[i]);
        }
    }
    printf("Word after clean process: %ls\n", tmpWord);
    wchar_t *cleanWord2 = (wchar_t*)malloc(sizeof(wchar_t*)*wcslen(tmpWord));
    wcscpy(cleanWord2, tmpWord);
    free(tmpWord);
    return cleanWord2;
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
    fp = fopen("dictionaries/portuguese-large.txt", "rb");

    HashTable KeysTable = InitializeKeysTable(10);
    HashTable WordsTable = InitializeWordsTable(1000000);

    InsertT9Keys(KeysTable);
    PrintHashKeysTable(KeysTable);

    wchar_t *tmpWord;
    wchar_t *cleanWord;

    while(fwscanf(fp,L"%ls", buffer) != EOF)
    {
        tmpWord = (wchar_t*)malloc(sizeof(wchar_t*)*wcslen(buffer));
        tmpWord = wcscpy(tmpWord, buffer);
        //printf("Line Size: %ld\n", wcslen(tmpWord));
        printf("Line: %ls\n", tmpWord);
        cleanWord = CleanWordProcess(tmpWord);
        //printf("cleanWord Size: %ld\n", wcslen(cleanWord));
        //printf("Cleaned Word: %ls\n", cleanWord);
        unsigned long res = StringToIntAccordingT9Keys(cleanWord, KeysTable);
        //printf("Res: %ld\n", res);
        InsertWord(tmpWord, res, WordsTable);
        //printf("\n\n");
    }

    PrintHashWordsTable(WordsTable);
    free(tmpWord);
    free(cleanWord);
    fclose(fp);
    return 0;
}