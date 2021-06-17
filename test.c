// C program to append the contents of
// source file to the destination file
// including header files
#include <stdio.h>
#include <wchar.h>
 
// Function that appends the contents
void appendFiles(char source[], wchar_t *word, char destination[])
{
    // declaring file pointers
    FILE *fp1, *fp2;
 
    // opening files
    fp1 = fopen(source, "a+");
    fp2 = fopen(destination, "a+");
 
    // If file is not found then return.
    if (!fp1 && !fp2) {
        printf("Unable to open/""detect file(s)\n");
        return;
    }
 
    char buf[100];
 
    // writing the contents of
    // source file to destination file.    
    //Copy all lines to output file if the file is empty
    fseek (fp2, 0, SEEK_END);
    int size = ftell(fp2);

    if (0 == size) {
        while (!feof(fp1)) {
            fgets(buf, sizeof(buf), fp1);
            fprintf(fp2, "%s", buf);
        }
    }
    fwprintf(fp2, L"\n");
    fwprintf(fp2, L"%ls", word);
    rewind(fp2);
}
 
// Driver Code
int main()
{
    char source[] = "dict/portuguese-small.txt", destination[] = "dict/output.txt";

    //wchar_t word[100]= "test";
    // calling Function with file names.
    wchar_t *word = L"test3";
    appendFiles(source, word, destination);
 
    return 0;
}