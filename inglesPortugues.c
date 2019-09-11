#include <stdlib.h>
#include <stdio.h>
#include <string.h>

typedef struct arvore{
    char palavra[25];
    char *ingles;
    struct arvore *dir;
    struct arvore *esq;
} arvore;


void addArv(arvore **raiz, arvore *new);
void showArv(arvore *raiz);
arvore * freeArv(arvore *raiz);

arvore * busca(arvore *no, int valor);

int main (){

    char linha[100];
    char palavra[100];
    int i = 0, cont = 0;

    arvore *raiz = NULL;

    arvore *novo = (arvore *) malloc(sizeof(arvore));
    novo->dir = NULL;
    novo->esq = NULL;

    scanf("%s", linha);
    while (linha[i] != '\0'){
        if(linha[i] != ':'){
            char a = linha[i];
            strcat(palavra, &a);
            cont++;
        } else {
            strcpy(novo->palavra, palavra);
            addArv(&raiz, novo);
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
            strcpy(novo->ingles, palavra);
            cont = 0;
        }
        i++;
    }

    showArv(raiz);

    return 0;
}

void addArv(arvore **raiz, arvore *new){

    if(*raiz == NULL){
        (*raiz) = (arvore *)malloc(sizeof(arvore));
            *raiz = new;
    }else{
        int returnCMP = strcmp( new->palavra , (*raiz)->palavra );
        if(returnCMP < 0)
            addArv( &((*raiz)->esq) , new );
        else if(returnCMP > 0)
            addArv( &((*raiz)->dir) , new );
    }

}
void showArv(arvore *raiz){
    if(raiz!=NULL){
        printf("{%s",raiz->palavra);
        showArv(raiz->esq);
        showArv(raiz->dir);
        printf("}");
    }
}