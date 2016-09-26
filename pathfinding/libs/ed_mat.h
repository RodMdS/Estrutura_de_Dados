/*

  Esse arquivo contém as seguintes classes e funções.
  Par - Define um par linha e coluna, bem como suas operações
  matriz - Define uma classe template para matrizes
  cmatriz - Define uma classe mínima para matrizes em c
  matchar_ - funções para entrada e saída de matrizes de caracteres
  MatrizView - Classe para desenho de Matrizes

*/

#ifndef MATRIZ_H
#define MATRIZ_H

#include <vector>
#include <iostream>

using namespace std;

struct Par{
    int l, c;
    Par(int l = 0, int c = 0){
        this->l = l;
        this->c = c;
    }

    Par operator+(const Par& other){
        return Par(l + other.l, c + other.c);
    }

    bool operator ==(const Par& other){
        return (l == other.l && c == other.c);
    }
};

template<class T> class matriz{
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

    //construtor passando numero de linhas
    //numero de colunas
    //e o elemento que inicia toda a matriz
    matriz(int nl, int nc, T init):
        _mat(nl, std::vector<T>(nc, init)){
        generate_all_pos();
    }

    //operador de atribuição
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

    //retorna o tamanho como um par
    Par size() const{
        return Par(sizeL(), sizeC());
    }

    //retorna true se o ponto estiver dentro da matriz
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

    //retorna true se o par está na matriz e possui esse valor
    bool equals(const Par& pos, const T& value) const {
        if(is_inside(pos))
            return(get(pos) == value);
        return false;
    }

    //retorna todas as posições da matriz
    const vector<Par>& posicoes() const{
        return this->all_pos;
    }

};

using matchar = matriz<char>;

#endif // MATRIZ_H

#ifndef CMATRIZ_H
#define CMATRIZ_H

#include <iostream>

struct cmatriz{
    char *data;
    int nl;
    int nc;
};

cmatriz * cmat_create(int nl, int nc){
    cmatriz * mat = new cmatriz;
    mat->nl = nl;
    mat->nc = nc;
    mat->data = new char[nl * nc];
    return mat;
}

void cmat_destroy(cmatriz * mat){
    delete(mat->data);
    delete(mat);
}

char* cmat_at(cmatriz * mat, int l, int c){
    static char error = '\0';
    if((l >= mat->nl) || (c >= mat->nc) || (l < 0) || (c < 0)){
        std::cerr << "Posicao l ou c invalida na matriz.\n";
        return &error;
    }
    return mat->data + (l * mat->nc) + c;

}

#endif // CMATRIZ_H

#ifndef MATCHAR_OPERATIONS_H
#define MATCHAR_OPERATIONS_H

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

matchar matchar_file_load(string nome){
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

void matchar_file_save(string nome, const matchar& mat){
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

//void file_interactive_create_mat(){
//    cout << "Digite as dimensoes da matriz que deseja criar \n";
//    cout << "linhas: ";
//    int nl, nc;
//    cin >> nl;
//    cout << "colunas: ";
//    cin >> nc;
//    cout << "Digite o caractere default. Opcoes " << my_colors->color_list() << endl;
//    cout << "default: ";
//    string def;
//    cin >> def;
//    matchar mat(nl, nc, def[0]);

//    paint_brush(mat, my_colors->color_list());
//    set_visible(false);
//    cout << "Digite o nome do arquivo que deseja salvar(sem espacos) terminando em .txt\n";
//    string nome;
//    cin >> nome;
//    file_save_mat(nome, mat);
//}

#endif // MATCHAR_OPERATIONS_H


#ifndef MATRIZVIEW
#define MATRIZVIEW
#include <SFML/Graphics.hpp>
#include "ed_base.h"


class MatrizView{
private:
    struct Offset{
        int left;
        int right;
        int up;
        int down;
    };
    MyPlayer * my_player;
    sf::RenderTexture * my_painel;

    Par buffer_size_mat;//guarda as dimensoes da ultima matriz printada
    Offset off;
    int larg{1};//largura da linha

    MatrizView()
    {
        off.up = 30;
        off.left = 30;
        off.right = 30;
        off.down = 30;

        my_player = MyPlayer::instance();
        my_painel = MyPlayer::getPainel();

        my_player->color_back = (sf::Color(216,216,255));
        my_player->color_front = (sf::Color::Black);
        off.up = my_player->offset_up;
    }

public:

    static MatrizView * instance(){
        static MatrizView smview;
        return &smview;
    }

    //permite ao usuario desenhar uma matriz. Salva o desenho em mat
    void paint_brush(matchar &mat, const string &colors){
        MyWindow::instance()->setVisible(true);
        char cor_atual = colors[0];
        bool isDown = false;
        bool acabou = false;
        sf::Vector2i lastPos;

        MyColors * cm = MyColors::instance();
        //sf::Color back = player->get_color_back();
        while(!acabou){
            sf::Event event;
            float lado = recalc_side(*MyWindow::instance(), mat.size());
            while(MyWindow::instance()->pollEvent(event)){
                MyWindow::instance()->processar_resize(event);
                if(MyWindow::instance()->processar_close(event))
                    acabou = true;
                if(event.type == sf::Event::KeyPressed){
                    if(event.key.code == sf::Keyboard::Return)
                        acabou = true;
                    else{
                        char c = cm->sf2char(event.key.code);
                        if(colors.find(c) != string::npos)
                            if(cm->exists(c))
                                cor_atual = c;
                    }
                }
                if(event.type == sf::Event::MouseButtonPressed){
                    if(event.mouseButton.button == sf::Mouse::Left){
                        isDown = true;
                        lastPos = pixel2mat(sf::Mouse::getPosition(*MyWindow::instance()), lado);
                    }

                }
                if(event.type == sf::Event::MouseButtonReleased){
                    if(event.mouseButton.button == sf::Mouse::Left)
                        isDown = false;
                }
            }

            //processar cliques no mouse de pinturas
            //if(sf::Mouse::isButtonPressed(sf::Mouse::Left)){
            if(isDown){
                sf::Vector2i newPos = pixel2mat(sf::Mouse::getPosition(*MyWindow::instance()), lado);
                auto path = MyPathMaker::make_path(sf::Vector2f(lastPos), sf::Vector2f(newPos));
                for(auto pmat : path)
                    if(mat.is_inside(pmat.y, pmat.x))
                        mat.get(pmat.y,pmat.x) = cor_atual;
                lastPos = newPos;
            }

            MyWindow::instance()->clear(my_player->color_back);
            string texto = string() + "Digite " + colors + " para trocar de Cor " +
                    "ou Enter para terminar!";
            MyWindow::instance()->draw(sfText(sf::Vector2f(off.left, 0), texto, my_player->color_front));
            paint_target_squares(*MyWindow::instance(), mat);
            MyWindow::instance()->display();
        }
    }


    //mostra um texto e retorna um ponto clicado para o usuario
    Par select_point(const matchar& mat, const string & texto){
        MyWindow::instance()->setVisible(true);
        sf::Vector2u square;
        bool acabou = false;
        square.x = 0;
        square.y = 0;
        while(!acabou){
            sf::Event event;
            float lado = recalc_side(*MyWindow::instance(), mat.size());
            while(MyWindow::instance()->pollEvent(event)){
                MyWindow::instance()->processar_resize(event);
                if(MyWindow::instance()->processar_close(event))
                    acabou = true;
                if(event.type == sf::Event::KeyPressed){
                    if(event.key.code == sf::Keyboard::Q)
                        acabou = true;
                }
                else if(event.type == sf::Event::MouseButtonPressed){
                    if(event.mouseButton.button == sf::Mouse::Left){
                        int px = event.mouseButton.x;
                        int py = event.mouseButton.y;
                        sf::Vector2i mouse(px, py);

                        auto pmat = pixel2mat(mouse, lado);
                        if(mat.is_inside(pmat.y, pmat.x)){
                            square = sf::Vector2u(pmat.x, pmat.y);
                            acabou = true;
                        }
                    }
                }
            }

            MyWindow::instance()->clear(my_player->color_back);
            MyWindow::instance()->draw(sfText(sf::Vector2f(off.left, 0), texto, my_player->color_front));
            paint_target_squares(*MyWindow::instance(), mat);
            MyWindow::instance()->display();
        }
        return Par(square.y, square.x);
    }

    sf::Vector2f get_paint_pos(const sf::RenderTarget &rt, const Par &pos, const Par &size){
        float side = recalc_side(rt, size);
        return sf::Vector2f(larg + off.left + pos.c * side, larg + off.up + pos.l * side);
    }

    //desenha a matriz no rendertarget usando o mapa de cores
    void paint_target_squares(sf::RenderTarget &rt, const matchar &mat){
        buffer_size_mat = mat.size();
        sf::RectangleShape rect;
        float side = recalc_side(rt, mat.size());
        rect.setSize(sf::Vector2f(side - larg, side - larg));
        for(int c = 0; c < mat.sizeC(); c++){
            for(int l = 0; l < mat.sizeL(); l++){
                rect.setFillColor(my_colors->get(mat.get(l,c)));
                rect.setPosition(get_paint_pos(rt, Par(l, c), mat.size()));
                rt.draw(rect);
            }
        }
    }

    //desenha a matriz no rendertarget usando o mapa de cores
    void paint_target_cell(const Par &pos, char color){
//        Par size = buffer_size_mat;
//        sf::RectangleShape rect;
//        float side = recalc_side(*my_painel, size);
//        rect.setSize(sf::Vector2f(side - larg, side - larg));
//        rect.setFillColor(MyColors::instance()->get(color));
//        rect.setPosition(get_paint_pos(*my_painel, pos, size));
//        my_painel->draw(rect);
        Par size = buffer_size_mat;
        sf::CircleShape circ;
        float side = recalc_side(*my_painel, size);
        circ.setRadius((side - larg)/2);
        //circ.setSize(sf::Vector2f(side - larg, side - larg));
        sf::Color cor = MyColors::instance()->get(color);
        //cor.a = 150;
        circ.setFillColor(cor);
        circ.setPosition(get_paint_pos(*my_painel, pos, size));
        my_painel->draw(circ);
    }

    //desenha os numeros
    void paint_target_nums(sf::RenderTarget &rt, const matriz<int>& nums){
        float side = recalc_side(rt, nums.size());
        //print numbers
        sfText text;
        text.setColor(sf::Color::Black);
        for(int c = 0; c < nums.sizeC(); c++){
            for(int l = 0; l < nums.sizeL(); l++){
                string str = to_string(nums.get(l, c));
                text.setString(str);
                if(str.size() == 1)
                    text.setCharacterSize(0.8 * side);
                else if(str.size() == 2)
                    text.setCharacterSize(0.65 * side);
                else
                    text.setCharacterSize(0.5 * side);
                text.setPosition(get_paint_pos(rt, Par(l, c), nums.size()));
                rt.draw(text);
            }
        }
    }

private:

    //retorna o lado do quadrado para o tamanho de tela atual
    float recalc_side(const sf::RenderTarget &rt, const Par &size){
        sf::Vector2u dim = rt.getSize();
        //cout <<"janela " << dim.x << " " << dim.y << endl;
        float propTela = dim.x/(float)dim.y;
        float propMat = size.c/(float)size.l;
        float lado = 0;
        if(propTela > propMat)
            lado = (dim.y - off.up - off.down)/(float)size.l;
        else
            lado = (dim.x - off.left - off.right)/(float)size.c;
        return lado;
    }


private:
    sf::Vector2i pixel2mat(const sf::Vector2i &pos, float lado){
        int y = (pos.y - off.up) / lado;
        int x = (pos.x - off.left) / lado;
        return sf::Vector2i(x, y);
    }
};

//FUNCOES SOBRE MATRIZES

//Faz uma pintura livre tal qual o paint brush
//sobre a matriz passada por parametro. A variavel
//colors define quais cores poderão ser utilizadas
//na pintura.
void mat_paint_brush(matchar& mat, const string& colors){
    MatrizView::instance()->paint_brush(mat, colors);
}

//Mostra o texto e espera o usuario clicar em uma posição
//valida na matriz
Par mat_get_click(const matchar& mat, const string & texto){
    return MatrizView::instance()->select_point(mat, texto);
}

//Pinta a matriz de caracteres na tela interpretando cada
//caracter como um código de cores
void mat_draw(const matchar& mat){
    MatrizView::instance()->paint_target_squares(*MyPlayer::getPainel(), mat);
}

//Imprime os numeros na tela como uma matriz.
//Normalmente utilizado apos imprimir a matriz de cores
//de mesma dimensão para por números dentro da matriz.
void mat_draw(const matriz<int>& nums){
    MatrizView::instance()->paint_target_nums(*MyPlayer::getPainel(), nums);
}

//Marca uma posição especifica da matriz.
//Deve ser utilizado depois de mat_paint
void mat_focus(const Par & pos, char color){
    MatrizView::instance()->paint_target_cell(pos, color);
}

#endif // MATRIZVIEW


