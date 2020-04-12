#include "Vizinhancas.h"

Vizinhancas::Vizinhancas()
{
    //ctor
    I = 0;
    J = 0;
    melhorCusto = __DBL_MAX__;
	auto time = std::chrono::system_clock::now();
	timeSwap = time - time;
	timeReInsertion = time - time;
	time2Opt = time - time;
}

void Vizinhancas::swap(double **distancia, int dimension, std::vector<int> &s, std::vector<std::vector<std::vector<double>>> &reOpt,
double melhorCusto) {
	auto inicio = std::chrono::system_clock::now();
	int melhorI = 0, melhorJ = 0, tamanho = s.size();
	double custo;
	
	for (int i = 1; i < tamanho - 1; i++) {
		for (int j = i + 1; j < tamanho - 1; j++) {
			if (j - i > 1) {
				custo = reOpt[1][0][i - 1] + reOpt[2][j][j]*(reOpt[0][0][i - 1] + distancia[s[i - 1]][s[j]]) +
						reOpt[2][i + 1][j - 1]*(reOpt[0][0][i - 1] + distancia[s[i - 1]][s[j]] + distancia[s[j]][s[i + 1]]) +
						reOpt[1][i + 1][j - 1] + reOpt[2][i][i]*(reOpt[0][0][i - 1] + distancia[s[i - 1]][s[j]] +
						distancia[s[j]][s[i + 1]] + reOpt[0][i + 1][j - 1] + distancia[s[j - 1]][s[i]]) +
						reOpt[2][j + 1][dimension]*(reOpt[0][0][i - 1] + distancia[s[i - 1]][s[j]] + distancia[s[j]][s[i + 1]] +
						reOpt[0][i + 1][j - 1] + distancia[s[j - 1]][s[i]] + distancia[s[i]][s[j + 1]]) +
						reOpt[1][j + 1][dimension];
			}
			else {
				custo = reOpt[1][0][i - 1] + reOpt[2][j][j]*(reOpt[0][0][i - 1] + distancia[s[i - 1]][s[j]]) +
						reOpt[2][i][i]*(reOpt[0][0][i - 1] + distancia[s[i - 1]][s[j]] + distancia[s[j]][s[i]]) +
						reOpt[2][j + 1][dimension]*(reOpt[0][0][i - 1] + distancia[s[i - 1]][s[j]] + distancia[s[j]][s[i]] +
						distancia[s[i]][s[j + 1]]) + reOpt[1][j + 1][dimension];

			}

			if (custo < melhorCusto) {
				melhorCusto = custo;
				melhorI = i;
				melhorJ = j;
			}
		}
	}

	I = melhorI;
	J = melhorJ;
	this->melhorCusto = melhorCusto;
	
	auto fim = std::chrono::system_clock::now();
	timeSwap += fim - inicio;
}

void Vizinhancas::n_re_insertion (double **distancia, int dimension, std::vector<int> &s,
std::vector<std::vector<std::vector<double>>> &reOpt, int n, double melhorCusto) {
	auto inicio = std::chrono::system_clock::now();
	int melhorI = 0, melhorJ = 0, tamanho = s.size();
	double custo;
	
	if (n == 1) {
		for (int i = 1; i < tamanho - 3; i++) {
			for (int j = i + 2; j < tamanho - 1; j++) {
				custo = reOpt[1][0][i - 1] + reOpt[2][i + 1][j]*(reOpt[0][0][i - 1] + distancia[s[i - 1]][s[i + 1]]) +
						reOpt[1][i + 1][j] + reOpt[2][i][i]*(reOpt[0][0][i - 1] + distancia[s[i - 1]][s[i + 1]] +
						reOpt[0][i + 1][j] + distancia[s[j]][s[i]]) + reOpt[2][j + 1][dimension]*(reOpt[0][0][i - 1] +
						distancia[s[i - 1]][s[i + 1]] + reOpt[0][i + 1][j] + distancia[s[j]][s[i]] + distancia[s[i]][s[j + 1]]) +
						reOpt[1][j + 1][dimension];

				if (custo < melhorCusto) {
					melhorCusto = custo;
					melhorI = i;
					melhorJ = j;
				}
			}
		}

		for (int i = 3; i < tamanho - 1; i++) {
			for (int j = i - 2; j > 0; j--) {
				custo = reOpt[1][0][j - 1] + reOpt[2][i][i]*(reOpt[0][0][j - 1] + distancia[s[j - 1]][s[i]]) +
						reOpt[2][j][i - 1]*(reOpt[0][0][j - 1] + distancia[s[j - 1]][s[i]] + distancia[s[i]][s[j]]) +
						reOpt[1][j][i - 1] + reOpt[2][i + 1][dimension]*(reOpt[0][0][j - 1] + distancia[s[j - 1]][s[i]] +
						distancia[s[i]][s[j]] + reOpt[0][j][i - 1] + distancia[s[i - 1]][s[i + 1]]) + reOpt[1][i + 1][dimension];

				if (custo < melhorCusto) {
					melhorCusto = custo;
					melhorI = i;
					melhorJ = j;
				}
			}
		}
	}
	else {
		for (int i = 1; i < tamanho - n - 1; i++) {
			for (int j = i + 1; j < tamanho - n; j++) {
				custo = reOpt[1][0][i - 1] + reOpt[2][i + n][j + n - 1]*(reOpt[0][0][i - 1] + distancia[s[i - 1]][s[i + n]]) +
						reOpt[1][i + n][j + n - 1] + reOpt[2][i][i + n - 1]*(reOpt[0][0][i - 1] + distancia[s[i - 1]][s[i + n]] +
						reOpt[0][i + n][j + n - 1] + distancia[s[j + n - 1]][s[i]]) + reOpt[1][i][i + n - 1] +
						reOpt[2][j + n][dimension]*(reOpt[0][0][i - 1] + distancia[s[i - 1]][s[i + n]] +
						reOpt[0][i + n][j + n - 1] + distancia[s[j + n - 1]][s[i]] + reOpt[0][i][i + n - 1] +
						distancia[s[i + n - 1]][s[j + n]]) + reOpt[1][j + n][dimension];

				if (custo < melhorCusto) {
					melhorCusto = custo;
					melhorI = i;
					melhorJ = j;
				}
			}
		}

		for (int i = n; i < tamanho - n; i++) {
			for (int j = i - 1; j > 0; j--) {
				custo = reOpt[1][0][j - 1] + reOpt[2][i][i + n - 1]*(reOpt[0][0][j - 1] + distancia[s[j - 1]][s[i]]) +
						reOpt[1][i][i + n - 1] + reOpt[2][j][i - 1]*(reOpt[0][0][j - 1] + distancia[s[j - 1]][s[i]] +
						reOpt[0][i][i + n - 1] + distancia[s[i + n - 1]][s[j]]) + reOpt[1][j][i - 1] +
						reOpt[2][i + n][dimension]*(reOpt[0][0][j - 1] + distancia[s[j - 1]][s[i]] + reOpt[0][i][i + n - 1] +
						distancia[s[i + n - 1]][s[j]] + reOpt[0][j][i - 1] + distancia[s[i - 1]][s[i + n]]) +
						reOpt[1][i + n][dimension];

				if (custo < melhorCusto) {
					melhorCusto = custo;
					melhorI = i;
					melhorJ = j;
				}
			}
		}
	}

	I = melhorI;
	J = melhorJ;
	this->melhorCusto = melhorCusto;

	auto fim = std::chrono::system_clock::now();
	timeReInsertion += fim - inicio;
}

void Vizinhancas::opt_2(double **distancia, int dimension, std::vector<int> &s, std::vector<std::vector<std::vector<double>>>
&reOpt, double melhorCusto) {
	auto inicio = std::chrono::system_clock::now();
	int melhorI = 0, melhorJ = 0, tamanho = s.size();
	double custo;
	
	for (int i = 1; i < tamanho - 4; i++) {
		for (int j = i + 3; j < tamanho - 1; j++) {
			custo = reOpt[1][0][i - 1] + reOpt[2][j][i]*(reOpt[0][0][i - 1] + distancia[s[i - 1]][s[j]]) + reOpt[1][j][i] +
					reOpt[2][j + 1][dimension]*(reOpt[0][0][i - 1] + distancia[s[i - 1]][s[j]] + reOpt[0][j][i] +
					distancia[s[i]][s[j + 1]]) + reOpt[1][j + 1][dimension];
			
			if (custo < melhorCusto) {
				melhorCusto = custo;
				melhorI = i;
				melhorJ = j;
			}
		}
	}

	I = melhorI;
	J = melhorJ;
	this->melhorCusto = melhorCusto;

	auto fim = std::chrono::system_clock::now();
	time2Opt += fim - inicio;
}

void Vizinhancas::preencheMovimentos(std::vector<int> &movimentosDeVizinhanca)
{
	movimentosDeVizinhanca.clear();
	movimentosDeVizinhanca.push_back(0); //swap
	movimentosDeVizinhanca.push_back(1); //re-insertion
	movimentosDeVizinhanca.push_back(2); //or-opt-2
	movimentosDeVizinhanca.push_back(3); //or-opt-3
	movimentosDeVizinhanca.push_back(4); //2-opt
}

int Vizinhancas::getI()
{
    return I;
}

int Vizinhancas::getJ()
{
    return J;
}

double Vizinhancas::getMelhorCusto()
{
    return melhorCusto;
}

std::chrono::duration<double> Vizinhancas::getTimeSwap()
{
	return timeSwap;
}
std::chrono::duration<double> Vizinhancas::getTimeReInsertion()
{
	return timeReInsertion;
}
std::chrono::duration<double> Vizinhancas::getTime2Opt()
{
	return time2Opt;
}