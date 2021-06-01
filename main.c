#include "fatal.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <locale.h>
#include <wchar.h>

#include "hashtable_keys.h"
#include "hashtable_words.h"

#define BUFFER_LENGTH 233

int main()
{   
    //
    // Notes: 
    //  ->code for read a file to wchar_t array : https://www.ibm.com/docs/en/i/7.4?topic=lf-fwscanf-read-data-from-stream-using-wide-character
    //
    //
    //
    //

    setlocale(LC_ALL, "");
    const wchar_t special_chars[13] = {L'á', L'à', L'â', L'ã',L'ç', L'é', L'ê', L'í', L'ó', L'ô', L'õ', L'ú'};
    FILE *fp;
    wchar_t buffer[BUFFER_LENGTH];
    fp = fopen("dictionaries/portuguese-small.txt", "rb");

    HashTable KeysTable = InitializeKeysTable(10);
    
    InsertT9Keys(KeysTable);
    PrintHashKeysTable(KeysTable);

    while(fwscanf(fp,L"%ls", buffer) != EOF)
    {
        printf("Line Size: %ld\n", wcslen(buffer));
        printf("Line: %ls\n", buffer);
    }
    fclose(fp);
    //while(fwscanf(fp, line)){
        
        
        /*
        for(int i=0; i<wcslen(word); i++)
        {
            //Key 2 char match
            if(word[i] == special_chars[0] || word[i] == special_chars[1] || word[i] == special_chars[2] || word[i] == special_chars[3] || word[i] == special_chars[4])
            {
                printf("MATCH WITH: á OR à OR â OR ã OR ç\n");
                char c = '2';
                strncat(result, &c, 1);
            }

            //Key 3 char match
            else if(word[i] == special_chars[5] || word[i] == special_chars[6])
            {
                printf("MATCH WITH: é OR ê\n");
                char c = '3';
                strncat(result, &c, 1);
            }

            //Key 4 char match
            else if(word[i] == special_chars[7])
            {
                printf("MATCH WITH: í\n");
                char c = '4';
                strncat(result, &c, 1);
            }

            //Key 6 char match
            else if(word[i] == special_chars[8] || word[i] == special_chars[9] || word[i] == special_chars[10])
            {
                printf("MATCH WITH: ó OR ô OR õ\n");
                char c = '6';
                strncat(result, &c, 1);
            }

            //Key 8 char match
            else if(word[i] == special_chars[11])
            {
                printf("MATCH WITH: ú\n");
                char c = '8';
                strncat(result, &c, 1);
            }
        }
        printf("String to Integers keys: %s\n\n\n", result);*/
    //}



    
    /*for(int i=0; i<wcslen(word); i++)
    {
        //Key 2 char match
        if(word[i] == special_chars[0] || word[i] == special_chars[1] || word[i] == special_chars[2] || word[i] == special_chars[3] || word[i] == special_chars[4])
        {
            printf("MATCH WITH: á OR à OR â OR ã OR ç\n");
            char c = '2';
            strncat(result, &c, 1);
        }

        //Key 3 char match
        else if(word[i] == special_chars[5] || word[i] == special_chars[6])
        {
            printf("MATCH WITH: é OR ê\n");
            char c = '3';
            strncat(result, &c, 1);
        }

        //Key 4 char match
        else if(word[i] == special_chars[7])
        {
            printf("MATCH WITH: í\n");
            char c = '4';
            strncat(result, &c, 1);
        }

        //Key 6 char match
        else if(word[i] == special_chars[8] || word[i] == special_chars[9] || word[i] == special_chars[10])
        {
            printf("MATCH WITH: ó OR ô OR õ\n");
            char c = '6';
            strncat(result, &c, 1);
        }

        //Key 8 char match
        else if(word[i] == special_chars[11])
        {
            printf("MATCH WITH: ú\n");
            char c = '8';
            strncat(result, &c, 1);
        }
    }*/

    //printf("String to Integers keys: %s\n", result);
    return 0;
}