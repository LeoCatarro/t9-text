//
//  Compile line: gcc -std=c99 -Wall hashtable_words.c hashtable_keys.c MCurtas.c -o MCurtas -lm
//

// Notes: - code for read a file to wchar_t array : https://www.ibm.com/docs/en/i/7.4?topic=lf-fwscanf-read-data-from-stream-using-wide-character
//        - use char* to fwrite() in file
//        - correct bug when try to insert multiple strings in updated dictionary

#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>
#include <string.h>
#include <locale.h>
#include <wchar.h>
#include <math.h>
#include <wctype.h>
#include <time.h>
#include <stdbool.h>
#include <unistd.h>

#include "fatal.h"
#include "hashtable_keys.h"
#include "hashtable_words.h"

#define BUFFER_LENGTH 400
#define WORDS_TABLE_SIZE 9000000
#define KEYS_TABLE_SIZE 10

//Open the dictionary via fileName passed as argument of the program
FILE *OpenDictionary(char *fileName, char* wayToOpen)
{
    FILE *fp;
    char filePath[BUFFER_LENGTH] = "dictionaries/";
    strcat(filePath, fileName);

    fp = fopen(filePath, wayToOpen);

    return fp;
}


//Close the dictionary
void CloseDictionary(FILE *file)
{
    fclose(file);
}


//Copies all words in fp to an updated dictionary and add the word passed as argument
void UpdateDictionary(wchar_t *word, FILE *fp, char *fileName)
{
    char ch;
    FILE *fp_updated;

    fp = OpenDictionary(fileName, "r");
    char filePath[BUFFER_LENGTH] = "dictionaries/updated-";
    strcat(filePath, fileName);
    printf("%s\n", filePath);
    fp_updated = OpenDictionary("output.txt", "a+");

    //Copy all lines to output file if the file is empty
    fseek (fp_updated, 0, SEEK_END);
    int size = ftell(fp_updated);

    if (0 == size) {
        while(( ch = fgetc(fp) ) != EOF)
            fputc(ch, fp_updated);
    }

    //Write the new word in the file
    fputws(L"\n",fp_updated);
    fputws(word, fp_updated);

    //Close the file
    CloseDictionary(fp_updated);
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

    //Verification if the files contains word's usage frequency
    /*fwscanf(fp,L"%ls", buffer);
    if(wcsstr(buffer, L",") != 0)
    {
        printf("Is a frequency file!\n");
        while(fwscanf(fp,L"%ls", buffer) != EOF)
        {
            wchar_t *outBuff;
            wchar_t *ptr = wcstok(buffer, L",", &outBuff);

            	while(outBuff != NULL)
                {
                    printf("'%s'\n", ptr);
                    ptr = strtok(NULL, ",");
                }

            tmpWord = (wchar_t*)malloc(sizeof(wchar_t*)*wcslen(buffer));
            tmpWord = wcscpy(tmpWord, buffer);
            cleanWord = CleanWordProcess(tmpWord);
            unsigned long res = StringToIntAccordingT9Keys(cleanWord, KeysTable);
            //InsertWordWithFrequency(tmpWord, res, WordsTable);
        }
    }*/
    //else
    //{
        printf("Is not a frequency file\n");

        while(fwscanf(fp,L"%ls", buffer) != EOF)
        {
            tmpWord = (wchar_t*)malloc(sizeof(wchar_t*)*wcslen(buffer));
            tmpWord = wcscpy(tmpWord, buffer);
            cleanWord = CleanWordProcess(tmpWord);
            unsigned long res = StringToIntAccordingT9Keys(cleanWord, KeysTable);
            InsertWord(tmpWord, res, WordsTable);
        }

        fwscanf(fp,L"%ls", buffer);
        tmpWord = (wchar_t*)malloc(sizeof(wchar_t*)*wcslen(buffer));
        tmpWord = wcscpy(tmpWord, buffer);
        cleanWord = CleanWordProcess(tmpWord);
        unsigned long res = StringToIntAccordingT9Keys(cleanWord, KeysTable);
        InsertWord(tmpWord, res, WordsTable);
    //}

    free(tmpWord);
    free(cleanWord);
    printf("Dictionary successfuly loaded!\n");
}


//Scans the word that doesnt exists and insert it in the current phrase and updated the dictionary
void InsertWordToMessageAndUpdateDict(wchar_t *phrase, FILE *fp, char* fileName)
{
    wchar_t wordToInsert[BUFFER_LENGTH];
    printf("Não existem mais sugestões; introduza a palavra do teclado\n");
    scanf("%ls", wordToInsert);
    wcscat(phrase, wordToInsert);
    UpdateDictionary(wordToInsert, fp, fileName);
}


int main(int argc, char* argv[])
{  
   
    clock_t begin = clock();
    setlocale(LC_ALL, ""); 
   
    //Open dictionary
    FILE *fp;
    fp = OpenDictionary(argv[1], "rb");

    //Initialize table for Keys and Words
    HashTable KeysTable = InitializeKeysTable(KEYS_TABLE_SIZE);
    HashTable WordsTable = InitializeWordsTable(WORDS_TABLE_SIZE);

    //Read the dictionary, process word by word and insert them in WordsTable
    

    ProcessData(fp, KeysTable, WordsTable);

    PrintHashWordsTable(WordsTable);

    clock_t end = clock();
    double time_spent = (double)(end - begin) / CLOCKS_PER_SEC;
    printf("EXEC TIME: %f s\n", time_spent);

    //Words sugggestions
    wchar_t phrase[BUFFER_LENGTH];      //Array to save the accepted suggested words
    int res;

    //Pogram menu
    printf("** Escreva a sua mensagem **\n");
    while(true)
    {
        scanf("%d", &res);

        switch(res)
        {
            long p;
            List L;
            Position P;
            char c;
     
            case 0:
                printf("Deseja sair da aplicação (s/n)? ");
                scanf(" %c", &c);
            
                if ('s' == c)
                    exit(0);

                break;
            

            case 1:
                printf("Mensagem: %ls", phrase);
                exit(1);
                break;
            

            default:
                p = HashWords(res, WordsTable->TableSize);
                L = WordsTable->TheLists[p];
                P = L->Next;

                //If the word doenst exists, user will type the word and it will be insert in the current message and the dictionary updated
                if(P == NULL)
                {
                    InsertWordToMessageAndUpdateDict(phrase, fp, argv[1]);
                }

                else
                {
                    printf("Sugestão: %ls, aceita(s/n)? ", P->Element);
                    scanf(" %c", &c);

                    //If the user accepts the suggestion, the word is inserted to the message
                    if('s' == c)
                    {
                        wcscat(phrase, L" ");
                        wcscat(phrase, P->Element);
                    }

                    //If the user dont accept it, suggest the next one until he accepts one or insert if dont have more words to suggest
                    else if('n' == c)
                    {
                        while(P->Next!=NULL && 'n' == c)
                        {
                            P = P->Next;
                            printf("Sugestão: %ls, aceita(s/n)? ", P->Element);
                            scanf(" %c", &c);
                        }
                        InsertWordToMessageAndUpdateDict(phrase, fp, argv[1]);
                    } 
                }
                break;
        }
    }

    //Close dictionary
    CloseDictionary(fp);

    return 0;
}