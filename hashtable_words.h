struct ListNode;
typedef struct ListNode *Position;

struct HashTbl;
typedef struct HashTbl *HashTable;

typedef Position List;


struct ListNode{
    wchar_t *Element;
    Position Next;
};


struct HashTbl{
    int TableSize;
    List *TheLists;
};


HashTable InitializeWordsTable( int TableSize );
void DestroyWordsTable( HashTable H );

Position FindWord(wchar_t * Key, long wordInInt, HashTable H );
void InsertWord(wchar_t * Key, long wordInInt, HashTable H );
wchar_t* RetrieveWord( Position P );
void PrintHashWordsTable(HashTable T);

HashTable DeleteWord(wchar_t * X, long wordInInt, HashTable T );
HashTable MakeEmpty( HashTable T );
