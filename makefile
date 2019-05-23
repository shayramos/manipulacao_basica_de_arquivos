# Nota: 'c++11' refere-se a C++ 11
all: projeto

projeto: projeto.cpp estrutura.cpp estrutura.h
	g++ -std=c++11 -o projeto projeto.cpp estrutura.cpp  
