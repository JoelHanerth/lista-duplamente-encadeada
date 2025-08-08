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

Aluno * criarAluno(std::string matricula, std::string cpf, std::string nome, float nota, int idade, std::string curso, std::string cidade){
    Aluno* novo = new Aluno;
    novo->prox = NULL;
    novo->ant = NULL;
    novo->matricula = matricula;
    novo->cpf = cpf;
    novo->nome = nome;
    novo->nota = nota;
    novo->idade = idade;
    novo->curso = curso;
    novo->cidade = cidade;

    return novo;
}

Aluno* criarAlunoDeLinha(std::string linha) {
    std::stringstream ss(linha);
    std::string campos[7];
    int i = 0;

    while (getline(ss, campos[i], ',') && i < 7) {
        i++;
    }
    return (criarAluno(campos[0], campos[1], campos[2], std::stof(campos[3]), std::stoi(campos[4]), campos[5], campos[6]));
}


void carregarArquivo(Alunos *alunos, std::string diretorio){
    std::ifstream arquivo(diretorio);
    std::string linha;

    if (arquivo.is_open()) {
        if (getline(arquivo, linha)){} // descarta a primeira linha;

        while (getline(arquivo, linha)){
            Aluno *novo_aluno = criarAlunoDeLinha(linha);
            inserir(alunos,novo_aluno);     
        }

        arquivo.close();
    }
    else printf("Erro ao abrir o arquivo!\n");  
}

Aluno* buscarPorMatricula(Alunos *alunos, std::string matricula) {
    Aluno *atual = alunos->inicio;

    while (atual != NULL) {
        if (atual->matricula == matricula) {
            printf("Aluno encontrado: %s - %s - %s\n", atual->matricula.c_str(), atual->cpf.c_str(), atual->nome.c_str());
            return atual;
        }
        atual = atual->prox;
    }
    printf("Aluno com matricula %s não encontrado.\n", matricula.c_str());
    return NULL;
}

Aluno* buscarPorCpf(Alunos *alunos, std::string cpf) {
    Aluno *atual = alunos->inicio;

    while (atual != NULL) {
        if (atual->cpf == cpf) {
            printf("Aluno encontrado: %s - %s - %s\n", atual->matricula.c_str(), atual->cpf.c_str(), atual->nome.c_str());
            return atual;
        }
        atual = atual->prox;
    }
    printf("Aluno com CPF %s não encontrado.\n", cpf.c_str());
    return NULL;
}

void excluir(Alunos *alunos, Aluno *aluno) {
    Aluno *atual = alunos->inicio;

    while (atual != NULL) {
        if (atual == aluno) {
            if (atual->ant != NULL) {
                atual->ant->prox = atual->prox;
            } else {
                alunos->inicio = atual->prox;
            }
            if (atual->prox != NULL) {
                atual->prox->ant = atual->ant;
            } else {
                alunos->final = atual->ant;
            }
            delete atual;
            alunos->quant--;
            return;
        }
        atual = atual->prox;
    }
}

bool confirmarExclusao(Alunos *alunos, Aluno *aluno) {
    char resposta;
    printf("Deseja excluir o aluno %s? (s/n): ", aluno->nome.c_str());
    std::cin >> resposta;
    if (resposta == 's' || resposta == 'S') {
        excluir(alunos, aluno);
        printf("Aluno %s excluido com sucesso.\n", aluno->nome.c_str());
        return true;
    } else {
        printf("Aluno %s nao excluido.\n", aluno->nome.c_str());
        return false;
    }
}

void menu(Alunos *alunos) {
    int opcao;
    std::string matricula;

    do {
        printf("\nMenu:\n");
        printf("1. Imprimir Alunos\n");
        printf("2. Buscar Aluno por Matricula\n");
        printf("3. Buscar Aluno por CPF\n");
        printf("4. Excluir Aluno\n");
        printf("5. Sair\n");
        printf("Escolha uma opcao: ");
        std::cin >> opcao;

        switch (opcao) {
            case 1: {
                imprime_alunos(alunos);
                break;
            }
            case 2: {
                printf("Digite a matricula do aluno: ");
                std::cin >> matricula;
                Aluno* alunoEncontradoMat = buscarPorMatricula(alunos, matricula);
                if (alunoEncontradoMat != NULL) {
                    confirmarExclusao(alunos, alunoEncontradoMat);
                }
                break;
            }
            case 3: {
                printf("Digite o CPF do aluno: ");
                std::cin >> matricula;
                Aluno* alunoEncontradoCpf = buscarPorCpf(alunos, matricula);
                if (alunoEncontradoCpf != NULL) {
                    confirmarExclusao(alunos, alunoEncontradoCpf);
                }
                break;
            }
            case 4: {
                printf("Digite a matricula do aluno a ser excluido: ");
                std::cin >> matricula;
                Aluno *aluno = buscarPorMatricula(alunos, matricula);
                if (aluno != NULL) {
                    confirmarExclusao(alunos, aluno);
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


Alunos listaAlunos;
int main(){
    inicializa(&listaAlunos);
    carregarArquivo(&listaAlunos,"../alunos_menor.csv");
    menu(&listaAlunos);
    return 0;
}