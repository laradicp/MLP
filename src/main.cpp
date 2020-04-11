#include "readData.h"
#include "ILS.h"
#include <ctime>
#include <chrono>

using namespace std;

double ** distancia; // matriz de adjacencia
int dimension;

int main(int argc, char** argv) {
	readData(argc, argv, &dimension, &distancia);

	long seed = time(NULL);
	cout << "Seed: " << seed << endl;
	srand(seed);

	auto inicio = chrono::system_clock::now();

	vector<int> melhorSAtual, melhorS;
	std::vector<std::vector<std::vector<double>>> reOpt(3, std::vector<std::vector<double>>(dimension + 1, std::vector<double>(dimension + 1, 0)));
	double melhorCusto = __DBL_MAX__;
	ILS ils = ILS(dimension);
	Vizinhancas vizinhancas = Vizinhancas();

	for (int cont = 0; cont < 10; cont++) {
		double melhorCustoAtual = __DBL_MAX__;

		ils.construcao(dimension, distancia);

		ils.reOptPreProcessing(dimension, distancia, reOpt, 0);
		double custo = reOpt[1][0][dimension]; 

		for (int iterILS = 0; iterILS < ils.getI(); iterILS++) {
			vector<int> movimentosDeVizinhanca;
			vizinhancas.preencheMovimentos(movimentosDeVizinhanca);

			ils.rvnd(dimension, distancia, movimentosDeVizinhanca, vizinhancas, reOpt, &custo);

			if (custo < melhorCustoAtual) {
				melhorSAtual = ils.getS();
				melhorCustoAtual = custo;
				iterILS = -1;
			} else {
				ils.setS(melhorSAtual); 
				custo = melhorCustoAtual;
			}

			ils.perturbacao(dimension);
			
			ils.reOptPreProcessing(dimension, distancia, reOpt, 0);
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