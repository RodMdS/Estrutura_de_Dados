#include "libs/ed_base.h"
#include "libs/ed_mat.h"

#include <fstream>
#include <iostream>
#include <cstdlib>
#include <ctime>
#include <stack>
#include <vector>
using namespace std;

int nlinhas = 20;
int ncolunas = 35;

//r red - vermelho
//g green - verde
//b blue - azul
//m magenta - rosa(?)
//c cyan - azul claro(?)
//y yellow - amarelo
//w white - branco
//k black - preto

vector<Par> pegar_vizinhos(Par par){
    vector<Par> vizinhos;
    vizinhos.push_back(Par(par.l, par.c - 1));
    vizinhos.push_back(Par(par.l, par.c + 1));
    vizinhos.push_back(Par(par.l - 1, par.c));
    vizinhos.push_back(Par(par.l + 1, par.c));
    return vizinhos;
}

vector<Par> shuffle(vector<Par> &vet){
    for(int i = 0; i < (int) vet.size(); i++){
        std::swap(vet[i], vet[rand() % (int) vet.size()]);
    }
    return vet;
}

bool eh_valido(matchar &mat, Par p){
    if(p.l < 1 || p.l >= mat.sizeL() - 1)
        return false;
    if(p.c < 1 || p.c >= mat.sizeC() - 1)
        return false;

    return true;
}

bool eh_furavel(matchar &mat, Par p){
    int cont = 0;

    if(!eh_valido(mat, p))
        return false;
    if(!mat.equals(p, 'k'))
        return false;

    auto vizinhos = pegar_vizinhos(p);
    for(auto viz : vizinhos){
        if(mat.equals(viz, 'w')){
            cont++;
        }
    }

    if(cont <= 1)
        return true;

    return false;
}

void mostrar_labirinto(matchar &mat, stack<Par> pilha){
    mat_draw(mat);
    while(!pilha.empty()){
        mat_focus(pilha.top(), 'y');
        pilha.pop();
    }
    ed_show();
}

void desenhar_labirinto(matchar &mat, stack<Par> &pilha){
    pilha.push(Par{1, 1});
    mat.get(pilha.top()) = 'w';
    mostrar_labirinto(mat, pilha);

    while(!pilha.empty()){
        auto adj = pegar_vizinhos(pilha.top());
        vector<Par> furaveis;
        for(auto par : adj){
            if(eh_furavel(mat, par)){
                furaveis.push_back(par);
            }
        }

        if(!furaveis.empty()){
            furaveis = shuffle(furaveis);
            pilha.push(furaveis.front());
            mat.get(pilha.top()) = 'w';
            mostrar_labirinto(mat, pilha);
        } else {
            pilha.pop();
            mostrar_labirinto(mat, pilha);

        }
    }
}

void encontrar_caminho(matchar &mat, Par origem, Par destino){
    stack<Par> pilha;

    if(!eh_valido(mat, origem)){
        return;
    }
    if(!eh_valido(mat, destino)){
        return;
    }
    if(origem == destino){
        mat.get(origem) = 'm';
        mat.get(destino) = 'm';
        return;
    }

    pilha.push(origem);

    while(!pilha.empty()){
        if(!mat.equals(pilha.top(), 'k')){
            vector<Par> possiveis;
            auto viz = pegar_vizinhos(pilha.top());
            viz = shuffle(viz);
            for(auto par : viz){
                 if(mat.equals(par, 'w')){
                     possiveis.push_back(par);
                 }
            }

            if(!possiveis.empty()){
                possiveis = shuffle(possiveis);
                mat.get(pilha.top()) = 'r';
                pilha.push(possiveis.front());
                mostrar_labirinto(mat, pilha);
                if(pilha.top() == destino){
                    while(!pilha.empty()){
                       mat.get(pilha.top()) = 'm';
                       pilha.pop();
                       mostrar_labirinto(mat, pilha);
                    }
                    return;
                }

            } else {
                mat.get(pilha.top()) = 'b';
                pilha.pop();
                mostrar_labirinto(mat, pilha);
            }
        }
    }
}

int main(){
    matchar mat(nlinhas, ncolunas, 'k');
    //mat_paint_brush(mat, "gw");

    stack<Par> pilha;
    desenhar_labirinto(mat, pilha);
    mat_draw(mat);
    ed_show();

    Par ori = mat_get_click(mat, "Escolha a origem: ");
    Par dest = mat_get_click(mat, "Escolha o destino: ");

    encontrar_caminho(mat, ori, dest);
    mat_draw(mat);
    ed_show();

    ed_lock();

    return 0;
}


