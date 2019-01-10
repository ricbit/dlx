SHELL := /bin/bash

all : cover_refactor cover_nospacer cover_original cover_mrv

cover_% : cover_%.cc
	g++ -std=c++17 -Wall -O3 -march=native -malign-data=cacheline -g $^ -o $@

perf.%.txt : cover_%
	echo "python genqueens.py 15 | ./$^" > script.sh
	chmod u+x script.sh
	perf stat -d --repeat 10 ./script.sh &> $@
	rm script.sh

queen15.%.txt : cover_%
	python genqueens.py 15 | valgrind --tool=cachegrind --branch-sim=yes --cachegrind-out-file=cachegrind.out ./$^
	cg_annotate --auto=yes cachegrind.out > $@
	rm cachegrind.out
