#include <stdio.h>
#include <string.h>
#include <stdlib.h>

int main () 
{
    FILE *f = fopen("portuguese-large.txt", "rb");
    fseek(f, 0, SEEK_END);
    long fsize = ftell(f);
    fseek(f, 0, SEEK_SET);  /* same as rewind(f); */

    char *string = malloc(fsize + 1);
    fread(string, 1, fsize, f);

    printf("%s", string);

    fclose(f);

    string[fsize] = 0;

    return 0;
}