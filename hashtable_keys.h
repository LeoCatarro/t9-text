typedef int ElementType;
typedef unsigned int Index;

/*#ifndef _HashSep_H
#define _HashSep_H*/

struct ListNode;
typedef struct ListNode *Position;

struct HashTbl;
typedef struct HashTbl *HashTable;

HashTable InitializeKeysTable( int TableSize );
void DestroyKeysTable( HashTable H );

Index FindKey(wchar_t Key, HashTable H );
Position FindNthKey(wchar_t Key, int Index, HashTable T );
//void Insert( ElementType Key, HashTable H );
void InsertNthKey(wchar_t Key, int Index, HashTable T );
void InsertT9Keys(HashTable T);
wchar_t RetrieveKey( Position P );
void PrintHashKeysTable(HashTable T);

//HashTable Delete( ElementType X, HashTable T );
//HashTable MakeEmpty( HashTable T );

//#endif