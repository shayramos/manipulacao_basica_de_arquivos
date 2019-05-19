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
			unsigned int h1, h2, h2_aux; //hash1, hash2 e hash2_auxiliar;

            h1 = key % b;
            h2_aux = ((int) key/b);
            if(h2_aux == 0){
                h2 = 1;
            }else{
				h2 = h2_aux % b;
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
	Registro* buffer = new Registro("", "", VAZIO); //buffer para leitura dos registros;
	
	//Varre de 0 a TAMANHO as posições;
	for(int i = 0; i < TAMANHO; i++){
			
			// pos_busca = hashingDuplo(reg->chave, i, TAMANHO);
			pos_busca=0;
			////Caso haja espaço ou não percorreu a tabela toda;
			arquivo.seekg(HEADER_OFFSET + (pos_busca * sizeof(Registro))); //Desloca-se para a posição do registro resultando do hash(k,i,b);
			
			arquivo.read((char*)buffer,sizeof(Registro));  //Realiza a leitura na posição
			arquivo.sync();


			if(buffer->status == OCUPADO){
				if(buffer->chave == reg->chave){
					cout << "registro com chave existente" << endl;
					delete buffer;
					return 1;
				}else{ //Caso o registro só esteja ocupado e não tenha chave igual, continua FOR
					continue;
				}


			}else{ //se status = LIVRE ou DELETADO então pode gravar o registro;
				arquivo.seekp(HEADER_OFFSET + pos_busca*sizeof(Registro));
				reg->status = OCUPADO;  //Atualiza status (ele não é atualizado antes por causa da possibilidade de tabela cheia e já-existência de chave);
				arquivo.write((char*)reg, sizeof(Registro)); 
				arquivo.flush();
				char string_clean[20] = "\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00"; //Serve para apagar bloco de bytes (limpar lixo da memória)
				// strncpy(reg->nome,string_clean,20);
				strncpy(reg->conteudo,string_clean,20);

				delete buffer;  //Apaga instância de buffer (evitar memory leak);
				return 2;		//Código de operação com sucesso;
			}		
	}
	delete buffer;
    return 0; //Caso a tabela esteja cheia
}

//Remove registro
int removerRegistro(fstream &arquivo, unsigned int key){
		Registro *buffer = new Registro("","",VAZIO);

		for(int i = 0; i < TAMANHO; i++){
				arquivo.seekg(HEADER_OFFSET + hashingDuplo(key,i,TAMANHO)*sizeof(Registro)); //Busca posição
				
				arquivo.read((char*)buffer, sizeof(Registro)); //Lê registro na posição 
				arquivo.sync(); //Sincroniza stream com buffer;

				//Caso encontre a chave, apaga o registro;
				// if( (buffer->chave == key) && (buffer->status == OCUPADO)){
				if( (buffer->status == OCUPADO)){
					
					//A string abaixo foi a maneira encontrada para apagar a string anterior (problemas com header <string>);
					char string_clean[20] = "\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00"; 
					
					arquivo.seekp(-sizeof(Registro),ios_base::cur);  //Recua em 1 posição (o .read anterior avançou 1 posição além)
					// buffer->chave = 0;
					// buffer->idade = 0;
					
					// strncpy(buffer->nome, string_clean,20);
					buffer->status = DELETADO;
					arquivo.write((char*)buffer, sizeof(Registro));
					arquivo.flush();
					delete buffer;
					return 1; //1 indica que apagou com sucesso;
				}
		}
	delete buffer;
	return 0; //0 indica que não encontrou a chave;
}

//Consulta registro
int consultarRegistro(fstream &arquivo, unsigned int key){
		Registro *buffer = new Registro("","",VAZIO);

		for(int i = 0; i < TAMANHO; i++){
				arquivo.seekg(HEADER_OFFSET + hashingDuplo(key,i,TAMANHO)*sizeof(Registro));
				arquivo.read((char*)buffer, sizeof(Registro));
				arquivo.sync();
				if( (buffer->chave == "key") && (buffer->status == OCUPADO)){ 
					// cout << "chave: " << buffer->chave << " " << buffer->nome << " " << buffer->idade << endl;
					delete buffer;
					return 1; //Encontrou a chave;
				}
		}
		cout << "nao ha registro com a chave: " << key << endl;
		delete buffer;
		return 0; //Não encontrou a chave;
}

//Exibe todos os registros
void exibirRegistros(fstream &arquivo){
	Registro* buffer = new Registro("","",VAZIO);

	int tamanho; 
	arquivo.seekg(0,ios_base::end); //'tamanho' armazena a posição final do arquivo;
    tamanho = arquivo.tellg();
	arquivo.seekg(0, ios_base::beg);


	do{
		arquivo.read((char*)buffer,sizeof(Registro));
		cout << "/////////////////// Registro " << (arquivo.tellg()/sizeof(Registro)) << "///////////////////////" << endl;
		cout << "chave: " << buffer->chave << endl;
		cout << "conteudo: " << buffer->conteudo << endl;
		// cout << "nome: " << buffer->nome << endl;
		// cout << "idade: " << buffer->idade << endl;
		cout << "status: " << statusText(buffer->status) << endl;
	}while(arquivo.tellg()!= tamanho);
	delete buffer;
}


///////////////// Função principal
int main(int argc, char* argv[]){
	
	if (argc != 2){
    	cout<<"usage: "<< argv[0] <<" <filename>\n";
		return 0;
	}
  	 
    // We assume argv[1] is a filename to open
    ifstream file_in(argv[1]); //Objeto para leitura;
	
    // Checkar se o arquivo abriu com sucesso
    if (!file_in.is_open()){
      cout << "Could not open file\n";
	  return 0;
  	}

	fstream arquivo; //Objeto para leitura/escrita;

	arquivo.open("database.bin", ios_base::in | ios_base::out | ios_base::binary ); //Abre o arquivo se já existir ou cria um novo caso contrário
	if(!arquivo.is_open()){
	
		//Cria-se um arquivo novo, fecha o mesmo e reabre com as flags ideais (in, out e binary);
		//Esse processo todo foi feito por causa de problemas com "append"
		arquivo.open("database.bin", ios_base::out);
		arquivo.close();
		arquivo.open("database.bin", ios_base::in | ios_base::out | ios_base::binary);
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
	Registro* regist = new Registro("", "", VAZIO); 
	

	//Se o arquivo for vazio (criado agora), insere as posições de registros-vazios;
	// if(tamanho == 0){
	// 	for(int i = 0 ; i < TAMANHO; i++){
	// 		arquivo.write((char*)regist, sizeof(Registro)); //Escreve 'TAMANHO' vezes no arquivo vazio (cada posição armazena um único registro neste caso de linear probing);
	// 	}
	// }
	
/*
*
*
*
		Menu do sistema (não tem exibição de texto)
*/

	std::string linha;
	while (std::getline(file_in, linha)){
		
		//INSERIR
		//////////////////////////////////////////////////////////////////////////////////////
		if(linha!="e" && linha!="c" && linha!="r"){
			regist->chave = linha;
			std::getline(file_in, linha);
			regist->conteudo = linha;
			//inserirRegistro(arquivo, regist);
			cout << linha << endl;
		}else{
		//FINALIZAR
		//////////////////////////////////////////////////////////////////////////////////////
			if(linha=="e"){
				file_in.close();
				return 0;
			}else{
			//CONSULTAR
			//////////////////////////////////////////////////////////////////////////////////////		
				if(linha=="c"){
					std::getline(file_in, linha);
					regist->chave = linha;
					//exibirRegistros(arquivo, regist->chave);
					cout << linha << endl;
				}else{
				//REMOVER
				//////////////////////////////////////////////////////////////////////////////////////
					if(linha=="r"){
						std::getline(file_in, linha);
						regist->chave = linha;
						// if(!removerRegistro(arquivo, regist->chave)){
						// 	cout << "nao existe registro com chave: " << regist->chave << endl;
						// }
						cout << linha << endl;
					}
				}
			}
		}
	}

	// unsigned int buffer_chave; //Armazena os valores de chave provisoriamente (usado em consulta);
	// char opcao = 'm'; //Armazena opcao do menu ('m' é um valor aleatório de inicializacao);
	// char entrada[100]; //Armazena o nome no caso de inserção (variável auxiliar p/ filtragem da quantidade de caracteres);
	
	// while(opcao != 'e'){  //Laço de repetição do menu		
    // // Lê uma opcao de funcionalidade;
	// 	cin >> opcao;
	// 	switch(opcao){
	// 		//////////////////////////////////////////////////////////////////////////////////////
	// 			case 'i': {
	// 						//cout << "  Opcao i inserir." << endl;
	// 						cin >> regist->chave ;
	// 						cin.ignore();  //Se não houver esse .ignore() e o seguinte ocorrerá bug e entrada fica em loop;
							
	// 						cin.getline(entrada,100); //Foi utilizado uma variável extra para leitura dos 20 caracteres de limite	
	// 						if(cin.gcount() <= 21){   // pois estava ocorrendo um loop infinito.
	// 							strncpy(regist->nome, entrada, cin.gcount() -1);  
	// 						}else{												  
	// 							strncpy(regist->nome,entrada,20);	  //Caso o nome tenha mais de 20 caracteres, pega
	// 						}										//somente os 20 primeiros

	// 						cin >> regist->idade;    //Lê a idade
												
	// 						        //tenta inserir novo registro na tabela
	// 						inserirRegistro(arquivo, regist);
	// 						break;	
	// 					  }
	// 		//////////////////////////////////////////////////////////////////////////////////////			  
	// 			case 'r':{
	// 						//cout << "  Opcao r remover." << endl;
	// 						cin >> regist->chave;
	// 						if(! removerRegistro(arquivo,regist->chave)){
	// 							cout << "nao existe registro com chave: " << regist->chave << endl;
	// 						}
	// 						break;	
	// 					}
	// 		//////////////////////////////////////////////////////////////////////////////////////			
	// 			case 'c':{
	// 						//cout << "  Opcao c consultar. \n";
	// 						cin >> regist->chave;
	// 						buffer_chave = regist->chave;					
	// 						consultarRegistro(arquivo, regist->chave);
	// 						break;	
	// 					}
	// 		//////////////////////////////////////////////////////////////////////////////////////			
	// 			case 'p':{  //Opcao de print dos registros;
	// 						exibirRegistros(arquivo);
	// 						cout << "//////////////////////////////////////////" << endl;
	// 						break;	
	// 					}
						
	// 			case 'e':{ //Opcao de fim de programa;
	// 						break;	
	// 					}
			
	// 		///////////////////////////////////////////
	// 		default:{ //Qualquer outra opcao serah considerada como invalida;
	// 					break;
	// 				}
	// 	 }	
	//  }

	// Fecha o arquivo;
	file_in.close();
	arquivo.close();
	delete regist;
	return 0;
}

