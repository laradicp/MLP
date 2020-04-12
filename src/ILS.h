#ifndef ILS_H
#define ILS_H
#include "Vizinhancas.h"
#include <algorithm>

class ILS
{
    public:
        ILS(int dimension);

        int getI();

        void setS(std::vector<int> s);

        std::vector<int> getS();

        void construcao(int dimension, double ** distancia);

        void reOptPreProcessing(int dimension, double **distancia, std::vector<std::vector<std::vector<double>>> &reOpt, int begin);

        void rvnd(int dimension, double ** distancia, std::vector<std::vector<std::vector<double>>> &reOpt, double *custo);

        void perturbacao(int dimension);

        std::chrono::duration<double> getTimePreProcessing();

        Vizinhancas vizinhancas;

    private:
        int I;
        std::vector<int> s;
        std::chrono::duration<double> timePreProcessing;
        typedef struct {
            double custo;
            int I;
        } tInsercao;
};

#endif
