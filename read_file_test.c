#include <stdio.h>
#include <string.h>

#define SIZE 20

/*   
int main(void)
{
    FILE * fp;
    char * line = NULL;
    size_t len = 0;
    size_t read;

    fp = fopen("dictionaries/portuguese-small.txt", "r");

    if (fp == NULL)
        exit(EXIT_FAILURE);

    while ((read = getline(&line, &len, fp)) != -1) {
        printf("Retrieved line of length %zu:\n", read);
        printf("%s", line);

        //Line To Numbers converter

    }

    //fclose(fp);

    return 0;

}*/