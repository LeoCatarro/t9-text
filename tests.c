#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>
#include <string.h>
#include <locale.h>
#include <wchar.h>
#include <math.h>
#include <wctype.h>
#include <time.h>
#include <stdbool.h>
#include <unistd.h>
#include <stringapiset.h>

  

wchar_t * char2wchar(const char* cchar) 
{     
    setlocale(LC_ALL, "");
    wchar_t m_wchar[len+1];    
    int len = MultiByteToWideChar( CP_ACP ,0,cchar ,strlen( cchar), NULL,0);   
    MultiByteToWideChar( CP_ACP ,0,cchar,strlen( cchar),m_wchar,len);     
    m_wchar[len]= '\0' ;    
    return m_wchar; 
}

