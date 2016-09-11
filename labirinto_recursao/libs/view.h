#ifndef MATRIZVIEW
#define MATRIZVIEW
#include <SFML/Graphics.hpp>

#include "player.h"
#include "matriz.h"

class MatrizView{
private:
    struct Offset{
        int left;
        int right;
        int up;
        int down;
    };

    Offset off;
    int larg{1};//largura da linha

public:
    MatrizView()
    {
        off.up = 30;
        off.left = 30;
        off.right = 30;
        off.down = 30;

        my_player->color_back = (sf::Color(216,216,255));
        my_player->color_front = (sf::Color::Black);
        off.up = my_player->offset_up;
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
    void paint_target_cell(const Par &size, const Par &pos, char color){
        sf::RectangleShape rect;
        float side = recalc_side(my_painel, size);
        rect.setSize(sf::Vector2f(side - larg, side - larg));
        rect.setFillColor(MyColors::instance()->get(color));
        rect.setPosition(get_paint_pos(my_painel, pos, size));
        my_painel.draw(rect);
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

//############################################################################

#define my_view (MyView::get_instance())

class MyView{
private:
    MatrizView matview;

    void check_push(){
        if(my_player->autopush)
            my_player->show();
    }

    MyView(){}

public:
    static MyView * get_instance(){
        static MyView view;
        return &view;
    }

    void set_visible(bool value){
        MyWindow::instance()->setVisible(value);
    }

    void set_size(int largura, int altura){
        MyWindow::instance()->setSize(sf::Vector2u(largura, altura));
    }

    void set_autopush(bool value){
        my_player->autopush = value;
    }

    void set_autoplay(bool value){
        my_player->autoplay = value;
    }

    void set_color(char color, int R, int G, int B){
        my_colors->set(color, R, G, B);
    }

    void wait(){
        MyPlayer::instance()->wait();
    }

    void show(){
        MyPlayer::instance()->show();
    }

    void paint(const matchar& mat){
        if(!MyWindow::instance()->isOpen())
            return;
        matview.paint_target_squares(my_painel, mat);
        check_push();
    }

    void paint(const matchar &mat, const matriz<int>& nums){
        if(!MyWindow::instance()->isOpen())
            return;
        matview.paint_target_squares(my_painel, mat);
        matview.paint_target_nums(my_painel, nums);
        check_push();
    }

    void paint(const matchar &mat, const Par & pos, char color){
        matview.paint_target_cell(mat.size(), pos, color);
    }


    void paint_brush(matchar& mat, const string& colors){
        matview.paint_brush(mat, colors);
    }

    Par select_point(const matchar& mat, const string & texto){
        return matview.select_point(mat, texto);
    }
};


#endif // MATRIZVIEW

