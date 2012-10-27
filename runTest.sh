g++ -Wall SSSP.c++ -o SSSP.app
./SSSP.app < ./tests/1.in > graph.out
diff graph.out ./tests/1.out