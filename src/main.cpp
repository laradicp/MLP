#include "readData.h"
#include <fstream>
#include <iostream>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <vector>
#include <algorithm>
#include <ctime>
#include <chrono>
#include <float.h>

using namespace std;

double ** distancia; // matriz de adjacencia
int dimension;

typedef struct {
	double custo;
	int I;
} tInsercao;

typedef struct {
	int I;
	int J;
	double melhorCusto;
} tVizinhanca;

void PreencheMovimentos(vector<int> &movimentosDeVizinhanca) {
	movimentosDeVizinhanca.clear();
	movimentosDeVizinhanca.push_back(0); //swap
	movimentosDeVizinhanca.push_back(1); //re-insertion
	movimentosDeVizinhanca.push_back(2); //or-opt-2
	movimentosDeVizinhanca.push_back(3); //or-opt-3
	movimentosDeVizinhanca.push_back(4); //2-opt
}

void Swap(tVizinhanca *vizinhanca, double **distancia, vector<int> &s, vector<vector<vector<long double>>> &reOpt, double melhorCusto) {
	int melhorI = 0, melhorJ = 0, tamanho = s.size();
	double custo;
	tVizinhanca *p;
	p = vizinhanca;

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

	p->I = melhorI;
	p->J = melhorJ;
	p->melhorCusto = melhorCusto;
}

void N_Re_insertion (tVizinhanca *vizinhanca, double **distancia, vector<int> &s, vector<vector<vector<long double>>> &reOpt, int n, double melhorCusto) {
	int melhorI = 0, melhorJ = 0, tamanho = s.size();
	double custo;
	tVizinhanca *p;
	p = vizinhanca;
	
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
				custo = reOpt[1][0][i - 1] + reOpt[2][i + 1][j]*(reOpt[0][0][i - 1] + distancia[s[i - 1]][s[i + 1]]) +
						reOpt[1][i + 1][j] + reOpt[2][i][i]*(reOpt[0][0][i - 1] + distancia[s[i - 1]][s[i + 1]] +
						reOpt[0][i + 1][j] + distancia[s[j]][s[i]]) + reOpt[1][i][i + n - 1] +
						reOpt[2][j + 1][dimension]*(reOpt[0][0][i - 1] + distancia[s[i - 1]][s[i + 1]] +
						reOpt[0][i + 1][j] + distancia[s[j]][s[i]] + distancia[s[i]][s[j + 1]]) + reOpt[1][j + 1][dimension];

				if (custo < melhorCusto) {
					melhorCusto = custo;
					melhorI = i;
					melhorJ = j;
				}
			}
		}

		for (int i = n; i < tamanho - n; i++) {
			for (int j = i - 1; j > 0; j--) {
				custo = reOpt[1][0][j - 1] + reOpt[2][i][i]*(reOpt[0][0][j - 1] + distancia[s[j - 1]][s[i]]) +
						reOpt[1][i][i + n - 1] + reOpt[2][j][i - 1]*(reOpt[0][0][j - 1] + distancia[s[j - 1]][s[i]] +
						reOpt[1][i][i + n - 1] + distancia[s[i]][s[j]]) + reOpt[1][j][i - 1] +
						reOpt[2][i + 1][dimension]*(reOpt[0][0][j - 1] + distancia[s[j - 1]][s[i]] + reOpt[1][i][i + n - 1] +
						distancia[s[i]][s[j]] + reOpt[0][j][i - 1] + distancia[s[i - 1]][s[i + 1]]) + reOpt[1][i + 1][dimension];

				if (custo < melhorCusto) {
					melhorCusto = custo;
					melhorI = i;
					melhorJ = j;
				}
			}
		}
	}

	p->I = melhorI;
	p->J = melhorJ;
	p->melhorCusto = melhorCusto;
}

void Opt_2(tVizinhanca *vizinhanca, double **distancia, vector<int> &s, vector<vector<vector<long double>>> &reOpt, double melhorCusto) {
	int melhorI = 0, melhorJ = 0, tamanho = s.size();
	double custo;
	tVizinhanca *p;
	p = vizinhanca;

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

	p->I = melhorI;
	p->J = melhorJ;
	p->melhorCusto = melhorCusto;
}

void reOptPreProcessing(double **distancia, vector<int> &s, vector<vector<vector<long double>>> &reOpt, int begin)
{
	//T: duracao (0)
	//C: custo (1)
	//W: numero de cidades (desconsidera depositos) (2)
	
	for (unsigned int i = begin; i < dimension; i++)
	{
		reOpt[0][i][i] = 0; //duracao igual a 0 do percurso de uma cidade para ela mesma
		reOpt[1][i][i] = 0; //custo igual a 0 do percurso de uma cidade para ela mesma

		if (s[i] == 0)
			reOpt[2][i][i] = 0; //deposito
		else 
			reOpt[2][i][i] = 1; //cidade
	}
	reOpt[2][dimension][dimension] = 1;

	for (unsigned int t = 2; t < dimension + 2; t++) //t varia de 2 a s.size()
	{
		for (unsigned int i = 0, j; (i < begin)&&(i < dimension + 2 - t); i++) //i varia de 0 a begin - 1
		{
			j = i + t - 1; //i e j sao as extremidades de um percurso de tamanho t
			if (j < begin) continue;

			reOpt[2][i][j] = reOpt[2][i][j - 1] + reOpt[2][j][j]; //comeca a preencher com i = 0 e j = 1, logo j - 1 == 0 == i
			//progressivamente se estabelecem as subsequencias
			reOpt[0][i][j] = reOpt[0][i][j - 1] + distancia[s[j - 1]][s[j]];
			reOpt[1][i][j] = reOpt[1][i][j - 1] + reOpt[2][j][j] * (reOpt[0][i][j - 1] + distancia[s[j - 1]][s[j]]) + reOpt[1][j][j];

			reOpt[2][j][i] = reOpt[2][i][j];
			reOpt[0][j][i] = reOpt[0][i][j];

			reOpt[1][j][i] = reOpt[1][j - 1][i] + reOpt[2][j - 1][i] * (reOpt[0][j][j] + distancia[s[j]][s[j - 1]]) + reOpt[1][j][j];
		}

		for (unsigned int i = begin, j; i < dimension + 2 - t; i++) //i varia de begin a s.size() - t
		{
			j = i + t - 1; //i e j sao as extremidades de um percurso de tamanho t

			reOpt[2][i][j] = reOpt[2][i][j - 1] + reOpt[2][j][j]; //comeca a preencher com i = 0 e j = 1, logo j - 1 == 0 == i
			//progressivamente se estabelecem as subsequencias
			reOpt[0][i][j] = reOpt[0][i][j - 1] + distancia[s[j - 1]][s[j]];
			reOpt[1][i][j] = reOpt[1][i][j - 1] + reOpt[2][j][j] * (reOpt[0][i][j - 1] + distancia[s[j - 1]][s[j]]) + reOpt[1][j][j];

			reOpt[2][j][i] = reOpt[2][i][j];
			reOpt[0][j][i] = reOpt[0][i][j];

			reOpt[1][j][i] = reOpt[1][j - 1][i] + reOpt[2][j - 1][i] * (reOpt[0][j][j] + distancia[s[j]][s[j - 1]]) + reOpt[1][j][j];
		}
	}
}

int main(int argc, char** argv) {
	readData(argc, argv, &dimension, &distancia);

	srand(time(NULL));

	auto inicio = chrono::system_clock::now();

	vector<int> s, melhorSAtual, melhorS;
	std::vector<std::vector<std::vector<long double>>> reOpt(3, std::vector<std::vector<long double>>(dimension + 1, std::vector<long double>(dimension + 1, 0)));
	double melhorCusto = DBL_MAX;
	int Iils;

	if (dimension > 150) Iils = dimension/2;
	else Iils = dimension;

	for (int cont = 0; cont < 50; cont++) {
		double melhorCustoAtual = DBL_MAX;
		
		vector<int> listaDeCandidatos;

		for (int i = 1; i < dimension; i++) {
			listaDeCandidatos.push_back(i);
		}
		
		s.clear();
		s.push_back(0);

		for (int i = 0; listaDeCandidatos.size(); i++) {
			tInsercao melhorVizinho = {DBL_MAX, 0};
			int tamanhoListaDeCandidatos = listaDeCandidatos.size();

			for (int j = 0; j < tamanhoListaDeCandidatos; j++) {
				if (melhorVizinho.custo < distancia[s[i]][listaDeCandidatos[j]])
				{
					melhorVizinho.custo = distancia[s[i]][listaDeCandidatos[j]];
					melhorVizinho.I = j;
				}

				s.push_back(listaDeCandidatos[melhorVizinho.I]);
				listaDeCandidatos.erase(listaDeCandidatos.begin() + melhorVizinho.I);
			}
		}

		s.push_back(s[0]);

		double custo = 0;
		for (int i = 0, n = dimension; i < dimension; i++, n--) {
			custo += n*distancia[s[i]][s[i + 1]];
		}

		reOptPreProcessing(distancia, s, reOpt, 0);

		for (int iterILS = 0; iterILS < Iils; iterILS++) {
			vector<int> movimentosDeVizinhanca;
			PreencheMovimentos(movimentosDeVizinhanca);

			tVizinhanca vizinhanca;
			while (movimentosDeVizinhanca.size() > 0) {
				int vizinho = rand() % movimentosDeVizinhanca.size(), aux;

				switch (movimentosDeVizinhanca[vizinho]) {
				case 0:
					Swap(&vizinhanca, distancia, s, reOpt, custo);

					if (vizinhanca.melhorCusto < custo) {
						int aux = s[vizinhanca.I];
						s[vizinhanca.I] = s[vizinhanca.J];
						s[vizinhanca.J] = aux;

						custo = vizinhanca.melhorCusto;

						reOptPreProcessing(distancia, s, reOpt, vizinhanca.I);
						PreencheMovimentos(movimentosDeVizinhanca);
					}
					else movimentosDeVizinhanca.erase(movimentosDeVizinhanca.begin() + vizinho);

					break;

				case 1:
					N_Re_insertion (&vizinhanca, distancia, s, reOpt, 1, custo);

					if (vizinhanca.melhorCusto < custo) {
						if (vizinhanca.J > vizinhanca.I) {
							s.insert(s.begin() + vizinhanca.J + 1, s[vizinhanca.I]);
							s.erase(s.begin() + vizinhanca.I);
							
							reOptPreProcessing(distancia, s, reOpt, vizinhanca.I);
						}
						else {
							s.insert(s.begin() + vizinhanca.J, s[vizinhanca.I]);
							s.erase(s.begin() + vizinhanca.I + 1);

							reOptPreProcessing(distancia, s, reOpt, vizinhanca.J);
						}

						custo = vizinhanca.melhorCusto;
						
						PreencheMovimentos(movimentosDeVizinhanca);
					}
					else movimentosDeVizinhanca.erase(movimentosDeVizinhanca.begin() + vizinho);

					break;

				case 2:
					N_Re_insertion (&vizinhanca, distancia, s, reOpt, 2, custo);

					if (vizinhanca.melhorCusto < custo) {
						if (vizinhanca.J > vizinhanca.I) {
							s.insert(s.begin() + vizinhanca.J + 2, s[vizinhanca.I + 1]);
							s.insert(s.begin() + vizinhanca.J + 2, s[vizinhanca.I]);
							s.erase(s.begin() + vizinhanca.I + 1);
							s.erase(s.begin() + vizinhanca.I);

							reOptPreProcessing(distancia, s, reOpt, vizinhanca.I);
						}
						else {
							s.insert(s.begin() + vizinhanca.J, s[vizinhanca.I + 1]);
							s.insert(s.begin() + vizinhanca.J, s[vizinhanca.I + 1]);
							s.erase(s.begin() + vizinhanca.I + 3);
							s.erase(s.begin() + vizinhanca.I + 2);

							reOptPreProcessing(distancia, s, reOpt, vizinhanca.J);
						}

						custo = vizinhanca.melhorCusto;

						PreencheMovimentos(movimentosDeVizinhanca);
					}
					else movimentosDeVizinhanca.erase(movimentosDeVizinhanca.begin() + vizinho);

					break;

				case 3:
					N_Re_insertion (&vizinhanca, distancia, s, reOpt, 3, custo);

					if (vizinhanca.melhorCusto < custo) {
						if (vizinhanca.J > vizinhanca.I) {
							s.insert(s.begin() + vizinhanca.J + 3, s[vizinhanca.I + 2]);
							s.insert(s.begin() + vizinhanca.J + 3, s[vizinhanca.I + 1]);
							s.insert(s.begin() + vizinhanca.J + 3, s[vizinhanca.I]);
							s.erase(s.begin() + vizinhanca.I + 2);
							s.erase(s.begin() + vizinhanca.I + 1);
							s.erase(s.begin() + vizinhanca.I);

							reOptPreProcessing(distancia, s, reOpt, vizinhanca.I);
						}
						else {
							s.insert(s.begin() + vizinhanca.J, s[vizinhanca.I + 2]);
							s.insert(s.begin() + vizinhanca.J, s[vizinhanca.I + 2]);
							s.insert(s.begin() + vizinhanca.J, s[vizinhanca.I + 2]);
							s.erase(s.begin() + vizinhanca.I + 5);
							s.erase(s.begin() + vizinhanca.I + 4);
							s.erase(s.begin() + vizinhanca.I + 3);

							reOptPreProcessing(distancia, s, reOpt, vizinhanca.J);
						}

						custo = vizinhanca.melhorCusto;

						PreencheMovimentos(movimentosDeVizinhanca);
					}
					else movimentosDeVizinhanca.erase(movimentosDeVizinhanca.begin() + vizinho);

					break;

				case 4:
					Opt_2(&vizinhanca, distancia, s, reOpt, custo);
					if (vizinhanca.melhorCusto < custo) {
						for (int k = 0; k < (vizinhanca.J - vizinhanca.I + 1) / 2; k++) { //para nao excluir o i
							int aux = s[vizinhanca.I + k];
							s[vizinhanca.I + k] = s[vizinhanca.J - k];
							s[vizinhanca.J - k] = aux;
						}

						custo = vizinhanca.melhorCusto;

						reOptPreProcessing(distancia, s, reOpt, vizinhanca.I);
						PreencheMovimentos(movimentosDeVizinhanca);
					}
					else movimentosDeVizinhanca.erase(movimentosDeVizinhanca.begin() + vizinho);
				}
			}

			if (custo < melhorCustoAtual) {
				melhorSAtual = s;
				melhorCustoAtual = custo;
				iterILS = -1;
			} else {
				s = melhorSAtual; 
				custo = melhorCustoAtual;
			}

			//perturbacao
			int ar[4];
			
			for (int i = 0; i < 4; i++){
				ar[i] = rand() % (dimension - 4) + 1;
			}
			sort(ar, ar + 4);

			for (int i = 0; i < 3; i++){
				if (ar[i + 1] <= ar[i])
					ar[i + 1] = ar[i] + 1;
			}
			
			if (ar[1] - ar[0] > dimension / 10 - 1)
				ar[1] = ar[0] + dimension / 10 - 1;
		
			if (ar[3] - ar[2] > dimension / 10 - 1)
				ar[3] = ar[2] + dimension / 10 - 1;
			
			int tamanho1 = ar[1] - ar[0] + 1, tamanho2 = ar[3] - ar[2] + 1;

			for (int i = tamanho1 - 1; i >= 0; i--) {
				s.insert(s.begin() + ar[2], s[ar[0] + i]);
				s.erase(s.begin() + ar[0] + i);
			}

			for (int i = 0; i < tamanho2; i++) {
				s.insert(s.begin() + ar[0], s[ar[2] + i]);
				s.erase(s.begin() + ar[2] + i + 1);
			}

			reOptPreProcessing(distancia, s, reOpt, 0);
			custo = reOpt[1][0][dimension];
		}

		if (melhorCustoAtual < melhorCusto) {
			melhorCusto = melhorCustoAtual;
			melhorS = melhorSAtual;
		}
	}

	auto fim = chrono::system_clock::now();
    chrono::duration<double> tempo = fim - inicio;

	for (int i = 0; i < melhorS.size(); i++) {
		printf("%d  ", melhorS[i] + 1);
	}
	
	printf("\nCusto: %lf\n", melhorCusto);
	printf("Tempo: %lf\n", tempo);

	return 0;
}