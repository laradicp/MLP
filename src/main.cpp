#include "readData.h"
#include "ILS.h"

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
	vector<vector<vector<double>>> reOpt(3, vector<vector<double>>(dimension + 1,
	vector<double>(dimension + 1, 0)));
	double melhorCusto = __DBL_MAX__;
	ILS ils = ILS(dimension);
	int numEscapes = 0, numVezes = 0, numMelhorias = 0, ultimaMelhoria;

	for (int cont = 0; cont < 25; cont++) {
		double melhorCustoAtual = __DBL_MAX__;

		ils.construcao(dimension, distancia);

		ils.reOptPreProcessing(dimension, distancia, reOpt, 0);
		double custo = reOpt[1][0][dimension]; 

		for (int iterILS = 0; iterILS < ils.getI(); iterILS++) {
			numVezes++;

			ils.rvnd(dimension, distancia, reOpt, &custo);

			if (custo < melhorCustoAtual) {
				melhorSAtual = ils.getS();
				melhorCustoAtual = custo;
				iterILS = -1;
				numEscapes++;
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
			numMelhorias++;
			ultimaMelhoria = cont + 1;
		}
	}
	
	auto fim = chrono::system_clock::now();
    chrono::duration<double> tempo = fim - inicio;

	for (int i = 0; i < melhorS.size(); i++) {
		cout << melhorS[i] + 1 << "  ";
	}
	
	cout << endl << "Custo: " << melhorCusto << endl;
	cout << "Tempo: " << tempo.count() << endl;
	cout << endl << "Swap: " << ils.vizinhancas.getTimeSwap().count() << endl;
	cout << "Re-insertion: " << ils.vizinhancas.getTimeReInsertion().count() << endl;
	cout << "2-opt: " << ils.vizinhancas.getTime2Opt().count() << endl;
	cout << endl << "Pre-processing: " << ils.getTimePreProcessing().count() << endl;
	cout << endl << "Numero total de escapes de otimos locais: " << numEscapes << endl;
	cout << "Numero medio de escapes de otimos locais: " << (double)numEscapes/numVezes << endl;
	cout << endl << "Numero de melhorias: " << numMelhorias << endl;
	cout << "Ultima melhoria: " << ultimaMelhoria << "/25" << endl;

	return 0;
}