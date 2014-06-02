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

using namespace std;

// Declarar funções
int menuInicial(), proximaColuna();
void prepararJogo(), jogar(vector< vector<string> >&), lerPalavrasDeFicheiro(), loadingScreen(int value), mostrarTela(vector< vector<string> > &), fazerJogada(char, int &, int&, vector< vector<string> > &), verificarPalavras(vector<vector<string>>&, int&);
vector<size_t> algoritmoNaive(vector<vector<string>>&, string, string);
string proximaLetra(), convertInt(int number);
vector<string> criarAlfabeto(), filtrarVetor(char, int = 0, int = 4290);
bool terminarJogo(vector< vector<string> >&, int &);
void menuOpcoes(), alterarControlos(), alterarDimensoes(), menuInstrucoes(), menuOptions();
bool linhaCheia(vector<vector<string>>, string&, int&);

// Variáveis globais
vector<string> dicionario;
vector<string> alfabeto = criarAlfabeto();
int dimensaoHorizontal = 10;
int dimensaoVertical = 10;
// Controlos
char CTRL_BAIXO = 80;
char CTRL_ESQ = 75;
char CTRL_DIR = 77;
char CTRL_CIMA = 72;

int main(){
	srand(time(NULL));
	lerPalavrasDeFicheiro();
	menuInicial();

	return 0;
}

int menuInicial(){
	while (true) {
		system("cls");

		cout << "********************************************************************************\n"
			<< "*                         Welcome to WordTetris Plus                           *\n"
			<< "********************************************************************************\n\n\n"
			<< "                                  M E N U                                       \n\n"
			<< "        M T                1   > Play Default Game                   U          \n"
			<< "  R                        2   > Options                   J                    \n"
			<< "        E        R         3   > Instructions                  C P    M         \n"
			<< "               S W N       ESC > Exit                8          D    L          \n";

		char tecla = _getch();

		switch (tecla) {
		case '1':
			prepararJogo();
			break;
		case '2':
			menuOptions();
			break;
		case '3':
			menuInstrucoes();
			break;
		case 27:
			return 0;
		}
	}
}

void lerPalavrasDeFicheiro(){
	int numeroPalavras = 0;
	bool terminarCiclo = false;
	loadingScreen(0);

	// LER FICHEIRO
	string stringTemp = "";
	ifstream myfile;
	myfile.open("words.txt");
	if (myfile.is_open()){
		while (!myfile.eof()){
			numeroPalavras++;
			if (numeroPalavras % 200 == 0)
				loadingScreen(numeroPalavras);

			getline(myfile, stringTemp);
			dicionario.push_back(stringTemp);
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
	vector<vector<string>> tela;

	for (int i = 0; i < dimensaoVertical; i++) {
		tela.push_back(vector<string>());
		for (int j = 0; j < dimensaoHorizontal; j++)
			tela[i].push_back(" ");
	}

	jogar(tela);
}


void jogar(vector<vector<string>>& tela){
	char teclaCarregada = NULL;
	int novaLetraColuna;
	int altura = 0;
	int pontuacao = 0;
	string proximaLetraParaJogar = proximaLetra();

	do {
		novaLetraColuna = proximaColuna();
		altura = 0;
		tela[altura][novaLetraColuna] = proximaLetraParaJogar;
		proximaLetraParaJogar = proximaLetra();
		do {
			mostrarTela(tela);

			cout << " Score: " + convertInt(pontuacao) << endl;
			cout << "------------------" << endl;
			cout << "Next letter: " + proximaLetraParaJogar << endl;
			cout << "> ESC to exit" << endl;

			_getch();
			teclaCarregada = _getch();

			if (teclaCarregada == 27) return;

			if (teclaCarregada == CTRL_CIMA){
				fazerJogada(teclaCarregada, altura, novaLetraColuna, tela);
				break;
			}
			else if ((altura + 1) < dimensaoVertical && tela[altura + 1][novaLetraColuna] == " ")
				fazerJogada(teclaCarregada, altura, novaLetraColuna, tela);
			else break;
		} while (altura < dimensaoVertical);

		verificarPalavras(tela, pontuacao);
	} while (terminarJogo(tela, novaLetraColuna));

	system("pause");
}

void verificarPalavras(vector<vector<string>>& tela, int& pontuacao) {
	for (unsigned int j = 0; j < dimensaoHorizontal; j++) {
		string palheiro = "";

		for (unsigned int i = 0; i < dimensaoVertical; i++) {
			palheiro += tela[i][j];
		}

		for (unsigned int k = 0; k < palheiro.length(); k++) {
			vector<string> dicFiltrado = filtrarVetor(palheiro[k] + 32);
			vector<size_t> matches;
			for (auto it_needle = dicFiltrado.cbegin(); it_needle != dicFiltrado.cend(); it_needle++) {
				matches = algoritmoNaive(tela, *it_needle, palheiro);
				if (!matches.empty()) break;
			}

			if (!matches.empty()) {
				for (unsigned int i = matches[0]; i <= matches[1]; i++)
					tela[i][j] = " ";

				pontuacao += matches[1] - matches[0] + 1;
				break;
			}
		}

	}

	for (unsigned int i = 0; i < dimensaoVertical; i++) {
		string palheiro = "";

		for (unsigned int j = 0; j < dimensaoHorizontal; j++) {
			palheiro += tela[i][j];
		}

		for (unsigned int k = 0; k < palheiro.length(); k++) {
			vector<string> dicFiltrado = filtrarVetor(palheiro[k] + 32);
			vector<size_t> matches;
			for (auto it_needle = dicFiltrado.cbegin(); it_needle != dicFiltrado.cend(); it_needle++) {
				matches = algoritmoNaive(tela, *it_needle, palheiro);
				if (!matches.empty()) break;
			}

			if (!matches.empty()) {
				for (unsigned int j = matches[0]; j <= matches[1]; j++)
					tela[i][j] = " ";

				pontuacao += matches[1] - matches[0] + 1;
				break;
			}
		}

	}

	for (int j = dimensaoVertical - 1; j >= 0; j--) {
		for (unsigned int k = 0; k < dimensaoHorizontal; k++) {
			if (j > 0 && tela[j][k] == " " && tela[j - 1][k] != " ") {
				tela[j][k] = tela[j - 1][k];
				tela[j - 1][k] = " ";
			}
		}
	}
}

bool terminarJogo(vector< vector<string> >&tela, int &proximoIntColuna){
	for (int i = 0; i < dimensaoHorizontal; i++){
		if (tela[0][i] != " " && proximoIntColuna == i)
			return false;
	}
	return true;
}

void mostrarTela(vector< vector<string> > &tela){

	system("cls");

	cout << "********************************************************************************\n"
		<< "*                               WordTetris Plus                                *\n"
		<< "********************************************************************************\n\n";

	for (int i = 0; i < dimensaoVertical; i++){
		cout << setw(28) << "| ";
		for (int j = 0; j < dimensaoHorizontal; j++)
			cout << tela[i][j] + " ";
		cout << "|\n " << setw(26) << "|" << setw(dimensaoHorizontal * 2 + 2) << "|" << "\n";
	}
}

string proximaLetra() {
	int  consoanteOuVogal = rand() % 100;

	if (consoanteOuVogal < 34){
		vector<string> vogais = { "A", "E", "I", "O", "U" };
		return vogais[rand() % vogais.size()];
	}
	else {
		vector<string> consoantes;
		for (unsigned int i = 0; i < alfabeto.size(); i++){
			if (alfabeto[i] != "A" || alfabeto[i] != "E" || alfabeto[i] != "I" || alfabeto[i] != "O" || alfabeto[i] != "U")
				consoantes.push_back(alfabeto[i]);
		}

		return consoantes[rand() % consoantes.size()];
	}
}

int proximaColuna(){
	return rand() % dimensaoHorizontal;
}

void loadingScreen(int value){
	vector<int> escolha = { 1, 2, 3, 4, 7, 9, 11, 13 };

	int aMostrar = value * 100 / 5000;
	if (escolha[(rand() % 8)] % 2 == 0){
		Sleep(500);
		system("CLS");

		cout << "********************************************************************************\n"
			<< "*                         LOADING ENGLISH DICTIONARY                           *\n"
			<< "********************************************************************************\n\n\n"
			<< "                                   " + static_cast<ostringstream*>(&(ostringstream() << aMostrar))->str() + " / 100%" << endl;
	}
}

void fazerJogada(char direcao, int &x, int &y, vector< vector<string> > &tela){
	string carater;
	bool escrito = false;

	if (direcao == CTRL_BAIXO){
		if (x + 1 >= dimensaoVertical) return;
		carater = tela[x][y];
		tela[x][y] = " ";
		tela[x + 1][y] = carater;
		x += 1;
	}
	else if (direcao == CTRL_DIR){
		if (y + 1 >= dimensaoHorizontal) return;
		carater = tela[x][y];
		tela[x][y] = " ";
		tela[x][y + 1] = carater;
		y += 1;
	}
	else if (direcao == CTRL_ESQ){
		if (y - 1 < 0) return;
		carater = tela[x][y];
		tela[x][y] = " ";
		tela[x][y - 1] = carater;
		y -= 1;
	}
	else if (direcao == CTRL_CIMA){
		carater = tela[x][y];
		tela[x][y] = " ";
		for (int i = x; i < dimensaoVertical; i++){
			if (tela[i][y] != " "){
				tela[i - 1][y] = carater;
				x = i - 1;
				escrito = true;
				return;
			}
		}

		if (!escrito){
			x = dimensaoVertical - 1;
			tela[x][y] = carater;
		}
	}
	else {
		if (x + 1 >= dimensaoVertical) return;
		else if (tela[x + 1][y] != " ") return;
		carater = tela[x][y];
		tela[x][y] = " ";
		tela[x + 1][y] = carater;
		x += 1;
	}
}

vector<string> criarAlfabeto(){
	string alfabeto = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
	vector<string> alfabetoCompleto;

	for (unsigned int i = 0; i < alfabeto.length(); i++) {
		stringstream aux;
		aux << alfabeto[i];
		alfabetoCompleto.push_back(aux.str());
	}

	return alfabetoCompleto;
}

vector<string> filtrarVetor(char primeiraLetra, int imin, int imax){
	vector<string> palavrasDeIndiceInferiorAoMidPoint;
	vector<string> palavrasDeIndiceSuperiorAoMidPoint;
	vector<string> aRetornar;
	// binary search

	while (imax >= imin) {
		int imid = imin + (imax - imin) / 2;

		if (dicionario[imid].at(0) == primeiraLetra) {
			int i = imid;

			/*
			*  Exemplo vetor { Axxx, Bas,bes,bis,bos,bos, Cxxx }
			*
			*  Palavra encontrada = bis
			*
			*  Metodo: Partindo do indice de "bis", subtrai-se uma em uma palavra, fazendo push_back() simultaneamente, até encontrar um indice em que a palavra nao começa com "b"
			*/

			while (i >= 0 && dicionario[i].at(0) == primeiraLetra){

				palavrasDeIndiceInferiorAoMidPoint.push_back(dicionario[i]);
				i--;
			}

			i = imid;

			while (i <= 4290 && dicionario[i].at(0) == primeiraLetra){

				palavrasDeIndiceSuperiorAoMidPoint.push_back(dicionario[i]);
				i++;
			}

			break;
		}
		else if (dicionario[imid].at(0) < primeiraLetra)
			imin = imid + 1;
		else
			imax = imid - 1;
	}

	/*
	*  De modo a facilitar o algoritmo de semelhança de strings, ordena-se as palavras alfabeticamente. No "dicionário" já se encontravam , contudo no passo anterior, a ordem reverteu-se
	*
	*
	*
	*/

	reverse(palavrasDeIndiceInferiorAoMidPoint.begin(), palavrasDeIndiceInferiorAoMidPoint.end());

	// push_back() para vetor final

	for (unsigned int i = 0; i < palavrasDeIndiceInferiorAoMidPoint.size(); i++){
		aRetornar.push_back(palavrasDeIndiceInferiorAoMidPoint[i]);
	}

	for (unsigned int i = 0; i < palavrasDeIndiceSuperiorAoMidPoint.size(); i++){
		aRetornar.push_back(palavrasDeIndiceSuperiorAoMidPoint[i]);
	}

	return aRetornar;
}

vector<size_t> algoritmoNaive(vector<vector<string>>& tela, string needle, string palheiro){
	for (size_t i = 0; i < palheiro.length(); i++)
		palheiro[i] += 32;

	vector<size_t> matches;

	if (needle.size() > palheiro.size())
		return matches;

	size_t needleSize = needle.size();
	size_t maximumIndex = palheiro.size() - needleSize;

	size_t needleIndex;
	for (size_t haystackIndex = 0; haystackIndex <= maximumIndex; haystackIndex++) {
		for (needleIndex = 0; needleIndex < needleSize && needle[needleIndex] == palheiro[haystackIndex + needleIndex]; needleIndex++);

		if (needleIndex == needleSize) {
			matches.push_back(haystackIndex);
			matches.push_back(haystackIndex + needleIndex - 1);
			return matches;
		}
	}

	return matches;
}

bool linhaCheia(vector<vector<string>> tela, string& linha, int& numeroLinhaCompleta){
	for (int i = 0; i < dimensaoVertical; i++) {
		numeroLinhaCompleta = i;
		linha = "";

		for (int j = 0; j < dimensaoHorizontal; j++) {
			if (tela[i][j] != " " && j == dimensaoHorizontal - 1) {
				linha += tela[i][j];
				return true;
			}
			else if (tela[i][j] == " ")
				break;
			else
				linha += tela[i][j];
		}
	}

	return false;
}

void menuOpcoes(){
	system("cls");

	cout << "********************************************************************************\n"
		<< "*                                 Controls                                     *\n"
		<< "********************************************************************************\n\n\n";

	cout << "\n  > Special key - VK_UP     \n\n"
		"  > Right key   - VK_RIGHT  \n\n"
		"  > Left key    - VK_LEFT   \n\n"
		"  > Down key    - VK_DOWN   \n";

	cout << "\n\n\n\n\n          Press ENTER to change them or ESC to return to the main menu\n\n";

	char tecla = _getch();

	switch (tecla)
	{
	case 13:
		alterarControlos();
		break;
	case 27:
		return;
	}

}

void alterarControlos(){
	system("cls");

	cout << "********************************************************************************\n"
		<< "*                                 Controls                                     *\n"
		<< "********************************************************************************\n\n\n";

	cout << "\n\n                           >>   Special key   <<     \n";
	cout << "\n\n\n\n\n\n\n\n                           Press the key you want\n\n";
	CTRL_CIMA = _getch();

	system("cls");

	cout << "********************************************************************************\n"
		<< "*                                 Controls                                     *\n"
		<< "********************************************************************************\n\n\n";

	cout << "\n\n                               >>   Right key   <<     \n";
	cout << "\n\n\n\n\n\n\n\n                           Press the key you want\n\n";
	CTRL_DIR = _getch();

	system("cls");

	cout << "********************************************************************************\n"
		<< "*                                 Controls                                     *\n"
		<< "********************************************************************************\n\n\n";

	cout << "\n\n                               >>   Left key   <<     \n";
	cout << "\n\n\n\n\n\n\n\n                           Press the key you want\n\n";
	CTRL_ESQ = _getch();

	system("cls");

	cout << "********************************************************************************\n"
		<< "*                                 Controls                                     *\n"
		<< "********************************************************************************\n\n\n";

	cout << "\n\n                               >>   Down key   <<     \n";
	cout << "\n\n\n\n\n\n\n\n                           Press the key you want\n\n";
	CTRL_BAIXO = _getch();
}

void alterarDimensoes(){
	system("cls");
	string stringTemp;
	cout << "********************************************************************************\n"
		<< "*                                 Dimension Options                            *\n"
		<< "********************************************************************************\n\n\n\n\n";

	cout << "  > Vertical dimension: ";
	getline(cin, stringTemp);
	dimensaoVertical = stoi(stringTemp);
	cout << endl << "\n  > Horizontal dimension: ";
	getline(cin, stringTemp);
	dimensaoHorizontal = stoi(stringTemp);
}

void menuInstrucoes(){

	system("cls");

	cout << "********************************************************************************\n"
		<< "*                               Instructions                                   *\n"
		<< "********************************************************************************\n\n\n";



	cout << "  This game  is an adapted version from a  well known game TETRIS developed by\n"
		<< "  Alexey Pajitnov, Dmitry Pavlovsky and Vadim Gerasimov whitch was released in\n"
		<< "  June 1984.\n"
		<< "  Our version consists of stacking letters  that come down the screen in order\n"
		<< "  to complete horizontal lines. When a line is formed, if a word is found, the\n"
		<< "  player wins certain points and the word is erased.\n"
		<< "  The game ends when the stack of letters reach the top of the screen.\n";

	cout << "\n\n\n\n\n                     Press ENTER  to return to the main menu\n\n";

	char tecla;

	do {
		tecla = _getch();
	} while (tecla != 13);
}

string convertInt(int number)
{
	stringstream ss;
	ss << number;
	return ss.str();
}

void menuOptions(){
	system("cls");
	cout << "********************************************************************************\n"
		<< "*                                 Options                                       *\n"
		<< "********************************************************************************\n\n\n\n\n"
		<< "  1 > Change dimensions\n\n"
		<< "  2 > Change keys\n\n"
		<< "  3 > Exit \n" << endl;

	char tecla;

	do {
		tecla = _getch();

		if (tecla == '1'){
			alterarDimensoes();
			break;
		}
		else if (tecla == '2'){
			menuOpcoes();
			break;
		}
		else if (tecla == '3')
			return;
	} while (true);
}
