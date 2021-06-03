#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <wchar.h>
#include <wctype.h>

#include "fatal.h"
#include "hashtable_keys.h"

#define MinTableSize (10)
typedef unsigned int Index;
typedef Position List;


struct ListNode{
    wchar_t Element;
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
Index HashKeys( wchar_t Key, int TableSize ){
    return Key % TableSize;
}

/* Initialize the Table, 
making the correspondent malloc() and allocate the array of lists and them Headers 
to use on HashTable positions */
HashTable InitializeKeysTable( int TableSize ){
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

int FindKey( wchar_t Key, HashTable H )
{
    for(int i=0 ; i < H->TableSize ; i++)
    {   
        Position P = H->TheLists[i]->Next;

        while(P != NULL)
        {
            if(P->Element == Key)
                return i;
            else
                P = P->Next;
        }
    }
    return -1;
}


/* Find the key in the given index */
Position FindNthKey(wchar_t Key, int Index, HashTable H )
{
    List L = H->TheLists[Index];
    Position P = L->Next;

    while( P != NULL && P->Element != Key)
        P = P->Next;
    
    return P;
}


/* Insert the Element Key passed as argument in HashTable H */
void InsertNthKey(wchar_t Key, int Index, HashTable H ){
    Position Pos, NewCell;
    List L;

    Pos = FindNthKey(Key, Index, H);

    /* Key is not found */
    if( Pos == NULL ){  
        NewCell = malloc(sizeof(struct ListNode ));

        if( NewCell == NULL )
            FatalError( "Out of space!!!" );
        
        else{
            L = H->TheLists[Index];
            NewCell->Next = L->Next;
            NewCell->Element = Key;
            L->Next = NewCell;
        }
    }

    /* Key is found in HashTable */
    else
    {   
        //If the key is found in HT, we need to create another node 
        //to insert the element inside the list of the current hashtable position
        NewCell = malloc( sizeof( struct ListNode ) );
        Pos->Next = NewCell;
        NewCell->Element = Key;
        NewCell->Next = NULL;
    }
}


/* Insert the Element Key passed as argument in HashTable H */
/*
void Insert( ElementType Key, HashTable H ){
    Position Pos, NewCell;
    List L;

    Pos = Find( Key, H );

    //Key is not found
    if( Pos == NULL ){  
        NewCell = malloc( sizeof( struct ListNode ) );

        if( NewCell == NULL )
            FatalError( "Out of space!!!" );
        
        else{
            L = H->TheLists[ Hash( Key, H->TableSize ) ];
            NewCell->Next = L->Next;
            NewCell->Element = Key; 
            L->Next = NewCell;
        }
    }

    //Key is found in HashTable
    else
    {   
        //If the key is found in HT, we need to create another node 
        //to insert the element inside the list of the current hashtable position
        NewCell = malloc( sizeof( struct ListNode ) );
        Pos->Next = NewCell;
        NewCell->Element = Key;
        NewCell->Next = NULL;
    }
}
*/

/* Print the Element in Node P */
wchar_t RetrieveKey( Position P ){
    return P->Element;
}


/* Free the ram occupied from HashTable */
void DestroyKeysTable( HashTable H ){
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
/*
HashTable Delete( ElementType X, HashTable T ){
    
    // Find the key of the Element X
    int key = Hash(X, T->TableSize);

    //Key finded
    if(key != -1)
    {
        //If the key is finded we need to iterate over the List of that Key Hashtable Position to find the X
        Position prevP = T->TheLists[key];
        Position P = T->TheLists[key]->Next;

        //X finded 
        while(P != NULL)
        {
            if(P->Element == X && P->Next != NULL)
            {   
                printf("Deleted %d at index %d from HashTable\n", P->Element, key);
                prevP->Next = P->Next->Next;
                return T;
            }

            else if(P->Element == X && P->Next == NULL)
            {
                prevP->Next = NULL;
                printf("Deleted %d at index %d from HashTable\n", P->Element, key);
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
*/


/* Free the allocated memory of Hashtable */
/*HashTable MakeEmpty( HashTable T ){
    for(int i=0 ; i<T->TableSize; i++)
    {
        Position P = T->TheLists[i];
        while(P->Next != NULL)
        {
            P = P->Next;
            free(P);
        }
    }

    printf("\nTODO()\n");
    return T;
}
*/

/* Display HashTable in Terminal */
void PrintHashKeysTable(HashTable T)
{
    printf("* Printing Keys HashTable *\n");

    for(int i=0 ; i < T->TableSize ; i++)
    {   
        Position P = T->TheLists[i]->Next;

        if(P != NULL)
        {
            printf("%d\t[", i);
            while(P != NULL)
            {
                printf("%lc", P->Element);
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

/* Inserts the T9 Keys and the characters for each key in hashtable */
void InsertT9Keys(HashTable T)
{
    //Key 2
    InsertNthKey(L'á', 2, T);
    InsertNthKey(L'a', 2, T);
    InsertNthKey(L'b', 2, T);
    InsertNthKey(L'c', 2, T);
    InsertNthKey(L'á', 2, T);
    InsertNthKey(L'à', 2, T);
    InsertNthKey(L'â', 2, T);
    InsertNthKey(L'ã', 2, T);
    InsertNthKey(L'ç', 2, T);
    //Key 3
    InsertNthKey(L'd', 3, T);
    InsertNthKey(L'e', 3, T);
    InsertNthKey(L'f', 3, T);
    InsertNthKey(L'é', 3, T);
    InsertNthKey(L'ê', 3, T);
    //Key 4
    InsertNthKey(L'g', 4, T);
    InsertNthKey(L'h', 4, T);
    InsertNthKey(L'i', 4, T);
    InsertNthKey(L'í', 4, T);
    //Key 5
    InsertNthKey(L'j', 5, T);
    InsertNthKey(L'k', 5, T);
    InsertNthKey(L'l', 5, T);
    //Key 6
    InsertNthKey(L'm', 6, T);
    InsertNthKey(L'n', 6, T);
    InsertNthKey(L'o', 6, T);
    InsertNthKey(L'ó', 6, T);
    InsertNthKey(L'ô', 6, T);
    InsertNthKey(L'õ', 6, T);
    //Key 7
    InsertNthKey(L'p', 7, T);
    InsertNthKey(L'q', 7, T);
    InsertNthKey(L'r', 7, T);
    InsertNthKey(L's', 7, T);
    //Key 8
    InsertNthKey(L't', 8, T);
    InsertNthKey(L'u', 8, T);
    InsertNthKey(L'v', 8, T);
    InsertNthKey(L'ú', 8, T);
    //Key 9
    InsertNthKey(L'w', 9, T);
    InsertNthKey(L'x', 9, T);
    InsertNthKey(L'y', 9, T);
    InsertNthKey(L'z', 9, T);
}

/* Converts a string like "olá" to the corresponding sequence of integers(t9-keys). */
unsigned int StringToIntAccordingT9Keys(wchar_t *word, HashTable KeysTable)
{

    wchar_t *cleanWord = (wchar_t*)malloc(sizeof(word));

    //Verification of upper case Letters
    for(int i=0 ; i<wcslen(word); i++)
    {
        cleanWord[i]= towlower(word[i]);
    }

    printf("Clean Word: %ls\n", cleanWord);
    unsigned int result = 0;

    for(int i=0; i<wcslen(cleanWord); i++)
        {
            int index = FindKey(cleanWord[i], KeysTable);
            result += index*(pow(10, wcslen(cleanWord)-i-1));
        }
    return result;
}
