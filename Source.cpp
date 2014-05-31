#include <iostream>
#include <conio.h>
#include <windows.h>
#include <vector>
#include <time.h> 
#include <string>
#include <fstream>
#include <sstream> 
#include <iomanip> 

#define VK_DOWN 80
#define VK_LEFT 75
#define VK_RIGHT 77

using namespace std;

// DECLARAR FUNCOES
int menuInicial(), proximaColuna();
void prepararJogo(), jogar(vector< vector<string> >&), lerPalavrasDeFicheiro(), loadingScreen(int value), mostrarTela(vector< vector<string> > &), fazerJogada(char , int &, int&, vector< vector<string> > &);
string proximaLetra();
bool terminarJogo(vector< vector<string> >&, int &);

// VARIAVEIS GLOBAIS
vector<string> palavrasDicionario;
vector<string> alfabeto = { "A", "B", "C", "D", "E", "F", "G", "H", "I","J","K","L","M","N","O","P","Q","R","S","T","U","V","X","Z","Y","W","K" };
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

	return;
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
	string proximaLetraParaJogar= proximaLetra();;

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

	return rand() % DimensaoHorizontal;
}

void loadingScreen(int value){

	vector<int> escolha = { 1, 3, 5, 7, 9, 11, 13, 2, 4 };

	int aMostrar = value * 100 / 5000;
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
	case VK_SPACE:
		carater = tela[x][y];
		tela[x][y] = " ";
		for (int i = x+1; i < DimensaoVertical; i++){
			if ( tela[i][y] != " "){
				tela[i - 1][y] = carater;
				x = i - 1;
				break;
			}
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