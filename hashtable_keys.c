#include "fatal.h"
#include "hashtable_keys.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


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
Index Hash( ElementType Key, int TableSize ){
    return Key % TableSize;
}

/* Initialize the Table, 
making the correspondent malloc() and allocate the array of lists and them Headers 
to use on HashTable positions */
HashTable InitializeTable( int TableSize ){
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
/*
Position Find( ElementType Key, HashTable H )
{
    List L = H->TheLists[Hash( Key, H->TableSize )];
    Position P = L->Next;

    while( P != NULL && P->Element != Key )
        //Probably need strcmp!!
        P = P->Next;
    
    return P;
}
*/


/* Find the key in the given index */
Position FindNth(char *Key, int Index, HashTable H )
{
    List L = H->TheLists[Index];
    Position P = L->Next;

    while( P != NULL && strcmp(P->Element, Key)!= 0)
        /* Probably need strcmp!! */
        P = P->Next;
    
    return P;
}


/* Insert the Element Key passed as argument in HashTable H */
void InsertNth(char *Key, int Index, HashTable H ){
    Position Pos, NewCell;
    List L;

    Pos = FindNth(Key, Index, H);

    /* Key is not found */
    if( Pos == NULL ){  
        NewCell = malloc(sizeof(struct ListNode ));

        if( NewCell == NULL )
            FatalError( "Out of space!!!" );
        
        else{
            L = H->TheLists[Index];
            NewCell->Next = L->Next;
            NewCell->Element = malloc(sizeof(Key));
            strcpy(NewCell->Element, Key);
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
        NewCell->Element = malloc(sizeof(Key));
        strcpy(NewCell->Element,Key);
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
const char* Retrieve( Position P ){
    return P->Element;
}


/* Free the ram occupied from HashTable */
void DestroyTable( HashTable H ){
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
void PrintHashTable(HashTable T)
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
                printf("%s", P->Element);
                P = P->Next;

                //If is not the last element
                if(P != NULL)
                    printf(", ");
            }
            printf("]\n");
        }
        else
            printf("%d\t[%s]\n", i, "--");
    }
}

/* Insert the T9 Keys in hashtable */
void InsertT9Keys(HashTable T)
{
    //Key 2
    InsertNth("a", 2, T);
    InsertNth("b", 2, T);
    InsertNth("c", 2, T);
    InsertNth("á", 2, T);
    InsertNth("à", 2, T);
    InsertNth("â", 2, T);
    InsertNth("ã", 2, T);
    InsertNth("ç", 2, T);
    //Key 3
    InsertNth("d", 3, T);
    InsertNth("e", 3, T);
    InsertNth("f", 3, T);
    InsertNth("é", 3, T);
    InsertNth("ê", 3, T);
    //Key 4
    InsertNth("g", 4, T);
    InsertNth("h", 4, T);
    InsertNth("i", 4, T);
    InsertNth("í", 4, T);
    //Key 5
    InsertNth("j", 5, T);
    InsertNth("k", 5, T);
    InsertNth("l", 5, T);
    //Key 6
    InsertNth("m", 6, T);
    InsertNth("n", 6, T);
    InsertNth("o", 6, T);
    InsertNth("ó", 6, T);
    InsertNth("ô", 6, T);
    InsertNth("õ", 6, T);
    //Key 7
    InsertNth("p", 7, T);
    InsertNth("q", 7, T);
    InsertNth("r", 7, T);
    InsertNth("s", 7, T);
    //Key 8
    InsertNth("t", 8, T);
    InsertNth("u", 8, T);
    InsertNth("v", 8, T);
    InsertNth("ú", 8, T);
    //Key 9
    InsertNth("w", 9, T);
    InsertNth("x", 9, T);
    InsertNth("y", 9, T);
    InsertNth("z", 9, T);
}
