#include "fatal.h"
#include "hashtable_words.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <wchar.h>


#define MAX_LINE_SIZE 233

#define MinTableSize (10)
typedef unsigned int Index;
typedef Position List;


struct ListNode{
    wchar_t *Element;
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
/*Index HashWords(const char *Key, int TableSize )
{
    unsigned int HashVal = 0;

    while( *Key != '\0' )
        HashVal += *Key++;
   
    return HashVal % TableSize;
}*/
unsigned int HashWords(int Key, int TableSize )
{  
    return Key % TableSize;
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
Position FindWord(wchar_t *Key, int wordinInt, HashTable H )
{   
    List L = H->TheLists[HashWords( wordinInt, H->TableSize )];
    Position P = L->Next;

    while( P != NULL && wcscmp(P->Element, Key)!=0 )
    {
        P = P->Next;
    }
    //printf("[Info]: Finded Node with Element [%s]", P->Element);
    return P;
}


/* Insert the Element Key passed as argument in HashTable H */
void InsertWord(wchar_t * Key, int wordInInt, HashTable H ){
    Position Pos, NewCell;
    List L;
    Pos = FindWord( Key, wordInInt, H );
    //Key is not found
    if( Pos == NULL ){  
        NewCell = malloc( sizeof( struct ListNode ) );
        NewCell->Element = (wchar_t*)malloc(sizeof(Key)+1);

        if( NewCell == NULL )
            FatalError( "Out of space!!!" );
        
        else{
            L = H->TheLists[ HashWords( wordInInt, H->TableSize ) ];
            NewCell->Next = L->Next;
            //NewCell->Element = Key; 
            NewCell->Element = Key;
            L->Next = NewCell;
            printf("Inserted: %ls at index: %d\n", Key, HashWords( wordInInt, H->TableSize ));
        }
    }

    //Key is found in HashTable
    else
    {   
        printf("Pos->Element: %ls\n", Pos->Element);
        //If the key is found in HT, we need to create another node 
        //to insert the element inside the list of the current hashtable position
        NewCell = malloc( sizeof( struct ListNode ) );
        NewCell->Element = (wchar_t*)malloc(sizeof(Key)+1);
        Pos->Next = NewCell;
        NewCell->Element = Key;
        NewCell->Next = NULL;
        printf("Inserted: %ls at index: %d\n", Key, HashWords( wordInInt, H->TableSize ));
    }
}

/* Print the Element in Node P */
wchar_t* RetrieveWord( Position P ){
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
HashTable DeleteWord(wchar_t * X, int wordInInt, HashTable T ){
    
    // Find the key of the Element X
    unsigned int key = HashWords(wordInInt, T->TableSize);

    //Key finded
    if(key != -1)
    {
        //If the key is finded we need to iterate over the List of that Key Hashtable Position to find the X
        Position prevP = T->TheLists[key];
        Position P = T->TheLists[key]->Next;

        //X finded 
        while(P != NULL)
        {
            if(wcscmp(P->Element, X) == 0 && P->Next != NULL)
            {   
                printf("Deleted [%ls] at index %d from HashTable\n", P->Element, key);
                prevP->Next = P->Next->Next;
                return T;
            }

            else if(wcscmp(P->Element, X) && P->Next == NULL)
            {
                prevP->Next = NULL;
                printf("Deleted [%ls] at index %d from HashTable\n", P->Element, key);
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
                P->Element[wcslen(P->Element)+1] = 0;
                printf("%ls", P->Element);
                printf(" %ld", wcslen(P->Element));
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