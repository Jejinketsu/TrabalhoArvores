#include <stdlib.h>
#include <stdio.h>
#include <string.h>

typedef struct arvore{
    char palavra[25];
    char ingles[500];
    struct arvore *dir;
    struct arvore *esq;
} arvore;

typedef struct unidade{
    arvore *arvore;
} unidade;

void addArv(arvore **raiz, arvore *new);
void showArv(arvore *raiz);
void busca(arvore *no, char *string);
void remover(arvore **raiz, char *string);
void deleteNode(arvore **raiz, char *string);
arvore * maisEsq(arvore *raiz);
arvore * freeArv(arvore *raiz);

int main (){

    char portugues[35];
    char equivalente[500];
    char palavra[100];
    char palavraBuscar[50];
    int i = 0, cont = 1, enter = 1;

    unidade *unidades = malloc(sizeof(unidade));

    while(enter != 0){

        printf("1. Cadastrar Unidade.\n2. Remover Palavra.\n3. Mostrar Unidade.\n4. Buscar Palavra.\n0. Sair\n");
        scanf("%d", &enter);
        switch (enter){
            case 1:
                unidades = realloc(unidades, sizeof(unidade)*cont);
                arvore *raiz = NULL;

                unidades[cont-1].arvore = raiz;

                printf("Cadastrar linha? 1/0\n");
                scanf("%d", &enter);
                while(enter != 0){
                    arvore *novo = (arvore *) malloc(sizeof(arvore));
                    novo->dir = NULL;
                    novo->esq = NULL;

                    scanf("%s %s", portugues, equivalente);
                    portugues[strlen(portugues)] = '\0';

                    strcpy(novo->palavra, portugues);
                    strcpy(novo->ingles, equivalente);

                    addArv(&unidades[cont-1].arvore, novo);

                    printf("Cadastrar linha? 1/0\n");
                    scanf("%d", &enter);
                }
                enter = 1; cont++;
                break;
            case 2:
                printf("Palavra a ser escluida : ");
                scanf("%s", palavraBuscar);
                
                printf("Qual unidade?\n");
                scanf("%d", &enter);
                // remover(&unidades[enter].arvore, palavraBuscar);
                deleteNode(&unidades[enter].arvore, palavraBuscar);
                enter = 1;
                break;
            case 3:
                printf("Qual unidade?\n");
                scanf("%d", &enter);
                if(enter < cont) showArv(unidades[enter].arvore); 
                else printf("Não existe essa unidade\n");
                enter = 1;
                break;
            case 4:
                printf("Palavra a ser buscada : ");
                scanf("%s", palavraBuscar);
                
                printf("Qual unidade?\n");
                scanf("%d", &enter);
                busca(unidades[enter].arvore,palavraBuscar);
                enter = 1;
            default : break;
        }
    }

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
        printf("{%s: %s",raiz->palavra, raiz->ingles);
        showArv(raiz->esq);
        showArv(raiz->dir);
        printf("}");
    }
}

void busca(arvore *no, char *string){
    if(no != NULL){
        int returnCMP = strcmp(string, no->palavra);
        if(returnCMP != 0){
            if(returnCMP < 0) busca(no->esq, string);
            else busca(no->dir, string);
        } else {
            printf("%s: %s", no->palavra, no->ingles);
        }
    }
}


void deleteNode(arvore **raiz, char *string){
    if(*raiz != NULL){
    
        int returnCMP = strcmp(string, (*raiz)->palavra);
        
        if(returnCMP < 0)
            deleteNode((&(*raiz)->esq), string);
        else if(returnCMP > 0)
            deleteNode((&(*raiz)->dir), string);
        else{
            // Verifico se o no é uma folha
            if((*raiz)->esq == NULL && (*raiz)->dir == NULL ){
                free(*raiz);
                *raiz = NULL;
            }
            else if((*raiz)->esq == NULL){
                arvore *aux = *raiz;
                *raiz = (*raiz)->dir;
                free(aux);
            }
            else if((*raiz)->dir == NULL){
                arvore *aux = *raiz;
                *raiz = (*raiz)->esq;
                free(aux);
            }
            else{
                arvore *aux = (*raiz)->esq;
                while (aux->dir != NULL) {
                    aux = aux->dir;
                }
                strcpy((*raiz)->palavra, aux->palavra);
                strcpy((*raiz)->ingles, aux->ingles); 
                
                strcpy(aux->palavra, string);
                deleteNode((&(*raiz)->esq) , string);
            }

        }
    }
}



arvore * maisEsq(arvore *raiz){
    arvore *aux = (arvore *) malloc(sizeof(arvore));
    if(raiz != NULL){
        aux = maisEsq(raiz->esq);
    } else {
        aux = raiz;
    }
    return aux;
}