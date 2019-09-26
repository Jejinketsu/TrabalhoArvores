#include <stdlib.h>
#include <stdio.h>
#include <string.h>

typedef struct text{
    char palavra[25];
    struct text *prox;
} text;

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

typedef struct unidade{
    arvore *arvore;
    char name[100];
} unidade;

int ehfolha(arvore *raiz);
void showArv(arvore *raiz);
void showList(text *lista);
text * addList(text *lista, char *ingles);

arvore * busca(arvore *no, char *string, int *slot);
arvore * addArv(arvore **raiz, char *string, char *promove, text **promoveT, arvore **pai);
arvore * adicionaNo(arvore **raiz, text *listaIng, char *string, arvore *paux);
arvore * quebraNo(arvore **raiz, char *string, text *listaIng, char *promove, text **promoveT, arvore *subArvore);
arvore * criaNo(char *string, text *listaIng, arvore *filhoEsq, arvore *filhoCen, arvore *filhoDir);

void cadastrar(char *fileName,unidade *unidades,int countUnidades);
void buscarPortugues(unidade *unidades, int contUnidades, char *string);

int main(){

    int enter = -1;
    int countUnidades = 0;
    unidade *unidades = malloc(sizeof(unidade));
    
    while(enter!=4){
        printf("1 - Cadastrar Unidade\n");
        printf("2 - Mostrar Unidade\n");
        printf("3 - Buscar Palavra Portugues\n");
        printf("4 - Sair\n");
        scanf("%d",&enter);

        char file[200];
        char palavra[50];
        int idx_unidade = 0;
        switch(enter){

            case 1:
                printf("Arquivo Com o dicionario ingles/portugues\n");
                scanf("%s",file);
                cadastrar(file,unidades,countUnidades);
                countUnidades++;
                break;
            case 2:
                printf("Qual unidade exibir? [0 a %d]\n",countUnidades-1);
                scanf("%d",&idx_unidade);
                printf("Unidade %d -----------------------------------\n",countUnidades-1);
                showArv(unidades[countUnidades-1].arvore);
                printf("----------------------------------------------\n");
                break;
            case 3:
                printf("Qual palavra deseja buscar?\n");
                scanf("%s", palavra);
                buscarPortugues(unidades, countUnidades, palavra);
        }

    }



    return 0;
}


void cadastrar(char *fileName,unidade *unidades,int countUnidades){

    arvore *raiz;
    raiz = NULL;
    char *promove = malloc(sizeof(char)*25);
    text **promoveT = malloc(sizeof(text*));

    FILE *file;
    char wordsFile[255];
    char wordENG[50];
    char wordsPtbr[156];
    char *stringCat;
    file = fopen(fileName, "r");

    char *unidadeNome = (char *) malloc(sizeof(char));

    while( (fscanf(file , "%s\n",wordsFile))!=EOF ){
        char *palavra_ingles = (char *) malloc(sizeof(char));
        char *palavra_portugues = (char *) malloc(sizeof(char));

        if(wordsFile[0]=='%'){
            int i;
            
            for(i=1; wordsFile[i]!='\0' ; i++)
                unidadeNome[i-1] = wordsFile[i];
            
            unidadeNome[i] = '\0';

        }else{
            int n = strlen(wordsFile);
            for(int i = 0 ; i < n; i++){
                if(wordsFile[i]!=':'){
                    palavra_ingles[i] = wordsFile[i];
                } else {
                    // Pego a palvara em Ingles
                    palavra_ingles[i] = '\0';


                    // Percorro a lista de palavras em portugues, cada uma vai ser um nó
                    int idxENG = 1 ; //Indice do vetor de palvras em ingles
                    int size = strlen(wordsFile);
                    
                    int x = i+1;
                    int slot;
                    
                    while(wordsFile[x]!='\0'){
                        palavra_portugues[idxENG-1] = wordsFile[x];
                        arvore *buscada = NULL;

                        if(wordsFile[x]== ','){
                            palavra_portugues[idxENG-1] = '\0';
                            idxENG = 0;
                            
                            buscada = busca(raiz, palavra_portugues, &slot);
                            if(buscada == NULL){
                                addArv(&raiz, palavra_portugues, promove, promoveT, NULL);
                                buscada = busca(raiz , palavra_portugues , &slot);
                                if(slot == 1) buscada->ingles1 = addList(buscada->ingles1, palavra_ingles);
                                else buscada->ingles2 = addList(buscada->ingles2, palavra_ingles);
                            } else {
                                if(slot == 1) buscada->ingles1 = addList(buscada->ingles1, palavra_ingles);
                                else buscada->ingles2 = addList(buscada->ingles2, palavra_ingles);
                            }
                        }

                        x++;
                        idxENG++;

                        if(wordsFile[x] =='\0'){
                            palavra_portugues[idxENG-1] ='\0';
                            
                            buscada = busca(raiz, palavra_portugues, &slot);
                            if(buscada == NULL){
                                addArv(&raiz, palavra_portugues, promove, promoveT, NULL);
                                buscada = busca(raiz , palavra_portugues , &slot);
                                if(slot == 1) buscada->ingles1 = addList(buscada->ingles1, palavra_ingles);
                                else buscada->ingles2 = addList(buscada->ingles2, palavra_ingles);
                            } else {
                                if(slot == 1) buscada->ingles1 = addList(buscada->ingles1, palavra_ingles);
                                else buscada->ingles2 = addList(buscada->ingles2, palavra_ingles);
                            }

                        }
                        
                        
                    }
                }
            }
        }
    }

    unidades[countUnidades].arvore = raiz;
    strcpy(unidades[countUnidades].name,unidadeNome);

}

void buscarPortugues(unidade *unidades, int contUnidades, char *string){
    int slot;
    for(int i = 0; i < contUnidades;i++){
        printf("Unidade %d\n", i);
        arvore *buscada = busca(unidades[i].arvore, string, &slot);
        if(buscada != NULL){
            if(slot == 1) showList(buscada->ingles1);
            else showList(buscada->ingles2);
        }
        printf("\n");
    }
}

arvore * addArv(arvore **raiz, char *string, char *promove, text **promoveT, arvore **pai){
    arvore *paux;

    if (*raiz == NULL) {
        *raiz = criaNo(string, NULL, NULL, NULL, NULL);
        paux = NULL;
    } else {
        if (ehfolha(*raiz)) {
            if ((*raiz)->nInfos == 1) {
                *raiz = adicionaNo(raiz, NULL, string, NULL);
                paux = NULL;
            } else{
                paux = quebraNo(raiz, string, NULL, promove, promoveT, NULL);
            }
        } else {
            if (strcmp(string, (*raiz)->palavra1) < 0) 
                paux = addArv(&((*raiz)->esq), string, promove, promoveT, raiz);
            else if (((*raiz)->nInfos == 1) || (strcmp(string, (*raiz)->palavra2) < 0))
                paux = addArv(&((*raiz)->meio), string, promove, promoveT, raiz);
            else
                paux = addArv(&((*raiz)->dir), string, promove, promoveT, raiz);
        }
    }

    if (paux != NULL) {
        if (pai == NULL) {
            *raiz = criaNo(promove, *promoveT, (*raiz), paux, NULL);
            paux = NULL;
        } else if ((*pai)->nInfos == 1) {
            *pai = adicionaNo(pai, *promoveT, promove, paux);
            paux = NULL;
        } else {
            char guardar[30];
            text *guardarIng = *promoveT;
            strcpy(guardar, promove);
            paux = quebraNo(pai, guardar, guardarIng, promove, promoveT, paux);
        }
    }

    return paux;
}

arvore * quebraNo(arvore **raiz, char *string, text *listaIng, char *promove, text **promoveT, arvore *subArvore){
    arvore *paux;
    int comparacao1 = strcmp(string, (*raiz)->palavra1);
    int comparacao2 = strcmp(string, (*raiz)->palavra2);
    
    if(comparacao1 < 0) {
        strcpy(promove, (*raiz)->palavra1);
        strcpy((*raiz)->palavra1, string);
        *promoveT = (*raiz)->ingles1;
        (*raiz)->ingles1 = listaIng;
    } else if(comparacao2 > 0) {
        strcpy(promove, (*raiz)->palavra2);
        strcpy((*raiz)->palavra2, string);
        *promoveT = (*raiz)->ingles2;
        (*raiz)->ingles2 = listaIng;
    } else {
        strcpy(promove, string);
        *promoveT = listaIng;
    }

    paux = criaNo((*raiz)->palavra2, (*raiz)->ingles2, (*raiz)->dir, subArvore, NULL);

    (*raiz)->nInfos = 1;
    strcpy((*raiz)->palavra2, "");
    (*raiz)->dir = NULL;
    (*raiz)->ingles2 = NULL;

    return paux;
}

arvore * criaNo(char *string, text *listaIng, arvore *filhoEsq, arvore *filhoCen, arvore *filhoDir){
    arvore *no = malloc(sizeof(arvore));
    
    no->dir = filhoDir;
    no->meio = filhoCen;
    no->esq = filhoEsq;
    
    strcpy(no->palavra1, string);
    no->nInfos = 1;
    no->ingles1 = listaIng;

    return no;
}

arvore * adicionaNo(arvore **raiz, text *listaIng, char *string, arvore *paux){
    int comparacao = strcmp(string, (*raiz)->palavra1);
    if(comparacao >= 0) {
        strcpy((*raiz)->palavra2, string);
        (*raiz)->dir = paux;
    }
    else {
        strcpy((*raiz)->palavra2, (*raiz)->palavra1);
        strcpy((*raiz)->palavra1, string);
        (*raiz)->ingles2 = (*raiz)->ingles1;
        (*raiz)->ingles1 = listaIng;
        (*raiz)->dir = (*raiz)->meio;
        (*raiz)->meio = paux;
    }
    (*raiz)->nInfos = 2;
    return (*raiz);
}

arvore * busca(arvore *no, char *string, int *slot){
    
    arvore *resultado = (arvore *) malloc(sizeof(arvore));
    resultado = NULL;
    if(no != NULL){

        int returnCMP1 = strcmp(string, no->palavra1);
        int returnCMP2 = strcmp(string, no->palavra2);
        *slot = 0;

        if(no->nInfos == 1){
            if(returnCMP1 == 0){
                *slot = 1;
                return no;
            }
            else if(returnCMP1 < 0)
                resultado = busca(no->esq , string , slot);
            else
                resultado = busca(no->meio , string , slot);
        }

        else{
            if(returnCMP1 == 0){
                *slot = 1;
                return no;
            }
            else if(returnCMP2 == 0){
                *slot = 2;
                return no;
            }
            else if(returnCMP1 < 0)
                resultado = busca(no->esq , string , slot);
            else if(returnCMP2>0)
                resultado = busca(no->dir , string , slot);
            else
                resultado = busca(no->meio , string , slot);

        }

    }

    return resultado;
}

int ehfolha(arvore *raiz){
    int folha = 0;
    if (raiz->dir == NULL && raiz->meio == NULL && raiz->esq == NULL)
        folha = 1;
    return folha;
}

void showArv(arvore *raiz){
    if(raiz!=NULL){
        printf("{%s ( ",raiz->palavra1);
        showList(raiz->ingles1);
        printf(") %s ( ", raiz->palavra2);
        showList(raiz->ingles2);
        printf(")");
        printf("}\n");
        showArv(raiz->esq);
        showArv(raiz->meio);
        showArv(raiz->dir);
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
    if(lista != NULL){
        printf("%s ", lista->palavra);
        showList(lista->prox);
    }
}