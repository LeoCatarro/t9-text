//
//  Compile line: gcc -std=c99 -Wall hashtable_words.c hashtable_keys.c main.c -o main -lm
//

// Notes: code for read a file to wchar_t array : https://www.ibm.com/docs/en/i/7.4?topic=lf-fwscanf-read-data-from-stream-using-wide-character

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
#define WORDS_TABLE_SIZE 1000000
#define KEYS_TABLE_SIZE 10

//Open the dictionary via fileName passed as argument of the program
FILE *OpenFile(char *fileName)
{
    FILE *fp;
    char filePath[BUFFER_LENGTH] = "dictionaries/";
    strcat(filePath, fileName);

    fp = fopen(filePath, "rb");

    return fp;
}


//Close the dictionary
void CloseFile(FILE *file)
{
    fclose(file);
}


//Processes the file readed word, converting it to lowercase, and "remove" '-' or '\''
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


//Read the lines from the file and insert them into WordsTable, after line been processed and word cleaned!
void ProcessData(FILE *fp, HashTable KeysTable, HashTable WordsTable)
{
    InsertT9Keys(KeysTable);        //Insert Keys in KeysTable
    
    printf("Loading lines...\n");

    wchar_t buffer[BUFFER_LENGTH];
    wchar_t *tmpWord;
    wchar_t *cleanWord;

    while(fwscanf(fp,L"%ls", buffer) != EOF)
    {
        tmpWord = (wchar_t*)malloc(sizeof(wchar_t*)*wcslen(buffer));
        tmpWord = wcscpy(tmpWord, buffer);
        cleanWord = CleanWordProcess(tmpWord);
        unsigned long res = StringToIntAccordingT9Keys(cleanWord, KeysTable);
        InsertWord(tmpWord, res, WordsTable);
    }

    free(tmpWord);
    free(cleanWord);
    printf("Lines loaded successfuly!\n");
}





int main(int argc, char* argv[])
{   
    setlocale(LC_ALL, "");
    
    //Open dictionary
    FILE *fp;
    fp = OpenFile(argv[1]);

    //Initialize table for Keys and Words
    HashTable KeysTable = InitializeKeysTable(KEYS_TABLE_SIZE);
    HashTable WordsTable = InitializeWordsTable(WORDS_TABLE_SIZE);

    //Read the dictionary, process word by word and insert them in WordsTable
    ProcessData(fp, KeysTable, WordsTable);

    PrintHashWordsTable(WordsTable);

    //Close dictionary
    CloseFile(fp);
    return 0;
}