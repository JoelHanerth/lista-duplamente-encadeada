/*
Crie mais 3 hashs de tamanhos distintos para o cpf, utilize alguma ideia sua para cada uma. (no lugar de usar os dois ultimos digitos do cpf). Faça uma com 1000 índices no vetor, 10000 índices e 100000 índices. Faça a analise do tempo de inserção de todos os elementos e veja o que ocorre.
Analise o impacto do tempo da busca por nome. Crie uma busca automática que busque por 10 nomes, e rode o tempo disso nos 4 hashs implementados.
Após isso, volte para a hash de 100, e mude a forma de ordenação na lista. Passe a ordenar por cpf, e após inserir tudo, rode um algoritmo de ordenação, de preferência o quicksort em cada lista da hash. Faça a comparação de tempo para inserir tudo entre essa forma e a forma inicial que é ordenando por nome.

Abaixo segue o tempo gasto com a adição feita pelo nome:
Tempo gasto com 100 hash: 
Tempo para carregar arquivo: 706.161 segundos
Tempo para buscar 10 alunos: 0.645 segundos

Tempo gasto com 1000 hash:
Tempo para carregar arquivo: 67.303 segundos
Tempo para buscar 10 alunos: 0.528 segundos

Tempo gasto com 10000 hash:
Tempo para carregar arquivo: 10.312 segundos
Tempo para buscar 10 alunos: 0.521 segundos

Tempo gasto com 100000 hash:
Tempo para carregar arquivo: 4.413 segundos
Tempo para buscar 10 alunos: 0.478 segundos
*/


#include <fstream>
#include <sstream>
#include <ctime>
#include <iostream>
#include <cmath>

#define TAMANHO_HASH_INICIAL 7

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
    int tamanhoAtualHash;
    int quantidade;
};

void inicializaHashTable(hashTable *tabela, int tamanho) {
    tabela->tamanhoAtualHash = tamanho;
    tabela->quantidade = 0;
    tabela->hash = new Aluno*[tabela->tamanhoAtualHash];
    tabela->hashOcupada = new bool[tabela->tamanhoAtualHash];
    for (int i = 0; i < tabela->tamanhoAtualHash; i++) {
        tabela->hashOcupada[i] = false;
    }
}

int calculaHash(string nome, int tamanho) {
    long sum = 0;
    
    for (char c : nome) {
        sum += c;
    }
    
    long cubo = sum * sum * sum;
    
    int indice = cubo % tamanho;
    if (indice < 0) indice += tamanho;
    return indice;
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
        }
    }

    delete[] tabela->hash;
    delete[] tabela->hashOcupada;

    tabela->hash = novaTabela.hash;
    tabela->hashOcupada = novaTabela.hashOcupada;
    tabela->tamanhoAtualHash = novoTamanho;
}

void inserir(hashTable *tabela, Aluno *novo_aluno) {
    if (tabela->quantidade >= tabela->tamanhoAtualHash * 0.6) {
        reHash(tabela);
        printf("Tabela rehashing realizada. Novo tamanho: %d\n", tabela->tamanhoAtualHash);
    }

    int indice = calculaHash(novo_aluno->nome, tabela->tamanhoAtualHash);
    while (tabela->hashOcupada[indice]) {
        indice = (indice + 1) % tabela->tamanhoAtualHash;
    }
    tabela->hash[indice] = novo_aluno;
    tabela->hashOcupada[indice] = true;
    tabela->quantidade++;
}

void carregarArquivo(hashTable *tabela, string diretorio) {
    printf("aqui\n");
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

void imprimir(hashTable *tabela){
    for (int i = 0; i < tabela->tamanhoAtualHash; i++) {
        if (tabela->hashOcupada[i]) {
            cout << "Matrícula: " << tabela->hash[i]->matricula << endl;
            cout << "CPF: " << tabela->hash[i]->cpf << endl;
            cout << "Nome: " << tabela->hash[i]->nome << endl;
            cout << "Nota: " << tabela->hash[i]->nota << endl;
            cout << "Idade: " << tabela->hash[i]->idade << endl;
            cout << "Curso: " << tabela->hash[i]->curso << endl;
            cout << "Cidade: " << tabela->hash[i]->cidade << endl;
            cout << "-------------------------" << endl;
        }
    }
}



hashTable tabela;

int main(){
    inicializaHashTable(&tabela, TAMANHO_HASH_INICIAL);
    carregarArquivo(&tabela,"../alunos_completosV2.csv");
    // imprimir(&tabela);
    return 0;
}