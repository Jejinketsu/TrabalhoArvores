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

int ehfolha(arvore *raiz);
void showArv(arvore *raiz);
arvore * busca(arvore *no, char *string,int *slot);
arvore * adicionaNo(arvore **raiz, char *string, arvore *paux);
arvore * addArv(arvore **raiz, char *string, char *promove, arvore **pai);
arvore * quebraNo(arvore **raiz, char *string, char *promove, arvore *subArvore);
arvore * criaNo(char *string, arvore *filhoEsq, arvore *filhoCen, arvore *filhoDir);
text * addList(text *lista , char *ingles);

int main(){

    arvore *raiz;
    raiz = NULL;
    char *promove = malloc(sizeof(char)*25);

    FILE *file;
	char wordsFile[255];
	char wordENG[50];
	char wordsPtbr[156];
	char *stringCat;
	file = fopen("words.txt", "r");

	char *unidade = (char *) malloc(sizeof(char));

    while( (fscanf(file , "%s\n",wordsFile))!=EOF ){
        char *palavra_ingles = (char *) malloc(sizeof(char));
		char *palavra_portugues = (char *) malloc(sizeof(char));

		if(wordsFile[0]=='%'){
			int i;
			
			for(i=1; wordsFile[i]!='\0' ; i++)
				unidade[i-1] = wordsFile[i];
			
			unidade[i] = '\0';

			// printf("%s\n",unidade);
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

			            if(wordsFile[x]== ',' ){
			                palavra_portugues[idxENG-1] = '\0';
			                idxENG = 0;
                            
                            arvore *novo = (arvore *) malloc(sizeof(arvore));
                            novo->dir = NULL;
                            novo->esq = NULL;
                            novo->meio = NULL;
                            novo->ingles1 = addList(novo->ingles1, palavra_ingles); 
                            
                            buscada = busca(raiz , palavra_portugues , &slot);
                            if(buscada == NULL){
			            	    addArv(&raiz, palavra_portugues, promove, NULL);
                            } else {
                                buscada->ingles1 = addList(novo->ingles1, palavra_ingles);
                            }
						}

						x++;
						idxENG++;

						if(wordsFile[x] =='\0'){
							palavra_portugues[idxENG-1] ='\0';
                            
                            buscada = busca(raiz , palavra_portugues , &slot);
                            if(buscada == NULL)           
                                addArv(&raiz, palavra_portugues, promove, NULL);

						}
						
						
					}
				}
            }
        }
    }


    showArv(raiz);
    printf("\n");

    return 0;
}

arvore * addArv(arvore **raiz, char *string, char *promove, arvore **pai){
    arvore *paux;

    if (*raiz == NULL) {
        *raiz = criaNo(string, NULL, NULL, NULL);
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
            *raiz = criaNo(promove, (*raiz), paux, NULL);
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
    arvore *paux;
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

    return paux;
}

arvore * criaNo(char *string, arvore *filhoEsq, arvore *filhoCen, arvore *filhoDir){
    arvore *no = malloc(sizeof(arvore));
    
    no->dir = filhoDir;
    no->meio = filhoCen;
    no->esq = filhoEsq;
    
    strcpy(no->palavra1, string);
    no->nInfos = 1;

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

arvore * busca(arvore *no, char *string, int *slot){
    
    arvore *resultado = (arvore *) malloc(sizeof(arvore));
    resultado = NULL;
    if(no != NULL){

        int returnCMP1 = strcmp(string, no->palavra1);
        int returnCMP2 = strcmp(string, no->palavra2);

        if(no->nInfos == 1){
            if(returnCMP1 == 0)
                return no;
            else if(returnCMP1 < 0)
                resultado = busca(no->esq , string , slot);
            else
                resultado = busca(no->meio , string , slot);
        }

        else{
            if(returnCMP1 == 0){
                return no;
            }
            else if(returnCMP2 == 0)
                return no;
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
        printf("{%s %s :",raiz->palavra1, raiz->palavra2);
        //showList(raiz->ingles);
        showArv(raiz->esq);
        showArv(raiz->meio);
        showArv(raiz->dir);
        printf("}");
    }
}
text *addList(text *lista , char *ingles){

    text *novo = (text *)malloc(sizeof(text));
    strcpy(novo->palavra , ingles);
    novo->prox = lista;
    text *aux;
    return novo;
}