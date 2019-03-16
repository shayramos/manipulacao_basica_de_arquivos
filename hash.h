#ifndef __HASH__
#define __HASH__ 1

const std::string pathIn = "/data/fileIn.txt";
const std::string pathOut = "/data/fileOut.txt";

class Hash { 

    public:
        int tamanho;
        std::list<RegisterData> *lista; 
    
    public: 
        Hash(int V);
  
        void inserirRegistro(RegisterData x); 
    
        void deleteRegistro(RegisterData x); 
    
        int hashFunction(int x) { 
            return (x % tamanho); 
        }
    }; 

#endif  // __HASH__
