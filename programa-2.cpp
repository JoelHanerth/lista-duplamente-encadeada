// tamanho com tabela hash 100
// tempo gasto: 11 minutos
// tamanho hash 10000
// tempo: 15 segundos


#include <cstdio>
#include <cstdlib>
#include <string>
#include <fstream>
#include <sstream>
#include <ctime>
#include <iostream>

#define TAM 10000

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

struct Alunos{
    Aluno *inicio;
    Aluno *final;
    int quant;
};

struct hashTable {
    Alunos tabela[TAM];
    int tamanho;
};


void inicializa(Alunos *lista){
    lista->inicio = NULL;
    lista->final = NULL;
    lista->quant = 0;
}

void inicializaHashTable(hashTable *tabela) {
    for (int i = 0; i < TAM; i++) {
        inicializa(&tabela->tabela[i]);
    }
    tabela->tamanho = 0;
}

bool inserir_simples(Alunos *alunos, Aluno *novo_aluno) {
    if (alunos->inicio == NULL && alunos->final == NULL) {
        alunos->inicio = novo_aluno;
        alunos->final = novo_aluno;
    } else {
        alunos->final->prox = novo_aluno;
        novo_aluno->ant = alunos->final;
        alunos->final = novo_aluno;
    }
    alunos->quant++;
    return true;
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

void imprime_alunos(hashTable *tabela) {
    int total = 0;
    for (int i = 0; i < TAM; i++) {
        Aluno *atual = tabela->tabela[i].inicio;
        while (atual != NULL) {
            printf("%s - %s - %s\n", atual->matricula.c_str(), atual->cpf.c_str(), atual->nome.c_str());
            atual = atual->prox;
            total++;
        }
    }
    printf("Alunos - quantidade %d:\n", total);
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

    int contador = 0;
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

        int indice = stoi(novo_aluno->cpf.substr(0, 3) + novo_aluno->cpf.substr(4, 1));
        // int indice = stoi(novo_aluno->cpf.substr(novo_aluno->cpf.length() - 2, 2));
        // printf("%d\n", indice);

        Alunos* alunos = &tabela->tabela[indice];
        if (inserir(alunos, novo_aluno)) {
            tabela->tamanho++;
        }

        contador++;
        if (contador % 10000 == 0)
        {
            printf("%d\n",contador);
        }  
    }

    arquivo.close();
}


// void carregarArquivo(hashTable *tabela, string diretorio){
//     ifstream arquivo(diretorio);
//     string linha;

//     int contador = 0;
//     if (arquivo.is_open()) {
//         if (getline(arquivo, linha)){} // descarta a primeira linha;

//         while (getline(arquivo, linha)){
//             Aluno *novo_aluno = criarAlunoDeLinha(linha);
//             // int ultimos_dois_digitos = stoi(novo_aluno->cpf.substr(novo_aluno->cpf.length() - 2, 2));

//             // dados para 10000
//             string tres_primeiros_digitos = (novo_aluno->cpf.substr(0, 3));
//             string quinto_digito_cpf = (novo_aluno->cpf.substr(4, 1));
//             int indice = stoi(tres_primeiros_digitos + quinto_digito_cpf);

//             Alunos* alunos = &tabela->tabela[indice];
//             if (inserir(alunos,novo_aluno)){
//                 tabela->tamanho++;
//             }
//             contador++;
//             if (contador % 10000 == 0)
//             {
//                 printf("%d\n",contador);
//             }     
//         }

//         arquivo.close();
//     }
//     else printf("Erro ao abrir o arquivo!\n");  
// }

Aluno* buscarPorMatricula(hashTable *tabela, string matricula) {
    for (int i = 0; i < TAM; i++) {
        Aluno *atual = tabela->tabela[i].inicio;
        while (atual != NULL) {
            if (atual->matricula == matricula) {
                printf("Aluno encontrado: %s - %s - %s\n", atual->matricula.c_str(), atual->cpf.c_str(), atual->nome.c_str());
                return atual;
            }
            atual = atual->prox;
        }
    }
    printf("Aluno com matricula %s não encontrado.\n", matricula.c_str());
    return NULL;
}

Aluno* buscarPorCpf(hashTable *tabela, string cpf) {
    for (int i = 0; i < TAM; i++) {
        Aluno *atual = tabela->tabela[i].inicio;
        while (atual != NULL) {
            if (atual->cpf == cpf) {
                printf("Aluno encontrado: %s - %s - %s\n", atual->matricula.c_str(), atual->cpf.c_str(), atual->nome.c_str());
                return atual;
            }
            atual = atual->prox;
        }
    }
    printf("Aluno com CPF %s não encontrado.\n", cpf.c_str());
    return NULL;
}

void excluir(hashTable *tabela, Aluno *aluno) {
    for (int i = 0; i < TAM; i++) {
        Aluno *atual = tabela->tabela[i].inicio;
        while (atual != NULL) {
            if (atual == aluno) {
                if (atual->ant != NULL) {
                    atual->ant->prox = atual->prox;
                } else {
                    tabela->tabela[i].inicio = atual->prox;
                }
                if (atual->prox != NULL) {
                    atual->prox->ant = atual->ant;
                } else {
                    tabela->tabela[i].final = atual->ant;
                }
                delete atual;
                tabela->tabela[i].quant--;
                tabela->tamanho--;
                return;
            }
            atual = atual->prox;
        }
    }
}

bool confirmarExclusao(hashTable *tabela, Aluno *aluno) {
    char resposta;
    printf("Deseja excluir o aluno %s? (s/n): ", aluno->nome.c_str());
    cin >> resposta;
    if (resposta == 's' || resposta == 'S') {
        excluir(tabela, aluno);
        printf("Aluno %s excluido com sucesso.\n", aluno->nome.c_str());
        return true;
    } else {
        printf("Aluno %s nao excluido.\n", aluno->nome.c_str());
        return false;
    }
}

void menu(hashTable *tabelaAlunos) {
    int opcao;
    string matricula;

    do {
        printf("\nMenu:\n");
        printf("1. Imprimir Alunos\n");
        printf("2. Buscar Aluno por Matricula\n");
        printf("3. Buscar Aluno por CPF\n");
        printf("4. Excluir Aluno\n");
        printf("5. Sair\n");
        printf("Escolha uma opcao: ");
        cin >> opcao;

        switch (opcao) {
            case 1: {
                imprime_alunos(tabelaAlunos);
                break;
            }
            case 2: {
                printf("Digite a matricula do aluno: ");
                cin >> matricula;
                Aluno* alunoEncontradoMat = buscarPorMatricula(tabelaAlunos, matricula);
                if (alunoEncontradoMat != NULL) {
                    confirmarExclusao(tabelaAlunos, alunoEncontradoMat);
                }
                break;
            }
            case 3: {
                printf("Digite o CPF do aluno: ");
                cin >> matricula;
                Aluno* alunoEncontradoCpf = buscarPorCpf(tabelaAlunos, matricula);
                if (alunoEncontradoCpf != NULL) {
                    confirmarExclusao(tabelaAlunos, alunoEncontradoCpf);
                }
                break;
            }
            case 4: {
                printf("Digite a matricula do aluno a ser excluido: ");
                cin >> matricula;
                Aluno *aluno = buscarPorMatricula(tabelaAlunos, matricula);
                if (aluno != NULL) {
                    confirmarExclusao(tabelaAlunos, aluno);
                }
                break;
            }
            case 5: {
                printf("Saindo...\n");
                break;
            }
            default: {
                printf("Opcao invalida. Tente novamente.\n");
            }
        }       
    } while (opcao != 5);
}


hashTable tabela;

int main(){
    clock_t inicio = clock();
    inicializaHashTable(&tabela);
    carregarArquivo(&tabela,"../alunos_completos.csv");


    clock_t fim = clock();
    double tempo_execucao = double(fim - inicio) / CLOCKS_PER_SEC;
    cout << "Tempo gasto para carregar arquivo: " << tempo_execucao << " segundos\n";
    printf("Quantidade de alunos: %d\n", tabela.tamanho);

    menu(&tabela);
    return 0;
}