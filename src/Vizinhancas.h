#ifndef VIZINHANCAS_H
#define VIZINHANCAS_H
#include <iostream>
#include <vector>

class Vizinhancas
{
    public:
        Vizinhancas();

        void swap(double **distancia, int dimension, std::vector<int> &s, std::vector<std::vector<std::vector<double>>> &reOpt,
        double melhorCusto);
        
        void n_re_insertion (double **distancia, int dimension, std::vector<int> &s, std::vector<std::vector<std::vector<double>>>
        &reOpt, int n, double melhorCusto);

        void opt_2(double **distancia, int dimension, std::vector<int> &s, std::vector<std::vector<std::vector<double>>> &reOpt, 
        double melhorCusto);

        void preencheMovimentos(std::vector<int> &movimentosDeVizinhanca);

        int getI();

        int getJ();

        double getMelhorCusto();

    private:
        int I, J;
	    double melhorCusto;
};

#endif