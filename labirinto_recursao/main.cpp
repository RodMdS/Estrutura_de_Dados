#include "libs/view.h"
#include "aluno/jogo_velha.h"
#include "aluno/exemplo_lab.h"
#include "aluno/teste_matrizes.h"
#include <vector>

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

bool eh_furavel(matchar &mat, Par par){
    if(par.l < 1 || par.l >= mat.sizeL() - 1){
        return false;
    }
    if(par.c < 1 || par.c >= mat.sizeC() - 1){
        return false;
    }

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
    if(par.l < 1 || par.l >= mat.sizeL() - 1){
        return;
    }
    if(par.c < 1 || par.c >= mat.sizeC() - 1){
        return;
    }

    if(mat.get(par) == 'k'){
        mat.get(par) = 'w';
        my_view->paint(mat);

        auto vet = achar_vizinhos(par);
        vet = shuffle(vet);
        for(auto par : vet){
            if(eh_furavel(mat, par)){
                desenhar_labirinto(mat, par);

            }
        }

        return ;
    }

    return;
}

bool eh_caminho(matchar &mat, Par par, Par destino){
    if(par.l < 0 || par.l >= mat.sizeL()){
        return false;
    }
    if(par.c < 0 || par.c >= mat.sizeC()){
        return false;
    }

    if(par == destino){
        mat.get(par) = 'y';
        my_view->paint(mat);
        return true;
    }

    if(mat.get(par) == 'w'){
        mat.get(par) = 'r';
        my_view->paint(mat);

        auto vet = achar_vizinhos(par);
        vet = shuffle(vet);
        for(auto vizinho : vet){
            if(eh_caminho(mat, vizinho, destino)){
                mat.get(vizinho) = 'y';
                my_view->paint(mat);
                return true;
            }
        }

        mat.get(par) = 'b';
        my_view->paint(mat);

        return false;
    }

    return false;
}

int main(){
    matchar mat(15, 20, 'k');

    Par inicio = my_view->select_point(mat, "Selecione o que inicia o labirinto");

    desenhar_labirinto(mat, inicio);
    my_view->paint(mat);

    Par p = my_view->select_point(mat, "Selecione o ponto inicial");
    Par destino = my_view->select_point(mat, "Selecione o ponto destino");

    eh_caminho(mat, p, destino);
    mat.get(p) = 'y';

    my_view->paint(mat);

    my_view->wait();

    return 0;
}


