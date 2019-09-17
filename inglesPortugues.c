#include <stdlib.h>
#include <stdio.h>
#include <string.h>

typedef struct arvore{
    char palavra[25];
    struct text *listaENG;
    struct arvore *dir;
    struct arvore *esq;
} arvore;

typedef struct unidade{
    arvore *arvore;
    char fileName[100];
} unidade;

typedef struct text{
    char palavra[25];
    struct text *prox;
} text;

text * addText(text *lista, char *new);
text * contain(text *lista, char *new);
void showList(text *lista);
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

                FILE *file;
                char wordsFile[255];
	
                char wordENG[50];
                char wordsPtbr[156];
	            char *stringCat;

                unidades[cont-1].arvore = raiz;
                char *character = (char *) malloc(sizeof(char));
                printf("Arquivo com as palavras\n");
                scanf("%s",unidades[cont-1].fileName);
                file = fopen(unidades[cont-1].fileName , "r");

                while( (fscanf(file , "%s\n",wordsFile))!=EOF ){
                    char *palavra_ingles = (char *) malloc(sizeof(char));
                    char *palavra_portugues = (char *) malloc(sizeof(char));

                    if(wordsFile[0]=='%'){
                        int i;
                        
                        for(i=1; wordsFile[i]!='\0' ; i++)
                            character[i-1] = wordsFile[i];
                        
                        character[i] = '\0';

                        printf("%s\n",character);
                    }
                    else{
                        for(int i = 0 ; i < strlen(wordsFile) ; i++){
                            if(wordsFile[i]!=':'){
                                palavra_ingles[i] = wordsFile[i];
                            }
                            else{
                                // Pego a palvara em Ingles
                                palavra_ingles[i] = '\0';
                                

                                // Percorro a lista de palavras em portugues, cada uma vai ser um nó
                                int idxENG = 1 ; //Indice do vetor de palvras em ingles
                                int size = strlen(wordsFile);
                                
                                int x = i+1;
                                
                                while(wordsFile[x]!='\0'){
                                    palavra_portugues[idxENG-1] = wordsFile[x];

                                    if(wordsFile[x]== ',' ){
                                        palavra_portugues[idxENG-1] = '\0';
                                        idxENG = 0;           
                                        printf("%s\n",palavra_portugues);

                                        arvore *novo = (arvore *) malloc(sizeof(arvore));
                                        novo->dir = NULL;
                                        novo->esq = NULL;
                                        novo->listaENG = NULL;

                                        strcpy(novo->palavra , palavra_portugues);
                                        novo->listaENG = contain(novo->listaENG, palavra_ingles);

                                        addArv(&unidades[cont-1].arvore, novo);

                                    }

                                    x++;
                                    idxENG++;

                                    if(wordsFile[x] =='\0'){
                                        palavra_portugues[idxENG-1] ='\0';
                                        printf("%s\n",palavra_portugues);
                                        arvore *novo = (arvore *) malloc(sizeof(arvore));
                                        novo->dir = NULL;
                                        novo->esq = NULL;
                                        novo->listaENG = NULL;

                                        strcpy(novo->palavra , palavra_portugues);
                                        novo->listaENG = contain(novo->listaENG, palavra_ingles);

                                        addArv(&unidades[cont-1].arvore, novo);
                                    }
                                    
                                    
                                }
                            }
                        }
                    }
                }
                
                /*
                while( (fscanf(file , "%s%s\n", wordPTBR,wordsENG))!=EOF ){

                    arvore *novo = (arvore *) malloc(sizeof(arvore));
                    novo->dir = NULL;
                    novo->esq = NULL;
                    
                    int size = strlen(wordPTBR);
                    wordPTBR[size-1] = '\0';
                    
                    strcpy(novo->palavra , wordPTBR);
                    strcpy(novo->ingles , wordsENG);
                    addArv(&unidades[cont-1].arvore, novo);
                    
                }

                */
                enter = 1; 
                cont++;
                break;
            case 2:
                printf("Palavra a ser escluida : ");
                scanf("%s", palavraBuscar);
                
                printf("Qual unidade?\n");
                scanf("%d", &enter);
                deleteNode(&unidades[enter].arvore, palavraBuscar);
                enter = 1;
                break;
            case 3:
                printf("Qual unidade?\n");
                scanf("%d", &enter);/
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
        printf("{%s: ",raiz->palavra);
        showList(raiz->listaENG);
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
            printf("%s: ", no->palavra);
            showList(no->listaENG);
            printf("\n");
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
                (*raiz)->listaENG = aux->listaENG; 
                
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

text * addText(text *lista, char *new){
    text *novo = (text *) malloc(sizeof(text));
    strcpy(novo->palavra, new);
    novo->prox = NULL;
    return novo;
}

text * contain(text *lista, char *new){
    if(lista == NULL){ 
        lista = addText(lista, new);
    } else {
        text *aux;
        for(aux = lista; aux != NULL; aux = aux->prox){
            if(strcmp(aux->palavra, new)) break;
            if(aux->prox == NULL) break;
        }
        if(aux->prox == NULL) aux->prox = addText(lista, new);  
    }
    return lista;
}

void showList(text *lista){
    text *aux;
    for(aux = lista; aux != NULL; aux = aux->prox)
        printf("%s ", aux->palavra);
}