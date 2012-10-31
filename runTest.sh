rm -f SSSP.app graph.out
g++ -lpthread -Wall SSSP.c++ -o SSSP.app

NUM_TEST=1

for ((test=1; test<=NUM_TEST ; test+=1)); do
	./SSSP.app < ./tests/$test.in 2>&1 | tee -a graph.out 
	#>graph.out 
	
	 
	if diff graph.out ./tests/$test.out >/dev/null ; then
	  echo "PASS $test"
	else
	  echo "FAIL $test"
	fi
	
	rm graph.out
done