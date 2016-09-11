#ifndef CMAT_H
#define CMAT_H

#include <iostream>

struct cmat{
    char *data;
    int nl;
    int nc;
};

cmat * mat_create(int nl, int nc){
    cmat * mat = new cmat;
    mat->nl = nl;
    mat->nc = nc;
    mat->data = new char[nl * nc];
    return mat;
}

void mat_destroy(cmat * mat){
    delete(mat->data);
    delete(mat);
}

char* mat_at(cmat * mat, int l, int c){
    static char error = '\0';
    if((l >= mat->nl) || (c >= mat->nc) || (l < 0) || (c < 0)){
        std::cerr << "Posicao l ou c invalida na matriz.\n";
        return &error;
    }
    return mat->data + (l * mat->nc) + c;
}

#endif // CMAT_H
