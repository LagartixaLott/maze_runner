#include <stdio.h>
#include <stack>
#include <stdlib.h>
#include<unistd.h>    
#include <thread>
#include <vector>
#include <mutex>  
using namespace std;
// Matriz de char representnado o labirinto
 char** maze; // Voce também pode representar o labirinto como um vetor de vetores de char (vector<vector<char>>)

// Numero de linhas e colunas do labirinto
int num_rows;
int num_cols;
// Bloqueio 
mutex m;
// Representação de uma posição
struct pos_t {
	int i;
	int j;
};
//Vetor das threads criadas
//vector<thread> threads_ad;
//Vetor das posições iniciais, pras threads poderem acessar
vector<pos_t> posicoes;
// Estrutura de dados contendo as próximas
// posicões a serem exploradas no labirinto
//std::stack<pos_t> valid_positions;
/* Inserir elemento: 

	 pos_t pos;
	 pos.i = 1;
	 pos.j = 3;
	 valid_positions.push(pos)
 */
// Retornar o numero de elementos: 
//    valid_positions.size();
// 
// Retornar o elemento no topo: 
//  valid_positions.top(); 
// 
// Remover o primeiro elemento do vetor: 
//    valid_positions.pop();


// Função que le o labirinto de um arquivo texto, carrega em 
// memória e retorna a posição inicial
pos_t load_maze(const char* file_name) {
	pos_t initial_pos;
	// Abre o arquivo para leitura (fopen)
	FILE* pasta = fopen(file_name,"r");
	// Le o numero de linhas e colunas (fscanf) 
	// e salva em num_rows e num_cols
	fscanf(pasta,"%d %d",&num_rows, &num_cols);
	printf(" Número de linhas e colunas armazenadas: %d %d\n",num_rows, num_cols) ;
	// Aloca a matriz maze (malloc)
	maze = (char**)malloc(num_cols*num_rows);
	for (int i = 0; i < num_rows; ++i)
		// Aloca cada linha da matriz
		maze[i]=(char*)malloc(num_cols+1);
	for (int i = 0; i < num_rows; ++i) {
		char linha[num_cols+1];
		fscanf(pasta,"%s",linha);
		for (int j = 0; j < num_cols; ++j) {
			// Le o valor da linha i+1,j do arquivo e salva na posição maze[i][j]
			// Se o valor for 'e' salvar o valor em initial_pos
			maze[i][j]=linha[j];
			if(maze[i][j]=='e'){
				initial_pos.i=i;
				initial_pos.j=j;
			}
		}
	}
	return initial_pos;
}

// Função que imprime o labirinto
void print_maze() {
	for (int i = 0; i < num_rows; ++i) {
		for (int j = 0; j < num_cols; ++j) {
			printf("%c", maze[i][j]);
		}
		printf("\n");
	}
}


// Função responsável pela navegação.
// Recebe como entrada a posição initial e retorna um booleando indicando se a saída foi encontrada
bool walk(pos_t pos) {
	// Repita até que a saída seja encontrada ou não existam mais posições não exploradas
	// Marcar a posição atual com o símbolo '.'
	int caminhos =0;
	vector<thread> threads_ad;
	printf("y:%d x:%d\n",pos.i,pos.j);
	if(maze==nullptr){
		return false;
	}
	if(abs(pos.i)>num_rows || abs(pos.j)>num_cols){
		printf("sem saída\n");
		return false;
	}
	
	 m.lock();
	 // Imprime o labirinto
	 // Limpa a tela
	 //Testar embaixo
	 maze[pos.i][pos.j]='o';
	 system("clear||cls");
	 print_maze();
	 usleep(10000);
		if(abs(pos.i)<num_rows-1){
		printf("baixo: %c\n",maze[pos.i+1][pos.j]);
		if(maze[pos.i+1][pos.j]=='x'){
			caminhos++;
			pos_t aux_pos;
			aux_pos.i=pos.i+1;
			aux_pos.j=pos.j;
			//usleep(1000000);
	        maze[pos.i][pos.j]='.';
			//m.unlock();
			threads_ad.push_back(thread(walk,aux_pos));
		}
		if(maze[pos.i+1][pos.j]=='s'){
			print_maze();
			m.unlock();
			return true;
		}
		}
		//Testar encima
		if(abs(pos.i)>0){
		printf("cima: %c\n",maze[pos.i-1][pos.j]);
		if(maze[pos.i-1][pos.j]=='x'){
			caminhos++;
			pos_t aux_pos;
			aux_pos.i=pos.i-1;
			aux_pos.j=pos.j;
			//usleep(100000);
	        maze[pos.i][pos.j]='.';
			//m.unlock();
			threads_ad.push_back(thread(walk,aux_pos));
		}
		if(maze[pos.i-1][pos.j]=='s'){
			print_maze();
			m.unlock();
			return true;
			
		}
		}
		//Testar na direita
		if(abs(pos.j)<num_cols-1){
		printf("direita: %c\n",maze[pos.i][pos.j+1]);
		if(maze[pos.i][pos.j+1]=='x'){
			caminhos++;
			pos_t aux_pos;
			aux_pos.i=pos.i;
			aux_pos.j=pos.j+1;
			//usleep(100000);
	        maze[pos.i][pos.j]='.';
			//m.unlock();
			threads_ad.push_back(thread(walk,aux_pos));
		}
		if(maze[pos.i][pos.j+1]=='s'){
			print_maze();
			m.unlock();
			return true;
			
		}
		}
		//Testar na esquerda
		if(abs(pos.j)>0){
			printf("esquerda: %c\n",maze[pos.i][pos.j-1]);
		if(maze[pos.i][pos.j-1]=='x'){
			caminhos++;
			pos_t aux_pos;
			aux_pos.i=pos.i;
			aux_pos.j=pos.j-1;
			//usleep(100000);
	        maze[pos.i][pos.j]='.';
			//m.unlock();
			threads_ad.push_back(thread(walk,aux_pos));
		}
		if(maze[pos.i][pos.j-1]=='s'){
			print_maze();
			m.unlock();
			return true;
			

		}
		}
		m.unlock();
		 if(caminhos>0){
		  for (auto& thread : threads_ad) {
            thread.join();
    	  }
	     }
		
	return false;
}

int main(int argc, char* argv[]) {
	// carregar o labirinto com o nome do arquivo recebido como argumento
	pos_t initial_pos = load_maze("../data/maze6.txt");
	// chamar a função de navegação
	printf("x t :%d y t:%d\n",initial_pos.i,initial_pos.j);
	bool exit_found = walk(initial_pos);
	
	// Tratar o retorno (imprimir mensagem)
	//printf("%c\n",maze[0][0]);
	//printf("%c\n",maze[1][6]);
	
	return 0;
}
