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

int lerInt(string msg = "Digite um valor: ") {
    string entrada;
    int valor;

    while(true) {
        cout << msg;
        getline(cin, entrada);

        stringstream ss(entrada);
        if (ss >> valor) {  // tenta extrair um inteiro
            char resto;
            if (!(ss >> resto)) {  // verifica se não há caracteres extras
                return valor;      // entrada válida
            }
        }

        cout << "Entrada invalida! Digite apenas numeros inteiros.\n";
    }
}

void salvarProduto(produtos &p){

    ofstream arquivo("produtos.txt", ios::app); 
        if (!arquivo.is_open()){
            cout << "Erro ao abrir o arquivo para salvar" << endl;
            return;
        }
    arquivo << p.nome << " " << p.valorV << " " << p.quanti << endl;
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
        size_t pos1 = linha.find_last_of(' '); //Procura a posição do último espaço na linha
        size_t pos2 = linha.find_last_of(' ', pos1 - 1); //Procura o penúltimo espaço, ou seja, o espaço antes do preço

        p[qnt].nome = linha.substr(0, pos2); //Pega os caracteres do início da linha até o penúltimo espaço pos2
        p[qnt].valorV = atof(linha.substr(pos2 + 1, pos1 - pos2 - 1).c_str()); //pega os caracteres entre o penúltimo e o último espaço, ou seja, o preço do produto
        p[qnt].quanti = atof(linha.substr(pos1 + 1).c_str()); // pega todos os caracteres após o último espaço, que é a quantidade em estoque.
        qnt++;
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
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    getline(cin, nomeTempor); //salvar nome dos produtos com espaço 
    
        //verifica se o produto ja foi cadastrado
        for(int i=0; i<qnt; i++){
           if(nomesIguais(p[i].nome, nomeTempor)){
            cout << "produto ja cadastrado."<<endl;
            return;
           }
        }
    
    p[qnt].nome = nomeTempor;
    cout << "digite o preco do produto: " << endl; 
    cin  >> p[qnt].valorV; 
    cout << "digite a quantidade em estoque: " << endl; 
    cin >> p[qnt].quanti; 
    
    salvarProduto(p[qnt]);

qnt++;

}

void ListarProdutos(produtos p[], int &qnt){
    cout << "--LISTA DE PRODUTOS--\n";
    cout << "cod // nome // preco //     quantidade\n";
    for(int i=0; i<qnt; i++){
        cout << "(" << i+1<<") - " << p[i].nome << " -  R$:" << p[i].valorV << " - " << p[i].quanti << endl;  
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

void MENU(int &opc){
    system("cls");
    cout << "                                MNSP-01\n";
    cout << "---MENU SUPERMERCADO---\n";
    cout << "1- Produtos      \n";
    cout << "2- Vendas         \n";
    cout << "3- Relatorios     \n";
    cout << "0- Sair            \n";
    cout << " ESCOLHA UMA OPCAO \n";
    opc = lerInteiro();

}

void adcCarrinho(produtos p[], int &qnt){

    if(qnt==0){
        cout << "nenhum produto no estoque.";
        system("pause");
        return; 
    }
    ListarProdutos(p, qnt);
        int cod, quant;
        cout << "digite o codigo do produto para adcionar no carrinho: "<<endl;
        cin >> cod;

        if(cod <1 || cod> qnt){
            cout << "codigo invalido.";
            system("pause");
            return;
        }

        cout << "digite a quantidade desejada. ";
        cin >> quant; 

        if (quant <=0 || quant > p[cod - 1].quanti){
            cout << "quantidade invalida. ";
            system("pause");
            return; 
        }
    ofstream temp("compra_temp.txt", ios::app);
    if(!temp.is_open()){
        cout << "erro ao abrir arquivo temporario. ";
        return;
    }

    temp << p[cod - 1].nome << ";" << quant << ";" << p[cod - 1].valorV << endl;
    temp.close();

    p[cod-1].quanti -=quant;

    ofstream arquivo("produtos.txt");
for (int i = 0; i < qnt; i++) {
    if (!p[i].nome.empty()) { // só grava se o nome não for vazio
        arquivo << p[i].nome << " " << p[i].valorV << " " << p[i].quanti << endl;
    }
}
arquivo.close();

        cout << "produto adcionado ao carrinho. ";
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

void MenuRelatorios(){
        int opc;
do{
    system("cls");
    cout << "                                  MNRLT-02\n";
    cout << "---MENU RELATORIOS---\n";
    cout << "1- log vendas\n";
    cout << "2- produtos mais vendidos \n";
    cout << "0- para voltar     \n";
    cout << " ESCOLHA UMA OPCAO \n";
    opc = lerInteiro();

    switch(opc){

        case 1: logVendas(); break;
        case 2: maioresVendas(); break;
        case 0: break;
        default: cout << "Opcao invalida\n"; system("pause"); break;
    
    }
}while(opc != 0); 

}

void MenuVendas(produtos p[], int &qnt){
    int opc;
do{
    system("cls");
    cout << "                                  MNPDV-02\n";
    cout << "---MENU VENDAS---\n";
    cout << "1- adcionar ao carrinho\n";
    cout << "2- Finalizar compra \n";
    cout << "0- para voltar     \n";
    cout << " ESCOLHA UMA OPCAO \n";
    opc = lerInteiro();

    switch(opc){

        case 1: adcCarrinho(p, qnt); break;
        case 2: finalizarCompra(); break;
        case 0: break;
        default: cout << "Opcao invalida\n"; system("pause"); break;
    
    }
}while(opc != 0); 
}

void MENUProdutos(produtos p[], int &qnt){
int opc;
do{
    system("cls");
    cout << "                                  MNSPP-02\n";
    cout << "---MENU PRODUTOS---\n";
    cout << "1- Cadastrar produtos\n";
    cout << "2- Listar produtos \n";
    cout << "3- remover item \n";
    cout << "0- para voltar     \n";
    cout << " ESCOLHA UMA OPCAO \n";
    cin >> opc; 

    switch(opc){

        case 1: cadastro(p, qnt); system("pause"); break;
        case 2: ListarProdutos(p, qnt); system("pause"); break;
        case 3: apagarProd(p, qnt); system("pause"); break;
        case 0: break;
        default: cout << "Opcao invalida\n"; system("pause"); break;
    
    }
}while(opc != 0); 
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
        case 3: MenuRelatorios(); break;
        case 0:;
            cout << "saindo..." <<endl; 
            break;
        default: 
            cout << "opcao invalida."<<endl;
        }
    }while(opc !=0);

    return 0;
}