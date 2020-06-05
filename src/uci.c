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

void uci_listen()
{
    printf("id name %s\n", ENGINE_NAME);
    printf("id author %s\n", ENGINE_AUTHOR);
#ifdef HASHTABLE
    printf("option name Hash type spin default %i min %i max %i\n", HASHTABLE_SIZE_DEFAULT, HASHTABLE_SIZE_MIN, HASHTABLE_SIZE_MAX);
#endif
    printf("uciok\n");

    // board & search information
    s_board board;
    s_search_settings settings;

    // search thread
    pthread_t search_thread;
    s_thread_data data;
    data.board = &board;
    data.settings = &settings;

    set_fen(&board, "startpos");

    char message[4096];
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
                printf("readyok\n");
                break;
            }
            else if(strncmp(part, "ucinewgame", 10) == 0)
            {
                hashtable_clear(hashtable);
                set_fen(&board, "startpos");
                break;
            }
            else if(strncmp(part, "position", 8) == 0)
            {
                part += 9;
                if(strncmp(part, "startpos", 8) == 0)
                {
                    set_fen(&board, "startpos");
                }
                else if(strncmp(part, "fen", 3) == 0)
                {
                    part += 4;
                    set_fen(&board, part);
                }
            }
            else if(strncmp(part, "moves", 5) == 0)
            {
                //part += 6;

                unsigned int i;
                for(i = 0; i < strlen(part)-4; ++i)
                {
                    if(part[i  ] < 'a' || 'h' < part[i  ]) {continue;}
                    if(part[i+1] < '1' || '8' < part[i+1]) {continue;}
                    if(part[i+2] < 'a' || 'h' < part[i+2]) {continue;}
                    if(part[i+3] < '1' || '8' < part[i+3]) {continue;}

                    move_make_ascii(&board, &part[i]);
                }
            }
            else if(strncmp(part, "display", 7) == 0)
            {
                display_board(&board);
                display_history(&board);
                print_moves(&board);
            }
            else if(strncmp(part, "go", 2) == 0)
            {
                /*
                if(pthread_kill(search_thread, 0))
                {
                    pthread_cancel(search_thread);
                }
                */

                // arbitrary default values 1+0
                settings.stop = 0;
                settings.wtime = 60000;
                settings.btime = 60000;
                settings.winc = 0;
                settings.binc = 0;
                settings.movestogo = 20;
                settings.depth = 0;
                settings.movetime = 0;
                // Not implemented yet
                settings.nodes = 0;
                settings.mate = 0;

                // This is a bit ugly
                while(part[1] != '\0')
                {
                    if(strncmp(part, "infinite", 8) == 0)
                    {
                        settings.depth = MAX_DEPTH;
                        break;
                    }
                    else if(strncmp(part, "wtime", 5) == 0)
                    {
                        part += 6;
                        settings.wtime = atoi(part);
                    }
                    else if(strncmp(part, "btime", 5) == 0)
                    {
                        part += 6;
                        settings.btime = atoi(part);
                    }
                    else if(strncmp(part, "winc", 4) == 0)
                    {
                        part += 5;
                        settings.winc = atoi(part);
                    }
                    else if(strncmp(part, "binc", 4) == 0)
                    {
                        part += 5;
                        settings.binc = atoi(part);
                    }
                    else if(strncmp(part, "movestogo", 9) == 0)
                    {
                        part += 10;
                        settings.movestogo = atoi(part);
                    }
                    else if(strncmp(part, "depth", 5) == 0)
                    {
                        part += 6;
                        settings.depth = atoi(part);
                    }
                    else if(strncmp(part, "movetime", 8) == 0)
                    {
                        part += 9;
                        settings.movetime = atoi(part);
                    }

                    part++;
                }

                if(settings.movestogo == 1)
                {
                    // Maintain a small period of buffer time for the search to end
                    settings.wtime -= 50;
                    settings.btime -= 50;
                }

                if(pthread_create(&search_thread, NULL, search_root, &data))
                {
                    fprintf(stderr, "Error creating thread\n");
                }
            }
            else if(strncmp(part, "stop", 4) == 0)
            {
                settings.stop = 1;
                pthread_join(search_thread, NULL);
            }
            else if(strncmp(part, "perft", 5) == 0)
            {
                part += 6;
                uint64_t nodes = 0ULL;
                int depth = atoi(part);

                for(int d = 1; d <= depth; ++d)
                {
                    clock_t start = clock();
                    nodes = perft_search(&board, d);
                    clock_t end = clock();
                    clock_t time_taken = 1000*(end-start)/CLOCKS_PER_SEC;
                    printf("info depth %i nodes %" PRIu64 " time %i\n", d, nodes, (int)time_taken);
                }
                printf("nodes %" PRIu64 "\n", nodes);
            }
            else if(strncmp(part, "setoption", 9) == 0)
            {
#ifdef HASHTABLE
                if(strncmp(part, "setoption name Hash value", 25) == 0)
                {
                    part += 26;
                    int size = atoi(part);

                    if(HASHTABLE_SIZE_MIN <= size && size <= HASHTABLE_SIZE_MAX)
                    {
                        while(size >= HASHTABLE_SIZE_MIN)
                        {
                            int r = hashtable_init(hashtable, size);

                            if(r != -1) {break;}

                            size = size>>1;
                        }

#ifndef NDEBUG
                        printf("Total size: %iMB\n", hashtable->size_bytes/1024/1024);
                        printf("Entry size: %" PRIu64 "B\n", sizeof(s_hashtable_entry));
                        printf("Max entries: %i\n", hashtable->max_entries);
#endif
                    }
                }
#endif
            }

            part++;
        }
    }

    return;
}
