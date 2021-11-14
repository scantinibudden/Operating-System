#include <stdio.h>

int main(void)
{
    int hola = 5;
    hola++;
    hola %= 5;

    printf("%d", hola);
    return 0;
}
