#include <stdlib.h>
#include <stdio.h>
#include <time.h>

typedef struct arvore{
    int valor;
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

    srand(time(NULL));
    float tempoTotal = 0;
    
    ocorrencia *lista = malloc(sizeof(ocorrencia));

    for(int i = 0; i < 30; i++){
        arvore *raiz = malloc(sizeof(arvore));
        raiz->valor = rand()%100;

        clock_t inicio = clock();

        for(int i = 1; i < 1000; i++){
            arvore *arv = malloc(sizeof(arvore));
            arv->valor = rand()%100;
            addArv(raiz, arv);
        }

        clock_t fim = clock();

        float tempo = (fim-inicio)*1000 / CLOCKS_PER_SEC;
        tempoTotal += tempo;

        inicio = clock();
        busca(raiz, 50);
        fim = clock();

        float tempoBusca = (fim-inicio)*1000 / CLOCKS_PER_SEC;

        int maiorP = altura(raiz, raiz->valor, 1);
        int menorP = altura(raiz, raiz->valor, 0);

        //freeArv(raiz);

        int diferenca = maiorP-menorP;
        contain(lista, diferenca);

        printf("----------Teste %d-------------\n", i+1);
        printf("Maior profundidade: %d\n", maiorP);
        printf("Menor profundidade: %d\n", menorP);
        mostrarLista(lista);
        printf("\nTempo de insercao: %.5fms\n", tempo);
        printf("Tempo de busca: %.5fms\n\n", tempoBusca);

    }

    return 0;
}

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
