all: projeto
projeto: projeto.cpp estrutura.cpp estrutura.h
	g++ -std=c++0x -o projeto projeto.cpp estrutura.cpp estrutura.h -ll
