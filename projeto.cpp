//============================================================================
// Name        : projeto.cpp
// Author      : Aline / Fernando / Hugo
// Version     : 1.0
// Copyright   : GPL
// Description : Hashing Arquivo
//============================================================================



#include "estrutura.h"


///// Cálculo do Hashing/ReHashing usando sondagem linear de uma função só ///////////////////
 
int hashing(int k, int i, int b){  //Hash com chave K , valor de iteração i (oriundo de um FOR) e divisor 'b';
		return (k+i)% b; //Retorna resto da divisão de (k+i) por b (valor de b é a quantia de posições (valor primo de preferência);
}

int inserirRegistro(fstream &arquivo, Registro* reg){
	// ->Mover o carrier pro inicio
	arquivo.seekg(0);

	int pos_ini;   //Armazena a posição "original" do hash (posição ideal da chave) 
	int pos_busca; //Armazena o retorno da função hash(k,i,b);
	Registro* buffer = new Registro(0,0,VAZIO, ""); //buffer para leitura dos registros;
	
	pos_ini =  hashing(reg->chave , 0 , TAMANHO); //Armazena a posição "ideal" de hash do registro para
												  //analisar se já percorreu a tabela toda e indicar tabela cheia;
	//Varre de 0 a TAMANHO as posições;
	for(int i = 0; i <= TAMANHO; i++){
			
			pos_busca = hashing(reg->chave, i, TAMANHO);
			//Se a posicao atual for igual a inicial e i > 0, indica que já circulou toda a estrutura e não há espaço disponível
			if((pos_busca == pos_ini) && (i > 0)){
				cout << "Tabela cheia." << endl;
				return 0;
			}

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

}

int removerRegistro(fstream &arquivo, unsigned int key){
		Registro *buffer = new Registro(0,0,VAZIO, "");
		//int pos_busca ; //= hashing(key, 0, TAMANHO);

		for(int i = 0; i < TAMANHO; i++){
				//pos_busca = hashing(key,i,TAMANHO);
				arquivo.seekg(HEADER_OFFSET + hashing(key,i,TAMANHO)*sizeof(Registro)); //Busca posição
				
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

Registro* buscarRegistro(fstream &arquivo, unsigned int key){
		Registro *buffer;
		//int pos_busca ; //= hashing(key, 0, TAMANHO);

		for(int i = 0; i < TAMANHO; i++){
				//pos_busca = hashing(key,i,TAMANHO);
				arquivo.seekg(HEADER_OFFSET + hashing(key,i,TAMANHO)*sizeof(Registro));
				arquivo.read((char*)buffer, sizeof(Registro));
				arquivo.sync();
				if(buffer->chave == key){
					return buffer;
				}
		}

		return NULL; //Retorna nulo caso não exista registro com esta chave;
}


void exibirRegistros(fstream &arquivo){
	Registro* buffer = new Registro(0,0,VAZIO," ");


	int tamanho; 
	arquivo.seekg(0,ios_base::end); //tamanho armazena a posição final do arquivo
    tamanho = arquivo.tellg();
	arquivo.seekg(0, ios_base::beg);


	do{
		arquivo.read((char*)buffer,sizeof(Registro));
		cout << "//////////////////////////////////////////" << endl;
		cout << "chave :" << buffer->chave << endl;
		cout << "nome: " << buffer->nome << endl;
		cout << "idade: " << buffer->idade << endl;
	}while(arquivo.tellg()!= tamanho);

}

int main(){
	
	char opcao = 'm'; //Armazena opcao do menu ('m' é um valor aleatório de inicializacao);
	
	
	fstream arquivo; //Objeto para leitura/escrita;

	arquivo.open("database.txt", ios_base::in | ios_base::out | ios_base::binary ); //Abre o arquivo se já existir ou cria um novo caso contrário
	if(!arquivo.is_open()){
		cout << "Arquivo inexistente/com problema. Criando novo" << endl;
		//Cria-se um arquivo novo, fecha o mesmo e reabre com as flags ideais (in, out e binary);
		//Esse processo todo foi feito por causa de problemas com "append"
		arquivo.open("database.txt", ios_base::out);
		arquivo.close();
		arquivo.open("database.txt", ios_base::in | ios_base::out | ios_base::binary);
	}
	


	
	
	

//	arquivo.seekp(HEADER_OFFSET + 0); //Retorna carrier sempre ao início após HEADER


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
	Registro* regist = new Registro(0, 0, VAZIO, " "); 
	
		
	
		
	//Se o arquivo for vazio, insere as posições de registros-vazios;
	if(tamanho == 0){
		for(int i = 0 ; i < TAMANHO; i++){
			arquivo.write((char*)regist, sizeof(Registro)); //Escreve 'TAMANHO' vezes no arquivo vazio (cada posição armazena um único registro neste caso de linear probing);
		}
	}
	




/*
*
*
*
		Menu do sistema APAGAR OS TEXTOS DESNECESSÁRIOS DEPOIS
*
*/

	cout << "////////////// Sistema Versao inicial /////////" << endl << endl;
	unsigned int buffer_chave; //Armazena os valores de chave provisoriamente (usado em consulta);

	while(opcao != 'e'){  //Laço de repetição do menu
		cout << endl << " Digite uma opcao do pdf:" << endl;
		cin >> opcao;
		
		switch(opcao){
			//////////////////////////////////////////////////////////////////////////////////////
				case 'i': {
							cout << "  Opcao i inserir." << endl;
							cin >> regist->chave ;
							cin >> regist->nome;
							cin >> regist->idade;

							//tenta inserir novo registro na tabela
							inserirRegistro(arquivo, regist);
							break;	
						  }
			//////////////////////////////////////////////////////////////////////////////////////			  
				case 'r':{
							cout << "  Opcao r remover." << endl;
							cin >> regist->chave;
							if(! removerRegistro(arquivo,regist->chave)){
								cout << "nao existe registro com chave: " << regist->chave << endl;
							}
							break;	
						}
			//////////////////////////////////////////////////////////////////////////////////////			
				case 'c':{
							cout << "  Opcao c consultar. \n";
							cin >> regist->chave;
							buffer_chave = regist->chave;					
							regist = buscarRegistro(arquivo, regist->chave);
							if(regist!= NULL){
								cout << "chave: " << regist->chave << " " << regist->nome << " " << regist->idade << endl;
							}else{
								cout << "nao ha registro com chave: " << buffer_chave << endl; 
							}
							
							break;	
							////
						}
			//////////////////////////////////////////////////////////////////////////////////////			
				case 'p':{
					    
							exibirRegistros(arquivo);
							break;	
						}
						
				case 'e':{
							cout << "  Opcao e. Finalizando" << endl << endl;
							break;	
						}
			
			///////////////////////////////////////////
			default:{
						cout << "  Opcao invalida. Digite outra" << endl;
						break;
					}
		 }	
	 }

	// Fecha o arquivo;
	arquivo.close();
	return 0;
}

