#include <iostream>
#include <conio.h>
#include <windows.h>
#include <algorithm>
#include <vector>
#include <time.h>
#include <string>
#include <fstream>
#include <sstream>
#include <iomanip>


#define VK_DOWN 80
#define VK_LEFT 75
#define VK_RIGHT 77
#define VK_UP 72

using namespace std;

// DECLARAR FUNCOES
int menuInicial(), proximaColuna();
void prepararJogo(), jogar(vector< vector<string> >&), lerPalavrasDeFicheiro(), loadingScreen(int value), mostrarTela(vector< vector<string> > &), fazerJogada(char , int &, int&, vector< vector<string> > &);
string proximaLetra();
vector<string> criarAlfabeto(),filtrarVetor(char ,int=0 ,int=4343 );
bool terminarJogo(vector< vector<string> >&, int &);

// VARIAVEIS GLOBAIS
vector<string> palavrasDicionario;
vector<string> alfabeto = criarAlfabeto();
int DimensaoHorizontal = 10;
int DimensaoVertical = 10;



int main(){

	lerPalavrasDeFicheiro();
	menuInicial();

	return 0;
}

int menuInicial(){

	system("cls");

	cout << "********************************************************************************\n"
		<< "*                         Welcome to WordTetrisPlus                            *\n"
		<< "********************************************************************************\n\n\n";
	cout << "                                  M E N U                                       \n\n"
		<< "        T T                1 > Play Default Game                   U            \n"
		<< "        R                  2 > Options                             J            \n"
		<< "        E        R         3 > Instrucions                  C P    M            \n"
		<< "               S U N       0 > Exit                         C D    L            \n";

	char tecla = getchar();

	switch (tecla)
	{
	case '1':
		prepararJogo();
		break;
	case '2':
		break;
	case '3':
		break;
	case '0':
		exit(0);
	}
}

void lerPalavrasDeFicheiro(){

	int numeroPalavras = 0;
	bool terminarCiclo = false;
	//return;
		loadingScreen(0);

		// LER FICHEIRO
		string stringTemp = "";
		ifstream myfile;
		myfile.open("words.txt"); // TODO: Alterar
		if (myfile.is_open()){
			while (!myfile.eof()){
				numeroPalavras++;
				if ( numeroPalavras % 200 == 0)
					loadingScreen(numeroPalavras);

				getline(myfile, stringTemp);
				palavrasDicionario.push_back(stringTemp);
			}
			myfile.close();
			terminarCiclo = true;
		}
		else {
			cout << "FILE NOT FOUND. PROGRAM WILL CLOSE IN 3 SECONDS" << endl;
			Sleep(3000);
			exit(1);
		}

		system("CLS");

}

void prepararJogo(){

	vector< vector<string> > tela;

	for (int i = 0; i < DimensaoVertical; i++) {
		tela.push_back(vector<string>());
		for (int j = 0; j < DimensaoHorizontal; j++)
			tela[i].push_back(" ");
	}

	jogar(tela);
}


void jogar(vector< vector<string> >&tela){

	char teclaCarregada = NULL;
	int novaLetraColuna;
	int altura =0;
	string proximaLetraParaJogar= proximaLetra();

	do{
		novaLetraColuna = proximaColuna();
		altura = 0;
		tela[altura][novaLetraColuna] = proximaLetraParaJogar;
		proximaLetraParaJogar = proximaLetra();
		do{
			mostrarTela(tela);
			cout << "\nProxima letra: " + proximaLetraParaJogar << endl;
			teclaCarregada = _getch();
			teclaCarregada = _getch();
			if (tela[altura+1][novaLetraColuna] == " ")
				fazerJogada(teclaCarregada,altura, novaLetraColuna,tela);
			else break;
		} while (altura != (DimensaoVertical-1));

	} while (terminarJogo(tela, novaLetraColuna));

	system("pause");
}

bool terminarJogo(vector< vector<string> >&tela,int &proximoIntColuna){


	for (int i = 0; i < DimensaoHorizontal; i++){
		if (tela[0][i] != " " && proximoIntColuna == i)
			return false;
	}
	return true;
}

void mostrarTela(vector< vector<string> > &tela){

	system("cls");

	cout << "********************************************************************************\n"
		 << "*                               WordTetris                                     *\n"
		 << "********************************************************************************\n\n\n";

	for (int i = 0; i < DimensaoVertical; i++){
		cout << setw(28) << "| ";
		for (int j = 0; j < DimensaoHorizontal; j++)
			cout << tela[i][j] + " ";
		cout << "|\n " << setw(26) << "|" << setw(DimensaoHorizontal * 2 + 2) << "|" << "\n";
	}
}

string proximaLetra(){

		srand(time(NULL));
		int indiceLetra = rand() % alfabeto.size();

		return alfabeto[indiceLetra];
}

int proximaColuna(){

	srand(time(NULL));
	return rand() % DimensaoHorizontal;
}

void loadingScreen(int value){

	vector<int> escolha;
	escolha.push_back(1);
	escolha.push_back(3);
	escolha.push_back(4);
	escolha.push_back(7);
	escolha.push_back(9);
	escolha.push_back(11);
	escolha.push_back(13);
	escolha.push_back(2);
	escolha.push_back(4);

	int aMostrar = value * 100 / 5000;
	srand(time(NULL));
	if (escolha.at((rand() % 9)) % 2 == 0){
		Sleep(500);
		system("CLS");

		cout << "********************************************************************************\n"
			<< "*                         LOADING ENGLISH DICTIONARY                           *\n"
			<< "********************************************************************************\n\n\n"
			<< "                                   " + static_cast<ostringstream*>(&(ostringstream() << aMostrar))->str() + " / 100%" << endl;
	}
}

void fazerJogada(char direcao,int &x, int &y, vector< vector<string> > &tela){
	string carater;
	bool escrito = false;

	switch(direcao){

	case VK_DOWN:
		if (x + 1 >= DimensaoVertical) break;
		carater = tela[x][y];
		tela[x][y] = " ";
		tela[x+1][y] = carater;
		x += 1;
		break;
	case VK_RIGHT:
		if (y + 1 >= DimensaoHorizontal) break;
		carater = tela[x][y];
		tela[x][y] = " ";
		tela[x ][y+1] = carater;
		y += 1;
		break;
	case VK_LEFT:
		if (y - 1 < 0) break;
		carater = tela[x][y];
		tela[x][y] = " ";
		tela[x][y - 1] = carater;
		y -= 1;
		break;
	case VK_UP:
		carater = tela[x][y];
		tela[x][y] = " ";
		
		for(int i = x; i < DimensaoVertical; i++){
			if( tela[i][y] != " "){
				tela[i][y] = carater;
				x = i;
				escrito = true;
			}
		}

		if(!escrito){
			x = 9;
			tela[x][y] = carater;
		}

		break;
	default:
		if (x + 1 >= DimensaoVertical) break;
		carater = tela[x][y];
		tela[x][y] = " ";
		tela[x + 1][y] = carater;
		x += 1;
		break;
	}
}

vector<string> criarAlfabeto(){

	vector<string> alfabetoCompleto;

	alfabetoCompleto.push_back("A");
	alfabetoCompleto.push_back("B");
	alfabetoCompleto.push_back("C");
	alfabetoCompleto.push_back("D");
	alfabetoCompleto.push_back("E");
	alfabetoCompleto.push_back("F");
	alfabetoCompleto.push_back("G");
	alfabetoCompleto.push_back("H");
	alfabetoCompleto.push_back("I");
	alfabetoCompleto.push_back("J");
	alfabetoCompleto.push_back("L");
	alfabetoCompleto.push_back("M");
	alfabetoCompleto.push_back("N");
	alfabetoCompleto.push_back("O");
	alfabetoCompleto.push_back("P");
	alfabetoCompleto.push_back("Q");
	alfabetoCompleto.push_back("R");
	alfabetoCompleto.push_back("S");
	alfabetoCompleto.push_back("T");
	alfabetoCompleto.push_back("U");
	alfabetoCompleto.push_back("V");
	alfabetoCompleto.push_back("X");
	alfabetoCompleto.push_back("Z");
	alfabetoCompleto.push_back("Y");
	alfabetoCompleto.push_back("W");
	alfabetoCompleto.push_back("K");

	return alfabetoCompleto;

}

vector<string> filtrarVetor(char primeiraLetra,int imin,int imax){

	vector<string> palavrasDeIndiceInferiorAoMidPoint;
	vector<string> palavrasDeIndiceSuperiorAoMidPoint;
	vector<string> aRetornar;
	// binary search
	
	if(imax > imin){

		int imid = (imax + imin)/2;

		if(palavrasDicionario[imid].at(0) > primeiraLetra)
			return filtrarVetor(primeiraLetra,imin,imid/2);
		else if( palavrasDicionario[imid].at(0) < primeiraLetra ){
			return filtrarVetor(primeiraLetra,imid/2,imax);
		}
		else{
			
			int i = imid;

			/*
			 *  Exemplo vetor { Axxx, Bas,bes,bis,bos,bos, Cxxx }
 			 *
			 *  Palavra encontrada = bis
			 *
			 *  Metodo: Partindo do indice de "bis", subtrai-se uma em uma palavra, fazendo push_back() simultaneamente, até encontrar um indice em que a palavra nao começa com "b"
			*/

			while(i>=0 && palavrasDicionario[i].at(0) == primeiraLetra){

				palavrasDeIndiceInferiorAoMidPoint.push_back(palavrasDicionario[i]);
				i--;
			}

			i = imid;

			while(i<4344 && palavrasDicionario[i].at(0) == primeiraLetra){

				palavrasDeIndiceSuperiorAoMidPoint.push_back(palavrasDicionario[i]);
				i++;
			}
		}

		/*
			 *  De modo a facilitar o algoritmo de semelhança de strings, ordena-se as palavras alfabéticamente. No "dicionario" já se encontravam , contudo no passo anterior, a ordem reverteu-se
			 *  
			 *  
			 *
			*/

			 reverse(palavrasDeIndiceInferiorAoMidPoint.begin(),palavrasDeIndiceInferiorAoMidPoint.end());


			 // push_back() para vetor final

			 for(int i = 0; i < palavrasDeIndiceInferiorAoMidPoint.size(); i++){
				 aRetornar.push_back(palavrasDeIndiceInferiorAoMidPoint[i]);
			 }

			 for(int i = 0; i < palavrasDeIndiceSuperiorAoMidPoint.size(); i++){
				 aRetornar.push_back(palavrasDeIndiceSuperiorAoMidPoint[i]);
			 }
	}


	return aRetornar;
}
