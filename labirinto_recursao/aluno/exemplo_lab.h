#ifndef EXEMPLO_LAB_H
#define EXEMPLO_LAB_H

#include "../libs/view.h"

#include <fstream>
#include <iostream>
#include <cstdlib>
#include <ctime>
using namespace std;

namespace aluno{
    int nlinhas = 20;
    int ncolunas = 35;

    matchar mat(nlinhas, ncolunas, 'g');
    matriz<int> matnum(nlinhas, ncolunas, 0);


    int queimar(int l, int c){
        if(!mat.is_inside(l, c))
            return 0;

        if(mat.get(l, c) == 'g'){
            mat.get(l, c) = 'r';

            //troque os paint para imprimir os numeros tambem
            my_view->paint(mat);
            //my_view->paint(mat, matnum);
            int qtd = 1 + queimar(l, c - 1) + queimar(l, c + 1)
                        + queimar(l - 1, c) + queimar(l + 1, c);
            return qtd;
        }

        return 0;
    }

    void queimada_aluno(){

        my_view->paint_brush(mat, "wg");
        Par pos = my_view->select_point(mat, "Escolha um ponto pintado da matriz para iniciar o fogo.");
        my_view->set_autoplay(true);
        int qtd = queimar(pos.l, pos .c);
        cout << "Queimaram " << qtd << " arvores \n";

        my_view->wait();
    }
}

#endif // EXEMPLO_LAB_H
