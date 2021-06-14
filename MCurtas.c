//
//  Compile line: gcc -std=c99 -Wall hashtable_words.c hashtable_keys.c MCurtas.c -o MCurtas -lm
//

// Notes: code for read a file to wchar_t array : https://www.ibm.com/docs/en/i/7.4?topic=lf-fwscanf-read-data-from-stream-using-wide-character

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <locale.h>
#include <wchar.h>
#include <math.h>
#include <wctype.h>
#include <time.h>

#include "fatal.h"
#include "hashtable_keys.h"
#include "hashtable_words.h"

#define BUFFER_LENGTH 233
#define WORDS_TABLE_SIZE 9000000
#define KEYS_TABLE_SIZE 10

//Open the dictionary via fileName passed as argument of the program
FILE *OpenDictionary(char *fileName)
{
    FILE *fp;
    char filePath[BUFFER_LENGTH] = "dictionaries/";
    strcat(filePath, fileName);

    fp = fopen(filePath, "rb");

    return fp;
}

//Copies all words in fp to an updated dictionary and add the word passed as argument
void UpdateDictionary(wchar_t *word, FILE *fp, char *fileName)
{
    char ch;
    FILE *fp_updated;
    fp = OpenDictionary(fileName);
    fp_updated = fopen("output.txt", "w");

    while(( ch = fgetc(fp) ) != EOF)
      fputc(ch, fp_updated);
    
    fwrite(word, sizeof(wcslen(word)), 1, fp_updated );

    fclose(fp);
    fclose(fp_updated);
    printf("Dictionary updated with word %ls\n", word);  
}


//Close the dictionary
void CloseDictionary(FILE *file)
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
            break;  
 
        else
            tmpWord[i]= towlower(word[i]);

    }
    wchar_t *wordClean = (wchar_t*)malloc(sizeof(wchar_t*)*wcslen(tmpWord));
    wcscpy(wordClean, tmpWord);
    free(tmpWord);
    return wordClean;
}


//Read the lines from the file and insert them into WordsTable, after line been processed and word cleaned!
void ProcessData(FILE *fp, HashTable KeysTable, HashTable WordsTable)
{
    InsertT9Keys(KeysTable);        //Insert Keys in KeysTable
    
    printf("Loading dictionary...\n");

    wchar_t buffer[BUFFER_LENGTH];
    wchar_t *tmpWord;
    wchar_t *cleanWord;

    while(fwscanf(fp,L"%ls", buffer) != EOF)
    {
        tmpWord = (wchar_t*)malloc(sizeof(wchar_t*)*wcslen(buffer));
        tmpWord = wcscpy(tmpWord, buffer);
        cleanWord = CleanWordProcess(tmpWord);
        unsigned long res = StringToIntAccordingT9Keys(cleanWord, KeysTable);
        //printf("%ld\n", res);
        InsertWord(tmpWord, res, WordsTable);
    }

    free(tmpWord);
    free(cleanWord);
    printf("Dictionary successfuly loaded!\n");
}





int main(int argc, char* argv[])
{  
    setlocale(LC_ALL, ""); 
    clock_t begin = clock();
   
    //Open dictionary
    FILE *fp;
    fp = OpenDictionary(argv[1]);

    //Initialize table for Keys and Words
    HashTable KeysTable = InitializeKeysTable(KEYS_TABLE_SIZE);
    HashTable WordsTable = InitializeWordsTable(WORDS_TABLE_SIZE);

    //Read the dictionary, process word by word and insert them in WordsTable
    ProcessData(fp, KeysTable, WordsTable);

    UpdateDictionary(L"catty", fp, argv[1]);

    //PrintHashWordsTable(WordsTable);

    clock_t end = clock();
    double time_spent = (double)(end - begin) / CLOCKS_PER_SEC;
    printf("EXEC TIME: %f s\n", time_spent);


    //Close dictionary
    CloseDictionary(fp);

    return 0;
}