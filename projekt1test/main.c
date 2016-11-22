#include <stdio.h>
#include <stdlib.h>

int main()
{
    char c[20] = "ahoj jak se mas. ";
    int i;

    for(i = 0; i < 20; i++)
    printf("%x ",c[i]);
    return 0;
}
