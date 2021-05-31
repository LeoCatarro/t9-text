#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <wchar.h>
#include <locale.h>
#include <time.h>

#define BILLION 1000000000L;
  
int main(void)
{
    struct timespec start, stop;
    double accum;

    wchar_t *word = L"á";
    setlocale(LC_ALL, "");

    clock_gettime(CLOCK_REALTIME, &start);

    /*FILE *f = fopen("dictionaries/portuguese.txt", "rb");
    fseek(f, 0, SEEK_END);
    long fsize = ftell(f);
    fseek(f, 0, SEEK_SET);  

    char *string = malloc(fsize + 1);
    fread(string, 1, fsize, f);
    string[fsize] = '\0';*/

    
    clock_gettime(CLOCK_REALTIME, &stop);
    
    accum = (stop.tv_sec - start.tv_sec) + (stop.tv_nsec - start.tv_nsec) / (double)BILLION;
    printf("\n%f\n", accum);

    //fclose(f);
    return 0;
}