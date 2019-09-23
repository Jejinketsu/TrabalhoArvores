#include <stdlib.h>
#include <stdio.h>
#include <string.h>

typedef struct arvore {
    int nInfos;
    char palavra1[25];
    char palavra2[25];
    struct text *ingles1;
    struct text *ingles2;
    struct arvore *dir;
    struct arvore *meio;
    struct arvore *esq;
} arvore;

int ehfolha(arvore *raiz);
void showArv(arvore *raiz);
arvore * adicionaNo(arvore **raiz, char *string, arvore *paux);
arvore * addArv(arvore **raiz, char *string, char *promove, arvore **pai);
arvore * quebraNo(arvore **raiz, char *string, char *promove, arvore *subArvore);
arvore ** criaNo(char *string, arvore *filhoEsq, arvore *filhoCen, arvore *filhoDir);

int main(){

    arvore **raiz = NULL;
    char *promove;
    
    addArv(raiz, "E-", promove, NULL);
    /*
    addArv(raiz, "H-", promove, NULL);
    addArv(raiz, "B-", promove, NULL);
    addArv(raiz, "D-", promove, NULL);
    addArv(raiz, "F-", promove, NULL);
    addArv(raiz, "L-", promove, NULL);
    addArv(raiz, "K-", promove, NULL);
    addArv(raiz, "G-", promove, NULL);
    addArv(raiz, "I-", promove, NULL);
    addArv(raiz, "J-", promove, NULL);
    addArv(raiz, "C-", promove, NULL);
    addArv(raiz, "A-", promove, NULL);
    */
    return 0;
}

arvore * addArv(arvore **raiz, char *string, char *promove, arvore **pai){
    arvore *paux;

    if (raiz == NULL) {
        raiz = criaNo(string, NULL, NULL, NULL);
        printf("No Criado\n");
        paux = NULL;
    } else {
        if (ehfolha(*raiz)) {
            if ((*raiz)->nInfos == 1) {
                *raiz = adicionaNo(raiz, string, NULL);
                paux = NULL;
            } else paux = quebraNo(raiz, string, promove, NULL);
        } else {
            if (strcmp(string, (*raiz)->palavra1) < 0) 
                paux = addArv(&((*raiz)->esq), string, promove, raiz);
            else if (((*raiz)->nInfos == 1) || (strcmp(string, (*raiz)->palavra2) < 0))
                paux = addArv(&((*raiz)->meio), string, promove, raiz);
            else
                paux = addArv(&((*raiz)->dir), string, promove, raiz);
        }
    }

    if (paux != NULL) {
        if (pai == NULL) {
            raiz = criaNo(promove, (*raiz), paux, NULL);
            paux = NULL;
        } else if ((*pai)->nInfos == 1) {
            *pai = adicionaNo(pai, promove, paux);
            paux = NULL;
        } else {
            char guardar[30];
            strcpy(guardar, promove);
            paux = quebraNo(pai, guardar, promove, paux);
        }
    }

    return paux;
}

arvore * quebraNo(arvore **raiz, char *string, char *promove, arvore *subArvore){
    arvore **paux;
    int comparacao1 = strcmp(string, (*raiz)->palavra1);
    int comparacao2 = strcmp(string, (*raiz)->palavra2);
    
    if(comparacao1 < 0) {
        strcpy(promove, (*raiz)->palavra1);
        strcpy((*raiz)->palavra1, string);
    } else if(comparacao2 > 0) {
        strcpy(promove, (*raiz)->palavra2);
        strcpy((*raiz)->palavra2, string);
    } else {
        strcpy(promove, string);
    }

    paux = criaNo((*raiz)->palavra2, (*raiz)->dir, subArvore, NULL);

    (*raiz)->nInfos = 1;
    (*raiz)->dir = NULL;

    return *paux;
}

arvore ** criaNo(char *string, arvore *filhoEsq, arvore *filhoCen, arvore *filhoDir){
    arvore **no = malloc(sizeof(arvore));
    
    printf("aqui\n");
    (*no)->dir = filhoDir;
    (*no)->meio = filhoCen;
    (*no)->esq = filhoEsq;
    
    strcpy((*no)->palavra1, string);
    (*no)->nInfos = 1;
    printf("alocado\n");
    return no;
}

arvore * adicionaNo(arvore **raiz, char *string, arvore *paux){
    int comparacao = strcmp(string, (*raiz)->palavra1);
    if(comparacao >= 0) {
        strcpy((*raiz)->palavra2, string);
        (*raiz)->dir = paux;
    }
    else {
        strcpy((*raiz)->palavra2, (*raiz)->palavra1);
        strcpy((*raiz)->palavra1, string);
        (*raiz)->dir = (*raiz)->meio;
        (*raiz)->meio = paux;
    }
    (*raiz)->nInfos = 2;
    return (*raiz);
}

int ehfolha(arvore *raiz){
    int folha = 0;
    if (raiz->dir == NULL && raiz->meio == NULL && raiz->esq == NULL)
        folha = 1;
    return folha;
}

void showArv(arvore *raiz){
    if(raiz!=NULL){
        printf("{%s %s :",raiz->palavra1, raiz->palavra2);
        //showList(raiz->ingles);
        showArv(raiz->esq);
        showArv(raiz->dir);
        printf("}");
    }
}