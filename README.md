# Baislicka

## Description
A chess engine written in C. Uses bitboards with fixed shift fancy movegen.

## Strength
- CCRL Blitz rated ~2200 [here](https://www.computerchess.org.uk/ccrl/404/)
- CCRL 40/15 rated ~2200 [here](https://www.computerchess.org.uk/ccrl/4040/)

## Build instructions
``` bash
git clone https://github.com/kz04px/Baislicka
mkdir Baislicka/build
cd Baislicka/build
cmake ..
make
```

## Tests
Some limited tests are included and can be run by using the above build instructions and running the resulting "baislicka-test" executable in the build directory.

## Platforms
Both Windows and Linux are currently supported.

## Contributors
Thanks to all those that have contributed to Baislicka in some way, no matter how small.
- [JohnAnthony](https://github.com/JohnAnthony)
- [chunkyguy](https://github.com/chunkyguy)
- [En-En-Code](https://github.com/En-En-Code)
- [tissatussa](https://github.com/tissatussa)

## Special thanks
- Everyone that has used Baislicka in their own engine testing, in tournaments, to play against, or anywhere else.

- Camilla, for inspiring the name "Baislicka".

- ##chessprogramming on irc.libera.chat

- The chess programming wiki.
https://www.chessprogramming.org

- Bluefever Software's video series on creating the Vice engine for inspiration to do the same.
https://www.youtube.com/watch?v=bGAfaepBco4&list=PLZ1QII7yudbc-Ky058TEaOstZHVbT-2hg

- Chris Moreton’s programming blog post on plain magic bitboards.
http://www.afewmorelines.com/understanding-magic-bitboards-in-chess-programming/

- Roman Hartmann, the creator of perftsuite.epd and ROCE.
http://www.rocechess.ch/perft.html

- Tomasz Michniewski's simplified evaluation function
https://www.chessprogramming.org/Simplified_Evaluation_Function

- Volker Annuss' work on fixed shift magics
http://www.talkchess.com/forum3/viewtopic.php?topic_view=threads&p=670709&t=60065

- Adam Hair's piece square tables
http://www.talkchess.com/forum3/viewtopic.php?topic_view=threads&p=551989&t=50840
