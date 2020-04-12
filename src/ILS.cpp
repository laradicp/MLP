#include "ILS.h"

ILS::ILS(int dimension)
{
    //ctor
    if (dimension > 100)
        I = 100;
	else
        I = dimension;

    auto time = std::chrono::system_clock::now();
    timePreProcessing = time - time;
}

int ILS::getI()
{
    return I;
}

void ILS::setS(std::vector<int> s)
{
    this->s = s;
}

std::vector<int> ILS::getS()
{
    return s;
}

void ILS::construcao(int dimension, double ** distancia)
{
    std::vector<int> listaDeCandidatos;
    
    for (int i = 1; i < dimension; i++) {
        listaDeCandidatos.push_back(i);
    }
    
    s.clear();
    s.push_back(0);

    for (int i = 0; listaDeCandidatos.size(); i++) {
        tInsercao melhorVizinho = {__DBL_MAX__, 0};
        int tamanhoListaDeCandidatos = listaDeCandidatos.size();

        for (int j = 0; j < tamanhoListaDeCandidatos; j++) {
            if (melhorVizinho.custo < distancia[s[i]][listaDeCandidatos[j]])
            {
                melhorVizinho.custo = distancia[s[i]][listaDeCandidatos[j]];
                melhorVizinho.I = j;
            }
        }

        s.push_back(listaDeCandidatos[melhorVizinho.I]);
        listaDeCandidatos.erase(listaDeCandidatos.begin() + melhorVizinho.I);
    }

    s.push_back(s[0]);
}

void ILS::reOptPreProcessing(int dimension, double **distancia, std::vector<std::vector<std::vector<double>>> &reOpt, int begin)
{
    auto inicio = std::chrono::system_clock::now();
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
		if (begin > dimension + 2 - t)
			for (unsigned int i = 0, j; i < dimension + 2 - t; i++) //i varia de 0 a s.size() - t
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
		else
		{
			for (unsigned int i = 0, j; i < begin; i++) //i varia de 0 a begin - 1
			{
				j = i + t - 1; //i e j sao as extremidades de um percurso de tamanho t
				if ((j < begin)) continue;

				reOpt[2][i][j] = reOpt[2][i][j - 1] + reOpt[2][j][j]; //comeca a preencher com i = 0 e j = 1, logo j - 1 == 0 == i
				//progressivamente se estabelecem as subsequencias
				reOpt[0][i][j] = reOpt[0][i][j - 1] + distancia[s[j - 1]][s[j]];
				reOpt[1][i][j] = reOpt[1][i][j - 1] + reOpt[2][j][j] * (reOpt[0][i][j - 1] + distancia[s[j - 1]][s[j]]) + reOpt[1][j][j];

				reOpt[2][j][i] = reOpt[2][i][j];
				reOpt[0][j][i] = reOpt[0][i][j];

				reOpt[1][j][i] = reOpt[1][j - 1][i] + reOpt[2][j - 1][i] * (reOpt[0][j][j] + distancia[s[j]][s[j - 1]]) + reOpt[1][j][j];
				
				// cout << i << endl;
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
    auto fim = std::chrono::system_clock::now();
    timePreProcessing += fim - inicio;
}

void ILS::rvnd(int dimension, double ** distancia, std::vector<std::vector<std::vector<double>>> &reOpt, double *custo)
{
    std::vector<int> movimentosDeVizinhanca;
    vizinhancas.preencheMovimentos(movimentosDeVizinhanca);

    while (movimentosDeVizinhanca.size() > 0) {
        int vizinho = rand() % movimentosDeVizinhanca.size(), aux;

        switch (movimentosDeVizinhanca[vizinho]) {
        case 0:
            vizinhancas.swap(distancia, dimension, s, reOpt, *custo);

            if (vizinhancas.getMelhorCusto() < *custo) {
                int aux = s[vizinhancas.getI()];
                s[vizinhancas.getI()] = s[vizinhancas.getJ()];
                s[vizinhancas.getJ()] = aux;

                *custo = vizinhancas.getMelhorCusto();
                
                reOptPreProcessing(dimension, distancia, reOpt, vizinhancas.getI());
                vizinhancas.preencheMovimentos(movimentosDeVizinhanca);
            }
            else movimentosDeVizinhanca.erase(movimentosDeVizinhanca.begin() + vizinho);

            break;

        case 1:
            vizinhancas.n_re_insertion(distancia, dimension, s, reOpt, 1, *custo);

            if (vizinhancas.getMelhorCusto() < *custo) {
                if (vizinhancas.getJ() > vizinhancas.getI()) {
                    s.insert(s.begin() + vizinhancas.getJ() + 1, s[vizinhancas.getI()]);
                    s.erase(s.begin() + vizinhancas.getI());
                    
                    reOptPreProcessing(dimension, distancia, reOpt, vizinhancas.getI());
                }
                else {
                    s.insert(s.begin() + vizinhancas.getJ(), s[vizinhancas.getI()]);
                    s.erase(s.begin() + vizinhancas.getI() + 1);

                    reOptPreProcessing(dimension, distancia, reOpt, vizinhancas.getJ());
                }

                *custo = vizinhancas.getMelhorCusto();

                vizinhancas.preencheMovimentos(movimentosDeVizinhanca);
            }
            else movimentosDeVizinhanca.erase(movimentosDeVizinhanca.begin() + vizinho);

            break;

        case 2:
            vizinhancas.n_re_insertion(distancia, dimension, s, reOpt, 2, *custo);

            if (vizinhancas.getMelhorCusto() < *custo) {
                if (vizinhancas.getJ() > vizinhancas.getI()) {
                    s.insert(s.begin() + vizinhancas.getJ() + 2, s[vizinhancas.getI() + 1]);
                    s.insert(s.begin() + vizinhancas.getJ() + 2, s[vizinhancas.getI()]);
                    s.erase(s.begin() + vizinhancas.getI() + 1);
                    s.erase(s.begin() + vizinhancas.getI());

                    reOptPreProcessing(dimension, distancia, reOpt, vizinhancas.getI());
                }
                else {
                    s.insert(s.begin() + vizinhancas.getJ(), s[vizinhancas.getI() + 1]);
                    s.insert(s.begin() + vizinhancas.getJ(), s[vizinhancas.getI() + 1]);
                    s.erase(s.begin() + vizinhancas.getI() + 3);
                    s.erase(s.begin() + vizinhancas.getI() + 2);

                    reOptPreProcessing(dimension, distancia, reOpt, vizinhancas.getJ());
                }

                *custo = vizinhancas.getMelhorCusto();
                
                vizinhancas.preencheMovimentos(movimentosDeVizinhanca);
            }
            else movimentosDeVizinhanca.erase(movimentosDeVizinhanca.begin() + vizinho);

            break;

        case 3:
            vizinhancas.n_re_insertion(distancia, dimension, s, reOpt, 3, *custo);

            if (vizinhancas.getMelhorCusto() < *custo) {
                if (vizinhancas.getJ() > vizinhancas.getI()) {
                    s.insert(s.begin() + vizinhancas.getJ() + 3, s[vizinhancas.getI() + 2]);
                    s.insert(s.begin() + vizinhancas.getJ() + 3, s[vizinhancas.getI() + 1]);
                    s.insert(s.begin() + vizinhancas.getJ() + 3, s[vizinhancas.getI()]);
                    s.erase(s.begin() + vizinhancas.getI() + 2);
                    s.erase(s.begin() + vizinhancas.getI() + 1);
                    s.erase(s.begin() + vizinhancas.getI());

                    reOptPreProcessing(dimension, distancia, reOpt, vizinhancas.getI());
                }
                else {
                    s.insert(s.begin() + vizinhancas.getJ(), s[vizinhancas.getI() + 2]);
                    s.insert(s.begin() + vizinhancas.getJ(), s[vizinhancas.getI() + 2]);
                    s.insert(s.begin() + vizinhancas.getJ(), s[vizinhancas.getI() + 2]);
                    s.erase(s.begin() + vizinhancas.getI() + 5);
                    s.erase(s.begin() + vizinhancas.getI() + 4);
                    s.erase(s.begin() + vizinhancas.getI() + 3);

                    reOptPreProcessing(dimension, distancia, reOpt, vizinhancas.getJ());
                }

                *custo = vizinhancas.getMelhorCusto();

                vizinhancas.preencheMovimentos(movimentosDeVizinhanca);
            }
            else movimentosDeVizinhanca.erase(movimentosDeVizinhanca.begin() + vizinho);

            break;

        case 4:
            vizinhancas.opt_2(distancia, dimension, s, reOpt, *custo);
            if (vizinhancas.getMelhorCusto() < *custo) {
                for (int k = 0; k < (vizinhancas.getJ() - vizinhancas.getI() + 1) / 2; k++) { //para nao excluir o i
                    int aux = s[vizinhancas.getI() + k];
                    s[vizinhancas.getI() + k] = s[vizinhancas.getJ() - k];
                    s[vizinhancas.getJ() - k] = aux;
                }

                *custo = vizinhancas.getMelhorCusto();

                reOptPreProcessing(dimension, distancia, reOpt, vizinhancas.getI());
                vizinhancas.preencheMovimentos(movimentosDeVizinhanca);
            }
            else movimentosDeVizinhanca.erase(movimentosDeVizinhanca.begin() + vizinho);
        }
    }
}

void ILS::perturbacao(int dimension)
{
    std::vector<int> ar(4);
    
    for (int i = 0; i < 4; i++){
        ar[i] = rand() % (dimension - 4) + 1;
    }
    sort(ar.begin(), ar.end());

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
}

std::chrono::duration<double> ILS::getTimePreProcessing()
{
    return timePreProcessing;
}