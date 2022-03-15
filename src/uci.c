#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include <pthread.h>
#include <time.h>
#include "defs.h"
#include "chess/move.h"
#include "chess/display.h"
#include "chess/perft.h"
#include "search/search.h"
#include "search/hashtable.h"
#include "uci.h"

int hashtable_size = HASHTABLE_SIZE_DEFAULT;

void isready()
{
    printf("readyok\n");
}

void ucinewgame(s_board *board)
{
    assert(board);

    hashtable_clear(&hashtable);
    set_fen(board, "startpos");
}

void position(s_board *board, char *part)
{
    assert(board);
    assert(part);

    part += 9;
    if(strncmp(part, "startpos", 8) == 0)
    {
        set_fen(board, "startpos");
    }
    else if(strncmp(part, "fen", 3) == 0)
    {
        part += 4;
        set_fen(board, part);
    }
}

void moves(s_board *board, char *part)
{
    assert(board);
    assert(part);

    for(unsigned int i = 0; i < strlen(part)-4; ++i)
    {
        if(part[i  ] < 'a' || 'h' < part[i  ]) {continue;}
        if(part[i+1] < '1' || '8' < part[i+1]) {continue;}
        if(part[i+2] < 'a' || 'h' < part[i+2]) {continue;}
        if(part[i+3] < '1' || '8' < part[i+3]) {continue;}

        move_make_ascii(board, &part[i]);
    }
}

void perft(s_board *board, char *part)
{
    assert(board);
    assert(part);

    part += 6;
    uint64_t nodes = 0ULL;
    int depth = atoi(part);

    for(int d = 1; d <= depth; ++d)
    {
        clock_t start = clock();
        nodes = perft_search(board, d);
        clock_t end = clock();
        clock_t time_taken = 1000*(end-start)/CLOCKS_PER_SEC;
        printf("info depth %i nodes %" PRIu64 " time %i\n", d, nodes, (int)time_taken);
    }
    printf("nodes %" PRIu64 "\n", nodes);
}

void setoption(char *part)
{
    assert(part);

    if(strncmp(part, "setoption name Hash value", 25) == 0)
    {
        part += 26;
        int size = atoi(part);
        if (size < HASHTABLE_SIZE_MIN)
        {
            hashtable_size = HASHTABLE_SIZE_MIN;
        }
        else if (size > HASHTABLE_SIZE_MAX)
        {
            hashtable_size = HASHTABLE_SIZE_MAX;
        }
        else
        {
            hashtable_size = size;
        }
    }
}

/*  go returns the number of characters read in part, so uci_listen
    can skip that many characters after the entire command is read.
    This prevents uci_listen from treating the "go" in "movestogo"
    as a second go command, which is known to cause issues. */
int go(pthread_t *search_thread, s_thread_data *data, char *part)
{
    assert(search_thread);
    assert(data);
    assert(part);
    assert(data->board);
    assert(data->settings);

    /*
    if(pthread_kill(search_thread, 0))
    {
        pthread_cancel(search_thread);
    }
    */

    // arbitrary default values 1+0
    data->settings->stop = 0;
    data->settings->wtime = 60000;
    data->settings->btime = 60000;
    data->settings->winc = 0;
    data->settings->binc = 0;
    data->settings->movestogo = 20;
    data->settings->depth = 0;
    data->settings->movetime = 0;
    // Not implemented yet
    data->settings->nodes = 0;
    data->settings->mate = 0;

    // This is a bit ugly
    int chars_read = 0;
    while(part[1] != '\0')
    {
        if(strncmp(part, "infinite", 8) == 0)
        {
            data->settings->depth = MAX_DEPTH;
            break;
        }
        else if(strncmp(part, "wtime", 5) == 0)
        {
            part += 6;
            chars_read += 6;
            data->settings->wtime = atoi(part);
        }
        else if(strncmp(part, "btime", 5) == 0)
        {
            part += 6;
            chars_read += 6;
            data->settings->btime = atoi(part);
        }
        else if(strncmp(part, "winc", 4) == 0)
        {
            part += 5;
            chars_read += 5;
            data->settings->winc = atoi(part);
        }
        else if(strncmp(part, "binc", 4) == 0)
        {
            part += 5;
            chars_read += 5;
            data->settings->binc = atoi(part);
        }
        else if(strncmp(part, "movestogo", 9) == 0)
        {
            part += 10;
            chars_read += 10;
            data->settings->movestogo = atoi(part);
        }
        else if(strncmp(part, "depth", 5) == 0)
        {
            part += 6;
            chars_read += 6;
            data->settings->depth = atoi(part);
        }
        else if(strncmp(part, "movetime", 8) == 0)
        {
            part += 9;
            chars_read += 9;
            data->settings->movetime = atoi(part);
        }

        part++;
        chars_read++;
    }

    if(data->settings->movestogo == 1)
    {
        // Maintain a small period of buffer time for the search to end
        data->settings->wtime -= 50;
        data->settings->btime -= 50;
    }

    if(pthread_create(search_thread, NULL, search_root, data))
    {
        fprintf(stderr, "Error creating thread\n");
    }
    
    return chars_read;
}

void uci_listen()
{
    printf("id name %s\n", ENGINE_NAME);
    printf("id author %s\n", ENGINE_AUTHOR);
    printf("option name Hash type spin default %i min %i max %i\n", HASHTABLE_SIZE_DEFAULT, HASHTABLE_SIZE_MIN, HASHTABLE_SIZE_MAX);
    printf("uciok\n");

    char message[4096];

    // Wait for the first isready before we do anything
    while(1)
    {
        if(!fgets(message, 4096, stdin))
        {
            return;
        }

        char *part = message;

        if(strncmp(part, "quit", 4) == 0)
        {
            return;
        }
        else if(strncmp(part, "isready", 7) == 0)
        {
            break;
        }
        else if(strncmp(part, "setoption", 9) == 0)
        {
            setoption(part);
        }
    }

    // Create hashtable
    hashtable_resize(&hashtable, hashtable_size);

    // Wait until the hashtable has been created before replying to isready
    isready();

    // board & search information
    s_board board;
    s_search_settings settings;

    // search thread
    pthread_t search_thread;
    s_thread_data data;
    data.board = &board;
    data.settings = &settings;

    set_fen(&board, "startpos");

    int exit = 0;
    while(!exit)
    {
        if(!fgets(message, 4096, stdin))
        {
            break;
        }

        char *part = message;

        while(part[0] != '\0')
        {
            if(strncmp(part, "quit", 4) == 0)
            {
                exit = 1;
                break;
            }
            else if(strncmp(part, "isready", 7) == 0)
            {
                isready();
                break;
            }
            else if(strncmp(part, "ucinewgame", 10) == 0)
            {
                ucinewgame(&board);
                break;
            }
            else if(strncmp(part, "position", 8) == 0)
            {
                position(&board, part);
            }
            else if(strncmp(part, "moves", 5) == 0)
            {
                moves(&board, part);
            }
            else if(strncmp(part, "display", 7) == 0)
            {
                display_board(&board);
                display_history(&board);
                print_moves(&board);
            }
            else if(strncmp(part, "go", 2) == 0)
            {
                part += go(&search_thread, &data, part);
            }
            else if(strncmp(part, "stop", 4) == 0)
            {
                settings.stop = 1;
                pthread_join(search_thread, NULL);
            }
            else if(strncmp(part, "perft", 5) == 0)
            {
                perft(&board, part);
            }
            else if(strncmp(part, "setoption", 9) == 0)
            {
                setoption(part);

                // Try recreate hashtable
                hashtable_resize(&hashtable, hashtable_size);
            }

            part++;
        }
    }

    hashtable_free(&hashtable);
}
