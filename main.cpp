#include <iostream>
#include <list>

using namespace std;

// Mostrar a lista de candidatos
void mostrar(list<int> lista){
    for(auto num : lista)
        cout << num << " | ";

    cout << endl;
}

int main() {
    int pretendentes = 0, escolhido = 0;

    cout << "Digite a quantidade de pretendentes da princesa: " << endl;
    cin >> pretendentes;

    cout << "Digite o número do pretendente que inicia o jogo: " << endl;
    cin >> escolhido;

    list<int> lista; // criando uma lista

    // preencher a lista com a quantidade de pretendentes
    for(int i = 0; i < pretendentes; i++){
        lista.push_back(i + 1);
    }

    // mostrar(lista); // exibir os pretendentes

    // colocar o pretendente escolhido pela princesa como o primeiro da lista
    while(lista.front() != escolhido){
        lista.push_back(lista.front());
        lista.pop_front();
    }

    // mostrar e matar os pretendentes, seguindo a lógica do problema
    while(lista.size() > 1){
        mostrar(lista);
        lista.push_back(lista.front());
        lista.pop_front();
        lista.pop_front();
    }

    mostrar(lista);

    return 0;
}

