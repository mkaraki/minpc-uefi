#include <uefi/uefi.h>

void readLine(char_t *c, int lim)
{
    for (int i = 0; i < lim; i++)
    {
        int chr = getchar();
        putchar(chr);
        if (chr == 8 && i > 0)
        {
            *(c + i - 1) = '\0';
            i -= 2;
            continue;
        }
        if (chr == L'\r')
        {
            putchar('\n');
            *(c + i) = '\0';
            break;
        }
        *(c + i) = (char_t)chr;
    }
}

int main(int argc, char **argv)
{
    ST->ConOut->ClearScreen(ST->ConOut);
    printf("MinPC\n");
    while (1)
    {
        char_t txt[100];
        printf("> ");
        readLine(txt, 100);
        printf(txt);
        printf("\n");
    }
    return 0;
}
