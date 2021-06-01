//typedef int ElementType;

/*#ifndef _HashSep_H
#define _HashSep_H*/

struct ListNode;
typedef struct ListNode *Position;

struct HashTbl;
typedef struct HashTbl *HashTable;

HashTable InitializeWordsTable( int TableSize );
void DestroyWordsTable( HashTable H );

Position FindWord(const char* Key, HashTable H );
void InsertWord(const char* Key, HashTable H );
const char* RetrieveWord( Position P );
void PrintHashWordsTable(HashTable T);

HashTable DeleteWord(const char* X, HashTable T );
HashTable MakeEmpty( HashTable T );
//#endif