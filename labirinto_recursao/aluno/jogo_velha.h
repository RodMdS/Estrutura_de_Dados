#ifndef VELHA_H
#define VELHA_H

#include "../libs/matriz.h"
#include "../libs/file_operations.h"
#include <list>
#include <vector>
#include <iostream>
#include "../libs/view.h"

using namespace std;

//Horizontal, vetical, crescente, decrescente
enum Direcao{Hor, Vert, Cresc, Desc};

//descolamentos para andar na matriz dada a direção
vector<Par> delta_before = {Par(0, -1), Par(-1, 0), Par( 1, -1), Par(-1, -1)};
vector<Par> delta_after= {Par(0,  1), Par(+1, 0), Par(-1,  1), Par( 1,  1)};


//escolhe aleatoriamente e retorna a referencia a um dos elementos do vetor.
Par escolha_um(const vector<Par>& vet){
    int ind = rand() % vet.size();
    return vet[ind];
}

//verifica se essa posição da matriz existe e bate com alguma das opções
bool match(const matriz<char> & mat, Par pos, const vector<char> &opcoes){
    if(mat.is_inside(pos) == false)
        return false;
    for(const char & op : opcoes)
        if(mat.get(pos) == op)
            return true;
    return false;
}

//retorna as posições dos elementos continuos dessa direção que batem com as opções
vector<Par> contar_iguais(const matriz<char>& mat, Par pos, vector<char> opcoes, Direcao dir){
    vector<Par> res;

    if(match(mat, pos, opcoes))
        res.push_back(pos);
    else
        return res;
    Par before = pos + delta_before[(int) dir];
    while(match(mat, before, opcoes)){
        res.push_back(before);
        before = before + delta_before[(int) dir];
    }

    Par after = pos + delta_after[(int) dir];
    while(match(mat, after, opcoes)){
        res.push_back(after);
        after = after + delta_after[(int) dir];
    }
    return res;
}

//contas quantas direcoes batem com essa quantidade de elementos
int contar_em_todas_direcoes(const matchar& mat, const Par &pos, const vector<char>& opcoes, int qtd){
    int cont = 0;
    for(int i = 0; i < 4; i++)
        if(contar_iguais(mat, pos, opcoes, (Direcao) i).size() == (size_t)qtd)
            cont++;
    return cont;
}

//imprima a matriz de inteiros
void print_mat_num(matriz<int> mnum){
    for(int l = 0; l < mnum.sizeL(); l++){
        for(int c = 0; c < mnum.sizeC(); c++)
            cout << mnum.get(l, c) << " ";
        cout << endl;
    }
}

//retorna as posicões onde esse marcador pode ganhar
vector<Par> encontrar_cheque_mates(matchar mat, int qtd, char marcador, char vazio){
    vector<Par> res;
    for(auto pos : mat.posicoes()){
        if(mat.get(pos) == vazio){
            mat.get(pos) = marcador;
            if(contar_em_todas_direcoes(mat, pos, {marcador}, qtd) > 0)
                res.push_back(pos);
            mat.get(pos) = vazio;
        }
    }
    return res;
}

//verifica se esse marcador ganhou
bool ganhou(matchar mat, int qtd, char marcador){
    for(auto pos : mat.posicoes()){
        if(mat.get(pos) == marcador){
            if(contar_em_todas_direcoes(mat, pos, {marcador}, qtd) > 0)
                return true;
        }
    }
    return false;
}

//verifica se foi empate
bool foi_empate(const matchar& mat, char vazio){
    for(auto pos : mat.posicoes())
        if(mat.get(pos) == vazio)
            return false;
    return true;
}

//Monta a heuristica para esse marcador
//Na heuristica cada posição vazia recebe um ponto por cada direção em que ainda
//pode gerar uma vitoria. No final, ele retorna a matriz com a pontuação de cada
//posição. Quanto mais alta a pontuação melhor
matriz<int> montar_matriz_heuristica(const matchar& mat, char marcador, char vazio, int qtd){
    matriz<int> cont(mat.sizeC(), mat.sizeL(), -1);
    for(auto pos : mat.posicoes())
        if(mat.get(pos) == vazio)
            cont.get(pos) = contar_em_todas_direcoes(mat, pos, {marcador, vazio}, qtd);
    return cont;
}

//filtra e retorna as melhores posições da matriz de heuristica
vector<Par> escolher_melhores_posicoes(const matriz<int>& mat){
    int max = 0;
    vector<Par> bests;
    for(auto pos : mat.posicoes()){
        if(mat.get(pos) == max)
            bests.push_back(pos);
        if(mat.get(pos) > max){
            max = mat.get(pos);
            bests.clear();
            bests.push_back(pos);
        }
    }
    return bests;
}

//permite ao jogador humano jogar com esse marcador, inclusive
//escolher as opcoes que a maquina ja escolheu para testar
void jogar_jogador(matchar& mat, char marcador){
    matchar_write(cout, mat);
    int l, c;
    cout << "Linha e Coluna: ";
    cin >> l >> c;
    mat.get(l, c) = marcador;
}

//permite a maquina jogar com esse marcador
void jogar_maquina(matchar &tab, int qtd, char marcador, char inimigo, char vazio){
    //verifica se consegue ganhar com uma jogada
    auto vet_win = encontrar_cheque_mates(tab, qtd, marcador, vazio);
    if(vet_win.size() > 0){
        tab.get(escolha_um(vet_win)) = marcador;
        return;
    }

    //verifica se o inimigo esta na iminencia de ganhar e bloqueia
    auto vet_block = encontrar_cheque_mates(tab, qtd, inimigo, vazio);
    if(vet_block.size() > 0){
        tab.get(escolha_um(vet_block)) = marcador;
        return;
    }

    //faz a heuristica e escolha aleatoriamente uma das melhores posicoes
    auto predicao = montar_matriz_heuristica(tab, marcador, vazio, qtd);
    auto bests = escolher_melhores_posicoes(predicao);
    if(bests.size() > 0)
        tab.get(escolha_um(bests)) = marcador;
}

void velha_main(){

    char XIS = 'X';
    char BOLA = 'O';
    char VAZIO = '.';

    int tam = 4; //O tamanho do tabuleiro
    int nvitoria = 4; //A quantidade de casas continuas necessarias à vitoria

    matriz<char> tabuleiro(tam, tam, VAZIO);
    my_view->set_size(900, 600);
    my_view->set_autoplay(true);
    my_view->set_color(XIS, 200, 0, 0);
    my_view->set_color(BOLA, 0, 200, 0);
    my_view->set_color(VAZIO, 240, 250, 250);


    while(my_window->isOpen()){
        tabuleiro = matchar(tam, tam, VAZIO);
        while(true){
            //jogar_jogador(tab, XIS);
            tabuleiro.get(my_view->select_point(tabuleiro, "Escolha sua opcao")) = XIS;
            //jogar_maquina(tabuleiro, nvitoria, XIS, BOLA, VAZIO);
            my_view->paint(tabuleiro);

            if(foi_empate(tabuleiro, VAZIO)){
                cout << "Deu empate" << endl;
                break;
            }

            if(ganhou(tabuleiro, nvitoria, XIS)){
                cout << "Jogador ganhou" << endl;
                break;
            }

            jogar_maquina(tabuleiro, nvitoria, BOLA, XIS, VAZIO);

            if(ganhou(tabuleiro, nvitoria, BOLA)){
                cout << "Maquina ganhou" << endl;
                break;
            }
            if(foi_empate(tabuleiro, VAZIO)){
                cout << "Deu empate" << endl;
                break;
            }

            my_view->paint(tabuleiro);
        }
        my_view->paint(tabuleiro);
        my_view->wait();
    }
}



#endif // VELHA_H











