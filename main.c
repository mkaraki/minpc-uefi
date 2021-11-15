#include <uefi/uefi.h>

void readLine(char_t *c, int lim)
{
    ST->ConOut->EnableCursor(ST->ConOut, 1);

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

    ST->ConOut->EnableCursor(ST->ConOut, 0);
}

int getCommandVerb(char_t *source, char_t *verb, int searchLen, int *argpos)
{
    for (int i = 0; i < searchLen; i++)
    {
        if (source[i] == L' ')
        {
            *(verb + i) = 0;
            *argpos = i + 1;
            return 1;
        }
        *(verb + i) = source[i];
    }
    return 0;
}

void genChar_T(unsigned short *from, char_t *to, int len)
{
    for (int i = 0; i < len; i++)
    {
        *(to + i) = (char_t)from[i];
    }
}

void commandHandler(char_t *verb, char_t *arg)
{
    if (strcmp(verb, CL("help")) == 0)
    {
        printf("help\n");
        printf("help    This help\n");
        printf("exit    Exit minpc\n");
    }
    else if (strcmp(verb, CL("exit")) == 0)
    {
        printf("Ok.");
        BS->Exit(IM, 0, 0, L"User put exit command");
    }
    else if (strcmp(verb, CL("clear")) == 0)
    {
        ST->ConOut->ClearScreen(ST->ConOut);
    }
    else if (strcmp(verb, CL("echo")) == 0)
    {
        printf("%s", arg);
    }
    else if (strcmp(verb, CL("tty")) == 0)
    {
        FILE *f = fopen(CL("/dev/serial9600"), CL("w"));
        fwrite(arg, 1, strlen(arg), f);
    }
    else
    {
        printf("Unknown command\n");
    }
}

int main(int argc, char **argv)
{
    ST->ConOut->ClearScreen(ST->ConOut);
    printf("MinPC\n");
    while (1)
    {
        char_t txt[100];
        char_t verb[32];
        int argpos = 0;
        printf("> ");
        readLine(txt, 100);
        getCommandVerb(txt, verb, 32, &argpos);
        char_t arg[100];
        strcpy(arg, txt + argpos);
        commandHandler(verb, arg);
        printf("\n");
    }
    return 0;
}
