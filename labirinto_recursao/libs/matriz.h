#ifndef MATRIZ_H
#define MATRIZ_H
#include <vector>
#include <iostream>

using namespace std;

struct Par{
    int l, c;
    Par(int l, int c){
        this->l = l;
        this->c = c;
    }
};

//sobrecarga para somar duas posicoes
Par operator +(const Par& left, const Par& right){
    return Par(left.l + right.l, left.c + right.c);
}

//sobrecarga para testar igualdade
bool operator ==(const Par& left, const Par& right){
    return (left.l == right.l && left.c == right.c);
}

template<class T>
class matriz{
protected:
    vector<vector<T>> _mat;
    vector<Par> all_pos;

    void test_pos(int l, int c = 0) const{
        if(!is_inside(l, c)){

            cerr << "Posicao l = " << l << ", c = " << c << " invalida!" << endl
                 << "Programa finalizado" << endl;
            exit(1);
        }
    }

    void generate_all_pos(){
        all_pos.clear();
        all_pos.reserve(sizeL() * sizeC());
        for(int l = 0; l < sizeL(); l++){
            for(int c = 0; c < sizeC(); c++)
                all_pos.push_back(Par(l, c));
        }
    }

public:
    matriz():
        _mat() {}

    //copy constructor
    matriz(const matriz& mat):
        _mat(mat._mat){
        generate_all_pos();
    }

    matriz(int nl, int nc, T init):
        _mat(nl, std::vector<T>(nc, init)){
        generate_all_pos();
    }

    matriz& operator= (const matriz& mat){
        this->_mat = mat._mat;
        generate_all_pos();
        return *this;
    }

    T& get(int l, int c){
        test_pos(l, c);
        return _mat[l][c];
    }

    T get(int l, int c) const {
        test_pos(l, c);
        return _mat[l][c];
    }

    int sizeL() const{
        return _mat.size();
    }

    int sizeC() const{
        if(_mat.size() == 0)
            return 0;
        return _mat[0].size();
    }

    Par size() const{
        return Par(sizeL(), sizeC());
    }

    bool is_inside(int l, int c) const{
        if(l < 0 || l >= sizeL() || c < 0 || c >= sizeC())
            return false;
        return true;
    }

    //Usando Pos
    T& get(const Par& p){
        return get(p.l, p.c);
    }

    T get(const Par& p) const {
        return get(p.l, p.c);
    }

    bool is_inside(const Par& p) const{
        return is_inside(p.l, p.c);
    }

    bool equals(const Par& pos, const T& value) const {
        if(is_inside(pos))
            return(get(pos) == value);
        return false;
    }

    const vector<Par>& posicoes() const{
        return all_pos;
    }
};

using matchar = matriz<char>;


#endif // MATRIZ_H
