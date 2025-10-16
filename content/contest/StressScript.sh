set -e
g++ code.cpp -std=c++17 -o code
g++ brute.cpp -std=c++17 -o brute
g++ gen.cpp -std=c++17 -o gen
g++ checker.cpp -std=c++17 -o checker
for((i=1; ; ++i)); do
  ./gen $i > input.txt
  ./code < input.txt > output.txt
  ./brute < input.txt > answer.txt
  ./checker > checker_log
  echo "Passed test: " $i
done