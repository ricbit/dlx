SHELL := /bin/bash

all : cover_refactor cover_nospacer cover_original cover_mrv cover_shortcut

cover_% : cover_%.cc
	g++ -std=c++17 -Wall -O3 -march=native -g $^ -o $@

perf.queens.%.txt : cover_%
	echo "python genqueens.py 9 | ./$^" > script.sh
	chmod u+x script.sh
	perf stat -d --repeat 3 ./script.sh &> $@
	rm script.sh

perf.random.%.txt : cover_%
	echo "./$^ < random.dlx" > script.sh
	chmod u+x script.sh
	perf stat -d --repeat 3 ./script.sh &> $@
	rm script.sh

cachegrind.%.txt : cover_%
	python genqueens.py 8 | valgrind --tool=cachegrind --branch-sim=yes --cachegrind-out-file=cachegrind.$^.out ./$^
	cg_annotate --auto=yes cachegrind.$^.out > $@
	rm cachegrind.$^.out
