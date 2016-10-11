#include "defs.h"
#include <string.h>
#include <pthread.h>

void uci_listen()
{
  int exit = 1;
  
  // board & search information
  s_board *board = (s_board*) malloc(1*sizeof(s_board));
  s_search_info *info = (s_search_info*) malloc(1*sizeof(s_search_info));
  
  // search thread
  pthread_t search_thread;
  s_thread_data data;
  data.board = board;
  data.info = info;
  
  set_fen(board, START_FEN);
  
  char message[4096];
  while(exit == 1)
  {
    char *r = fgets(message, 4096, stdin);
    if(r == NULL)
    {
      break;
    }
    
    char *part = message;
    
    while(part[0] != '\0')
    {
      if(strncmp(part, "quit", 4) == 0)
      {
        exit = 0;
        break;
      }
      else if(strncmp(part, "isready", 7) == 0)
      {
        GUI_Send("readyok\n");
        break;
      }
      else if(strncmp(part, "ucinewgame", 10) == 0)
      {
        hashtable_clear(hashtable);
        break;
      }
      else if(strncmp(part, "position", 8) == 0)
      {
        part += 9;
        if(strncmp(part, "startpos", 8) == 0)
        {
          set_fen(board, START_FEN);
        }
        else if(strncmp(part, "fen", 3) == 0)
        {
          part += 4;
          set_fen(board, part);
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
          
          move_make_ascii(board, &part[i]);
          board->turn = 1-(board->turn);
        }
      }
      else if(strncmp(part, "display", 7) == 0)
      {
        display_board(board);
        display_history(board);
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
        info->wtime = 60000;
        info->btime = 60000;
        info->winc = 0;
        info->binc = 0;
        info->movestogo = 20;
        
        // This is a bit ugly
        while(part[1] != '\0')
        {
          if(strncmp(part, "infinite", 8) == 0)
          {
            info->wtime = INT_MAX;
            info->btime = INT_MAX;
            break;
          }
          
          if(strncmp(part, "wtime", 5) == 0)
          {
            part += 6;
            info->wtime = atoi(part);
          }
          else if(strncmp(part, "btime", 5) == 0)
          {
            part += 6;
            info->btime = atoi(part);
          }
          else if(strncmp(part, "winc", 4) == 0)
          {
            part += 5;
            info->winc = atoi(part);
          }
          else if(strncmp(part, "binc", 4) == 0)
          {
            part += 5;
            info->binc = atoi(part);
          }
          else if(strncmp(part, "movestogo", 9) == 0)
          {
            part += 10;
            info->movestogo = atoi(part);
          }
          
          part++;
        }
        
        info->depth = -1;
        info->nodes = -1;
        info->mate = -1;
        info->movetime = -1;
            
        if(info->movestogo == 1)
        {
          info->wtime -= 50;
          info->btime -= 50;
        }
        
        search_info_set(*info);
        
        if(pthread_create(&search_thread, NULL, search_base, &data))
        {
          fprintf(stderr, "Error creating thread\n");
        }
      }
      else if(strncmp(part, "stop", 4) == 0)
      {
        info->time_max = 0;
        search_info_set(*info);
        pthread_join(search_thread, NULL);
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
                printf("Entry size: %"PRIdPTR"B\n", sizeof(s_hashtable_entry));
                printf("Max entries: %i\n", hashtable->max_entries);
              #endif
            }
          }
        #endif
      }
      
      part++;
    }
  }
  
  free(board);
  free(info);
  return;
}