all: projeto

projeto: registro.hpp registro.cpp heap.hpp heap.cpp main.cpp runs.hpp runs.cpp
	g++ -std=c++11 registro.hpp registro.cpp heap.hpp heap.cpp main.cpp runs.hpp runs.cpp -o projeto

base:
	python3 archiveGenerator.py

clean:
	rm -f ./projeto