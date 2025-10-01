#include <iostream>
#include <string>
#include <sstream>
#include <fstream>
#include <ctime>
#include <limits>
#include <algorithm>
#include <conio.h>

#define MAX 1000
using namespace std; 

string getHorarioAtual() {
    time_t agora = time(0);
    tm *ltm = localtime(&agora);

    char buffer[80];
    strftime(buffer, sizeof(buffer), "%d/%m/%Y %H:%M:%S", ltm);

    return string(buffer);
}

struct produtos
{
    string nome; 
    float quanti; 
    double valorV; 
    bool unidade;
};

int lerInteiro(){
    int valor; 
    while (true){
        cin >> valor; 
        if (cin.fail()){
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(),'\n'); //descarta a entrada invalida
            cout << "Entrada invalida, digite um numero: ";
        } else{
            cin.ignore(numeric_limits<streamsize>::max(), '\n'); 
            return valor; 
        }
    }
}

double lerDouble(){
    double valor;
    while(true){
        cin >> valor;
        if(cin.fail()){
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(),'\n');
            cout << "Entrada invalida, digite um numero: ";
        }else{
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            return valor;
        }
    }
}

void setas(char &tecla, int &opc, int maxOpc) {
    tecla = getch();

    if (tecla == 72) { // cima
        opc--;
        if(opc < 0) opc = maxOpc -1; 
    } else if (tecla == 80) { // baixo
        opc++; 
        if(opc >= maxOpc) opc = 0; 
}
}

void salvarProduto(produtos &p){

    ofstream arquivo("produtos.txt", ios::app); 
        if (!arquivo.is_open()){
            cout << "Erro ao abrir o arquivo para salvar" << endl;
            return;
        }
    arquivo << p.nome << ";" << p.valorV << ";" << p.unidade << ";" << p.quanti << endl;
    arquivo.close();

}

void carregarProdutos(produtos p[], int &qnt) {
    ifstream arquivo("produtos.txt");
    if (!arquivo.is_open()) {
        cout << "Arquivo nao encontrado. Comecando com estoque vazio." << endl;
        qnt = 0;
        return;
    }

    string linha;
    qnt = 0;
    while (getline(arquivo, linha)) {
        stringstream ss(linha); 
        string nome, valorStr, unidadeStr, quantiStr;

        if(getline(ss, nome, ';') && getline(ss, valorStr, ';') && getline(ss, unidadeStr, ';') && getline(ss, quantiStr, ';')){
            p[qnt].nome = nome;

            stringstream ss1(valorStr);
            ss1 >> p[qnt].valorV;

            stringstream ss2(unidadeStr);
            ss2 >> p[qnt].unidade;

            stringstream ss3(quantiStr);
            ss3 >> p[qnt].quanti;

            qnt++;
        }
    }

    arquivo.close();
}

bool nomesIguais(string a, string b) {
    transform(a.begin(), a.end(), a.begin(), ::tolower);
    transform(b.begin(), b.end(), b.begin(), ::tolower);
    return a == b;
}

void cadastro(produtos p[], int &qnt){
    string nomeTempor;
    cout << "---CADASTRO PRODUTOS---" << endl;
    cout << "nome do produto: " << endl; 
    //para limpar o buffer
    //cin.ignore(numeric_limits<streamsize>::max(), '\n');
    getline(cin, nomeTempor); //salvar nome dos produtos com espaço 
    
        //verifica se o produto ja foi cadastrado
        for(int i=0; i<qnt; i++){
           if(nomesIguais(p[i].nome, nomeTempor)){
            cout << "produto ja cadastrado."<<endl;
            return;
           }
        }
    int opc = 0;
    char tecla;
    p[qnt].nome = nomeTempor;
    cout << "digite o preco do produto: " << endl;
    p[qnt].valorV = lerDouble();
    do{
        cout << "Este produto é em unidade ou em kg?" << endl;
        cout << (opc == 0 ? ">  " : " ") << "KG\n";
        cout << (opc == 1 ? ">  " : " ") << "UNIDADE\n";

        setas(tecla, opc, 2);
        system("cls");

    }while(tecla != 13);

    p[qnt].unidade = opc;

    cout << "digite a quantidade em estoque: " << endl;
    if(p[qnt].unidade == 0){
        p[qnt].quanti = lerDouble();
    }else if(p[qnt].unidade == 1){
        p[qnt].quanti = lerInteiro();
    };
    
    salvarProduto(p[qnt]);

qnt++;

}

void ListarProdutos(produtos p[], int &qnt){
    cout << "--LISTA DE PRODUTOS--\n";
    cout << "COD // NOME // PRECO // TIPO // QUNATIDADE\n";
    for(int i=0; i<qnt; i++){
        string tipo;
        if(p[i].unidade == 1){
            tipo = "UN";
        }else if(p[i].unidade == 0){
            tipo = "KG";
        }
        cout << "(" << i+1<<") - " << p[i].nome << " -  R$:" << p[i].valorV << " - " << tipo << " - " << p[i].quanti << endl;  
    }
}

void apagarProd(produtos p[], int &qnt) {
    if (qnt == 0) {
        cout << "Nenhum produto cadastrado." << endl;
        return;
    }

    ListarProdutos(p, qnt);

    int indice;
    cout << "Digite o numero do produto que deseja remover: ";
    cin >> indice;

    if (indice < 1 || indice > qnt) {
        cout << "Indice invalido!" << endl;
        return;
    }

    string nomeRemover = p[indice-1].nome;

    ifstream arquivo("produtos.txt");
    ofstream temporario("produtos_temp.txt");

    if (!arquivo.is_open() || !temporario.is_open()) {
        cout << "Erro ao abrir arquivos." << endl;
        return;
    }

    string linha;
    bool removido = false;

    while (getline(arquivo, linha)) {
        size_t pos1 = linha.find_last_of(' ');
        size_t pos2 = linha.find_last_of(' ', pos1 - 1);
        string nomeLinha = linha.substr(0, pos2);

        if (nomesIguais(nomeLinha, nomeRemover)) {
            removido = true;
            continue; // não grava
        }
        temporario << linha << endl;
    }

    arquivo.close();
    temporario.close();

    remove("produtos.txt");
    rename("produtos_temp.txt", "produtos.txt");

    if (removido) {
        cout << "Produto removido com sucesso!" << endl;
        carregarProdutos(p, qnt); // atualiza vetor
    } else {
        cout << "Erro: produto nao encontrado no arquivo!" << endl;
    }
}

void adcCarrinho(produtos p[], int &qnt){
    if(qnt == 0){
        cout << "Nenhum produto no estoque.\n";
        system("pause");
        return; 
    }

    int opc = 0; // índice do produto selecionado
    char tecla;

    // Seleção do produto com setas
    do{
        system("cls");
        string tipo;
        cout << "--- SELECIONE UM PRODUTO ---\n";
        for(int i = 0; i < qnt; i++){
            if(i == opc){
                cout << "> "; 
            }else{
                cout << "  ";
            };
            if(p[i].unidade == 1){
                tipo = "UN";
            }else if(p[i].unidade == 0){
                tipo = "KG";
            }
            cout << p[i].nome << " - R$" << p[i].valorV << " - " << p[i].quanti << " - " << tipo << endl;
        }

        setas(tecla, opc, qnt);

    } while(tecla != 13); // Enter seleciona

    int cod = opc; // produto selecionado
    float quant;
    cout << "Digite a quantidade desejada: ";
    if(p[cod].unidade == 1){
        quant = lerInteiro();
    }else if(p[cod].unidade == 0){
        quant = lerDouble();
        
    }
    if(quant <= 0 || quant > p[cod].quanti){
        cout << "Quantidade invalida.\n";
        system("pause");
        return;
    };
    // Salva no carrinho temporário
    ofstream temp("compra_temp.txt", ios::app);
    temp << p[cod].nome << ";" << p[cod].valorV << ";" << p[cod].unidade << ";" << quant << endl;
    temp.close();

    // Atualiza estoque
    p[cod].quanti -= quant;
    ofstream arquivo("produtos.txt");
    for(int i = 0; i < qnt; i++){
        if(!p[i].nome.empty())
            arquivo << p[i].nome << ";" << p[i].valorV << ";" << p[i].unidade << ";" << p[i].quanti << endl;
    }
    arquivo.close();

    cout << "Produto adicionado ao carrinho!\n";
    system("pause");
}

void finalizarCompra() {
    ifstream temp("compra_temp.txt");
    if (!temp.is_open()) {
        cout << "Nenhuma compra em andamento!\n";
        system("pause");
        return;
    }

    string metodoPagamento;
    cout << "Digite o metodo de pagamento (Dinheiro/Cartao/Pix): ";
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    getline(cin, metodoPagamento);

    ofstream log("historico_vendas.txt", ios::app);
    if (!log.is_open()) {
        cout << "Erro ao abrir log de vendas!\n";
        return;
    }

    log << "===== VENDA FINALIZADA =====\n";
    log << "Data/Hora: " << getHorarioAtual() << "\n";

    string linha;
    double total = 0;
    while (getline(temp, linha)) {
        size_t p1 = linha.find(';');
        size_t p2 = linha.find(';', p1 + 1);

        string nome = linha.substr(0, p1);
        string qtdStr = linha.substr(p1 + 1, p2 - p1 - 1);
        string precoStr = linha.substr(p2 + 1);

        int qtd;
        double preco;

        stringstream ss1(qtdStr);
        ss1 >> qtd;

        stringstream ss2(precoStr);
        ss2 >> preco;

        double subtotal = qtd * preco;
        total += subtotal;

        log << nome << " | " << qtd << " | R$ " << subtotal << "\n";
    }

    log << "Total: R$ " << total << endl;
    log << "Metodo de pagamento: " << metodoPagamento << endl;
    log << "=============================\n\n";

    log.close();
    temp.close();

    remove("compra_temp.txt"); // apaga temporário

    cout << "Compra finalizada com sucesso!\n";
    system("pause");
}

void logVendas(){
    ifstream log("historico_vendas.txt");
    if(!log.is_open()){
        cout << "nenhuma venda registrada ainda\n";
        system("pause");
        return; 
    }

    string linha;
    cout << "======= RELATORIO DE VENDAS =======\n\n";
    while(getline(log, linha)){
        cout << linha << endl;
    }
    cout << "\n===================================\n";

    log.close();
    system("pause");


}

void maioresVendas(){
     ifstream log("historico_vendas.txt");
    if (!log.is_open()) {
        cout << "Nenhuma venda registrada ainda.\n";
        system("pause");
        return;
    }

    string nomes[MAX];
    int qntds[MAX];
    int totalProdutos=0; 

    string linha;
    while(getline(log, linha)){
        if (linha.size() > 2 && linha[0] == '-'){
            string nome;
            int qtd=0;
            stringstream ss(linha);
            char lixo;
            ss>>lixo; // para ignorar o '-'
            getline(ss, nome, '|'); //pega o nome ate '|'
            ss>>qtd; //pega a quantidade

             // remover espaços extras do nome
            while (!nome.empty() && (nome[nome.size() - 1] == ' ' || nome[nome.size() - 1] == '\t')) {
    nome.erase(nome.size() - 1);
}

                 // Verifica se já existe no vetor
                bool encontrado = false;
                for(int i=0; i<totalProdutos; i++){
                    if(nomes[i] == nome){
                        qntds[i] +=qtd; 
                        encontrado = true;
                        break;
                    }
                }

                if(!encontrado && totalProdutos < MAX){

                    nomes[totalProdutos] = nome; 
                    qntds[totalProdutos] = qtd;
                    totalProdutos++;
                }
        }
    }
    
    cout << "\n=== PRODUTOS MAIS VENDIDOS ===\n";
    for (int i = 0; i < totalProdutos; i++) {
        cout << nomes[i] << " -> " << qntds[i] << " unidades vendidas\n";
    }
    cout << "==============================\n";

    log.close();
    system("pause");

}

void produtosEmFalta(produtos p[], int &qnt){
    bool faltando = false;
    cout << "\nDigite a quantidade minima que quer consultar: ";
    int qntMin = lerInteiro();  

    // Vetor auxiliar só com os produtos filtrados
    produtos filtrados[MAX];
    int qntFiltrados = 0;

    for (int i = 0; i < qnt; i++) {
        if (p[i].quanti <= qntMin) {
            filtrados[qntFiltrados] = p[i];
            qntFiltrados++;
            faltando = true;
        }
    }

    if (!faltando) {
        cout << "✅ O estoque está abastecido.\n";
        system("pause");
        return;
    }

    // Ordenar os filtrados por quantidade (Bubble Sort)
    for (int i = 0; i < qntFiltrados - 1; i++) {
        for (int j = 0; j < qntFiltrados - i - 1; j++) {
            if (filtrados[j].quanti > filtrados[j+1].quanti) {
                produtos temp = filtrados[j];
                filtrados[j] = filtrados[j+1];
                filtrados[j+1] = temp;
            }
        }
    }

    // Mostrar os produtos em falta
    cout << "\n-- PRODUTOS EM FALTA (ordenados) --\n";
    cout << "nome // preco // quantidade\n";
    for (int i = 0; i < qntFiltrados; i++) {
        cout << filtrados[i].nome 
             << " - R$ " << filtrados[i].valorV 
             << " - " << filtrados[i].quanti << endl;
    }

    cout << "=========================\n";
    system("pause");
}


void MenuRelatorios(produtos p[], int qnt){
        int opc = 1;
        char tecla;
        bool sair = false;
while(!sair){
    system("cls");
    cout << "                                  MNRLT-02\n";
    cout << "---MENU RELATORIOS---\n";
    cout << (opc == 1 ? ">  " : " ") << "Log Vendas\n";
    cout << (opc == 2 ? ">  " : " ") << "Produtos mais vendidos\n";
    cout << (opc == 3 ? ">  " : " ") << "Produtos em falta\n";
    cout << (opc == 0 ? ">  " : " ") << "Voltar";

    setas(tecla, opc, 4);
    if(tecla == 13){
    switch(opc){

        case 1: logVendas(); break;
        case 2: maioresVendas(); break;
        case 3: produtosEmFalta(p, qnt); break;
        case 0: sair=true; break;
            }
        }
    } 
    
}

void MenuVendas(produtos p[], int &qnt){
    char tecla; 
    int opc = 1;
    bool sair=false; 
while(!sair){
    system("cls");
    cout << "                                  MNPDV-02\n";
    cout << "---MENU VENDAS---\n";
    cout << (opc == 1 ? ">  " : " ") << "Adcionar ao carrinho\n";
    cout << (opc == 2 ? ">  " : " ") << "Finalizar compra \n";
    cout << (opc == 0 ? ">  " : " ") << "Voltar\n";
    
    setas(tecla, opc, 3);

    if(tecla == 13){
       switch(opc){

        case 1: adcCarrinho(p, qnt); break;
        case 2: finalizarCompra(); break;
        case 0: sair = true; break;
            }
        }
    }
}

void MENUProdutos(produtos p[], int &qnt){
    int opc=1;
    char tecla; 
    bool sair=false;
while(!sair){
    system("cls");
    cout << "                                  MNSPP-02\n";
    cout << "---MENU PRODUTOS---\n";
    cout << (opc == 1 ? ">  " : " ") <<"Cadastrar produtos\n";
    cout << (opc == 2 ? ">  " : " ")<< "Listar produtos \n";
    cout << (opc == 3 ? ">  " : " ")<< "Remover item \n";
    cout << (opc == 0 ? ">  " : " ")<< "Voltar     \n";
    
    setas(tecla, opc, 4);
    if(tecla == 13){
       switch(opc){

        case 1: cadastro(p, qnt); system("pause"); break;
        case 2: ListarProdutos(p, qnt); system("pause"); break;
        case 3: apagarProd(p, qnt); system("pause"); break;
        case 0: sair = true;break;
        default: cout << "Opcao invalida\n"; system("pause"); break;
            }
        }
    };
}

void MENU(int &opc){
    char tecla;
    opc = 1;

    do{

    system("cls");
    cout << "                                MNSP-01\n";
    cout << "---MENU SUPERMERCADO---\n";
    cout << (opc == 1 ? ">  " : " ") << "Produtos\n";
    cout << (opc == 2 ? ">  " : " ") << "Vendas\n";
    cout << (opc == 3 ? ">  " : " ") << "Relatorios\n";
    cout << (opc == 0 ? ">  " : " ") << "Sair\n";
    
    setas(tecla, opc, 4);
    }while (tecla != 13); 
}

int main(){
    produtos v[MAX];
    int qnt=0;
    int opc; 
    
    carregarProdutos(v, qnt);

    do{
        MENU(opc); 

    switch (opc){
        case 1: MENUProdutos(v, qnt); break;
        case 2: MenuVendas(v, qnt); break;
        case 3: MenuRelatorios(v, qnt); break;
        case 0:
            cout << "saindo..." <<endl; 
            break;
        default: 
            cout << "opcao invalida."<<endl;
        }
    }while(opc !=0);

    return 0;
}