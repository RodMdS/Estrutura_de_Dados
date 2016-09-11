#ifndef FILE_OPERATIONS_H
#define FILE_OPERATIONS_H

#include "player.h"
#include "matriz.h"
#include "view.h"

//imprime a matriz
//pode usar matchar_write(cout, mat)
//para imprimir para saida padrao
void matchar_write(ostream& ost, const matchar& mat){
    for(int l = 0; l < mat.sizeL(); l++){
        for(int c = 0; c < mat.sizeC(); c++){
            ost << mat.get(l, c);
        }
        ost << endl;
    }
}

//carrega a matriz
//Pode usar do arquivo ou cin
matchar matchar_read(istream& ist){
    int nl, nc;
    ist >> nl >> nc;
    ist.ignore(1000, '\n');
    matchar mat(nl, nc, ' ');
    for(int i = 0; i < nl; i++) {
        string line;
        getline(ist, line);
        for(int c = 0; c < nc; c++)
            mat.get(i, c) = line[c];
    }
    return mat;
}

#include <iostream>
#include <fstream>
using namespace std;

matchar file_load_mat(string nome){
    string path = "../matrizes/input/" + nome;
    cout << path << endl;
    ifstream ifs(path);
    if(!ifs.is_open()){
        cout << "Arquivo nao pode ser aberto \n";
        exit(0);
    }
    matchar mat = matchar_read(ifs);
    ifs.close();
    cout << "Arquivo carregado com sucesso!." << endl;
    return mat;
}

void file_save_mat(string nome, const matchar& mat){
    string path = "../matrizes/input/" + nome;
    cout << path << endl;
    ofstream ofs(path);
    if(!ofs.is_open()){
        cout << "Arquivo nao pode ser aberto \n";
        exit(0);
    }
    ofs << mat.sizeL() << " " << mat.sizeC() << endl;
    matchar_write(ofs, mat);
    ofs.close();
    cout << "Arquivo salvo com sucesso!." << endl;
}

void file_interactive_create_mat(){
    cout << "Digite as dimensoes da matriz que deseja criar \n";
    cout << "linhas: ";
    int nl, nc;
    cin >> nl;
    cout << "colunas: ";
    cin >> nc;
    cout << "Digite o caractere default. Opcoes " << my_colors->color_list() << endl;
    cout << "default: ";
    string def;
    cin >> def;
    matchar mat(nl, nc, def[0]);

    my_view->paint_brush(mat, my_colors->color_list());
    my_view->set_visible(false);
    cout << "Digite o nome do arquivo que deseja salvar(sem espacos) terminando em .txt\n";
    string nome;
    cin >> nome;
    file_save_mat(nome, mat);
}

#endif // FILE_OPERATIONS_H
