#include <stdlib.h>
#include <stdio.h>
#include <time.h>

typedef struct ocorrencia{
    int num, casos;
    struct ocorrencia *prox;
}ocorrencia;

ocorrencia * addNum(ocorrencia *lista, int new);
ocorrencia * contain(ocorrencia *lista, int num);
void mostrarLista(ocorrencia *lista);

int main(){

    srand(time(NULL));
    ocorrencia *lista = NULL;

    for(int i = 0; i < 100; i++){
        int num = rand()%25;
        printf("%d ", num);
        lista = contain(lista, num);
    }

    printf("\n");
    mostrarLista(lista);

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