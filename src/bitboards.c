#include "defs.h"

uint64_t magicNumberRook[] = {
  0xa180022080400230L, 0x40100040022000L, 0x80088020001002L, 0x80080280841000L, 0x4200042010460008L, 0x4800a0003040080L, 0x400110082041008L, 0x8000a041000880L, 0x10138001a080c010L, 0x804008200480L, 0x10011012000c0L, 0x22004128102200L, 0x200081201200cL, 0x202a001048460004L, 0x81000100420004L, 0x4000800380004500L, 0x208002904001L, 0x90004040026008L, 0x208808010002001L, 0x2002020020704940L, 0x8048010008110005L, 0x6820808004002200L, 0xa80040008023011L, 0xb1460000811044L, 0x4204400080008ea0L, 0xb002400180200184L, 0x2020200080100380L, 0x10080080100080L, 0x2204080080800400L, 0xa40080360080L, 0x2040604002810b1L, 0x8c218600004104L, 0x8180004000402000L, 0x488c402000401001L, 0x4018a00080801004L, 0x1230002105001008L, 0x8904800800800400L, 0x42000c42003810L, 0x8408110400b012L, 0x18086182000401L, 0x2240088020c28000L, 0x1001201040c004L, 0xa02008010420020L, 0x10003009010060L, 0x4008008008014L, 0x80020004008080L, 0x282020001008080L, 0x50000181204a0004L, 0x102042111804200L, 0x40002010004001c0L, 0x19220045508200L, 0x20030010060a900L, 0x8018028040080L, 0x88240002008080L, 0x10301802830400L, 0x332a4081140200L, 0x8080010a601241L, 0x1008010400021L, 0x4082001007241L, 0x211009001200509L, 0x8015001002441801L, 0x801000804000603L, 0xc0900220024a401L, 0x1000200608243L
};

uint64_t magicNumberBishop[] = {
  0x2910054208004104L, 0x2100630a7020180L, 0x5822022042000000L, 0x2ca804a100200020L, 0x204042200000900L, 0x2002121024000002L, 0x80404104202000e8L, 0x812a020205010840L, 0x8005181184080048L, 0x1001c20208010101L, 0x1001080204002100L, 0x1810080489021800L, 0x62040420010a00L, 0x5028043004300020L, 0xc0080a4402605002L, 0x8a00a0104220200L, 0x940000410821212L, 0x1808024a280210L, 0x40c0422080a0598L, 0x4228020082004050L, 0x200800400e00100L, 0x20b001230021040L, 0x90a0201900c00L, 0x4940120a0a0108L, 0x20208050a42180L, 0x1004804b280200L, 0x2048020024040010L, 0x102c04004010200L, 0x20408204c002010L, 0x2411100020080c1L, 0x102a008084042100L, 0x941030000a09846L, 0x244100800400200L, 0x4000901010080696L, 0x280404180020L, 0x800042008240100L, 0x220008400088020L, 0x4020182000904c9L, 0x23010400020600L, 0x41040020110302L, 0x412101004020818L, 0x8022080a09404208L, 0x1401210240484800L, 0x22244208010080L, 0x1105040104000210L, 0x2040088800c40081L, 0x8184810252000400L, 0x4004610041002200L, 0x40201a444400810L, 0x4611010802020008L, 0x80000b0401040402L, 0x20004821880a00L, 0x8200002022440100L, 0x9431801010068L, 0x1040c20806108040L, 0x804901403022a40L, 0x2400202602104000L, 0x208520209440204L, 0x40c000022013020L, 0x2000104000420600L, 0x400000260142410L, 0x800633408100500L, 0x2404080a1410L, 0x138200122002900L    
};

uint64_t occupancyMaskRook[] = {
  0x101010101017eL, 0x202020202027cL, 0x404040404047aL, 0x8080808080876L, 0x1010101010106eL, 0x2020202020205eL, 0x4040404040403eL, 0x8080808080807eL, 0x1010101017e00L, 0x2020202027c00L, 0x4040404047a00L, 0x8080808087600L, 0x10101010106e00L, 0x20202020205e00L, 0x40404040403e00L, 0x80808080807e00L, 0x10101017e0100L, 0x20202027c0200L, 0x40404047a0400L, 0x8080808760800L, 0x101010106e1000L, 0x202020205e2000L, 0x404040403e4000L, 0x808080807e8000L, 0x101017e010100L, 0x202027c020200L, 0x404047a040400L, 0x8080876080800L, 0x1010106e101000L, 0x2020205e202000L, 0x4040403e404000L, 0x8080807e808000L, 0x1017e01010100L, 0x2027c02020200L, 0x4047a04040400L, 0x8087608080800L, 0x10106e10101000L, 0x20205e20202000L, 0x40403e40404000L, 0x80807e80808000L, 0x17e0101010100L, 0x27c0202020200L, 0x47a0404040400L, 0x8760808080800L, 0x106e1010101000L, 0x205e2020202000L, 0x403e4040404000L, 0x807e8080808000L, 0x7e010101010100L, 0x7c020202020200L, 0x7a040404040400L, 0x76080808080800L, 0x6e101010101000L, 0x5e202020202000L, 0x3e404040404000L, 0x7e808080808000L, 0x7e01010101010100L, 0x7c02020202020200L, 0x7a04040404040400L, 0x7608080808080800L, 0x6e10101010101000L, 0x5e20202020202000L, 0x3e40404040404000L, 0x7e80808080808000L 
};

uint64_t occupancyMaskBishop[] = {
  0x40201008040200L, 0x402010080400L, 0x4020100a00L, 0x40221400L, 0x2442800L, 0x204085000L, 0x20408102000L, 0x2040810204000L, 0x20100804020000L, 0x40201008040000L, 0x4020100a0000L, 0x4022140000L, 0x244280000L, 0x20408500000L, 0x2040810200000L, 0x4081020400000L, 0x10080402000200L, 0x20100804000400L, 0x4020100a000a00L, 0x402214001400L, 0x24428002800L, 0x2040850005000L, 0x4081020002000L, 0x8102040004000L, 0x8040200020400L, 0x10080400040800L, 0x20100a000a1000L, 0x40221400142200L, 0x2442800284400L, 0x4085000500800L, 0x8102000201000L, 0x10204000402000L, 0x4020002040800L, 0x8040004081000L, 0x100a000a102000L, 0x22140014224000L, 0x44280028440200L, 0x8500050080400L, 0x10200020100800L, 0x20400040201000L, 0x2000204081000L, 0x4000408102000L, 0xa000a10204000L, 0x14001422400000L, 0x28002844020000L, 0x50005008040200L, 0x20002010080400L, 0x40004020100800L, 0x20408102000L, 0x40810204000L, 0xa1020400000L, 0x142240000000L, 0x284402000000L, 0x500804020000L, 0x201008040200L, 0x402010080400L, 0x2040810204000L, 0x4081020400000L, 0xa102040000000L, 0x14224000000000L, 0x28440200000000L, 0x50080402000000L, 0x20100804020000L, 0x40201008040200L     
};

const int magicNumberShiftsRook[] = {
  52,53,53,53,53,53,53,52,53,54,54,54,54,54,54,53,
  53,54,54,54,54,54,54,53,53,54,54,54,54,54,54,53,
  53,54,54,54,54,54,54,53,53,54,54,54,54,54,54,53,
  53,54,54,54,54,54,54,53,52,53,53,53,53,53,53,52
};

const int magicNumberShiftsBishop[] = {
  58,59,59,59,59,59,59,58,59,59,59,59,59,59,59,59,
  59,59,57,57,57,57,59,59,59,59,57,55,55,57,59,59,
  59,59,57,55,55,57,59,59,59,59,57,57,57,57,59,59,
  59,59,59,59,59,59,59,59,58,59,59,59,59,59,59,58
};

uint64_t magicMovesRook[64][4096] = {{0}}; // Tested: 4095
uint64_t magicMovesBishop[64][512] = {{0}}; // Tested: 511
uint64_t magicMovesKnight[64];
uint64_t magicMovesKing[64];

uint64_t occupancyVariation[64][4096] = {{0}}; // Tested: 4095
uint64_t rookOccupancyAttackSet[64][4096] = {{0}}; // Tested: 4095
uint64_t bishopOccupancyAttackSet[64][512] = {{0}}; // Tested: 511

const int index64[64] = {
    0, 47,  1, 56, 48, 27,  2, 60,
   57, 49, 41, 37, 28, 16,  3, 61,
   54, 58, 35, 52, 50, 42, 21, 44,
   38, 32, 29, 23, 17, 11,  4, 62,
   46, 55, 26, 59, 40, 36, 15, 53,
   34, 51, 20, 43, 31, 22, 10, 45,
   25, 39, 14, 33, 19, 30,  9, 24,
   13, 18,  8, 12,  7,  6,  5, 63
};

void generateKnightMoves()
{
  int sq;
  for(sq = 0; sq < 64; ++sq)
  {
    uint64_t from = (uint64_t)1<<sq;
    magicMovesKnight[sq]  = (from<<17) & (~U64_COL_H); // Up 2 left 1
    magicMovesKnight[sq] |= (from<<15) & (~U64_COL_A); // Up 2 right 1
    magicMovesKnight[sq] |= (from>>17) & (~U64_COL_A); // Down 2 right 1
    magicMovesKnight[sq] |= (from>>15) & (~U64_COL_H); // down 2 left 1
    magicMovesKnight[sq] |= (from<<10) & ~(U64_COL_G|U64_COL_H); // Left 2 up 1
    magicMovesKnight[sq] |= (from>>6)  & ~(U64_COL_G|U64_COL_H); // Left 2 down 1
    magicMovesKnight[sq] |= (from<<6)  & ~(U64_COL_A|U64_COL_B); // Right 2 up 1
    magicMovesKnight[sq] |= (from>>10) & ~(U64_COL_A|U64_COL_B); // Right 2 down 1
  }
}

void generateKingMoves()
{
  int sq;
  for(sq = 0; sq < 64; ++sq)
  {
    uint64_t from = (uint64_t)1<<sq;
    magicMovesKing[sq]  = (from<<8); // Up 1
    magicMovesKing[sq] |= (from>>8); // Down 1
    magicMovesKing[sq] |= (from<<1) & (~U64_COL_H); // Left 1
    magicMovesKing[sq] |= (from>>1) & (~U64_COL_A); // Right 1
    magicMovesKing[sq] |= (from<<7) & (~U64_COL_A); // Up 1 Right 1
    magicMovesKing[sq] |= (from<<9) & (~U64_COL_H); // Up 1 Left 1
    magicMovesKing[sq] |= (from>>7) & (~U64_COL_H); // Down 1 Left 1
    magicMovesKing[sq] |= (from>>9) & (~U64_COL_A); // Down 1 Right 1
  }
}

void generateOccupancyVariations(int isRook)
{
  int i, j, bitRef;
  uint64_t mask;
  int variationCount;
  int setBitsInMask[64] = {0};
  int setBitsInIndex[64] = {0};
  int bitCount[64] = {0};
  int c;
  
  for(bitRef = 0; bitRef <= 63; ++bitRef)
  {
    mask = isRook ? occupancyMaskRook[bitRef] : occupancyMaskBishop[bitRef];
    
    int yyy = 0;
    for(c = 0; c < 64; ++c)
    {
      if(mask>>c & 1)
      {
        setBitsInMask[yyy] = c;
        yyy++;
      }
    }
    setBitsInMask[yyy] = -1;
    
    bitCount[bitRef] = 0;
    for(c = 0; c < 64; ++c)
    {
      if(mask>>c & 1)
      {
        bitCount[bitRef]++;
      }
    }
    
    variationCount = (1 << bitCount[bitRef]);
    for(i = 0; i < variationCount; ++i)
    {
      occupancyVariation[bitRef][i] = 0;
      
      int zzz = 0;
      for(c = 0; c < 32; ++c)
      {
        if(i>>c & 1)
        {
          setBitsInIndex[zzz] = c;
          zzz++;
        }
      }
      setBitsInIndex[zzz] = -1;
      
      for(j = 0; setBitsInIndex[j] != -1; ++j)
      {
        occupancyVariation[bitRef][i] |= ((uint64_t)1 << setBitsInMask[setBitsInIndex[j]]);
      }
      
      if(isRook)
      {
        for(j=bitRef+8; j<=55 && (occupancyVariation[bitRef][i] & ((uint64_t)1 << j)) == 0; j+=8)
          {if(j >= 0 && j <= 63) {rookOccupancyAttackSet[bitRef][i] |= ((uint64_t)1 << j);}}
        
        for(j=bitRef-8; j>=8 && (occupancyVariation[bitRef][i] & ((uint64_t)1 << j)) == 0; j-=8)
          {if(j>=0 && j<=63) rookOccupancyAttackSet[bitRef][i] |= ((uint64_t)1 << j);}
        
        for(j=bitRef+1; j%8!=7 && j%8!=0 && (occupancyVariation[bitRef][i] & ((uint64_t)1 << j)) == 0; ++j)
          {if(j>=0 && j<=63) rookOccupancyAttackSet[bitRef][i] |= ((uint64_t)1 << j);}
        
        for(j=bitRef-1; j%8!=7 && j%8!=0 && j>=0 && (occupancyVariation[bitRef][i] & ((uint64_t)1 << j)) == 0; --j)
          {if(j>=0 && j<=63) rookOccupancyAttackSet[bitRef][i] |= ((uint64_t)1 << j);}
      }
      else
      {        
        for(j=bitRef+9; j%8!=7 && j%8!=0 && j<=55 && (occupancyVariation[bitRef][i] & ((uint64_t)1 << j)) == 0; j+=9)
          {if(j>=0 && j<=63) bishopOccupancyAttackSet[bitRef][i] |= ((uint64_t)1 << j);}
          
        for(j=bitRef-9; j%8!=7 && j%8!=0 && j>=8 && (occupancyVariation[bitRef][i] & ((uint64_t)1 << j)) == 0; j-=9)
          {if(j>=0 && j<=63) bishopOccupancyAttackSet[bitRef][i] |= ((uint64_t)1 << j);}
          
        for(j=bitRef+7; j%8!=7 && j%8!=0 && j<=55 && (occupancyVariation[bitRef][i] & ((uint64_t)1 << j)) == 0; j+=7)
          {if(j>=0 && j<=63) bishopOccupancyAttackSet[bitRef][i] |= ((uint64_t)1 << j);}
          
        for(j=bitRef-7; j%8!=7 && j%8!=0 && j>=8 && (occupancyVariation[bitRef][i] & ((uint64_t)1 << j)) == 0; j-=7)
          {if(j>=0 && j<=63) bishopOccupancyAttackSet[bitRef][i] |= ((uint64_t)1 << j);}
      }
    }
  }
}

void generateMoveDatabase(int isRook)
{
  uint64_t validMoves;
  int variations, bitCount;
  int bitRef, i, j, magicIndex;
  
  for(bitRef = 0; bitRef <= 63; ++bitRef)
  {
    bitCount = 0;
    if(isRook)
    {
      int counter = 0;
      while(counter < 64)
      {
        if(occupancyMaskRook[bitRef]>>counter & 1) {bitCount++;}
        counter++;
      }
    }
    else
    {
      int counter = 0;
      while(counter < 64)
      {
        if(occupancyMaskBishop[bitRef]>>counter & 1) {bitCount++;}
        counter++;
      }
    }
    variations = (1 << bitCount);
    
    for(i = 0; i < variations; ++i)
    {
      validMoves = 0;
      if(isRook)
      {
        magicIndex = (occupancyVariation[bitRef][i] * magicNumberRook[bitRef]) >> magicNumberShiftsRook[bitRef];

        for(j=bitRef+8; j<=63; j+=8)         {validMoves |= ((uint64_t)1 << j); if((occupancyVariation[bitRef][i] & ((uint64_t)1 << j)) != 0) break;}
        for(j=bitRef-8; j>=0; j-=8)          {validMoves |= ((uint64_t)1 << j); if((occupancyVariation[bitRef][i] & ((uint64_t)1 << j)) != 0) break;}
        for(j=bitRef+1; j%8!=0; j++)         {validMoves |= ((uint64_t)1 << j); if((occupancyVariation[bitRef][i] & ((uint64_t)1 << j)) != 0) break;}
        for(j=bitRef-1; j%8!=7 && j>=0; j--) {validMoves |= ((uint64_t)1 << j); if((occupancyVariation[bitRef][i] & ((uint64_t)1 << j)) != 0) break;}
        
        magicMovesRook[bitRef][magicIndex] = validMoves;
      }
      else
      {
        magicIndex = (occupancyVariation[bitRef][i] * magicNumberBishop[bitRef]) >> magicNumberShiftsBishop[bitRef];

        for(j=bitRef+9; j%8!=0 && j<=63; j+=9) {validMoves |= ((uint64_t)1 << j); if((occupancyVariation[bitRef][i] & ((uint64_t)1 << j)) != 0) break;}
        for(j=bitRef-9; j%8!=7 && j>=0; j-=9)  {validMoves |= ((uint64_t)1 << j); if((occupancyVariation[bitRef][i] & ((uint64_t)1 << j)) != 0) break;}
        for(j=bitRef+7; j%8!=7 && j<=63; j+=7) {validMoves |= ((uint64_t)1 << j); if((occupancyVariation[bitRef][i] & ((uint64_t)1 << j)) != 0) break;}
        for(j=bitRef-7; j%8!=0 && j>=0; j-=7)  {validMoves |= ((uint64_t)1 << j); if((occupancyVariation[bitRef][i] & ((uint64_t)1 << j)) != 0) break;}

        magicMovesBishop[bitRef][magicIndex] = validMoves;
      }
    }
  }
}

uint64_t magic_moves_hor_ver(uint64_t pieces_all, int pos)
{
  assert(0 <= pos && pos <= 63);
  
  uint64_t blockers = pieces_all & occupancyMaskRook[pos];
  int index = (blockers * magicNumberRook[pos]) >> magicNumberShiftsRook[pos];
  return magicMovesRook[pos][index];
}

uint64_t magic_moves_diagonal(uint64_t pieces_all, int pos)
{
  assert(0 <= pos && pos <= 63);
  
  uint64_t blockers = pieces_all & occupancyMaskBishop[pos];
  int index = (blockers * magicNumberBishop[pos]) >> magicNumberShiftsBishop[pos];
  return magicMovesBishop[pos][index];
}

uint64_t magic_moves_knight(int pos)
{
  assert(0 <= pos && pos <= 63);
  
  return magicMovesKnight[pos];
}

uint64_t magic_moves_king(int pos)
{
  assert(0 <= pos && pos <= 63);
  
  return magicMovesKing[pos];
}

int u64_to_sq(uint64_t pos)
{
  assert(pos);
  
  return index64[((pos ^ (pos-1)) * 0x03f79d71b4cb0a89) >> 58];
}

int u64_col(uint64_t pos)
{
  assert(pos);
  
  if(pos&U64_COL_A) {return 0;}
  if(pos&U64_COL_B) {return 1;}
  if(pos&U64_COL_C) {return 2;}
  if(pos&U64_COL_D) {return 3;}
  if(pos&U64_COL_E) {return 4;}
  if(pos&U64_COL_F) {return 5;}
  if(pos&U64_COL_G) {return 6;}
  return 7;
}

int u64_row(uint64_t pos)
{
  assert(pos);
  
  if(pos&U64_ROW_1) {return 0;}
  if(pos&U64_ROW_2) {return 1;}
  if(pos&U64_ROW_3) {return 2;}
  if(pos&U64_ROW_4) {return 3;}
  if(pos&U64_ROW_5) {return 4;}
  if(pos&U64_ROW_6) {return 5;}
  if(pos&U64_ROW_7) {return 6;}
  return 7;
}

void bitboards_init()
{
  #ifndef NDEBUG
    printf("Bitboard debug info:\n");
    printf("Rook table: %"PRIdPTR"KB\n", sizeof(magicMovesRook)/1024);
    printf("Bishop table: %"PRIdPTR"KB\n", sizeof(magicMovesBishop)/1024);
    printf("\n");
  #endif
  
  generateOccupancyVariations(0);
  generateMoveDatabase(0);
  generateOccupancyVariations(1);
  generateMoveDatabase(1);
  generateKnightMoves();
  generateKingMoves();
}

uint64_t pinned_pieces_white(s_board* board, int sq)
{
  assert(board != NULL);
  assert(0 <= sq && sq <= 63);
  
  uint64_t pinned = 0;
  uint64_t pos;
  uint64_t blockers;
  
  // Bishops and Queens
  blockers = magic_moves_diagonal((board->colour[WHITE]|board->colour[BLACK]), sq) & board->colour[WHITE];
  
  while(blockers)
  {
    pos = blockers & ~(blockers-1);
    
    if(magic_moves_diagonal((board->colour[WHITE]|board->colour[BLACK]) ^ pos, sq) & (board->colour[BLACK] & (board->combined[BISHOPS]|board->combined[QUEENS])))
    {
      pinned ^= pos;
    }
    
    blockers = blockers ^ pos;
  }
  
  // Rooks and Queens
  blockers = magic_moves_hor_ver((board->colour[WHITE]|board->colour[BLACK]), sq) & board->colour[WHITE];
  
  while(blockers)
  {
    pos = blockers & ~(blockers-1);
    
    if(magic_moves_hor_ver((board->colour[WHITE]|board->colour[BLACK]) ^ pos, sq) & (board->colour[BLACK] & (board->combined[ROOKS]|board->combined[QUEENS])))
    {
      pinned ^= pos;
    }
    
    blockers = blockers ^ pos;
  }
  
  return pinned;
}

uint64_t pinned_pieces_black(s_board* board, int sq)
{
  assert(board != NULL);
  assert(0 <= sq && sq <= 63);
  
  uint64_t pinned = 0;
  uint64_t pos;
  uint64_t blockers;
  
  // Bishops and Queens
  blockers = magic_moves_diagonal((board->colour[WHITE]|board->colour[BLACK]), sq) & board->colour[BLACK];
  
  while(blockers)
  {
    pos = blockers & ~(blockers-1);
    
    if(magic_moves_diagonal((board->colour[WHITE]|board->colour[BLACK]) ^ pos, sq) & (board->colour[WHITE] & (board->combined[BISHOPS]|board->combined[QUEENS])))
    {
      pinned ^= pos;
    }
    
    blockers = blockers ^ pos;
  }
  
  // Rooks and Queens
  blockers = magic_moves_hor_ver((board->colour[WHITE]|board->colour[BLACK]), sq) & board->colour[BLACK];
  
  while(blockers)
  {
    pos = blockers & ~(blockers-1);
    
    if(magic_moves_hor_ver((board->colour[WHITE]|board->colour[BLACK]) ^ pos, sq) & (board->colour[WHITE] & (board->combined[ROOKS]|board->combined[QUEENS])))
    {
      pinned ^= pos;
    }
    
    blockers = blockers ^ pos;
  }
  
  return pinned;
}
