//typedef int ElementType;


#ifndef _HashSep_H
#define _HashSep_H

struct ListNode;
typedef struct ListNode *Position;

struct HashTbl;
typedef struct HashTbl *HashTable;

HashTable InitializeTable( int TableSize );
void DestroyTable( HashTable H );

Position Find(const char* Key, HashTable H );
void Insert(const char* Key, HashTable H );
const char* Retrieve( Position P );
void PrintHashTable(HashTable T);

HashTable Delete(const char* X, HashTable T );
HashTable MakeEmpty( HashTable T );

#endif