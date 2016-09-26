#include "libs/ed_base.h"
#include "libs/ed_mat.h"

#include <fstream>
#include <iostream>
#include <cstdlib>
#include <ctime>
#include <queue> //fila

using namespace std;

int nlinhas = 15;
int ncolunas = 20;

//MAPA DE CORES
//r red
//g green
//b blue
//m magenta
//c cyan -
//y yellow
//w white
//k black

vector<Par> adjacentes(Par p){
    vector<Par> adj(8, p);
    adj[0].c -= 1; // oeste
    adj[1] = p + Par(-1, -1); // noroeste
    adj[2].l -= 1; // norte
    adj[3] = p + Par(-1, 1); // nordeste
    adj[4].c += 1; // leste
    adj[5] = p + Par(1, 1); // sudeste
    adj[6].l += 1; // sul
    adj[7] = p + Par(1, -1); // sudoeste

    return adj;
}

//se houver caminho, preencha a matriz de distancias
//e retorne true
bool buscar(matchar &mat, matriz<int> &dist, Par inicio, char cor, Par dest){
    queue<Par> fila;
    mat.get(inicio) = 'c';//marcar
    dist.get(inicio) = 0;
    fila.push(inicio);

    while(fila.size() != 0){
        mat.get(fila.front()) = 'r';
        for(auto elem : adjacentes(fila.front())){
            if(mat.is_inside(elem)){
                if(mat.get(elem) == cor){
                    mat.get(elem) = 'c';
                    fila.push(elem);
                    dist.get(elem) = dist.get(fila.front()) + 1;
                    mat_draw(mat);
                    mat_focus(elem, 'y');
                    mat_draw(dist);
                    ed_show();
                    if(elem == dest)
                        return true;
                }
            }
        }
        fila.pop();
    }

    return false;
}

void mostrar_caminho(matchar &mat, matriz<int> &dist, Par inicio, Par destino){
    if(destino == inicio){
        mat.get(destino) = 'm';
        return;
    }

    queue<Par> fila;
    fila.push(destino);
    mat.get(fila.front()) = 'm';
    mat_draw(mat);
    mat_draw(dist);
    ed_show();

    while(fila.size() > 0){
        for(Par par : adjacentes(fila.front())){
            if(mat.equals(par, 'r')){
                if(dist.get(par) < dist.get(fila.front())){
                    mat.get(par) = 'm';
                    mat_draw(mat);
                    mat_draw(dist);
                    ed_show();
                    fila.push(par);
                    break;
                }
            }
        }
        fila.pop();
    }

    fila.pop();
    return;
}

int main(){
    //cria uma matriz de caracteres de 15 linhas por 20 colunas, e preenche todos os elementos
    //com 'y'
    matriz<char> mat(nlinhas, ncolunas, 'g');
    matriz<int> dist(nlinhas, ncolunas, -1);

    //chama o metodo de desenho livre onde a cor primeira default eh branca
    mat_paint_brush(mat, "wg");

    Par inicio = mat_get_click(mat, "escolha inicio");
    Par destino = mat_get_click(mat, "escolha destino");
    char cor = mat.get(inicio);
    bool achou = buscar(mat, dist, inicio, cor, destino);
    if(achou)
        mostrar_caminho(mat, dist, inicio, destino);

    ed_lock();//impede que termine abruptamente

    return 0;
}



