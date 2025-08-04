#include <cstdio>
#include <cstdlib>
#include <string>

struct Aluno
{
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

struct Alunos
{
    Aluno *inicio;
    Aluno *final;
    int quant;
};

void inicializa(Alunos *lista){
    lista->inicio = NULL;
    lista->final = NULL;
    lista->quant = 0;
}

bool inserir(
    Alunos *alunos, 
    std::string matricula,
    std::string cpf,
    std::string nome,
    float nota,
    int idade,
    std::string curso,
    std::string cidade
){  
    Aluno *novo, *atual, *atual2 = NULL;
    novo = new Aluno{NULL, NULL, matricula, cpf, nome, nota, idade, curso, cidade};

    // lista vazia
    if (alunos->inicio == NULL){
        alunos->inicio=novo;
        alunos->final=novo;
        alunos->quant++;
        return true;
    }

    atual = alunos->inicio;
    while (atual != NULL){
        if (atual->cpf == novo->cpf || atual->matricula == novo->matricula) return false;
        if (atual2 == NULL && atual->nome >= novo->nome) atual2 = atual;
        atual = atual->prox;
    }
    // adicionar no primeiro da lista
    if (atual2 == alunos->inicio){
        novo->prox=atual2;
        atual2->ant=novo;
        alunos->inicio=novo;
    }
    // colocar no final
    else if(atual2==NULL){
        alunos->final->prox=novo;
        novo->ant=alunos->final;
        alunos->final=novo;
    }
    // adiciona no meio
    else{
        novo->ant=atual2->ant;
        novo->prox=atual2;
        atual2->ant->prox=novo;
        atual2->ant=novo;
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


Alunos listaAlunos;
int main(){
    inicializa(&listaAlunos);

    inserir(&listaAlunos, std::string("1"), std::string("1"), std::string("A"), 8.5f, 20, std::string("Engenharia de Computação"), std::string("Vitória"));

    inserir(&listaAlunos, std::string("2"), std::string("2"), std::string("B"), 7.2f, 22, std::string("Ciência da Computação"), std::string("Colatina"));

    inserir(&listaAlunos, std::string("3"), std::string("3"), std::string("C"), 9.1f, 19, std::string("Engenharia Elétrica"), std::string("São Mateus"));

    inserir(&listaAlunos, std::string("4"), std::string("5"), std::string("D"), 6.8f, 23, std::string("Sistemas de Informação"), std::string("Aracruz"));

    inserir(&listaAlunos, std::string("5"), std::string("5"), std::string("E"), 8.9f, 21, std::string("Engenharia Mecânica"), std::string("Linhares"));


    imprime_alunos(&listaAlunos);
    return 0;
}