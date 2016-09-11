#include "libs/ed_base.h"
#include "libs/ed_mat.h"

#include <fstream>
#include <iostream>
#include <cstdlib>
#include <ctime>
#include <vector>
using namespace std;

int nlinhas = 20;
int ncolunas = 35;

//r red
//g green
//b blue
//m magenta
//c cyan -
//y yellow
//w white
//k black

vector<Par> achar_vizinhos(Par par){
    vector<Par> vizinhos;
    vizinhos.push_back(Par(par.l, par.c - 1));
    vizinhos.push_back(Par(par.l, par.c + 1));
    vizinhos.push_back(Par(par.l - 1, par.c));
    vizinhos.push_back(Par(par.l + 1, par.c));

    return vizinhos;
}

vector<Par> shuffle(vector<Par> &vizinhos){
    for(int i = 0; i < (int) vizinhos.size(); i++){
        std::swap(vizinhos[i], vizinhos[rand() % (int) vizinhos.size()]);
    }
    return vizinhos;
}

bool eh_valido(matchar &mat, Par p){
    if(p.l < 1 || p.l >= mat.sizeL() - 1)
        return false;
    if(p.c < 1 || p.c >= mat.sizeC() - 1)
        return false;

    return true;
}

bool eh_furavel(matchar &mat, Par par){
    if(!eh_valido(mat, par))
        return false;
    if(!mat.is_inside(par))
        return false;

    int cont = 0;
    auto vet = achar_vizinhos(par);
    for(auto par : vet){
        if(mat.get(par) == 'w'){
            cont++;
        }
    }

    if(cont <= 1){
        return true;
    }

    return false;
}

void desenhar_labirinto(matchar &mat, Par par){
    if(!eh_valido(mat, par))
        return;
    if(!mat.is_inside(par))
        return;

    if(mat.get(par) == 'k'){
        mat.get(par) = 'w';
        mat_draw(mat);
        ed_show();
        auto vet = achar_vizinhos(par);
        vet = shuffle(vet);
        for(auto par : vet){
            if(eh_furavel(mat, par)){
                desenhar_labirinto(mat, par);
            }
        }

        return;
    }
    return;
}

bool eh_caminho(matchar &mat, Par origem, Par destino){
    if(!eh_valido(mat, origem))
        return false;
    if(!eh_valido(mat, destino))
        return false;
    if(origem == destino){
        mat.get(origem) = 'y';
        mat_draw(mat);
        ed_show();
        return true;
    }

    if(mat.get(origem) == 'w'){
        mat.get(origem) = 'r';
        mat_draw(mat);
        ed_show();

        auto vet = achar_vizinhos(origem);
        vet = shuffle(vet);
        for(auto vizinho : vet){
            if(eh_caminho(mat, vizinho, destino)){
                mat.get(vizinho) = 'y';
                mat_draw(mat);
                ed_show();
                return true;
            }
        }

        mat.get(origem) = 'b';
        mat_draw(mat);
        ed_show();
        return false;
    }
    return false;
}

int main(){
    matchar mat(15, 20, 'k');

    Par inicio = mat_get_click(mat, "Selecione o que inicia o labirinto");

    desenhar_labirinto(mat, inicio);
    mat_draw(mat);
    ed_show();

    Par origem = mat_get_click(mat, "Selecione o ponto origem: ");
    Par destino = mat_get_click(mat, "Selecione o ponto destino: ");

    eh_caminho(mat, origem, destino);
    mat.get(origem) = 'y';

    mat_draw(mat);
    ed_show();

    ed_lock();

    return 0;
}


