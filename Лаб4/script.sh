g++ prog_1.cpp -Wall -o prog1
g++ prog_2.cpp -Wall -o prog2
g++ prog_3.cpp -Wall -o prog3
RED='\033[0;31m'
NC='\033[0m'
echo "\033[0;31m\t Fork example \033[0m"
./prog2 1 2 3
echo "\n\n\033[0;31m\t Clone example \033[0m"
./prog3 1 2 3 4
rm prog1 prog2 prog3