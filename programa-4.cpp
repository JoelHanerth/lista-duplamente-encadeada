/*
Crie mais 3 hashs de tamanhos distintos para o cpf, utilize alguma ideia sua para cada uma. (no lugar de usar os dois ultimos digitos do cpf). Faça uma com 1000 índices no vetor, 10000 índices e 100000 índices. Faça a analise do tempo de inserção de todos os elementos e veja o que ocorre.
Analise o impacto do tempo da busca por nome. Crie uma busca automática que busque por 10 nomes, e rode o tempo disso nos 4 hashs implementados.
Após isso, volte para a hash de 100, e mude a forma de ordenação na lista. Passe a ordenar por cpf, e após inserir tudo, rode um algoritmo de ordenação, de preferência o quicksort em cada lista da hash. Faça a comparação de tempo para inserir tudo entre essa forma e a forma inicial que é ordenando por nome.

Abaixo segue o tempo gasto com a adição feita pelo cpf:
Tempo gasto com 100 hash: 
Tempo gasto para carregar arquivo: 269.549 segundos
Tempo gasto para ordenar alunos: 0.875 segundos
*/


#include <fstream>
#include <sstream>
#include <ctime>
#include <iostream>
#include <vector>
#include <algorithm>

#define TAM 100
#define QUANT_CARACTER 2

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
    Aluno *atual = NULL;
    
    // lista vazia
    if (alunos->inicio == NULL){
        alunos->inicio = aluno;
        alunos->final = aluno;
        alunos->quant++;
        return true;
    }

    atual = alunos->inicio;
    while (atual != NULL){
        if (atual->cpf >= aluno->cpf) {
            if (atual->cpf == aluno->cpf) return false;
            break;
        }
        atual = atual->prox;
    }

    if (atual == alunos->inicio){
        aluno->prox = atual;
        atual->ant = aluno;
        alunos->inicio = aluno;
    }
    else if(atual == NULL){
        alunos->final->prox = aluno;
        aluno->ant = alunos->final;
        alunos->final = aluno;
    }
    else{
        aluno->ant = atual->ant;
        aluno->prox = atual;
        atual->ant->prox = aluno;
        atual->ant = aluno;
    }
    alunos->quant++;
    return true;
}

void ordenarPorNome(hashTable *tabela_hash_alunos) {
    for (int i = 0; i < TAM; i++) {
        Alunos *alunos = &tabela_hash_alunos->tabela[i];
        if (alunos->quant <= 1) continue;

        vector<Aluno *> vetor_alunos(alunos->quant);

        Aluno *atual = alunos->inicio;
        for (int j = 0; j < alunos->quant; j++) {
            vetor_alunos[j] = atual;
            atual = atual->prox;
        }

        sort(
            vetor_alunos.begin(),
            vetor_alunos.end(),
            [](Aluno *a, Aluno *b) { return a->nome < b->nome; }
        );

        for (size_t j = 0; j < vetor_alunos.size() - 1; j++) {
            vetor_alunos[j]->prox = vetor_alunos[j + 1];
            vetor_alunos[j + 1]->ant = vetor_alunos[j];
        }

        vetor_alunos[0]->ant = nullptr;
        vetor_alunos.back()->prox = nullptr;
        alunos->inicio = vetor_alunos[0];
        alunos->final = vetor_alunos.back();
    }
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


int obter_indice( string cpf, int quantidade_caracter) {
    if (quantidade_caracter < 2) return -1;

    if (quantidade_caracter == 2) {
        return(stoi(cpf.substr(cpf.length() - 2, 2)));
    }
    if (quantidade_caracter == 3){
        return(stoi(cpf.substr(0, 3)));
    }
    if (quantidade_caracter == 4) {
        return(stoi(cpf.substr(0, 3) + cpf.substr(4, 1)));
    }
    if (quantidade_caracter == 5) {
        return(stoi(cpf.substr(0, 3) + cpf.substr(4, 2)));
    }
    return -1;
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

        int indice = obter_indice((campos[1]), QUANT_CARACTER);

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

Aluno* buscarPorNome(hashTable *tabela, string nome) {
    for (int i = 0; i < TAM; i++) {
        Aluno *atual = tabela->tabela[i].inicio;
        while (atual != NULL) {
            if (atual->nome >= nome) {

                if (atual->nome == nome) {
                    // printf("Aluno encontrado: %s - %s - %s\n", atual->matricula.c_str(), atual->cpf.c_str(), atual->nome.c_str());
                    return atual;
                }
                break;
            }
            atual = atual->prox;
        }
    }
    printf("Aluno com nome %s não encontrado.\n", nome.c_str());
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
        printf("4. Buscar 10 alunos por nome:\n");
        printf("5. Excluir Aluno\n");
        printf("6. Sair\n");
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
            case 4:{
                string lista_nomes[10] = {
                    "Iolanda Prado Palmer", 
                    "Cristiane Mann Rossi", 
                    "Bradley Viana Campbell", 
                    "Philip Chamorro Anastácio", 
                    "Germano Nascimento Morgan", 
                    "Aureliano Guy Black", 
                    "Bartolomeu Shaw Rei", 
                    "Arquimedes Antonelli Armstrong",
                    "Flávio Briggs Soledade",
                    "José Porto Pardo"
                };
                clock_t inicio = clock();
                for (int i = 0; i < 10; i++) {
                    buscarPorNome(tabelaAlunos, lista_nomes[i]);
                }
                clock_t fim = clock();
                double tempo_execucao = double(fim - inicio) / CLOCKS_PER_SEC;
                cout << "Tempo adicionar 10 alunos: " << tempo_execucao << " segundos\n";
                break;
            }
            case 5: {
                printf("Digite a matricula do aluno a ser excluido: ");
                cin >> matricula;
                Aluno *aluno = buscarPorMatricula(tabelaAlunos, matricula);
                if (aluno != NULL) {
                    confirmarExclusao(tabelaAlunos, aluno);
                }
                break;
            }
            case 6: {
                printf("Saindo...\n");
                break;
            }
            default: {
                printf("Opcao invalida. Tente novamente.\n");
            }
        }       
    } while (opcao != 6);
}


hashTable tabela;

int main(){
    clock_t inicio = clock();
    inicializaHashTable(&tabela);
    carregarArquivo(&tabela,"../alunos_completosV2.csv");


    clock_t fim = clock();
    double tempo_execucao = double(fim - inicio) / 1000;
    cout << "Tempo gasto para carregar arquivo: " << tempo_execucao << " segundos\n";
    printf("Quantidade de alunos: %d\n", tabela.tamanho);

    ordenarPorNome(&tabela);
    clock_t fim_2 = clock();
    double tempo_execucao_2 = double(fim_2 - fim) / 1000;
    cout << "Tempo gasto para ordenar alunos: " << tempo_execucao_2 << " segundos\n";

    menu(&tabela);
    return 0;
}