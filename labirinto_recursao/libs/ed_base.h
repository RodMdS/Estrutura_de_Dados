//#############################################################################
/*
Neste arquivo você encontrará todas as classes e métodos para o Player

sfLine - Uma Classe para desenhar uma linha
sfText - Uma classe de texto que carrega uma fonte padrão do ubuntu
MyBuffer - Uma classe para gerenciar o buffer de estados
MyColors - Uma classe singleton para gerenciar o mapa de cores
MyWindow - Uma RenderWindow singleton para o projeto
MyPlayer - Contem os singleton player de estados e o RenderBuffer de Desenho
MyPathMaker - Calcula linhas e posições, utilizado para desenhar

*/
//#############################################################################

#ifndef SFLINE_H
#define SFLINE_H

#include <SFML/Graphics.hpp>

class sfLine : public sf::Drawable
{
public:
    sfLine(const sf::Vector2f& point1, const sf::Vector2f& point2,
           float thickness = 1, sf::Color color = sf::Color::Red):
        _begin(point1), _end(point2), _color(color), _thickness(thickness)
    {
        update_vertices();
    }

    sf::Vector2f get_begin(){
        return _begin;
    }

    sf::Vector2f get_end(){
        return _end;
    }

    float get_thickness(){
        return _thickness;
    }

    sf::Color get_color(){
        return _color;
    }

    void set_color(sf::Color color){
        this->_color = color;
        update_vertices();
    }

    void set_end(sf::Vector2f end){
        this->_end = end;
        update_vertices();
    }

    void set_begin(sf::Vector2f begin){
        this->_begin = begin;
        update_vertices();
    }

    void set_thickness(float thickness){
        _thickness = thickness;
        update_vertices();
    }


    void draw(sf::RenderTarget &target, sf::RenderStates states) const
    {
        (void)states;
        target.draw(_vertices,4,sf::Quads);
    }


private:
    sf::Vector2f _begin;
    sf::Vector2f _end;
    sf::Vertex _vertices[4];
    sf::Color _color;
    float _thickness;


    void update_vertices(){
        sf::Vector2f direction = _end - _begin;
        sf::Vector2f unitDirection = direction/std::sqrt(direction.x*direction.x+direction.y*direction.y);
        sf::Vector2f unitPerpendicular(-unitDirection.y,unitDirection.x);

        sf::Vector2f offset = (_thickness/2.f)*unitPerpendicular;

        _vertices[0].position = _begin + offset;
        _vertices[1].position = _end + offset;
        _vertices[2].position = _end - offset;
        _vertices[3].position = _begin - offset;

        for (int i=0; i<4; ++i)
            _vertices[i].color = _color;
    }
};

#endif // SFLINE_H


//####################################################################################

#ifndef SFTEXT_H
#define SFTEXT_H

#include <SFML/Graphics.hpp>
#include <iostream>

class sfText : public sf::Text
{
public:
    sfText(sf::Vector2f pos = sf::Vector2f(0, 0), std::string texto = "", sf::Color color = sf::Color::White, int size = 16)
    {
        this->setFont(*this->get_default_font());
        this->setColor(color);
        this->setPosition(pos);
        this->setString(texto);
        this->setCharacterSize(size);
    }

private:


    static sf::Font * get_default_font(){
        const std::string _path = "/usr/share/fonts/truetype/freefont/FreeMonoBold.ttf";
        static sf::Font font;
        static bool init = true;
        if(init){
            init = false;
            if(!font.loadFromFile(_path))
                std::cerr << "Font " << _path << " nao encontrada." << std::endl;
        }
        return &font;
    }


};

#endif // SFTEXT_H

//###########################################################################

#ifndef MBUFFER_H
#define MBUFFER_H

#include <list>
#include <iostream>

template<typename T>
class MyBuffer
{
private:
    uint _indice;
    uint _first;
    uint _max_size; //maximo a frente ou atras
    typename std::list<T>::iterator _it;
    std::list<T> _list;
public:
    MyBuffer(uint max_size, T first){
        //inserindo uma funcao vazia que nao faz nada so pra inicializar
        //os indices e o vetor
        _list.push_back(first);
        _it = _list.begin();
        _indice = 0;
        _first = 0;
        _max_size = max_size;
    }

    void set_max_size(uint size){
        _max_size = size;
    }

    bool exists(uint indice){
        if(indice >= _first)
            if(indice < _first + _list.size())
                return true;
        return false;
    }

    bool go_to(uint indice){
        if(exists(indice)){
            if(indice > pos_actual()){
                go_foward(indice - pos_actual());
            }
            if(indice < pos_actual()){
                go_back(pos_actual() - indice);
                return true;
            }
            return true;
        }
        return false;
    }

    void push(const T& t){
        _list.push_back(t);
        if(size() > (int)_max_size){
            _list.pop_front();
            _first++;
            if(_indice > 0)
                _indice--;
            else
                _indice = 0;
        }
    }

    bool is_full(){
        return _list.size() >= _max_size;
    }

    int size(){
        return _list.size();
    }

    const T& get_it(){
        return *_it;
    }

    uint pos_first(){
        return _first;
    }

    uint pos_actual(){
        return _first + _indice;
    }

    uint pos_last(){
        return _first + _list.size() - 1;
    }

    //se nao conseguiu andar pra frente eh porque nao conseguiu dar todos
    //os passos por falta de estados
    bool go_foward(uint steps){
        if(_indice + steps < _list.size() ){
            _indice += steps;
            std::advance(_it, steps);
            return true;
        }
        return false;
    }

    void go_last(){
        _indice = _list.size() - 1;
        _it = std::prev(_list.end());
    }

    bool go_back(uint steps){
        if(_indice >= steps){
            _indice -= steps;
            while(steps--)
                _it = std::prev(_it);
            return true;
        }
        return false;
    }
    void go_first(){
        _indice = 0;
        _it = _list.begin();
    }
};

#endif // MBUFFER_H

//#####################################################################

#ifndef COLORMAP_H
#define COLORMAP_H
#include <SFML/Graphics.hpp>

#include <map>
#define my_colors (MyColors::instance())

class MyColors
{
private:
    std::map<char, sf::Color> mapa;

    MyColors(){
        mapa['r'] = sf::Color::Red;
        mapa['g'] = sf::Color::Green;
        mapa['b'] = sf::Color::Blue;
        mapa['y'] = sf::Color::Yellow;
        mapa['c'] = sf::Color::Cyan;
        mapa['m'] = sf::Color::Magenta;
        mapa['w'] = sf::Color::White;
        mapa['k'] = sf::Color::Black;
        mapa['.'] = sf::Color(133, 155, 213);
        mapa['#'] = sf::Color(20, 20, 20);
        mapa[' '] = sf::Color(255, 235, 230);
    }

public:

    static MyColors * instance(){
        static MyColors map;
        return &map;
    }

    std::string color_list(){
        std::string colors;
        char c[2];
        c[1] = '\0';
        for(const std::pair<char, sf::Color>& p : mapa){
            c[0] = p.first;
            colors.append(c);
        }
        return colors;
    }

    void set(char color, int R, int G, int B){
        mapa[color] = sf::Color(R, G, B);
    }

    sf::Color get(char color){
        auto it = mapa.find(color);
        if(it == mapa.end())
            return sf::Color::White;
        return it->second;
    }

    bool exists(char color){
        return (mapa.find(color) != mapa.end());
    }

    //void update(const sf::Keyboard::Key key, char *cor);
    char sf2char(sf::Keyboard::Key key){
        if(key >= sf::Keyboard::A       and key <= sf::Keyboard::Z)
            return key - sf::Keyboard::A + 'a';
        if(key >= sf::Keyboard::Num0    and key <= sf::Keyboard::Num9)
            return key - sf::Keyboard::Num0 + '0';
        if(key >= sf::Keyboard::Numpad0 and key <=sf::Keyboard::Num9)
            return key - sf::Keyboard::Numpad0 + '0';
        return '0';
    }
};

inline MyColors * colormap(){
    return MyColors::instance();
}
#endif // COLORMAP_H

//################################################################

#ifndef MYWINDOW_H
#define MYWINDOW_H

#define my_window (MyWindow::instance())

#include <vector>
#include <SFML/Graphics.hpp>

class MyWindow : public sf::RenderWindow{

private:
    //bool _isVisible{true};

    MyWindow():
        sf::RenderWindow(sf::VideoMode(default_largura, default_altura), "QXCODE ED")
    {
        this->setFramerateLimit(50);
        this->clear();
        this->display();
        sf::sleep(sf::milliseconds(100));
        this->setVisible(false);
    }

public:

    static const int default_altura = 700;
    static const int default_largura = 1000;

    static MyWindow * instance(){
        static MyWindow static_window;
        return &static_window;
    }


    //sobrecarga do setVisible original
//    virtual void setVisible(bool visible){
//        _isVisible = visible;
//        sf::RenderWindow::setVisible(visible);
//    }

    bool processar_close(const sf::Event& evt){
        if(evt.type == sf::Event::Closed){
            this->close();
            return true;
        }
        return false;
    }

    bool processar_resize(const sf::Event& evt){
        if (evt.type == sf::Event::Resized){
            this->setView(sf::View(sf::FloatRect(0, 0, evt.size.width, evt.size.height)));
            return true;
        }
        return false;
    }
};

#endif // MYWINDOW_H

//################################################################################



#ifndef MYPLAYER_H
#define MYPLAYER_H

#include <iostream>
#include <string>
#include <cmath>
#include <list>
#include <queue>

#include <SFML/Graphics.hpp>

//#define my_player (MyPlayer::instance())
//#define my_painel (MyPlayer::instance()->painel)

class MyPlayer{
private:
    sf::Clock _clock;
    MyBuffer<sf::Texture> _buffer;
    bool _state_missing{false};
    bool _waiting{false};
    int _destiny{0};//proximo estado a ser mostrado


    MyPlayer():
        _buffer(100, sf::Texture())
    {
        painel.create(MyWindow::default_largura, MyWindow::default_altura);
    }

public:
    sf::Color color_back{sf::Color::Black};
    sf::Color color_front{sf::Color::White};
    const uint offset_up = 40;
    bool autoplay{false};
    bool autopush{true};
    sf::RenderTexture painel;
    int jump{1}; //define o tamanho do salto


    //Retorna o player
    static MyPlayer * instance(){
        static MyPlayer _player;
        return &_player;
    }

    static sf::RenderTexture * getPainel(){
        return &instance()->painel;
    }


    //Espera a janela ser fechada
    //se estava esperando um salto de varios estados,
    //volta a mostrar o estado atual
    virtual void wait(){
        _waiting = true;
        _show();
        MyWindow::instance()->setVisible(false);
        _waiting = false;
    }

    //Altera o tamanho do buffer
    virtual void set_buffer_size(uint size){
        _buffer.set_max_size(size);
    }

    //Funcao utilizada pelo pintor para salvar os estado
    virtual void show(){
        MyWindow::instance()->setVisible(true);

        if(MyWindow::instance()->isOpen()){
            painel.display();
            _buffer.push(painel.getTexture());
            if(painel.getSize() != MyWindow::instance()->getSize())
                painel.create(MyWindow::instance()->getSize().x, MyWindow::instance()->getSize().y);
            painel.clear(this->color_back);
            _buffer.go_foward(1);
            _show();
        }
    }

private:

    virtual void _show(){
        //salto impossivel, setar para ultima posicao
        if(_waiting && !_buffer.go_to(_destiny)){
            _destiny = _buffer.pos_last();
            _buffer.go_last();
        }

        //esperando estado
        if(!_waiting && !_buffer.go_to(_destiny))
            return;

        _state_missing = false;
        while(MyWindow::instance()->isOpen()){
            if(!_waiting && _state_missing)
                return;
            if(autoplay)
                walk(jump);

            sf::Event evt;
            while(MyWindow::instance()->pollEvent(evt)){
                MyWindow::instance()->processar_resize(evt);
                MyWindow::instance()->processar_close(evt);
                process_controles(evt);
                if(evt.type == sf::Event::KeyPressed){
                    if(evt.key.code == sf::Keyboard::Escape){
                        if(_waiting)
                            return;
                    }
                }
            }

            MyWindow::instance()->clear(color_back);
            show_actual();
            print_label();
            MyWindow::instance()->display();
        }
    }



    void show_actual(){
        auto sprite = sf::Sprite(_buffer.get_it());
        auto wsize = MyWindow::instance()->getSize();
        auto ratiox = wsize.x/(float)sprite.getLocalBounds().width;
        auto ratioy = (wsize.y)/(float)sprite.getLocalBounds().height;
        auto menor = std::min(ratiox, ratioy);
        sprite.setScale(sf::Vector2f(menor, menor));
        MyWindow::instance()->draw(sprite);
    }


    void process_controles(const sf::Event& evt){
        if(evt.type == sf::Event::KeyPressed){
            if(evt.key.code == sf::Keyboard::Right){walk(jump);}
            else if(evt.key.code == sf::Keyboard::Left){walk(-jump); autoplay = false;}
            else if(evt.key.code == sf::Keyboard::Return){autoplay = !autoplay;}
            else if(evt.key.code == sf::Keyboard::Up){if(jump < 500) jump *= 2;}
            else if(evt.key.code == sf::Keyboard::Down){if(jump > 1) jump /= 2;}
        }
    }

    //mostra os comandos na parte superior da tela
    void print_label(){

        std::string title_left = " Jump|First|Atual|Last |";


        char estado[200];
        sprintf(estado,     "%4d |%4d |%4d |%4d |" ,
                jump, _buffer.pos_first(), _buffer.pos_actual(), _buffer.pos_last());

        MyWindow::instance()->draw(sfText(sf::Vector2f(0, 0), title_left, color_front));
        std::string state = estado;
        if(_waiting)
            state += "(Fim. Digite Esc para Sair!)";
        else
            state += "(Processando)";
        MyWindow::instance()->draw(sfText(sf::Vector2f(0, 20), state, color_front));

        std::string title_right = " Step  Move |  Speed  | Autoplay";
        std::string teclas =      " Left/Right | Up/Down | Enter   ";

        std::string autoOp =      "                              ##";

        auto dim = MyWindow::instance()->getSize();
        float width = sfText(sf::Vector2f(0, 0), title_right).getLocalBounds().width;

        MyWindow::instance()->draw(sfText(sf::Vector2f(dim.x - width - 10, 0), title_right, color_front));
        MyWindow::instance()->draw(sfText(sf::Vector2f(dim.x - width - 10, 15), teclas, color_front));

        sf::Color colorAuto = sf::Color::Red;
        if(autoplay)
            colorAuto = sf::Color::Green;
        MyWindow::instance()->draw(sfText(sf::Vector2f(dim.x - width - 10, 15), autoOp, colorAuto));
        MyWindow::instance()->draw(sfLine(sf::Vector2f(0, 40), sf::Vector2f(dim.x, 40), 1, color_front));
    }

    //anda nos estados para frente e para trás chamando métodos do buffer
    void walk(int step){
        int atual = _buffer.pos_actual();
        _destiny = atual + step;
        if(_destiny > (int)_buffer.pos_last())
            if(_waiting)
                _destiny = _buffer.pos_last();
        if(step > 0){
            if(!_buffer.go_to(_destiny))
            {
                _state_missing = true;
                return;
            }
        }else{
            if(!_buffer.go_to(_destiny))
            {
                _buffer.go_first();
            }
        }
    }
};

#endif //MPLAYER_H



#ifndef PATH_H
#define PATH_H

#include <SFML/System/Vector2.hpp>
#include <vector>
using namespace std;
using namespace sf;
class MyPathMaker{
public:
    static vector<Vector2f> make_path(Vector2f a, Vector2f b)
    {
        vector<Vector2f> path;
        if(a.x == b.x && a.y == b.y){
            path.push_back(Vector2f(a.x, a.y));
            return path;
        }

        if( fabs(a.x - b.x) > fabs(a.y - b.y) )
            make_x_path(a, b, path);
        else
            make_y_path(a, b, path);
        return path;
    }

private:
    static void make_y_path(Vector2f a, Vector2f b, vector<Vector2f> &path)
    {
        double x, y = a.y;
        double d=(b.x - a.x)/(b.y - a.y);
        while(1)//controle de parada e incremento no corpo do laco
        {
            x = a.x + (d*(y - a.y));
            //fn(x, y, param);
            path.push_back(Vector2f(x, y));

            if(a.y < b.y)
            {
                if(y >= b.y)
                    return;
                y++;
            }
            if(a.y > b.y)
            {
                if(y <= b.y)
                    return;
                y--;
            }
        }
    }

    static void make_x_path(Vector2f a, Vector2f b, vector<Vector2f> &path)
    {
        double x = a.x, y;
        double d=(b.y - a.y)/(b.x - a.x);
        while(1)
        {
            y = a.y + (d*(x - a.x));
            //fn(x, y, param);
            path.push_back(Vector2f(x, y));

            if(a.x < b.x)
            {
                if(x >= b.x)
                    return;
                x++;
            }
            if(a.x > b.x)
            {
                if(x <= b.x)
                    return;
                x--;
            }
        }
    }
};

#endif // PATH_H

#ifndef TERMINAL_H
#define TERMINAL_H

void get_anything(){
    char c;
    cin >> noskipws >> c;
    //cin.ignore(1000, '\n');
}

void clear() {
    // CSI[2J clears screen, CSI[H moves the cursor to top-left corner
    std::cout << "\x1B[2J\x1B[H";
    //std::cout << "\x1B[H";
}

#endif


#ifndef ED_H
#define ED_H

//BASE FUNCTIONS

//salva o buffer e mostra o novo estado
void ed_show(){
    MyPlayer::instance()->show();
}

//trava o player para nao terminar a simulação
void ed_lock(){
    MyPlayer::instance()->wait();
}


//SETS
//faz a janeja aparecer ou desaparecer
void ed_set_visible(bool value){
    MyWindow::instance()->setVisible(value);
}

//configura o tamanho default da janela
void ed_set_size(int largura, int altura){
    MyWindow::instance()->setSize(sf::Vector2u(largura, altura));
}

//seta o autoplayer pra true ou false
void ed_set_autoplay(bool value){
    MyPlayer::instance()->autoplay = value;
}

//adiciona ou muda uma cor do mapa de cores
void ed_set_color(char color, int R, int G, int B){
    my_colors->set(color, R, G, B);
}
#endif // ED_H
