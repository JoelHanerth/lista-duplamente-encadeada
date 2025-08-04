# Lista Duplamente Encadeada com Alunos

Este projeto tem como objetivo implementar uma **lista duplamente encadeada** para armazenar e manipular informações de alunos lidas a partir de um arquivo CSV.

## Funcionalidades

* **Leitura de Arquivo CSV**
  Lê automaticamente os dados dos alunos a partir de um arquivo `.csv`.

* **Armazenamento em Lista Duplamente Encadeada**
  Os alunos são armazenados em uma estrutura de lista duplamente encadeada, permitindo navegação eficiente para frente e para trás.

* **Inserção Ordenada por Nome**
  A inserção de alunos é feita de forma ordenada com base no **nome**, mantendo a lista sempre organizada.

* **Validação de Dados Duplicados**
  Antes de inserir um novo aluno, o sistema verifica se **CPF** ou **Matrícula** já existem. Duplicatas não são permitidas.

* **Busca por Matrícula ou CPF**
  Permite pesquisar um aluno específico usando a matrícula ou o CPF.

* **Remoção de Aluno**
  Após localizar o aluno, é possível optar por removê-lo da lista.

## Estrutura do CSV

O arquivo CSV deve conter os seguintes campos (em qualquer ordem):

```
matricula,cpf,nome,nota,idade,curso,cidade
```

Exemplo:

```
20251001,12345678900,Ana Clara Silva,8.5,20,Engenharia de Computação,Vitória
```

## Como Usar

1. Clone este repositório.
2. Compile o programa com seu compilador C/C++ favorito.
3. Certifique-se de que o arquivo CSV esteja no mesmo diretório do executável.
4. Execute o programa e siga as instruções no terminal.

## Tecnologias

* Linguagem C/C++
* Estrutura de dados: Lista Duplamente Encadeada
