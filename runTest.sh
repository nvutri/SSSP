rm -f SSSP.app graph.out
g++ -Wall SSSP.c++ -o SSSP.app
NUM_TEST=1
for ((test=1; test<=NUM_TEST ; test+=1)); do
	./SSSP.app < ./tests/$test.in > graph.out 
	if diff graph.out ./tests/1.out >/dev/null ; then
	  echo "PASS $test"
	else
	  echo "FAIL $test"
	fi
done