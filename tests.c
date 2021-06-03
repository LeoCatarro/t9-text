#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <wchar.h>
#include <locale.h>
#include <time.h>
#include <wctype.h>

  
int main(void)
{
    wchar_t *str = L"Aqui";

    for(int i=0 ; i<wcslen(str); i++)
    {
        putwchar(towlower(str[i]));
    }
    return 0;
}