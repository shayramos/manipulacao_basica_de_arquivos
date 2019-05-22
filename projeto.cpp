//============================================================================
// Name        : projeto.cpp
// Author      : Aline / Fernando / Hugo
// Version     : 1.0
// Copyright   : GPL
// Description : Gerencia de arquivo com chave/valor
//============================================================================
#include "estrutura.h"



/*
Hashing com encadeamento open-addressing
-> colocar para verificar se a chave buscada bate com a chave do registro em todos os casos (evitar casos
de colisão de hash e acabar imprimindo registros erroneos);


*/
//Função p/ converter string 'chave' em uint e calcular Hash H = val MOD TAMANHO (unsigned long long int);
unsigned long long int HashString(const char* texto){
		string str = texto;
		unsigned long long int hash = 0;
		// A constante '31' se refere ao número 2^n -1 quando n=5 (número primo de Marsenne)
		for(string::iterator it = str.begin(); it != str.end(); it++){
			hash = (31*hash + *(it)) % TAMANHO; 
		}
		return hash;
} 


//O bool 'swt' (switch) serve para alternar quando deve verificar a partir do final ou do início do arquivo por uma posição vazia;
//A função 'inserirRegistro' ficou um tanto grande e com parte de código que podia ser reduzido à funções, mas por falta de tempo
//não conseguimos reorganizá-la para tal;
void inserirRegistro(fstream &arquivo, Registro *reg, bool swt){

		//Calcula a posição ideal da chave de reg;
		unsigned long long int hash_calculado = HashString(reg->chave);
		//Buffer para leitura do arquivo e comparação com reg;
		Registro* buffer = new Registro("","", -1, -1,VAZIO);

		//Move para a posição do hash calculado
		arquivo.seekg(HEADER_OFFSET + hash_calculado*sizeof(Registro)); 
	
		arquivo.read((char*)buffer,sizeof(Registro));  //Realiza a leitura na posição
		arquivo.sync(); //Sincroniza;

		if(buffer->status == OCUPADO){
				//Verificar se o registro na posição é "dono dela";
				unsigned long long int hash_local = HashString(buffer->chave);

				if(hash_local == hash_calculado){ //Se a posição atual for dona do lugar
					long long int i, index_next, pos_buffer, pos_anterior;


					//Vai seguindo a lista encadeada enquanto não encontrar proximo lambda (-1);
					pos_buffer = hash_local;
					pos_anterior = -1;

					while(buffer->proximo != -1){
						pos_anterior = pos_buffer;
						pos_buffer = buffer->proximo;
						arquivo.seekg(HEADER_OFFSET + (buffer->proximo)*sizeof(Registro));
						arquivo.read((char*)buffer, sizeof(Registro));
						arquivo.sync();
					}
					

					//A partir daqui, buffer estará armazenando o último elemento da lista encadeada com
					//hash_local == hash_calculado;

					//buffer extra p/ varredura;
					Registro *buff_extra = new Registro("", "", -1, -1, VAZIO);

					//Esse FOR servirá para ir varrendo o arquivo do fim até o início na tentativa de
					//encontrar um espaço vazio
                if(!swt){  //Se o swt (switch) for '0', faz do final pro início;   
					for(i = TAMANHO; i >= 1; i--){
						index_next = (i-1)*sizeof(Registro);
						arquivo.seekg(HEADER_OFFSET + index_next); //Move ponteiro para último registro;
					    arquivo.read((char*)buff_extra, sizeof(Registro)); 
						arquivo.sync();

						if(buff_extra->status != OCUPADO){ //Se for um local vago; 
							arquivo.seekp(HEADER_OFFSET + index_next); //Move ponteiro de escrita;
							reg->status = OCUPADO;
							reg->anterior = pos_buffer; 
							reg->proximo = -1;
							arquivo.write((char*) reg, sizeof(Registro));
							arquivo.flush();

							//Retorna à posição de conflito (posição original) para atualizar o ponteiro  para a posição i
							buffer->proximo = i-1;  
							arquivo.seekp(HEADER_OFFSET + pos_buffer*sizeof(Registro));
							arquivo.write((char*) buffer, sizeof(Registro));
							arquivo.flush();

							 //Limpando;
							string clean_chave (20,'\x00');
							string clean_valor(50,'\x00');
							strcpy(reg->chave,clean_chave.c_str());
							strcpy(reg->valor,clean_valor.c_str());
							break;
						}
						
					}
                }else{ //Se swt = 1;
                    for(i = 1; i <= TAMANHO; i++){
						index_next = (i-1)*sizeof(Registro);
						arquivo.seekg(HEADER_OFFSET + index_next); //Move ponteiro para último registro;
					    arquivo.read((char*)buff_extra, sizeof(Registro)); 
						arquivo.sync();

						if(buff_extra->status != OCUPADO){ //Se for um local vago; 
							arquivo.seekp(HEADER_OFFSET + index_next); //Move ponteiro de escrita;
							reg->status = OCUPADO;
							reg->anterior = pos_buffer; 
							reg->proximo = -1;
							arquivo.write((char*) reg, sizeof(Registro));
							arquivo.flush();

							//Retorna à posição de conflito (posição original) para atualizar o ponteiro  para a posição i
							buffer->proximo = i-1;  
							arquivo.seekp(HEADER_OFFSET + pos_buffer*sizeof(Registro));
							arquivo.write((char*) buffer, sizeof(Registro));
							arquivo.flush();

							 //Limpando;
							string clean_chave (20,'\x00');
							string clean_valor(50,'\x00');
							strcpy(reg->chave,clean_chave.c_str());
							strcpy(reg->valor,clean_valor.c_str());
							break;
						}
						
					}

                }

					delete buff_extra;
                

				}else{	//Caso a posição atual não seja dona do lugar e precise ceder a quem tem direito;
					long long int i, index_next;

					//Fazer Swap entre registros;
					Registro *buff_extra = new Registro("", "", -1, -1, VAZIO);

					//Copia dos dados de buffer p/ buff_extra para realizar swap;	
					strcpy(buff_extra->chave ,buffer->chave);
					strcpy(buff_extra->valor, buffer->valor);
					buff_extra->status = buffer->status;
					buff_extra->anterior = buffer->anterior;
					buff_extra->proximo = buffer->proximo;


					//Procura e move buff_extra para uma nova posição e atualiza o apontador do item anterior à buff_extra na lista;
					//Se não houver posição sobrando, não executa a inserção

                    if(!swt){
					  for(i = TAMANHO; i>=1; i--){
						index_next = (i-1)*sizeof(Registro);
						arquivo.seekg(HEADER_OFFSET + index_next);
						arquivo.read((char*)buffer, sizeof(Registro));
						arquivo.sync();

						if(buffer->status != OCUPADO){ //Se posição estiver vazia, então escreve nela;
							arquivo.seekp(HEADER_OFFSET + index_next);
							//buff_extra->status = OCUPADO;
							arquivo.write((char*)buff_extra, sizeof(Registro));
							arquivo.flush();

							//Atualizar o registro que apontava para buff_extra;

							arquivo.seekg(HEADER_OFFSET + buff_extra->anterior*sizeof(Registro));
							arquivo.read((char*)buffer, sizeof(Registro));
							arquivo.sync();

							buffer->proximo = i-1;
							arquivo.seekp(HEADER_OFFSET + buff_extra->anterior*sizeof(Registro));
							arquivo.write((char*)buffer, sizeof(Registro));
							arquivo.flush();

						
						
								//Escreve reg na posição correta;
							arquivo.seekp(HEADER_OFFSET + hash_calculado*sizeof(Registro));
							reg->status = OCUPADO;
							arquivo.write((char*) reg , sizeof(Registro));
							arquivo.flush();

							//Limpando;
							string clean_chave (20,'\x00');
							string clean_valor(50,'\x00');
							strcpy(reg->chave,clean_chave.c_str());
							strcpy(reg->valor,clean_valor.c_str());
							break;
						
						}


					}
                }else{
            	  for(i = 1; i <= TAMANHO; i++){
						index_next = (i-1)*sizeof(Registro);
						arquivo.seekg(HEADER_OFFSET + index_next);
						arquivo.read((char*)buffer, sizeof(Registro));
						arquivo.sync();

						if(buffer->status != OCUPADO){ //Se posição estiver vazia, então escreve nela;
							arquivo.seekp(HEADER_OFFSET + index_next);
							//buff_extra->status = OCUPADO;
							arquivo.write((char*)buff_extra, sizeof(Registro));
							arquivo.flush();

							//Atualizar o registro que apontava para buff_extra;

							arquivo.seekg(HEADER_OFFSET + buff_extra->anterior*sizeof(Registro));
							arquivo.read((char*)buffer, sizeof(Registro));
							arquivo.sync();

							buffer->proximo = i-1;
							arquivo.seekp(HEADER_OFFSET + buff_extra->anterior*sizeof(Registro));
							arquivo.write((char*)buffer, sizeof(Registro));
							arquivo.flush();

						
						
								//Escreve reg na posição correta;
							arquivo.seekp(HEADER_OFFSET + hash_calculado*sizeof(Registro));
							reg->status = OCUPADO;
							arquivo.write((char*) reg , sizeof(Registro));
							arquivo.flush();

							//Limpando;
							string clean_chave (20,'\x00');
							string clean_valor(50,'\x00');
							strcpy(reg->chave,clean_chave.c_str());
							strcpy(reg->valor,clean_valor.c_str());
							break;
						
						}
                    }
                }    
                    delete buff_extra;
				}



		}else{ //Caso status = VAZIO 
			reg->status = OCUPADO;
            reg->anterior = -1;
            reg->proximo = -1;
			arquivo.seekp(HEADER_OFFSET + hash_calculado*sizeof(Registro));
			arquivo.write((char*) reg, sizeof(Registro));
			arquivo.flush();

			//Limpando reg:
			string clean_chave (20,'\x00');
			string clean_valor(50,'\x00');
			strcpy(reg->chave,clean_chave.c_str());
			strcpy(reg->valor,clean_valor.c_str());
            reg->anterior = -1;
            reg->proximo = -1;
            reg->status = VAZIO;
		}
		delete buffer;
}








//Protótipo 1 (apaga mesmo os ponteiros anterior e proximo) -> para casos onde não há conflito de chaves
void removerRegistro(fstream &arquivo, const char* chave){
		Registro *buffer = new Registro("", "", -1, -1, VAZIO);
        Registro* buff_extra = new Registro("","",-1,-1,VAZIO);

		//Variáveis locais auxiliares para os índices de registros, bem como valores calculados de hash;
		 long long int hash = HashString(chave);
		 long long int hash_swap, posicao, index_next, index_swap;

		//Será utilizado para "limpar" strings do registro
		string clean_chave(20,'\x00');
		string clean_valor(50,'\x00');

		do{
			arquivo.seekg(HEADER_OFFSET + hash * sizeof(Registro));
			arquivo.read((char*)buffer, sizeof(Registro));
			arquivo.sync();

			hash_swap = hash;  //Salva o valor atual do hash para apagar registro;
			hash = buffer->proximo;	

            if(!strcmp(buffer->chave, chave)){ //Se a chave na posição for igual a que estamos procurando
				        //Verificar se o registro a ser apagado tem valor "proximo" != -1:
						//I) caso tenha, desloca esse proximo para a posição-raiz e apaga a posicao antiga do "proximo"
						//II) caso nao tenha, basta apagar direto
					
					index_next = hash * sizeof(Registro);
					index_swap = hash_swap * sizeof(Registro);


					if(buffer->proximo != -1){ //Se buffer->proximo != -1 (existe ao menos 1 registro em seguida)
						
						//Aqui será realizado o swap de registros e apagado o desejado;
						
						arquivo.seekg(HEADER_OFFSET + index_next); //Lê o "proximo" registro
						arquivo.read((char*)buff_extra, sizeof(Registro)); //e armazena em buff_extra;
						arquivo.sync();
                        

                        //Atualizando o ponteiro "anterior":
                        buff_extra->anterior = buffer->anterior;

						arquivo.seekp(HEADER_OFFSET + index_swap); //Vai para a posição original para inserir o "proximo"
						arquivo.write((char*)buff_extra, sizeof(Registro)); //registro nela (swap)
						arquivo.flush();
                        

                        
                        posicao = buff_extra->proximo; 

                        //se buff_extra->proximo != 1, então podemos rearranjar com o elemento seguinte;
                        if(buff_extra->proximo != -1){
                            arquivo.seekg(HEADER_OFFSET + (posicao * sizeof(Registro)));
                            arquivo.read((char*)buff_extra, sizeof(Registro));
                            arquivo.sync();
                        
                            buff_extra->anterior = hash_swap; //Atualiza ponteiro "anterior" do registro seguinte ao que foi reordenado
                            arquivo.seekp(HEADER_OFFSET + (posicao * sizeof(Registro)));
                            arquivo.write((char*)buff_extra, sizeof(Registro));
                            arquivo.flush();
                        }
                        

						//Preenchendo com 0's os bytes de chave e valor;
						strcpy(buffer->chave,clean_chave.c_str());
						strcpy(buffer->valor,clean_valor.c_str());
						
						//Valores-padrão de um registro vazio;
						buffer->anterior = -1;
						buffer->proximo = -1;
						buffer->status = VAZIO;
						
						
						//Apagando o registro antigo de "proximo" (finalizando swap)
						arquivo.seekp(HEADER_OFFSET + index_next); //Vai para a posição "proximo" para finalizar o swap
						arquivo.write((char*)buffer, sizeof(Registro));
						arquivo.flush();

						//Atualiza hash com  o valor original(valor em que !strcmp foi true) e elimina buffer extra;

                        hash = hash_swap;

					}else{
						//Caso o valor de "proximo" == -1 (registro no fim da lista encadeada);
							//Preenchendo com 0's os bytes de chave e valor;
						strcpy(buffer->chave,clean_chave.c_str());
						strcpy(buffer->valor,clean_valor.c_str());
						
                        

                        //Se existir algum registro apontando para o que vai ser excluído (este for o último de uma lista com > 1 elemento):
                        if(buffer->anterior != -1){
                            arquivo.seekg(HEADER_OFFSET + buffer->anterior*sizeof(Registro));
                            arquivo.read((char*)buff_extra, sizeof(Registro));
                            arquivo.sync();

                            buff_extra->proximo = -1; 
                            
                            arquivo.seekp(HEADER_OFFSET + buffer->anterior*sizeof(Registro));
                            arquivo.write((char*)buff_extra, sizeof(Registro));
                            arquivo.flush();

                            buffer->anterior = -1;
						    buffer->proximo = -1;
						    buffer->status = VAZIO;
                		    arquivo.seekp(HEADER_OFFSET + index_next); //Vai para a posição "proximo" para finalizar o swap
                            arquivo.write((char*)buffer, sizeof(Registro));
						    arquivo.flush();

                        }else{ //Caso seja um elemento único;
                                       
                            buffer->anterior = -1;
						    buffer->proximo = -1;
						    buffer->status = VAZIO;
                            arquivo.seekp(HEADER_OFFSET + index_swap); //Vai para a posição "proximo" para finalizar o swap
                            arquivo.write((char*)buffer, sizeof(Registro));
						    arquivo.flush();
                        }

					}
			}
			
		}while(hash != -1);


	delete buffer;
    delete buff_extra;
}


void consultarRegistro(fstream &arquivo, const char* chave){
		Registro *buffer = new Registro("","",-1,-1 ,VAZIO);

		unsigned long long int hash = HashString(chave);
		
		do{
			arquivo.seekg(HEADER_OFFSET + hash*sizeof(Registro));
			arquivo.read((char*)buffer, sizeof(Registro));
			arquivo.sync();	
			if(!strcmp(buffer->chave, chave)){ //Se a chave na posição for igual a que estamos procurando
				cout << buffer->chave << " " << buffer->valor << endl; 
			}
			hash = buffer->proximo;
		}while(buffer->proximo != -1);
		
	
		
		delete buffer;
}



  //APAGUE ESTE MÉTODO (É SOMENTE PARA DEBUG!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!);
//Exibe todos os registros

/*void exibirRegistros(fstream &arquivo){
	Registro* buffer = new Registro("","",-1,-1,VAZIO);


	int tamanho; 
	arquivo.seekg(0,ios_base::end); //'tamanho' armazena a posição final do arquivo;
    tamanho = arquivo.tellg();
	arquivo.seekg(0, ios_base::beg);


	do{
		arquivo.read((char*)buffer,sizeof(Registro));
		cout << "/////////////////// Registro " << (arquivo.tellg()/sizeof(Registro)) -1  << "///////////////////////" << endl;
		cout << "chave: " << buffer->chave << endl;
		cout << "valor: " << buffer->valor << endl;
		cout << "anterior: " << buffer->anterior << endl;
		cout << "proximo: " << buffer->proximo << endl;
		cout << "status: " << buffer->status << endl;
	}while(arquivo.tellg()!= tamanho);
	delete buffer;
}*/



///////////////// Função principal
int main(int argc, char* argv[]){
	
	fstream arquivo_inicial; //Arquivo de dados inicial (será buscado caso seja passado como parâmetro);
	fstream arquivo; //Objeto para leitura/escrita;
	
	

    //Versão com formato binário
	//arquivo.open("database.bin", ios_base::in | ios_base::out | ios_base::binary ); //Abre o arquivo se já existir ou cria um novo caso contrário


	arquivo.open("database.bin", ios_base::in | ios_base::out | ios_base::binary); //Abre o arquivo se já existir ou cria um novo caso contrário
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



//AJUSTAR ESTE TRECHO!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
	int tamanho = 0;
	arquivo.seekg(0,ios_base::end); //Avança proximo até o último byte para que seja possível "medir" o arquivo;
	tamanho = arquivo.tellg();
	arquivo.seekg(0,ios_base::beg); //Retorna proximo para o início do arquivo;
	
	// "regist" é um registro-auxiliar (servirá de buffer de escrita e remoção de registros);
	Registro* regist = new Registro("", "", -1, -1, VAZIO );  //Lembrando que '-1' = lambda (proximo)
	


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

	//unsigned int buffer_chave; //Armazena os valores de chave provisoriamente (usado em consulta);
	char opcao = 'm'; //Armazena opcao do menu ('m' é um valor aleatório de inicializacao);
	char entrada[100]; //Armazena o nome no caso de inserção (variável auxiliar p/ filtragem da quantidade de caracteres);
    bool swt = 0;

	while(opcao != 'e'){  //Laço de repetição do menu
		
    
		// Lê uma opcao de funcionalidade;
		cin >> opcao;
		cin.ignore(); //Ignorar '\n';

		switch(opcao){
			//////////////////////////////////////////////////////////////////////////////////////
				case 'i': { //Inserção;
							cin.getline(entrada,100);
							
							if(cin.gcount() < 20){   //se forem < (20 caracteres + '\n')
								strncpy(regist->chave, entrada, cin.gcount());  
							}else{												  
								strncpy(regist->chave,entrada, 20);	  //Caso o nome tenha mais de 20 caracteres, pega
							}			


							cin.getline(entrada,100); //Foi utilizado uma variável extra para leitura dos 20 caracteres de limite
							if(cin.gcount() < 50){   // pois estava ocorrendo um loop infinito.
								strncpy(regist->valor, entrada, cin.gcount());  
							}else{												  
								strncpy(regist->valor,entrada,50);	  //Caso o nome tenha mais de 20 caracteres, pega
							}										//somente os 20 primeiros
							
							//tenta inserir novo registro na tabela
							inserirRegistro(arquivo, regist, swt);
                            swt = !swt; //Alterna valor de swt (switch)
							break;
							
						  }
			//////////////////////////////////////////////////////////////////////////////////////			  
				case 'r':{
							cin.getline(entrada,100);
							if(cin.gcount() < 20){   //se forem < (20 caracteres + '\n')
								strncpy(regist->chave, entrada, cin.gcount());  
							}else{												  
								strncpy(regist->chave,entrada, 20);	  //Caso o nome tenha mais de 20 caracteres, pega
							}		
							removerRegistro(arquivo,regist->chave);
							break;	
						}
			//////////////////////////////////////////////////////////////////////////////////////			
				case 'c':{ //Consulta de chaves e valores
							
							cin.getline(entrada,100);				 
							if(cin.gcount() < 20){   //se forem < (20 caracteres + '\n')
								strncpy(regist->chave, entrada, cin.gcount());  
							}else{												  
								strncpy(regist->chave,entrada, 20);	  //Caso o nome tenha mais de 20 caracteres, pega
							}					
							consultarRegistro(arquivo, regist->chave);
							break;	
						}

					//DEBUG - REMOVER DEPOIS
					case 'p':{  //Opcao de print dos registros;
							exibirRegistros(arquivo);
							cout << "//////////////////////////////////////////" << endl;
							break;	
						}
			//////////////////////////////////////////////////////////////////////////////////////			
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
	delete regist;
	return 0;
}

