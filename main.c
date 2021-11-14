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
    char_t v_help[5];
    genChar_T(L"help", v_help, 5);

    char_t v_exit[5];
    genChar_T(L"exit", v_exit, 5);

    char_t v_clear[6];
    genChar_T(L"clear", v_clear, 6);

    char_t v_echo[5];
    genChar_T(L"echo", v_echo, 5);

    if (strcmp(verb, v_help) == 0)
    {
        printf("help\n");
        printf("help    This help\n");
        printf("exit    Exit minpc\n");
    }
    else if (strcmp(verb, v_exit) == 0)
    {
        printf("Ok.");
        BS->Exit(IM, 0, 0, L"User put exit command");
    }
    else if (strcmp(verb, v_clear) == 0)
    {
        ST->ConOut->ClearScreen(ST->ConOut);
    }
    else if (strcmp(verb, v_echo) == 0)
    {
        printf("%s", arg);
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
