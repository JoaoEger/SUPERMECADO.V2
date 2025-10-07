#include <iostream>
#include <string>//string, getline
#include <sstream>//stringstream
#include <fstream>//ifstream, ofstream
#include <ctime>//time_t, tm, localtime, strftime
#include <limits>//numeric_limits
#include <algorithm>//transform
#include <conio.h>//getch()
using namespace std; 

#define MAX 1000
//Adiciona uma quantidade de meses a uma data e retorna a nova data com os meses adicionados
tm AdicionarMeses(tm data, int meses){
    int ano = data.tm_year;
    int mes = data.tm_mon + meses;

    while(mes > 11){
        mes -= 12;
        ano += 1;
    }

    data.tm_mon = mes;
    data.tm_year = ano;
    return data;
}
//Retorna a data e hora atual formatadas como string (dd/mm/aaaa hh:mm:ss)
string GetHorarioAtual() {
    time_t agora = time(0);
    tm *ltm = localtime(&agora);

    char buffer[80];
    strftime(buffer, sizeof(buffer), "%d/%m/%Y %H:%M:%S", ltm);

    return string(buffer);
}
//Struct referente aos produtos cadastrados no sistema
struct Produtos
{
    string nome; 
    float quanti; 
    double valorV; 
    bool unidade;
};
//Lê e retorna um número inteiro validando a entrada
int LerInteiro(){
    int valor; 
    while (true){
        cin >> valor; 
        if (cin.fail() || cin.peek() == '.'){
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(),'\n');
            cout << "Entrada invalida, digite um numero inteiro: ";
            continue;
        }
        if(valor <= 0){
            cout << "Entrada invalida, nao pode ser zero ou menor que zero: ";
            continue;
        }
            cin.ignore(numeric_limits<streamsize>::max(), '\n'); 
            return valor; 
    }
}
//Lê e retorna um número double validando a entrada
double LerDouble(){
    double valor;
    while(true){
        cin >> valor;
        if(cin.fail()){
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(),'\n');
            cout << "Entrada invalida, digite um numero: ";
            continue;
        }

        if(valor <= 0){
            cout << "Entrada invalida, nao pode ser zero ou menor que zero: ";
            continue;
        }
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            return valor;
    }
}
//Lê a tecla apertada, e atualiza a opc referente ao valor da tecla
void Setas(char &tecla, int &opc, int maxOpc) {
    tecla = getch();

    if (tecla == 72) { // cima
        opc--;
        if(opc < 0) opc = maxOpc -1; 
    } else if (tecla == 80) { // baixo
        opc++; 
        if(opc >= maxOpc) opc = 0; 
}
}
//Salva o produto no arquivo "produtos.txt"
void SalvarProduto(Produtos &p){

    ofstream arquivo("produtos.txt", ios::app); 
        if (!arquivo.is_open()){
            cout << "Erro ao abrir o arquivo para salvar" << endl;
            return;
        }
    arquivo << p.nome << ";" << p.valorV << ";" << p.unidade << ";" << p.quanti << endl;
    arquivo.close();

}
//Carrega os produtos do arquivo "produtos.txt" para o vetor p[]
void CarregarProdutos(Produtos p[], int &qnt) {
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
// Compara nomes de produtos ignorando maiúsculas e minúsculas
bool NomesIguais(string a, string b) {
    transform(a.begin(), a.end(), a.begin(), ::tolower);
    transform(b.begin(), b.end(), b.begin(), ::tolower);
    return a == b;
}
//Cadastra um novo produto ou atualiza um existente, validando nome, preço, tipo e quantidade
void Cadastro(Produtos p[], int &qnt){
    string nomeTempor;
    cout << "---CADASTRO PRODUTOS---" << endl;
    cout << "nome do produto: " << endl; 

    getline(cin, nomeTempor); //salvar nome dos produtos com espaço

    nomeTempor.erase(0, nomeTempor.find_first_not_of(' '));
    nomeTempor.erase(nomeTempor.find_last_not_of(' ') + 1);

    if(nomeTempor.empty()){
    cout << "Erro, o nome nao pode ficar vazio"<< endl;
    return; 
    }
        //verifica se o produto ja foi cadastrado
        for(int i=0; i<qnt; i++){
            if(NomesIguais(p[i].nome, nomeTempor)){
                int opc = 0;
                char tecla;
                do{
                    system("cls");
                    cout << "Produto ja cadastrado."<<endl;
                    cout << "Deseja atualizar o valor ou quantiadade?" << endl;
                    cout << (opc == 0 ? ">  " : " ") << "Nao" << endl;
                    cout << (opc == 1 ? ">  " : " ") << "Sim" << endl;

                    Setas(tecla, opc, 2);
                    system("cls");
                }while(tecla != 13);
                    if(opc == 1){
                        cout << "Digiter o preco do produto:" << endl;
                        p[i].valorV = LerDouble();

                        cout << "Digite a quantidade em estoque:" << endl;
                        if(p[i].unidade == 0){
                            p[i].quanti = LerDouble();

                        }else if(p[i].unidade == 1){
                            p[i].quanti = LerInteiro();

                        }
                    }
            return;
           }
        }
    int opc = 0;
    char tecla;
    p[qnt].nome = nomeTempor;
    cout << "Digite o preco do produto: " << endl;
    p[qnt].valorV = LerDouble();

    do{
        cout << "Este produto é em unidade ou em kg?" << endl;
        cout << (opc == 0 ? ">  " : " ") << "KG\n";
        cout << (opc == 1 ? ">  " : " ") << "UNIDADE\n";

        Setas(tecla, opc, 2);
        system("cls");

    }while(tecla != 13);

    p[qnt].unidade = opc;

    cout << "Digite a quantidade em estoque: " << endl;
    if(p[qnt].unidade == 0){
        p[qnt].quanti = LerDouble();
    }else if(p[qnt].unidade == 1){
        p[qnt].quanti = LerInteiro();
    };
    
    SalvarProduto(p[qnt]);

qnt++;

}
//Exibe a lista completa de produtos cadastrados com código, nome, preço, quantidade e tipo
void ListarProdutos(Produtos p[], int &qnt){
    cout << "--LISTA DE PRODUTOS--\n";
    cout << "COD | NOME | PRECO | QUANTIDADE | TIPO\n";
    for(int i=0; i<qnt; i++){
        string tipo;
        if(p[i].unidade == 1){
            tipo = "UN";
        }else if(p[i].unidade == 0){
            tipo = "KG";
        }
        cout << "(" << i+1<<") | " << p[i].nome << " |  R$:" << p[i].valorV << " | " << p[i].quanti << " | " << tipo << endl;  
    }
}
//Remove um produto do arquivo apenas com a confirmação do usuário
void ApagarProd(Produtos p[], int &qnt) {
    if (qnt == 0) {
        cout << "Nenhum produto cadastrado." << endl;
        system("pause");
        return;
    }       
    int opc=0;
    char tecla;
    bool sair= false; 

    while(!sair){
        system("cls");
        cout << "--- REMOVER PRODUTO ---\n\n";

        for(int i=0; i<qnt;i++){
            cout << (opc == i ? ">  " : "  ");
            string tipo = (p[i].unidade ? "UN" : "KG");
            cout << p[i].nome << " - R$" << p[i].valorV << " - " << p[i].quanti << " " << tipo << endl;
        }
        cout << (opc == qnt ? ">  " : "  ") << "Voltar\n";

        Setas(tecla, opc, qnt +1);
        
        if(tecla == 13){
            if(opc==qnt){
                sair = true;
                break;
            }

            int confirmar = 0;
            char teclaConfirmar;
            do{
                system("cls");
                cout << "Tem certeza que deseja excluir '" << p[opc].nome << "'?\n\n";
                cout << (confirmar == 0 ? ">  " : "  ") << "Sim\n";
                cout << (confirmar == 1 ? ">  " : "  ") << "Nao\n";

                Setas(teclaConfirmar, confirmar, 2);
            } while(teclaConfirmar != 13);
            if(confirmar == 0){
                string nomeRemover = p[opc].nome;
                ifstream arquivo("produtos.txt");
                ofstream temporario("produtos_temp.txt");

                if(!arquivo.is_open() || !temporario.is_open()){
                    cout << "Erro ao abrir os arquivos" << endl;
                    system("pause");
                    return;
                }
                string linha;
                bool removido = false;

                while(getline(arquivo, linha)){
                stringstream ss(linha);
                string nome, valorStr, unidadeStr, quantiStr;
                    getline(ss, nome, ';');
                    getline(ss, valorStr, ';');
                    getline(ss, unidadeStr, ';');
                    getline(ss, quantiStr, ';');

                if(NomesIguais(nome, nomeRemover)){
                    removido = true;
                    continue;
                }
                temporario<<linha<<endl;
            }
            arquivo.close();
            temporario.close(); 

            remove("produtos.txt");
            rename("produtos_temp.txt", "produtos.txt");

            if(removido){
                cout << "Produto removido com sucesso\n";
                system("pause");
                CarregarProdutos(p,qnt);
            } else{
                cout << "Erro: produto nao encontrado\n";
                system("pause");
                }
            }
        }
    }
}
//Adiciona um produto ao carrinho, atualizando a quantidade em estoque e salvando em um arquivo temporario
void AdcCarrinho(Produtos p[], int &qnt){
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

        Setas(tecla, opc, qnt);

    } while(tecla != 13); // Enter seleciona

    int cod = opc; // produto selecionado
    float quant;
    cout << "Digite a quantidade desejada: ";
    if(p[cod].unidade == 1){
        quant = LerInteiro();
    }else if(p[cod].unidade == 0){
        quant = LerDouble();
    }
    if(quant > p[cod].quanti){
        cout << "Quantidade invalida.\n";
        system("pause");
        return;
    };
    //Salva no carrinho temporário
    ofstream temp("compra_temp.txt", ios::app);
    temp << p[cod].nome << ";" << p[cod].valorV << ";" << p[cod].unidade << ";" << quant << endl;
    temp.close();

    //Atualiza estoque
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
//Exibe todos os itens presentes no carrinho temporário, mostrando subtotal e total parcial
void VerCarrinho(){
  ifstream temp("compra_temp.txt");
  if(!temp.is_open()){
    cout << "Carrinho vazio.\n";
    system("pause");
    return; 
  }

  string linha;
  double total=0;
  cout << "\n--- ITENS NO CARRINHO ---\n";
  while(getline(temp, linha)){
    stringstream ss(linha);
    string nome, precoStr, unidadeStr, qtdStr;

        getline(ss, nome, ';');
        getline(ss, precoStr, ';');
        getline(ss, unidadeStr, ';');
        getline(ss, qtdStr, ';');

        double preco, qtd;
        stringstream(precoStr) >> preco;
        stringstream(qtdStr) >> qtd; 

        double subtotal = preco * qtd;
        total += subtotal;

        cout << nome << " | " << qtd << " | x R$" << preco << " = R$" << subtotal << endl;
  }
  cout << "-----------------------------\n";
  cout << "Total parcial: R$ " << total << endl;

  temp.close();
  system("pause"); 
}
//Remove itens do carrinho temporário
void RemoverDoCarrinho(){

    ifstream temp("compra_temp.txt");
    if(!temp.is_open()){
        cout << "Carrinho vazio\n";
        system("pause");
        return;
    }

    struct itemCarrinho{
        string nome;
        double preco;
        bool unidade;
        double quant;
    };

    itemCarrinho carrinho[MAX]; 
    int qnt = 0;
    string linha;

    while(getline(temp, linha)){
        stringstream ss(linha);
        string nome, precoStr, unidadeStr, qtdStr;

        getline(ss, nome, ';');
        getline(ss, precoStr, ';');
        getline(ss, unidadeStr, ';');
        getline(ss, qtdStr, ';');

        stringstream(precoStr) >> carrinho[qnt].preco;
        stringstream(unidadeStr) >> carrinho[qnt].unidade;
        stringstream(qtdStr) >> carrinho[qnt].quant;
        carrinho[qnt].nome = nome;
        qnt++;
    }
    temp.close();

    if(qnt==0){
        cout << "Carrinho vazio\n";
        system("pause");
        return;
    }
    int opc=0;
    char tecla;
    bool sair = false;

    while(!sair){
        system("cls");
        cout << "--- REMOVER ITEM DO CARRINHO ---\n";
        for(int i = 0; i < qnt; i++){
            cout << (opc == i ? "> ": "  ");
            cout << carrinho[i].nome << " - R$" << carrinho[i].preco << " x " << carrinho[i].quant << endl; 
        }
        cout << (opc == qnt ? "> " : "  ") << "Voltar\n";

        Setas(tecla, opc, qnt+1);

        if(tecla == 13){
            if(opc==qnt){
                sair = true;
            } else{
                for(int j = opc; j < qnt-1; j++) 
                carrinho[j] = carrinho [j+1];
                qnt--;
                cout << "Item removido\n";
                system("pause");
                if(qnt==0) sair=true;
            }
        }
    }

    ofstream tempOut("compra_temp.txt");
    for(int i=0;i<qnt;i++)
    tempOut << carrinho[i].nome << ";" << carrinho[i].preco << ";" << carrinho[i].unidade << ";" << carrinho[i].quant << endl;
    tempOut.close();
}
//Permite ao usuário escolher a forma de pagamento (à vista com desconto ou parcelado)
int MetodoPagamento(){
    int opc=0;
    char tecla;
    string metodos[] = {"A vista (5% de desconto)","Parcelado em ate 12x"};

    int total=2; 

    do{
        system("cls");
        cout << "--- FORMA DE PAGAMENTO ---\n\n";
            for(int i=0; i<total;i++){
                cout << (opc==i ? "> " : "  ") << metodos[i] << endl;
            }

        Setas(tecla,opc, total);
    } while(tecla !=13);

    return opc; 
}
//Calcula total, aplica desconto ou juros, gera parcelas, registra no histórico e limpa carrinho
void FinalizarCompra() {
    ifstream temp("compra_temp.txt");
    if (!temp.is_open()) {
        cout << "Nenhuma compra em andamento!\n";
        system("pause");
        return;
    }
    
    string linha;
    double total = 0;

    cout << "\n--- ITENS DA COMPRA ---\n";

    while (getline(temp, linha)) {
       stringstream ss(linha);
        string nome, precoStr, unidadeStr, qtdStr;
        double preco, qtd;

        getline(ss, nome, ';');
        getline(ss, precoStr, ';');
        getline(ss, unidadeStr, ';');
        getline(ss, qtdStr, ';');

        stringstream(precoStr) >> preco;
        stringstream(qtdStr) >> qtd;

        double subtotal = qtd * preco;
        total += subtotal;


        cout << nome << " | " << qtd << " x R$" << preco << " = R$" << subtotal << "\n";
    }
    temp.close();
    
    int forma = MetodoPagamento();

    double valorFinal;
    int parcelas = 1;

    if(forma == 0){
        valorFinal = total * 0.95;
        parcelas = 1;
    }else if(forma == 1){
        cout << "Digite o numero de parcelas (ate 3x sem juros, apos isso, ate em 12x com 10% de juros):" << endl;
        parcelas = LerInteiro();
        if(parcelas < 1) parcelas = 1;
        if(parcelas > 12) parcelas = 12;
        if(parcelas >= 1 && parcelas <= 3) valorFinal = total;
        if(parcelas >= 4 && parcelas <= 12) valorFinal = total * 1.10;
    }

    double valorParcela = valorFinal / parcelas;
    time_t agora = time(0);
    tm *ltm = localtime(&agora);
    if(forma == 1){
        cout << "\n--- RESUMO DO PAGAMENTO ---\n";
        cout << "Total original: R$ " << total <<endl;
        cout << "Total final: R$ " << valorFinal << endl;
        cout << "Parcelas: " << parcelas << "x de R$ " << valorParcela << endl;
        cout << "------------------------------\n";
        for (int i = 0; i < parcelas; i++) {
            tm vencimento = AdicionarMeses(*ltm, i);
            char buffer[20];
            strftime(buffer, sizeof(buffer), "%d/%m/%Y", &vencimento);
            cout << "Parcela " << i+1 << ": R$ " << valorParcela << " - Vencimento: " << buffer << endl;
        }
    }else{
        cout << "\n--- RESUMO DO PAGAMENTO ---\n";
        cout << "Total original: R$ " << total <<endl;
        cout << "Total final: R$ " << valorFinal << endl;
        cout << "------------------------------\n";
    }

    ofstream log("historico_vendas.txt", ios::app);
    log << "===== VENDA FINALIZADA =====\n";
    log << "Data/Hora: " << GetHorarioAtual() << "\n";
    log << "Total original: R$ " << total << "\n";
    log << "Total final: R$ " << valorFinal << "\n";
    log << "Parcelas: " << parcelas << "x de R$ " << valorParcela << "\n";
    
    for (int i = 0; i < parcelas; i++) {
        tm vencimento = AdicionarMeses(*ltm, i);
        char buffer[20];
        strftime(buffer, sizeof(buffer), "%d/%m/%Y", &vencimento);
        log << "Parcela " << i+1 << ": R$ " << valorParcela << " - Vencimento: " << buffer << "\n";
    }
    log << "=============================\n\n";
    log.close();

    remove("compra_temp.txt"); // limpa carrinho
    cout << "\nCompra finalizada com sucesso!\n";
    system("pause");
}
//Exibe o histórico completo de vendas registradas no arquivo
void LogVendas(){
    ifstream log("historico_vendas.txt");
    if(!log.is_open()){
        cout << "Nenhuma venda registrada ainda\n";
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
//Exibe os produtos mais vendidos, contabilizando a quantidade total de cada item registrado no histórico
void MaioresVendas() {
    ifstream log("historico_vendas.txt");
    if (!log.is_open()) {
        cout << "Nenhuma venda registrada ainda.\n";
        system("pause");
        return;
    }

    string nomes[MAX];
    int qntds[MAX];
    int totalProdutos = 0;

    string linha;
    while (getline(log, linha)) {
        if (linha.find(" | ") != string::npos && linha.find("R$") != string::npos) {
            stringstream ss(linha);
            string nome, qtdStr, lixo;
            double subtotal;
            int qtd;

            getline(ss, nome, '|'); 
            getline(ss, qtdStr, '|'); 
            ss >> lixo >> subtotal; 

            while (!nome.empty() && (nome[0] == ' ' || nome[0] == '\t'))
                nome.erase(0, 1);
            while (!nome.empty() && (nome.back() == ' ' || nome.back() == '\t'))
                nome.pop_back();

            stringstream ssQtd(qtdStr);
            ssQtd >> qtd;

            bool encontrado = false;
            for (int i = 0; i < totalProdutos; i++) {
                if (nomes[i] == nome) {
                    qntds[i] += qtd;
                    encontrado = true;
                    break;
                }
            }

            if (!encontrado && totalProdutos < MAX) {
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
//Lista produtos onde o estoque está abaixo de uma quantidade mínima fornecida
void ProdutosEmFalta(Produtos p[], int &qnt){
    bool faltando = false;
    cout << "\nDigite a quantidade minima que quer consultar: ";
    int qntMin = LerInteiro();
    string unidade;

    // Vetor auxiliar só com os produtos filtrados
    Produtos filtrados[MAX];
    int qntFiltrados = 0;

    for (int i = 0; i < qnt; i++) {
        if (p[i].quanti <= qntMin) {
            filtrados[qntFiltrados] = p[i];
            qntFiltrados++;
            faltando = true;
        }
    }

    if (!faltando) {
        cout << "O estoque esta abastecido.\n";
        system("pause");
        return;
    }

    //Ordenar os filtrados por quantidade (Bubble Sort)
    for (int i = 0; i < qntFiltrados - 1; i++) {
        for (int j = 0; j < qntFiltrados - i - 1; j++) {
            if (filtrados[j].quanti > filtrados[j+1].quanti) {
                Produtos temp = filtrados[j];
                filtrados[j] = filtrados[j+1];
                filtrados[j+1] = temp;
            }
        }
    }

    //Mostrar os produtos em falta
    cout << "\n-- PRODUTOS EM FALTA (ordenados) --\n";
    cout << "NOME | PRECO | QUANTIDADE | UNIDADE\n";
    for (int i = 0; i < qntFiltrados; i++) {
        if(filtrados[i].unidade == 0){
            unidade = "KG";
        }else if(filtrados[i].unidade == 1){
            unidade = "UN";
        }
        cout << filtrados[i].nome << " | R$ " << filtrados[i].valorV << " | " << filtrados[i].quanti << " | " << unidade <<endl;
    }

    cout << "=========================\n";
    system("pause");
}
//Menu interativo para acessar relatório de venda e estoque
void MenuRelatorios(Produtos p[], int qnt){
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

    Setas(tecla, opc, 4);
    if(tecla == 13){
    switch(opc){

        case 1: LogVendas(); break;
        case 2: MaioresVendas(); break;
        case 3: ProdutosEmFalta(p, qnt); break;
        case 0: sair=true; break;
            }
        }
    } 
    
}
//Menu interativo para adicionar ao carrinho, visualizar, finalizar ou remover itens
void MenuVendas(Produtos p[], int &qnt){
    char tecla; 
    int opc = 1;
    bool sair = false; 
while(!sair){
    system("cls");
    cout << "                                  MNPDV-02\n";
    cout << "---MENU VENDAS---\n";
    cout << (opc == 1 ? ">  " : " ") << "Adcionar ao carrinho\n";
    cout << (opc == 2 ? ">  " : " ") << "Visualizar carrinho\n";
    cout << (opc == 3 ? ">  " : " ") << "Finalizar compra\n";
    cout << (opc == 4 ? ">  " : " ") << "Remover item do carrinho\n";
    cout << (opc == 0 ? ">  " : " ") << "Voltar\n";
    
    Setas(tecla, opc, 5);

    if(tecla == 13){
       switch(opc){

        case 1: AdcCarrinho(p, qnt); break;
        case 2: VerCarrinho(); break;
        case 3: FinalizarCompra(); break;
        case 4: RemoverDoCarrinho(); break;
        case 0: sair = true; break;
            }
        }
    }
}
//Menu interativo para cadastrar, listar ou remover produtos
void MENUProdutos(Produtos p[], int &qnt){
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
    
    Setas(tecla, opc, 4);
    if(tecla == 13){
       switch(opc){

        case 1: Cadastro(p, qnt); system("pause"); break;
        case 2: ListarProdutos(p, qnt); system("pause"); break;
        case 3: ApagarProd(p, qnt); system("pause"); break;
        case 0: sair = true;break;
        default: cout << "Opcao invalida\n"; system("pause"); break;
            }
        }
    };
}
//Menu principal do sistema, onde pode escolher entre Produtos, Vendas, Relatorios ou Sair
int MENU(){
    char tecla;
    int opc = 1;

    while(true){

        system("cls");
        cout << "                                MNSP-01\n";
        cout << "---MENU SUPERMERCADO---\n";
        cout << (opc == 1 ? ">  " : " ") << "Produtos\n";
        cout << (opc == 2 ? ">  " : " ") << "Vendas\n";
        cout << (opc == 3 ? ">  " : " ") << "Relatorios\n";
        cout << (opc == 0 ? ">  " : " ") << "Sair\n";
    
        Setas(tecla, opc, 4);
        if(tecla == 13){
            return opc;
        }
    }
}

int main(){
    Produtos v[MAX];
    int qnt=0;
    
    CarregarProdutos(v, qnt);

    while(true){
        int opc = MENU();
        switch (opc){
        case 1: MENUProdutos(v, qnt); break;
        case 2: MenuVendas(v, qnt); break;
        case 3: MenuRelatorios(v, qnt); break;
        case 0: cout << "Saindo..."; return 0;
        }
    }
}