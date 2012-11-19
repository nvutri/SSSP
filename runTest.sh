rm -f SSSP.app
g++ -ansi -O3  -pedantic -lpthread -Wall SSSP.c++ -o SSSP.app

FOLDER=./tests/SIZE_18
NUM_TEST=10
test=10
echo "TEST SUITE: $FOLDER $1"

res1=$(date +%s.%N)

./SSSP.app DT 12 $1 < $FOLDER/$test.in

res2=$(date +%s.%N)
echo "delta: 1/$dt RUN $test --- $(echo "$res2 - $res1"|bc ) sec"
rm -f $OUTPUT
