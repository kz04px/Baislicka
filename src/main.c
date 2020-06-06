#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "defs.h"
#include "chess/bitboards.h"
#include "chess/perft.h"
#include "chess/zobrist.h"
#include "search/hashtable.h"
#include "uci.h"

int main()
{
    srand(0x480ec5cb);
    bitboards_init();
    key_init();

    // kill buffering
    setbuf(stdout, NULL);
    setbuf(stdin, NULL);
    setvbuf(stdout, NULL, _IONBF, 0);
    setvbuf(stdin, NULL, _IONBF, 0);

#ifndef NDEBUG
        printf("DEBUG BUILD\n");
#endif

    char message[4096];
    if (!fgets(message, 4096, stdin))
    {
        return 1;
    }

    if(strncmp(message, "uci", 3) == 0)
    {
        uci_listen();
    }
    else if(strncmp(message, "xboard", 6) == 0)
    {
        printf("xboard protocol not implemented\n");
    }
    else if(strncmp(message, "about", 5) == 0)
    {
        printf("Engine:  %s\n", ENGINE_NAME);
        printf("Version: %s\n", ENGINE_VERSION);
        printf("Author:  %s\n", ENGINE_AUTHOR);
        printf("\n");

        printf("Compiled:\n");
        printf("Date: %s\n", __DATE__);
        printf("Time: %s\n", __TIME__);
        printf("\n");

        printf("Max search depth: %i\n", MAX_DEPTH);
        printf("Default hash size: %iMB\n", HASHTABLE_SIZE_DEFAULT);
        printf("sizeof(s_board): %" PRIu64 "B\n", sizeof(s_board));
        printf("sizeof(s_move):  %" PRIu64 "B\n", sizeof(s_move));
        printf("sizeof(s_hashtable_entry):  %" PRIu64 "B\n", sizeof(s_hashtable_entry));
        printf("Hashtable entries per MB:  %" PRIu64 "\n", 1024*1024/sizeof(s_hashtable_entry));
        printf("\n");
    }
    else
    {
        printf("Unknown protocol\n");
    }

    return 0;
}
