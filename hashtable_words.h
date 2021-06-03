struct ListNode;
typedef struct ListNode *Position;

struct HashTbl;
typedef struct HashTbl *HashTable;

HashTable InitializeWordsTable( int TableSize );
void DestroyWordsTable( HashTable H );

Position FindWord(wchar_t * Key, int wordInInt, HashTable H );
void InsertWord(wchar_t * Key, int wordInInt, HashTable H );
wchar_t* RetrieveWord( Position P );
void PrintHashWordsTable(HashTable T);

HashTable DeleteWord(wchar_t * X, int wordInInt, HashTable T );
HashTable MakeEmpty( HashTable T );
