#include "fatal.h"
#include "hashtable_words.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


#define MAX_LINE_SIZE 233

#define MinTableSize (10)
typedef unsigned int Index;
typedef Position List;


struct ListNode{
    char *Element;
    Position Next;
};


struct HashTbl{
    int TableSize;
    List *TheLists;
};


/* Return next prime; assume N >= MinTableSize */
static int NextPrime( int N ){
    int i;

    if( N % 2 == 0 )
        N++;
    for( ; ; N += 2 ){
        for( i = 3; i * i <= N; i += 2 )
            if( N % i == 0 )
                goto ContOuter;  /* Sorry about this! */
        return N;
      ContOuter: ;
    }
}


/* Hash function for ints */
Index HashWords( const char *Key, int TableSize )
{
    unsigned int HashVal = 0;

    while( *Key != '\0' )
        HashVal += *Key++;
   
    return HashVal % TableSize;
}

/* Test hash function */

/* Initialize the Table, 
making the correspondent malloc() and allocate the array of lists and them Headers 
to use on HashTable positions */
HashTable InitializeWordsTable( int TableSize ){
    HashTable H;
    int i;

    if( TableSize < MinTableSize ){
        Error( "Table size too small" );
        return NULL;
    }

    /* Allocate table */
    H = malloc( sizeof( struct HashTbl ) );
    if( H == NULL )
        FatalError( "Out of space!!!" );

        H->TableSize = NextPrime( TableSize );

    /* Allocate array of lists */
    H->TheLists = malloc( sizeof( List ) * H->TableSize );
    if( H->TheLists == NULL )
        FatalError( "Out of space!!!" );

    /* Allocate list headers */
    for( i = 0; i < H->TableSize; i++ ){
        H->TheLists[ i ] = malloc( sizeof( struct ListNode ) );
        if( H->TheLists[ i ] == NULL )
            FatalError( "Out of space!!!" );
        else
            H->TheLists[ i ]->Next = NULL;
    }

    return H;
}


/* Find a Key in HashTable */
Position FindWord(const char *Key, HashTable H )
{
    List L = H->TheLists[HashWords( Key, H->TableSize )];
    Position P = L->Next;

    while( P != NULL && strcmp(P->Element, Key)!=0 )
    {
        P = P->Next;
    }
    //printf("[Info]: Finded Node with Element [%s]", P->Element);
    return P;
}


/* Insert the Element Key passed as argument in HashTable H */
void InsertWord(const char* Key, HashTable H ){
    Position Pos, NewCell;
    List L;

    Pos = FindWord( Key, H );

    //Key is not found
    if( Pos == NULL ){  
        NewCell = malloc( sizeof( struct ListNode ) );
        NewCell->Element = malloc(sizeof(Key));

        if( NewCell == NULL )
            FatalError( "Out of space!!!" );
        
        else{
            L = H->TheLists[ HashWords( Key, H->TableSize ) ];
            NewCell->Next = L->Next;
            //NewCell->Element = Key; 
            strcpy(NewCell->Element, Key);
            L->Next = NewCell;
        }
    }

    //Key is found in HashTable
    else
    {   
        //If the key is found in HT, we need to create another node 
        //to insert the element inside the list of the current hashtable position
        NewCell = malloc( sizeof( struct ListNode ) );
        NewCell->Element = malloc(sizeof(Key));
        Pos->Next = NewCell;
        //NewCell->Element = Key;
        strcpy(NewCell->Element, Key);
        NewCell->Next = NULL;
    }
}

/* Print the Element in Node P */
const char* RetrieveWord( Position P ){
    return P->Element;
}


/* Free the ram occupied from HashTable */
void DestroyWordsTable( HashTable H ){
    int i;

    for( i = 0; i < H->TableSize; i++ )
    {
        Position P = H->TheLists[ i ];
        Position Tmp;

        while( P != NULL )
        {
            Tmp = P->Next;
            free( P );
            P = Tmp;
        }
    }

    free( H->TheLists );
    free( H );
}


/* Removes the Element X from the HashTable */
HashTable DeleteWord(const char* X, HashTable T ){
    
    // Find the key of the Element X
    int key = HashWords(X, T->TableSize);

    //Key finded
    if(key != -1)
    {
        //If the key is finded we need to iterate over the List of that Key Hashtable Position to find the X
        Position prevP = T->TheLists[key];
        Position P = T->TheLists[key]->Next;

        //X finded 
        while(P != NULL)
        {
            if(strcmp(P->Element, X) == 0 && P->Next != NULL)
            {   
                printf("Deleted [%s] at index %d from HashTable\n", P->Element, key);
                prevP->Next = P->Next->Next;
                return T;
            }

            else if(strcmp(P->Element, X) && P->Next == NULL)
            {
                prevP->Next = NULL;
                printf("Deleted [%s] at index %d from HashTable\n", P->Element, key);
                return T;
            }

            prevP = P;
            P = P->Next;       
        }
    }

    //Key not finded 
    else
        FatalError("Element not present in HashTable");
    
    return T;
}


/* Free the allocated memory of Hashtable */
HashTable MakeEmpty( HashTable T ){
    /*for(int i=0 ; i<T->TableSize; i++)
    {
        Position P = T->TheLists[i];
        while(P->Next != NULL)
        {
            P = P->Next;
            free(P);
        }
    }*/

    printf("\nTODO()\n");
    return T;
}


/* Display HashTable in Terminal */
void PrintHashWordsTable(HashTable T)
{
    printf("* Printing HashTable *\n");

    for(int i=0 ; i < T->TableSize ; i++)
    {   
        Position P = T->TheLists[i]->Next;

        if(P != NULL)
        {
            printf("%d\t[", i);
            while(P != NULL)
            {
                P->Element[strcspn(P->Element, "\n")] = 0;
                printf("%s", P->Element);
                P = P->Next;

                //If is not the last element
                if(P != NULL)
                    printf(", ");
            }
            printf("]\n");
        }
        /*else
            printf("%d\t[%s]\n", i, "--");*/
    }
}


/* Change the characters of the word to the corresponding integer T9Key value */
/*void WordToT9Keys(char *word)
{
    //setlocale(LC_ALL, "");
    int result=0;
    //wprintf("%ls", word);
    for(int i=0 ; i<strlen(word) ; i++)
    {
        if(strcasecmp(word[i], "á") == 0)
        {
            printf("ITS á");
        }
    }

    //return result;
}*/