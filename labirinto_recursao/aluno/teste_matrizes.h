#ifndef TESTE_MATRIZES_H
#define TESTE_MATRIZES_H

#include "../libs/matriz.h"
#include "../libs/file_operations.h"

#include <iostream>
#include <fstream>

using namespace std;

void teste_matrizes(){
    ifstream ist("../matrizes/input/input_teste.txt");
    if(!ist.is_open())
    {
        cout << "Arquivo nao encontrado" << endl;
        exit(1);
    }

    matchar mat = matchar_read(ist);

    int li, ci;
    ist >> li >> ci;

    ist.close();

    cout << "nl " << mat.sizeL() << endl;
    cout << "nc " << mat.sizeC() << endl;

    matchar_write(cout, mat);

    cout << "li " << li << endl;
    cout << "ci " << ci << endl;

    cout << mat.get(3, 3) << endl;
    cout << mat.get(-3, 3) << endl;
}

#endif // TESTE_MATRIZES_H
