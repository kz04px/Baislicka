#include "defs.h"
#include "bitboards.h"
#include "hashtable.h"
#include "perft.h"
#include "uci.h"
#include <string.h>

int main()
{
  srand(0x480ec5cb);

  bitboards_init();

  #ifdef HASHTABLE
    key_init();
    hashtable = malloc(1*sizeof(*hashtable));
    hashtable->entries = NULL;

    int size = HASHTABLE_SIZE_DEFAULT;
    while(size >= HASHTABLE_SIZE_MIN)
    {
      int r = hashtable_init(hashtable, size);

      if(r != -1) {break;}

      size = size>>1;
    }
  #endif

  ksc_king[WHITE] = U64_E1 | U64_G1;
  ksc_king[BLACK] = U64_E8 | U64_G8;

  ksc_rook[WHITE] = U64_F1 | U64_H1;
  ksc_rook[BLACK] = U64_F8 | U64_H8;

  qsc_king[WHITE] = U64_E1 | U64_C1;
  qsc_king[BLACK] = U64_E8 | U64_C8;

  qsc_rook[WHITE] = U64_D1 | U64_A1;
  qsc_rook[BLACK] = U64_D8 | U64_A8;

  // kill buffering
  setbuf(stdout, NULL);
  setbuf(stdin, NULL);
  setvbuf(stdout, NULL, _IONBF, 0);
  setvbuf(stdin, NULL, _IONBF, 0);

  char message[4096];
  fgets(message, 4096, stdin);
  if(strncmp(message, "uci", 3) == 0)
  {
    uci_listen();
  }
  else if(strncmp(message, "xboard", 6) == 0)
  {
    GUI_Send("xboard protocol not implemented\n");
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
    #ifdef HASHTABLE
      printf("sizeof(s_hashtable_entry):  %" PRIu64 "B\n", sizeof(s_hashtable_entry));
      printf("Hashtable entries per MB:  %" PRIu64 "\n", 1024*1024/sizeof(s_hashtable_entry));
    #endif
    printf("\n");

    printf("Search algorithm: ");
    #if defined(ALPHA_BETA)
      printf("Alpha-Beta Search\n");
    #elif defined(PVS)
      printf("Principal Variation Search\n");
    #else
      printf("None\n");
    #endif

    printf("Capture sorting: ");
    #if defined(CAPTURE_SORT_SEE)
      printf(" SEE\n");
    #elif defined(CAPTURE_SORT_MVVLVA)
      printf(" MVV-LVA\n");
    #else
      printf(" None\n");
    #endif

    printf("Quiet sorting: ");
    #if defined(QUIET_SORT_PST)
      printf("   PST\n");
    #elif defined(QUIET_SORT_HISTORY_HEURISTIC)
      printf("   History heuristic\n");
    #elif defined(QUIET_SORT_SEE)
      printf("   SEE\n");
    #else
      printf("   None\n");
    #endif
    printf("\n");

    #ifdef QUIESCENCE_SEARCH
      printf("Enabled  - Quiescence search\n");
    #else
      printf("Disabled - Quiescence search\n");
    #endif

    #ifdef HASHTABLE
      printf("Enabled  - Transposition table\n");
    #else
      printf("Disabled - Transposition table\n");
    #endif

    #ifdef NULL_MOVE
      printf("Enabled  - Null move pruning\n");
    #else
      printf("Disabled - Null move pruning\n");
    #endif

    #ifdef KILLER_MOVES
      printf("Enabled  - Killer move heuristic\n");
    #else
      printf("Disabled - Killer move heuristic\n");
    #endif

    #ifdef SORT_MOVES
      printf("Enabled  - Move sorting\n");
    #else
      printf("Disabled - Move sorting\n");
    #endif

    #ifdef ASPIRATION_WINDOW
      printf("Enabled  - Aspiration windows\n");
    #else
      printf("Disabled - Aspiration windows\n");
    #endif

    #ifdef LMR
      printf("Enabled  - Late Move Reductions\n");
    #else
      printf("Disabled - Late Move Reductions\n");
    #endif

    #ifdef DELTA_PRUNING
      printf("Enabled  - Delta Pruning\n");
    #else
      printf("Disabled - Delta Pruning\n");
    #endif

    #ifdef TAPERED_EVAL
      printf("Enabled  - Tapered eval\n");
    #else
      printf("Disabled - Tapered eval\n");
    #endif

    #ifdef PASSED_PAWN_EVAL
      printf("Enabled  - Passed pawn eval\n");
    #else
      printf("Disabled - Passed pawn eval\n");
    #endif

    #ifdef BACKWARD_PAWN_EVAL
      printf("Enabled  - Backward pawn eval\n");
    #else
      printf("Disabled - Backward pawn eval\n");
    #endif

    #ifdef KING_SAFETY
      printf("Enabled  - King safety eval\n");
    #else
      printf("Disabled - King safety eval\n");
    #endif

    #ifdef KILLER_MOVES_2
      printf("Enabled  - Second killer move\n");
    #else
      printf("Disabled - Second killer move\n");
    #endif

    #ifdef PIECE_MOBILITY
      printf("Enabled  - Piece mobility\n");
    #else
      printf("Disabled - Piece mobility\n");
    #endif

    printf("\n");
    getchar();
  }
  else
  {
    GUI_Send("Unknown protocol\n");
  }

  return 0;
}
