typedef int ElementType;


/*#ifndef _HashSep_H
#define _HashSep_H*/

struct ListNode;
typedef struct ListNode *Position;

struct HashTbl;
typedef struct HashTbl *HashTable;

HashTable InitializeKeysTable( int TableSize );
void DestroyKeysTable( HashTable H );

//Position Find( ElementType Key, HashTable H );
Position FindNthKey(char *Key, int Index, HashTable T );
//void Insert( ElementType Key, HashTable H );
void InsertNthKey(char *Key, int Index, HashTable T );
void InsertT9Keys(HashTable T);
const char* RetrieveKey( Position P );
void PrintHashKeysTable(HashTable T);

//HashTable Delete( ElementType X, HashTable T );
//HashTable MakeEmpty( HashTable T );

//#endif