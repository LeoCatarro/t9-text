//
//  Compile line: gcc -std=c99 -Wall hashtable_words.c hashtable_keys.c MCurtas.c -o MCurtas -lm
//

// Notes: - code for read a file to wchar_t array : https://www.ibm.com/docs/en/i/7.4?topic=lf-fwscanf-read-data-from-stream-using-wide-character
//        - use char* to fwrite() in file
//        - correct bug when try to insert multiple strings in updated dictionary

// Notes for words frequency implementation:
//        - add an integer/long in ListNode struct
//        - change insert function in hashtable_words.c to insert with frequency if is frequency file

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

//Open the dictionary via fileName passed as argument of the program, if the dictionary is inside the dictionaries folder
FILE *OpenDictionary(char *fileName, char* wayToOpen)
{
    FILE *fp;
    char filePath[BUFFER_LENGTH] = "dict/";
    strcat(filePath, fileName);

    fp = fopen(filePath, wayToOpen);

    return fp;
}


//Close the dictionary
void CloseDictionary(FILE *fp)
{
    fclose(fp);
}


//Copy the content of the file passed as src to dst
void CopyDictionaryAndUpdated(char* source, char* destination, wchar_t* word)
{
    // declaring file pointers
    FILE *fp1, *fp2;
 
    // opening files
    fp1 = fopen(source, "a+");
    fp2 = fopen(destination, "a+");
 
    // If file is not found then return.
    /*if (!fp1 && !fp2) {
        printf("Unable to open/""detect file(s)\n");
        return;
    }*/
 
    char buf[100];
 
    // writing the contents of
    // source file to destination file.    
    //Copy all lines to output file if the file is empty
    fseek (fp2, 0, SEEK_END);
    int size = ftell(fp2);

    if (0 == size) {
        while (!feof(fp1)) {
            fgets(buf, sizeof(buf), fp1);
            fprintf(fp2, "%s", buf);
        }
    }
    fwprintf(fp2, L"\n");
    fwprintf(fp2, L"%ls", word);
    rewind(fp2);
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
    fwscanf(fp,L"%ls", buffer);

    if(wcsstr(buffer, L",") != 0)
    {
        //printf("Is a frequency file!\n");
        wchar_t *wordFreq;

        while(fwscanf(fp,L"%ls", buffer) != EOF)
        {
            wchar_t* bufferAux;
            wchar_t *word = wcstok(buffer, L",", &bufferAux);
            wordFreq = wcstok(NULL, L",", &bufferAux);
            long freq = wcstol(wordFreq, NULL, 10);

            tmpWord = (wchar_t*)malloc(sizeof(wchar_t*)*wcslen(buffer));
            tmpWord = wcscpy(tmpWord, word);
            cleanWord = CleanWordProcess(tmpWord);
            unsigned long res = StringToIntAccordingT9Keys(cleanWord, KeysTable);
            InsertWordAccordingFrequency(tmpWord, freq, res, WordsTable);
        }

        //Process the last line of the file
        long freq = wcstol(wordFreq, NULL, 10);
        tmpWord = (wchar_t*)malloc(sizeof(wchar_t*)*wcslen(buffer));
        tmpWord = wcscpy(tmpWord, buffer);
        cleanWord = CleanWordProcess(tmpWord);
        unsigned long res = StringToIntAccordingT9Keys(cleanWord, KeysTable);
        InsertWordAccordingFrequency(tmpWord, freq, res, WordsTable);
    }

    else
    {
        fseek(fp, 0, SEEK_SET); //Sets again the pointer to the start of the file
        //printf("Is not a frequency file\n");

        while(fwscanf(fp,L"%ls", buffer) != EOF)
        {
            tmpWord = (wchar_t*)malloc(sizeof(wchar_t*)*wcslen(buffer));
            tmpWord = wcscpy(tmpWord, buffer);
            cleanWord = CleanWordProcess(tmpWord);
            unsigned long res = StringToIntAccordingT9Keys(cleanWord, KeysTable);
            InsertWord(tmpWord, res, WordsTable);
        }

        //Process the last line of the file
        tmpWord = (wchar_t*)malloc(sizeof(wchar_t*)*wcslen(buffer));
        tmpWord = wcscpy(tmpWord, buffer);
        cleanWord = CleanWordProcess(tmpWord);
        unsigned long res = StringToIntAccordingT9Keys(cleanWord, KeysTable);
        InsertWord(tmpWord, res, WordsTable);
    }

    free(tmpWord);
    free(cleanWord);
    printf("Dictionary successfuly loaded!\n");
}


//Scans the word that doesnt exists and insert it in the current phrase and updated the dictionary
wchar_t *InsertWordInHashAndPhrase(wchar_t *wordToInsert, wchar_t *phrase, HashTable WordsTable, HashTable KeysTable)
{
    wchar_t *tmpWord;
    wchar_t *cleanWord;
    
    //Append the word to current phrase
    wcscat(phrase, wordToInsert);
    wcscat(phrase, L" ");

    //Clean the word to insert it in the WordsTable
    tmpWord = (wchar_t*)malloc(sizeof(wchar_t*)*wcslen(wordToInsert));
    tmpWord = wcscpy(tmpWord, wordToInsert);
    cleanWord = CleanWordProcess(tmpWord);
    unsigned long res = StringToIntAccordingT9Keys(cleanWord, KeysTable);
    InsertWord(wordToInsert, res, WordsTable);

    return wordToInsert;
}


int main(int argc, char* argv[])
{  
    FILE *fp;
    int res;
    wchar_t phrase[BUFFER_LENGTH];  //Array to save the accepted suggested words
    setlocale(LC_ALL, ""); 
    clock_t begin = clock();
    fp = OpenDictionary(argv[1], "a+");

    HashTable KeysTable = InitializeKeysTable(KEYS_TABLE_SIZE);
    HashTable WordsTable = InitializeWordsTable(WORDS_TABLE_SIZE);

    //Read the dictionary, process word by word and insert them in WordsTable
    ProcessData(fp, KeysTable, WordsTable);

    //Stops the clock and calculate the loading dictionary time
    clock_t end = clock();
    double time_spent = (double)(end - begin) / CLOCKS_PER_SEC;
    printf("Loading Time: %f s\n\n", time_spent);

    PrintHashKeysTable(KeysTable);  //Printing keys to users know them

    //Pogram menu
    printf("** Escreva a sua mensagem **\n");
    while(true)
    {
        scanf("%d", &res);

        switch(res)
        {
            List L;
            Position P;
            char c;

            //Ask if user wants to leave the application
            case 0:
                printf("Deseja sair da aplicação (s/n)? ");
                scanf(" %c", &c);
            
                if ('s' == c)
                    exit(0);

                break;
            
            //Write the message and exit the progam
            case 1:
                printf("Mensagem: %ls", phrase);
                exit(1);
                break;
            
            //Users inserts a word
            default:
                L = WordsTable->TheLists[HashWords(res, WordsTable->TableSize)];
                P = L->Next;

                //Check ifthe word does not exists, 
                //user will type the word and it will be insert in the current message and the dictionary updated
                if(P == NULL)
                {
                    wchar_t wordToInsert[BUFFER_LENGTH];
                    printf("Não existem mais sugestões; introduza a palavra do teclado\n");
                    scanf("%ls", wordToInsert);
                    
                    wchar_t *inserted = InsertWordInHashAndPhrase(wordToInsert, phrase, WordsTable, KeysTable);
                    CopyDictionaryAndUpdated(argv[1], "dict/output.txt", inserted);
                }
                else
                {
                    printf("Sugestão: %ls, aceita(s/n)? ", P->Element);
                    scanf(" %c", &c);

                    //If the user accepts the suggestion, the word is inserted to the message
                    if('s' == c)
                    {
                        wcscat(phrase, P->Element);
                        wcscat(phrase, L" ");
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
                        wchar_t wordToInsert[BUFFER_LENGTH];
                        printf("Não existem mais sugestões; introduza a palavra do teclado\n");
                        scanf("%ls", wordToInsert);

                        wchar_t *inserted = InsertWordInHashAndPhrase(wordToInsert, phrase, WordsTable, KeysTable);
                        CopyDictionaryAndUpdated(argv[1], "dict/output.txt", inserted);
                    } 
                }
                break;
        }
    }
    //Close dictionary
    CloseDictionary(fp);
    return 0;
}