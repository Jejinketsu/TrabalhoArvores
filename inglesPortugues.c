#include <stdlib.h>
#include <stdio.h>
#include <string.h>

typedef struct arvore{
    char palavra[25];
    struct text *ingles;
    struct arvore *dir;
    struct arvore *esq;
} arvore;

typedef struct unidade{
    arvore *arvore;
    char name[100];
    char fileName[100];
} unidade;

typedef struct text{
    char palavra[25];
    struct text *prox;
} text;

void addArv(arvore **raiz, arvore *new);
void showArv(arvore *raiz);
arvore * busca(arvore *no, char *string);
void remover(arvore **raiz, char *string);
void deleteNode(arvore **raiz, char *string);
arvore * freeArv(arvore *raiz);
void showList(text *lista);
text *addList(text *lista , char *ingles);

int main (){

    char portugues[35];
    char equivalente[500];
    char palavra[100];
    char palavraBuscar[50];
    int i = 0, cont = 1, enter = 1;

    unidade *unidades = malloc(sizeof(unidade));

    while(enter != 0){

        printf("\n1. Cadastrar Unidade.\n2. Remover Palavra.\n3. Mostrar Unidade.\n4. Buscar Palavra.\n0. Sair\n");
        scanf("%d", &enter);
        switch (enter){
            case 1:
                unidades = realloc(unidades, sizeof(unidade)*cont);
                arvore *raiz = NULL;

                char wordsFile[255];
                
                char wordENG[50];
                char wordsPtbr[156];
                char *stringCat;

                unidades[cont-1].arvore = raiz;
                printf("Arquivo com as palavras\n");
                scanf("%s",unidades[cont-1].fileName);
                
                FILE *file = fopen(unidades[cont-1].fileName, "r");

                // Nome da unidade
                char *unidadeName = (char *) malloc(sizeof(char));

                while( (fscanf(file , "%s\n", wordsFile))!=EOF ){
                    
                    // Alocação de variação para o tratamento da String
                    char *palavra_ingles = (char *) malloc(sizeof(char));
                    char *palavra_portugues = (char *) malloc(sizeof(char));

                    // Pego o nome da unidade a ser cadastrada
                    if(wordsFile[0]=='%'){
                        int i;
                        
                        for(i=1; wordsFile[i]!='\0' ; i++)
                            unidadeName[i-1] = wordsFile[i];
                        
                        unidadeName[i] = '\0';
                        strcpy(unidades[cont-1].name , unidadeName);

                    // Tratamento das palavras em ingles e portugues
                    }else{
                        for(int i = 0 ; i < strlen(wordsFile) ; i++){
                            if(wordsFile[i]!=':'){
                                palavra_ingles[i] = wordsFile[i];
                            }
                            else{

                                // Preparo para inserir as palavras na arvore
                                arvore *no_busca = (arvore *) malloc(sizeof(arvore));


                                // Pego a palvara em Ingles
                                palavra_ingles[i] = '\0';

                                // Percorro a lista de palavras em portugues, cada uma vai ser um nó
                                int idxENG = 1 ; //Indice do vetor de palvras em ingles
                                int size = strlen(wordsFile);
                                
                                int x = i+1;
                                
                                while(wordsFile[x]!='\0'){
                                    palavra_portugues[idxENG-1] = wordsFile[x];

                                    // Exibi as N-1 palavras
                                    if(wordsFile[x]== ',' ){
                                        palavra_portugues[idxENG-1] = '\0';
                                        idxENG = 0;
                                       
                                        arvore *novo = (arvore *) malloc(sizeof(arvore));
                                        novo->dir = NULL;
                                        novo->esq = NULL;
                                        novo->ingles = addList(novo->ingles, palavra_ingles);

                                        no_busca = busca(unidades[cont-1].arvore,palavra_portugues);
                                        if(no_busca == NULL){
                                            strcpy(novo->palavra , palavra_portugues);
                                            addArv(&unidades[cont-1].arvore, novo);
                                        }else{

                                            no_busca->ingles = addList(no_busca->ingles, palavra_ingles);
                                        }           

                                    }

                                    x++;
                                    idxENG++;

                                    // Exibe a N palavra
                                    if(wordsFile[x] =='\0'){
                                        palavra_portugues[idxENG-1] ='\0';

                                        arvore *novo = (arvore *) malloc(sizeof(arvore));
                                        novo->dir = NULL;
                                        novo->esq = NULL;
                                        novo->ingles = addList(novo->ingles, palavra_ingles);

                                        no_busca = busca(unidades[cont-1].arvore,palavra_portugues);
                                        if(no_busca == NULL){
                                            strcpy(novo->palavra , palavra_portugues);
                                            addArv(&unidades[cont-1].arvore, novo);;
                                        }else{
                                            no_busca->ingles = addList(no_busca->ingles, palavra_ingles);
                                        }  

                                    }                                   
                                }
                            }
                        }
                    }                    
                }

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
                arvore * res = busca(unidades[enter].arvore,palavraBuscar);
                printf("%s\n", );
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
        printf("{%s :",raiz->palavra);
        showList(raiz->ingles);
        showArv(raiz->esq);
        showArv(raiz->dir);
        printf("}");
    }
}

arvore *busca(arvore *no, char *string){
    
    arvore *resultado = NULL;

    if(no != NULL){
        int returnCMP = strcmp(string, no->palavra);
        if(returnCMP != 0){
            if(returnCMP < 0) busca(no->esq, string);
            else busca(no->dir, string);
        } else {
            resultado = no;
        }
    }

    return resultado;
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
                (*raiz)->ingles =  aux->ingles; 
                
                strcpy(aux->palavra, string);
                deleteNode((&(*raiz)->esq) , string);
            }

        }
    }
}


text *addList(text *lista , char *ingles){
    text *novo = (text *)malloc(sizeof(text));
    strcpy(novo->palavra , ingles);
    novo->prox = lista;
    text *aux;
    return novo;
}


void showList(text *lista){
    text *aux;
    for(aux = lista; aux != NULL; aux = aux->prox)
        printf("%s ", aux->palavra);
}

