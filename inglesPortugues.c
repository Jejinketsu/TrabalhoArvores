#include <stdlib.h>
#include <stdio.h>
#include <string.h>

typedef struct arvore{
    char palavra[25];
    char *ingles;
    struct arvore *dir;
    struct arvore *esq;
} arvore;

typedef struct ocorrencia{
    int num, casos;
    struct ocorrencia *prox;
}ocorrencia;

ocorrencia * addNum(ocorrencia *lista, int new);
ocorrencia * contain(ocorrencia *lista, int num);
void mostrarLista(ocorrencia *lista);
void addArv(arvore *raiz, arvore *new);
arvore * freeArv(arvore *raiz);
int nos(arvore *no);
int ramos(arvore *no);
int profundidade(arvore *no, int valor);
int descendentes(arvore *no);
int altura(arvore *no, int valor, int maior);

arvore * busca(arvore *no, int valor);

int main (){

    char linha[100];
    char palavra[100];
    int i = 0, cont = 0;

    scanf("%s", linha);
    while (linha[i] != '\0'){
        if(linha[i] != ':'){
            char a = linha[i];
            strcat(palavra, &a);
            cont++;
        } else {
            printf("%s", palavra);
            cont = 0;
            i += 2; break;
        }
        i++;
    }

    while(linha[i] != '\0'){
        if(linha[i] != ','){
            char a = linha[i];
            strcat(palavra, &a);
            cont++;
        } else {
            printf("%s", palavra);
            cont = 0;
        }
        i++;
    }

    return 0;
}
/*
ocorrencia * addNum(ocorrencia *lista, int new){
    ocorrencia *novo = (ocorrencia *) malloc(sizeof(ocorrencia));
    novo->num = new;
    novo->casos = 1;
    novo->prox = NULL;
    return novo;
}

ocorrencia * contain(ocorrencia *lista, int num){
    if(lista == NULL){ 
        lista = addNum(lista, num);
    } else {
        ocorrencia *aux;
        for(aux = lista; aux != NULL; aux = aux->prox){
            if(aux->num == num){
                aux->casos += 1;
                break;
            }
            if(aux->prox == NULL) break;
        }
        if(aux->prox == NULL) aux->prox = addNum(lista, num);  
    }
    return lista;
}

void mostrarLista(ocorrencia *lista){
    if(lista != NULL){
        printf("%d: %d, ", lista->num, lista->casos);
        mostrarLista(lista->prox);
    }
}

void addArv(arvore *raiz, arvore *new){
    if(raiz != NULL){
        if(raiz->valor < new->valor){
            if(raiz->dir == NULL) raiz->dir = new;
            else addArv(raiz->dir, new);
        } else { 
            if(raiz->esq == NULL) raiz->esq = new;
            else addArv(raiz->esq, new);
        }
    }
}

arvore * freeArv(arvore *raiz){
    if(raiz != NULL){
        freeArv(raiz->dir);
        freeArv(raiz->esq);
        free(raiz);
    }
    return NULL;
}

int altura(arvore *no, int valor, int maior){
    int Altura = 0;
    arvore *aux = malloc(sizeof(arvore));

    aux = busca(no, valor);

    int interna(arvore *no){
        int esq = 0, dir = 0;

        if(no->esq != NULL) esq += interna(no->esq) + 1;
        if(no->dir != NULL) dir += interna(no->dir) + 1;

        if(maior) return esq > dir ? esq : dir;
        else return esq < dir ? esq : dir;
        
    }

    if(aux != NULL) Altura = interna(aux);

    return Altura;
}

arvore * busca(arvore *no, int valor){
    arvore *aux = malloc(sizeof(arvore));
    if(no != NULL){
        if(no->valor != valor){
            if(no->valor >= valor) aux = busca(no->esq, valor);
            else aux = busca(no->dir, valor);
        } else {
            aux = no;
        }
    }
    return aux;
}
*/