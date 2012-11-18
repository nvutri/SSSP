rm -f SSSP.app
g++ -ansi -O3  -pedantic -lpthread -Wall SSSP.c++ -o SSSP.app

FOLDER=./tests/SIZE_18
OUTPUT=./graph.out
NUM_TEST=10
echo "TEST SUITE: $FOLDER $1"

for ((test=0; test<=NUM_TEST ; test+=1)); do

	res1=$(date +%s.%N)
	
	#SSSP.app [Algorithm] [Number of Thread]
	
	./SSSP.app $1 12 < $FOLDER/$test.in > $OUTPUT
	
	res2=$(date +%s.%N)
	 
	if diff $OUTPUT $FOLDER/$test.out >/dev/null ; then
	  echo "PASS $test --- $(echo "$res2 - $res1"|bc ) sec"
	else
	  echo "FAIL $test --- $(echo "$res2 - $res1"|bc ) sec"
	fi
	rm -f $OUTPUT
done