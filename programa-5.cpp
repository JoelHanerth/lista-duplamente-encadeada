#include <fstream>
#include <sstream>
#include <ctime>
#include <iostream>
#include <cmath>
#include <locale>

#define TAMANHO_HASH_INICIAL 11

using namespace std;

struct Aluno{
    Aluno *prox;
    Aluno *ant;
    string matricula;
    string cpf;
    string nome;
    float nota;
    int idade;
    string curso;
    string cidade;
};

struct hashTable {
    Aluno **hash;
    bool *hashOcupada;
    bool *hashRemovida;
    int tamanhoAtualHash;
    int quantidade;
};

void inicializaHashTable(hashTable *tabela, int tamanho) {
    tabela->tamanhoAtualHash = tamanho;
    tabela->quantidade = 0;
    tabela->hash = new Aluno*[tabela->tamanhoAtualHash];
    tabela->hashOcupada = new bool[tabela->tamanhoAtualHash];
    tabela->hashRemovida = new bool[tabela->tamanhoAtualHash];
    for (int i = 0; i < tabela->tamanhoAtualHash; i++) {
        tabela->hashOcupada[i] = false;
        tabela->hashRemovida[i] = false;
    }
}

// int calculaHash(string nome, int tamanho) {
//     long sum = 0;
    
//     for (char c : nome) {
//         sum += c;
//     }
    
//     long cubo = sum * sum;
    
//     int indice = cubo % tamanho;
//     if (indice < 0) indice += tamanho;
//     return indice;
// }

int calculaHash(string nome, int tamanho) {
    unsigned int hash = 0;
    
    // FNV-1a hash algorithm
    for (char c : nome) {
        hash ^= c;
        hash *= 16777619; // FNV prime
    }
    
    return hash % tamanho;
}

// Function to return nearest prime number
int prime(int n)
{
    // All prime numbers are odd except two
    if (n & 1)
        n -= 2;
    else
        n--;

    int i, j;
    for (i = n; i >= 2; i -= 2) {
        if (i % 2 == 0)
            continue;
        for (j = 3; j <= sqrt(i); j += 2) {
            if (i % j == 0)
                break;
        }
        if (j > sqrt(i))
            return i;
    }
    // It will only be executed when n is 3
    return 2;
}

void reHash(hashTable *tabela){
    int novoTamanho = prime(tabela->tamanhoAtualHash * 2);

    hashTable novaTabela;
    inicializaHashTable(&novaTabela, novoTamanho);

    for (int i = 0; i < tabela->tamanhoAtualHash; i++) {
        if (tabela->hashOcupada[i]) {
            Aluno *aluno = tabela->hash[i];
            int indice = calculaHash(aluno->nome, novoTamanho);
            while (novaTabela.hashOcupada[indice]) {
                indice = (indice + 1) % novoTamanho;
            }
            novaTabela.hash[indice] = aluno;
            novaTabela.hashOcupada[indice] = true;
            novaTabela.hashRemovida[indice] = false;
        }
    }

    delete[] tabela->hash;
    delete[] tabela->hashOcupada;
    delete[] tabela->hashRemovida;

    tabela->hash = novaTabela.hash;
    tabela->hashOcupada = novaTabela.hashOcupada;
    tabela->hashRemovida = novaTabela.hashRemovida;
    tabela->tamanhoAtualHash = novoTamanho;
}

void inserir(hashTable *tabela, Aluno *novo_aluno) {
    if (tabela->quantidade >= tabela->tamanhoAtualHash * 0.6) {
        reHash(tabela);
        printf("Tabela rehashing realizada. Novo tamanho: %d\n", tabela->tamanhoAtualHash);
    }

    int indice = calculaHash(novo_aluno->nome, tabela->tamanhoAtualHash);
    while (tabela->hashOcupada[indice]) {
        if (tabela->hash[indice]->nome == novo_aluno->nome) {
            tabela->hash[indice] = novo_aluno;
            return;
        }
        indice = (indice + 1) % tabela->tamanhoAtualHash;
    }
    tabela->hash[indice] = novo_aluno;
    tabela->hashOcupada[indice] = true;
    tabela->hashRemovida[indice] = false;
    tabela->quantidade++;
}

void carregarArquivo(hashTable *tabela, string diretorio) {
    ifstream arquivo(diretorio);
    string linha;
    if (!arquivo.is_open()) {
        printf("Erro ao abrir o arquivo!\n");
        return;
    }
    
    // Descarta a primeira linha (cabeçalho)
    getline(arquivo, linha);
    
    while (getline(arquivo, linha)) {
        stringstream ss(linha);
        string campos[7];
        int i = 0;
        
        while (getline(ss, campos[i], ',') && i < 7) i++;
        
        Aluno *novo_aluno = new Aluno{
            NULL, NULL,
            campos[0],
            campos[1],
            campos[2],
            stof(campos[3]),
            stoi(campos[4]),
            campos[5],
            campos[6]
        };

        inserir(tabela, novo_aluno);
    }
    
    arquivo.close();
}

void imprimirAluno(Aluno *aluno){
    cout << "Aluno encontrado:\n";
    cout << "Matrícula: " << aluno->matricula << endl;
    cout << "CPF: " << aluno->cpf << endl;
    cout << "Nome: " << aluno->nome << endl;
    cout << "Nota: " << aluno->nota << endl;
    cout << "Idade: " << aluno->idade << endl;
    cout << "Curso: " << aluno->curso << endl;
    cout << "Cidade: " << aluno->cidade << endl;
}
void imprimir(hashTable *tabela){
    cout << "-------------------------" << endl;
    for (int i = 0; i < tabela->tamanhoAtualHash; i++) {
        if (tabela->hashOcupada[i]) {
            imprimirAluno(tabela->hash[i]);
            cout << "-------------------------" << endl;
        }
    }
}
bool buscarNome(hashTable *tabela, string nome){
    int indice = calculaHash(nome, tabela->tamanhoAtualHash);
    int indice_inicial = indice;
    
    while (tabela->hashOcupada[indice] || tabela->hashRemovida[indice]) {
        if (tabela->hashOcupada[indice] && tabela->hash[indice]->nome == nome) {
            imprimirAluno(tabela->hash[indice]);
            return true;
        }
        indice = (indice + 1) % tabela->tamanhoAtualHash;
        if (indice == indice_inicial) break;
    }
    
    cout << "Aluno com nome " << nome << " não encontrado." << endl;
    return false;
}


bool excluir(hashTable *tabela, string nome) {
    int indice = calculaHash(nome, tabela->tamanhoAtualHash);
    int indice_inicial = indice;
    
    while (tabela->hashOcupada[indice] || tabela->hashRemovida[indice]) {
        if (tabela->hashOcupada[indice] && tabela->hash[indice]->nome == nome) {
            delete tabela->hash[indice];
            tabela->hashOcupada[indice] = false;
            tabela->hashRemovida[indice] = true;
            tabela->quantidade--;
            return true;
        }
        indice = (indice + 1) % tabela->tamanhoAtualHash;
        if (indice == indice_inicial) break;
    }
    return false;
}

void confirmarExclusao(hashTable *tabela, string nome){
    string opcao;
    if (buscarNome(tabela, nome)){
        cout << "Deseja excluír esse aluno? [S/N]";
        cin >> opcao;
        if (opcao == "s" || opcao == "S") {
            if (excluir(tabela, nome)) {
                printf("Aluno com nome %s excluido com sucesso.\n", nome.c_str());
            } else {
                printf("Erro ao excluir aluno com nome %s.\n", nome.c_str());
            }
        } else {
            printf("Aluno com nome %s nao excluido.\n", nome.c_str());
        }
    }
}



void menu(hashTable *tabela){
    int opcao;
    string matricula;
    string nome;

    do {
        printf("\nMenu:\n");
        printf("1. Imprimir Alunos\n");
        printf("2. Buscar Aluno\n");
        printf("3. Excluir Aluno\n");
        printf("4. Sair\n");
        printf("Escolha uma opcao: ");
        cin >> opcao;

        switch (opcao) {
            case 1:
                imprimir(tabela);
                break;
            case 2:
                cout << "Digite o nome do aluno a ser buscado: ";
                cin.ignore();
                getline(cin, nome);
                buscarNome(tabela, nome);
                break;
            case 3:
                cout << "Digite o nome do aluno a ser excluído: ";
                cin.ignore();
                getline(cin, nome);
                confirmarExclusao(tabela, nome);
                break;
            case 4:
                printf("Saindo...\n");
                break;
            default:
                printf("Opcao invalida!\n");
                break;
        }
    } while (opcao != 4);
}

hashTable tabela;

int main(){
    setlocale(LC_ALL, "pt_BR.UTF-8");
    clock_t inicio = clock();
    inicializaHashTable(&tabela, TAMANHO_HASH_INICIAL);
    carregarArquivo(&tabela,"../alunos_completosV2.csv");
    // imprimir(&tabela);
    clock_t fim = clock();
    double tempo = double(fim - inicio) / CLOCKS_PER_SEC;
    cout << "Tempo de execução: " << tempo << " segundos" << endl;
    cout << "Quantidade de alunos: " << tabela.quantidade << endl;
    menu(&tabela);
    return 0;
}