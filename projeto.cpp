//============================================================================
// Name        : projeto.cpp
// Author      : Aline / Fernando / Hugo
// Version     : 1.0
// Copyright   : GPL
// Description : Gerência de arquivo de registros com Hashing Duplo
//============================================================================
#include "estrutura.h"


///// Cálculo do Hashing/ReHashing usando hashing duplo ///////////////////

 unsigned int hashingDuplo(unsigned int key, int i, int b){ //Protótipo de função (útil mas custosa em operações);
			unsigned int h1, h2;

            h1 = key % b;
            h2 = ((int) key/b) % b;
            if(h2 == 0){
                h2 = 1;
            }

            return (h1 + (i*h2)) % b; //O fato de realizar uma operação de "%" aqui serve para circular 
}                                  // a lista de forma que não seja necessário usar um IF para circular a mesma;

//Exibe o status do registro em formato de texto
string statusText(int status){
	switch(status){
		case(VAZIO):
			return "Vazio";
		case(OCUPADO):
			return "Ocupado";
		case(DELETADO):
			return "Deletado";
		default:
			return " ";
	}
}

//Insere registro em arquivo

int inserirRegistro(fstream &arquivo, Registro* reg){
	// ->Mover o carrier pro inicio
	arquivo.seekg(0);

	unsigned int pos_busca; //Armazena o retorno da função hash(k,i,b);
	Registro* buffer = new Registro(0,0,VAZIO, ""); //buffer para leitura dos registros;
	
	//Varre de 0 a TAMANHO as posições;
	for(int i = 0; i < TAMANHO; i++){
			
			pos_busca = hashingDuplo(reg->chave, i, TAMANHO);
			//Se a posicao atual for igual a inicial e i > 0, indica que já circulou toda a estrutura e não há espaço disponível
			//if((pos_busca == pos_ini) && (i > 0)){
				//cout << "tabela cheia." << endl;
			//	return 0;
			//}

			////Caso haja espaço ou não percorreu a tabela toda;
			arquivo.seekg(HEADER_OFFSET + (pos_busca * sizeof(Registro))); //Desloca-se para a posição do registro resultando do hash(k,i,b);
			
			arquivo.read((char*)buffer,sizeof(Registro));  //Realiza a leitura na posição
			arquivo.sync();


			if(buffer->status == OCUPADO){
				if(buffer->chave == reg->chave){
					cout << "registro com chave existente" << endl;
					return 1;
				}else{ //Caso o registro só esteja ocupado e não tenha chave igual, continua FOR
					continue;
				}


			}else{ //se status = LIVRE ou DELETADO então pode gravar o registro;
				arquivo.seekp(HEADER_OFFSET + pos_busca*sizeof(Registro));
				reg->status = OCUPADO;  //Atualiza status (ele não é atualizado antes por causa da possibilidade de tabela cheia e já-existência de chave);
				arquivo.write((char*)reg, sizeof(Registro)); 
				arquivo.flush();
				return 2;		
			}		
	}
    return 0; //Caso a tabela esteja cheia
}

//Remove registro
int removerRegistro(fstream &arquivo, unsigned int key){
		Registro *buffer = new Registro(0,0,VAZIO, "");

		for(int i = 0; i < TAMANHO; i++){
				//pos_busca = hashing(key,i,TAMANHO);
				arquivo.seekg(HEADER_OFFSET + hashingDuplo(key,i,TAMANHO)*sizeof(Registro)); //Busca posição
				
				arquivo.read((char*)buffer, sizeof(Registro)); //Lê registro na posição 
				arquivo.sync(); //Sincroniza stream com buffer;

				//Caso encontre a chave, apaga o registro;
				if( (buffer->chave == key) && (buffer->status == OCUPADO)){
					//A string abaixo foi a maneira encontrada para apagar a string anterior (problemas com header <string>);
					char string_clean[20] = "\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00"; 
					
					arquivo.seekp(-sizeof(Registro),ios_base::cur);  //Recua em 1 posição (o .read anterior avançou 1 posição além)
					buffer->chave = 0;
					buffer->idade = 0;
					
					strncpy(buffer->nome, string_clean,20);
					buffer->status = DELETADO;
					arquivo.write((char*)buffer, sizeof(Registro));
					arquivo.flush();
					return 1; //1 indica que apagou com sucesso;
				}
		}
	return 0; //0 indica que não encontrou a chave;
}

//Consulta registro
Registro* consultarRegistro(fstream &arquivo, unsigned int key){
		Registro *buffer;

		for(int i = 0; i < TAMANHO; i++){
				arquivo.seekg(HEADER_OFFSET + hashingDuplo(key,i,TAMANHO)*sizeof(Registro));
				arquivo.read((char*)buffer, sizeof(Registro));
				arquivo.sync();
				if( (buffer->chave == key) && (buffer->status == OCUPADO)){ 
					return buffer;
				}
		}

		return NULL; //Retorna nulo caso não exista registro com esta chave;
}

//Exibe todos os registros
void exibirRegistros(fstream &arquivo){
	Registro* buffer = new Registro(0,0,VAZIO," ");


	int tamanho; 
	arquivo.seekg(0,ios_base::end); //'tamanho' armazena a posição final do arquivo;
    tamanho = arquivo.tellg();
	arquivo.seekg(0, ios_base::beg);


	do{
		arquivo.read((char*)buffer,sizeof(Registro));
		cout << "//////////////////////////////////////////" << endl;
		cout << "chave: " << buffer->chave << endl;
		cout << "nome: " << buffer->nome << endl;
		cout << "idade: " << buffer->idade << endl;
		cout << "status: " << statusText(buffer->status) << endl;
	}while(arquivo.tellg()!= tamanho);

}



///////////////// Função principal
int main(){
	
	fstream arquivo; //Objeto para leitura/escrita;

	arquivo.open("database.txt", ios_base::in | ios_base::out | ios_base::binary ); //Abre o arquivo se já existir ou cria um novo caso contrário
	if(!arquivo.is_open()){
	//	cout << "Arquivo inexistente/com problema. Criando novo" << endl;
		//Cria-se um arquivo novo, fecha o mesmo e reabre com as flags ideais (in, out e binary);
		//Esse processo todo foi feito por causa de problemas com "append"
		arquivo.open("database.txt", ios_base::out);
		arquivo.close();
		arquivo.open("database.txt", ios_base::in | ios_base::out | ios_base::binary);
	}
	

/*
 Neste trecho será verificado se o arquivo está vazio ou já é um banco de dados existente -> verificar a possibilidade de usar flags
 * na abertura para realizar essa tarefa
 * 
 * Se o arquivo tiver tamanho 0, considera-se como novo e será necessário armazenar as estruturas (blocos de registros vazios)
*/
	int tamanho = 0;
	arquivo.seekg(0,ios_base::end); //Avança apontador até o último byte para que seja possível "medir" o arquivo;
	tamanho = arquivo.tellg();
	arquivo.seekg(0,ios_base::beg); //Retorna apontador para o início do arquivo;
	
	/* 
	*	Trecho de inicialização
	*
	*
	*
	*/
	
	// "regist" é um registro-auxiliar (servirá de buffer de escrita e remoção de registros);
	Registro* regist = new Registro(0, 0, VAZIO, ""); 
	

	//Se o arquivo for vazio (criado agora), insere as posições de registros-vazios;
	if(tamanho == 0){
		for(int i = 0 ; i < TAMANHO; i++){
			arquivo.write((char*)regist, sizeof(Registro)); //Escreve 'TAMANHO' vezes no arquivo vazio (cada posição armazena um único registro neste caso de linear probing);
		}
	}
	
/*
*
*
*
		Menu do sistema (não tem exibição de texto)
*/

	unsigned int buffer_chave; //Armazena os valores de chave provisoriamente (usado em consulta);
	char opcao = 'm'; //Armazena opcao do menu ('m' é um valor aleatório de inicializacao);

	while(opcao != 'e'){  //Laço de repetição do menu
		
    // Lê uma opcao de funcionalidade;
		
		cin >> opcao;

		switch(opcao){
			//////////////////////////////////////////////////////////////////////////////////////
				case 'i': {
							//cout << "  Opcao i inserir." << endl;
							cin >> regist->chave ;
							cin.ignore();  //Se não houver esse .ignore() e o seguinte ocorrerá bug e entrada fica em loop;
							cin.getline(regist->nome,20);
							cin >> regist->idade;
							        //tenta inserir novo registro na tabela
							inserirRegistro(arquivo, regist);
							break;	
						  }
			//////////////////////////////////////////////////////////////////////////////////////			  
				case 'r':{
							//cout << "  Opcao r remover." << endl;
							cin >> regist->chave;
							if(! removerRegistro(arquivo,regist->chave)){
								cout << "nao existe registro com chave: " << regist->chave << endl;
							}
							break;	
						}
			//////////////////////////////////////////////////////////////////////////////////////			
				case 'c':{
							//cout << "  Opcao c consultar. \n";
							cin >> regist->chave;
							buffer_chave = regist->chave;					
							regist = consultarRegistro(arquivo, regist->chave);
							if(regist!= NULL){
								cout << "chave: " << regist->chave << " " << regist->nome << " " << regist->idade << endl;
							}else{
								cout << "nao ha registro com chave: " << buffer_chave << endl; 
							}	
							break;	
						}
			//////////////////////////////////////////////////////////////////////////////////////			
				case 'p':{  //Opcao de print dos registros;
							exibirRegistros(arquivo);
							cout << "//////////////////////////////////////////" << endl;
							break;	
						}
						
				case 'e':{ //Opcao de fim de programa;
							break;	
						}
			
			///////////////////////////////////////////
			default:{ //Qualquer outra opcao serah considerada como invalida;
						break;
					}
		 }	
	 }

	// Fecha o arquivo;
	arquivo.close();
	return 0;
}

