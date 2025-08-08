#include <cstdio>
#include <cstdlib>
#include <string>
#include <fstream>
#include <sstream>
#include <ctime>
#include <iostream>

struct Aluno{
    Aluno *prox;
    Aluno *ant;
    std::string matricula;
    std::string cpf;
    std::string nome;
    float nota;
    int idade;
    std::string curso;
    std::string cidade;
};

struct Alunos{
    Aluno *inicio;
    Aluno *final;
    int quant;
};

void inicializa(Alunos *lista){
    lista->inicio = NULL;
    lista->final = NULL;
    lista->quant = 0;
}

bool inserir(Alunos *alunos, Aluno *aluno){ 
    Aluno *atual, *atual2 = NULL;
    
    // lista vazia
    if (alunos->inicio == NULL){
        alunos->inicio=aluno;
        alunos->final=aluno;
        alunos->quant++;
        return true;
    }

    atual = alunos->inicio;
    while (atual != NULL){
        if (atual->cpf == aluno->cpf || atual->matricula == aluno->matricula) return false;
        if (atual2 == NULL && atual->nome >= aluno->nome) atual2 = atual;
        atual = atual->prox;
    }
    // adicionar no primeiro da lista
    if (atual2 == alunos->inicio){
        aluno->prox=atual2;
        atual2->ant=aluno;
        alunos->inicio=aluno;
    }
    // colocar no final
    else if(atual2==NULL){
        alunos->final->prox=aluno;
        aluno->ant=alunos->final;
        alunos->final=aluno;
    }
    // adiciona no meio
    else{
        aluno->ant=atual2->ant;
        aluno->prox=atual2;
        atual2->ant->prox=aluno;
        atual2->ant=aluno;
    }
    alunos->quant++;
    return true;
}

void imprime_alunos(Alunos *alunos) {
    Aluno *atual = alunos->inicio;
    
    printf("Alunos - quantidade %d:\n",alunos->quant);
    
    while (atual != NULL) {
        printf("%s - %s - %s\n", atual->matricula.c_str(), atual->cpf.c_str(), atual->nome.c_str());
        atual = atual->prox;
    } 
}

Aluno* criarAlunoDeLinha(std::string linha) {
    std::stringstream ss(linha);
    std::string campos[7];
    int i = 0;

    while (getline(ss, campos[i], ',') && i < 7) {
        i++;
    }

    Aluno* novo = new Aluno;
    novo->prox = NULL;
    novo->ant = NULL;
    novo->matricula = campos[0];
    novo->cpf = campos[1];
    novo->nome = campos[2];
    novo->nota = std::stof(campos[3]);
    novo->idade = std::stoi(campos[4]);
    novo->curso = campos[5];
    novo->cidade = campos[6];

    return novo;
}


void carregarArquivo(Alunos *alunos, std::string diretorio){
    std::ifstream arquivo(diretorio);
    std::string linha;

    if (arquivo.is_open()) {
        if (getline(arquivo, linha)){} // descarta a primeira linha;

        int contador = 0, contador2 = 0;
        clock_t inicio = clock();
        clock_t inicio2 = clock();
        while (getline(arquivo, linha)){
            Aluno *novo_aluno = criarAlunoDeLinha(linha);
            inserir(alunos,novo_aluno);
            contador++;
            
            if (contador >= 10000){
                contador2 +=contador;
                clock_t fim = clock();
                double tempo_execucao = double(fim - inicio) / CLOCKS_PER_SEC;
                printf("repeticao numero %d -",contador2);
                std::cout << "Tempo total de execucao: " << tempo_execucao << " segundos - ";

                double tempo_execucao2 = double(fim - inicio2) / CLOCKS_PER_SEC;
                std::cout << "Tempo entre intervalo de execucao: " << tempo_execucao2 << " segundos\n";
                inicio2 = clock();

                contador = 0;
            }
        }
        arquivo.close();
    }
    else printf("Erro ao abrir o arquivo!\n");  
}

Alunos listaAlunos;
int main(){
    clock_t inicio = clock();

    inicializa(&listaAlunos);
    carregarArquivo(&listaAlunos,"../alunos_completos.csv");
    // imprime_alunos(&listaAlunos);
    printf("Quantidade de alunos %d\n",listaAlunos.quant);

    clock_t fim = clock();
    double tempo_execucao = double(fim - inicio) / CLOCKS_PER_SEC;
    std::cout << "Tempo de execucao: " << tempo_execucao << " segundos\n";
    return 0;
}