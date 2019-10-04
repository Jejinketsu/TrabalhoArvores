#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#define tam 10000
#define repeat 30

typedef struct arv arvoreAVL;

struct arv{
    arvoreAVL *dir ,*esq;
    int enderecoInicio, enderecoFim;
    char status;
};

arvoreAVL* nova_arvore();
int ehFolha(arvoreAVL *no);
arvoreAVL* criarFolha(int *status, int enderecoInicio, int enderecoFim);
int alocar(arvoreAVL** raiz, arvoreAVL** pai, int qtdBlocos, int *status, int fim);
int liberar(arvoreAVL** raiz, arvoreAVL** pai, int qtdBlocos, int* status, int fim);
int maior(int a, int b);
int inverterStatus(int status);
void rotacaoRR(arvoreAVL** raiz);
int altura_do_no(arvoreAVL* raiz);
int inserirAVL(arvoreAVL** raiz, arvoreAVL* NO, int fim);
int buscaEspaco(arvoreAVL *raiz, int espaco);
void concatenaNo(arvoreAVL **raiz);
void mostraArvore(arvoreAVL* raiz);

#define TESTES 30

void main(){
    arvoreAVL *raiz = NULL;
    int fim, status = 0, menuOpc = 1;

    while(menuOpc != 0){
        printf("1. Criar árvore\n2. Mostrar Arvore\n3. Alocar no\n4. Liberar no\n0.sair\nin: ");
        scanf("%d", &menuOpc);

        switch (menuOpc){
            case 1:
                printf("Qual a quantidade de blocos em MB? ");
                scanf("%d", &fim);
                printf("O primeiro bloco é livre ou coupado? (1. Livre/0. Ocupado): ");
                scanf("%d", &status);

                int auxInicio = 0, auxFim = 0;
                while(auxFim <= fim){
                    printf("Digite o inicio e fim do bloco: ");
                    scanf("%d %d", &auxInicio, &auxFim);
                    inserirAVL(&raiz, criarFolha(&status, auxInicio, auxFim), fim);
                    status = inverterStatus(status);
                    if(fim == auxFim) break;
                }
                break;
        
            case 2:
                mostraArvore(raiz);
                break;

            case 3:
                printf("Digite a quantidade de blocos para alocar: ");
				int auxalocar;
				scanf("%d", &auxalocar);
                int procuraAloc = buscaEspaco(raiz, auxalocar);
                if( alocar(&raiz,NULL, auxalocar, &status, fim) == 0)
					printf("Espaço insuficiente.\n");
				else
					printf("Alocação %d bem sucedida.\n", procuraAloc);
                break;

            case 4:
                printf("Digite a quantidade de blocos para liberar: ");
				int auxliberar;
				scanf("%d", &auxliberar);
                int procuraLib = buscaEspaco(raiz, auxliberar);
                if( liberar(&raiz,NULL, auxliberar, &status, fim) == 0)
					printf("Espaço %d insuficiente.\n", procuraLib);
				else
					printf("Liberação %d bem sucedida.\n", procuraLib);
                break;

            default:
                break;
        }
    }
}

arvoreAVL * nova_arvore(){
    return NULL;
}

int ehFolha(arvoreAVL *no){
    return (no->dir == NULL && no->esq == NULL) ? 1 : 0;
}

arvoreAVL * criarFolha(int *status, int enderecoInicio, int enderecoFim){
    arvoreAVL* novoNo = (arvoreAVL*) malloc(sizeof(arvoreAVL));
    novoNo->esq = NULL;
    novoNo->dir = NULL;
    novoNo->enderecoInicio = enderecoInicio;
    novoNo->enderecoFim = enderecoFim;

    if(*status == 0) novoNo->status = 'O';
    else novoNo->status = 'L';

    return novoNo;
}

int maior(int a, int b){
    return a>b ? a : b;
}

int inverterStatus(int status){
    return status == 1 ? 0 : 1;
}

void rotacaoRR(arvoreAVL** raiz){
    arvoreAVL *aux;
    aux = (*raiz)->dir;
    (*raiz)->dir = aux->esq;
    aux->esq = *raiz;
    *raiz = aux;
}

int altura_do_no(arvoreAVL* raiz){
    int n = 0;
    if(raiz == NULL)
        n = -1;
    else
        n = maior(altura_do_no(raiz->esq), altura_do_no(raiz->dir))+1;
    return n;
}

int inserirAVL(arvoreAVL** raiz, arvoreAVL* NO, int fim){    
    int inseriu = 1;
    if(*raiz==NULL){
        *raiz = NO;
    }else{
        if(fim >= NO->enderecoInicio){
            if(NO->enderecoInicio > (*raiz)->enderecoFim){
                inserirAVL(&(*raiz)->dir, NO, fim);

                if(abs(altura_do_no((*raiz)->esq) - altura_do_no((*raiz)->dir)) == 2)
                    rotacaoRR(raiz);
            }
        }else{
            inseriu = 0;
        }
    }

    return inseriu;
}

int alocar(arvoreAVL** raiz, arvoreAVL** pai, int qtdBlocos, int *status, int fim){
	int resul = 0;
	if(*raiz != NULL){
		resul = alocar(&(*raiz)->esq,raiz, qtdBlocos, status, fim);
		if(resul == 0){
			if((*raiz)->status == 'L'){
				int qtdLocal = (*raiz)->enderecoFim - (*raiz)->enderecoInicio + 1;
				if(pai == NULL && ehFolha((*raiz))){
					if(qtdLocal == qtdBlocos){
						(*raiz)->status = 'O';
                        *status = inverterStatus(*status);
					}else{
						int newfim = (*raiz)->enderecoFim - qtdBlocos;
						int auxfim = (*raiz)->enderecoFim;
						(*raiz)->enderecoFim = newfim;
						int newinicio = newfim+1;
						inserirAVL(raiz, criarFolha(status, newinicio,auxfim), fim);
					}
					resul = 1;
				}else if(ehFolha(*raiz)){
					if(qtdLocal == qtdBlocos){
						int newfim = (*pai)->enderecoFim + qtdBlocos;
						(*pai)->enderecoFim = newfim;
						free(*raiz);
						*raiz = NULL;
						resul = 1;
					}else if(qtdLocal > qtdBlocos){
						if((*pai)->esq == *raiz){
							int newfim = (*raiz)->enderecoFim - qtdBlocos;
							int newinicio = (*pai)->enderecoInicio - qtdBlocos;
							(*raiz)->enderecoFim = newfim;
							(*pai)->enderecoInicio = newinicio;
						}else{
							int newinicio = (*raiz)->enderecoInicio + qtdBlocos;
							int newfim = (*pai)->enderecoFim + qtdBlocos;
							(*raiz)->enderecoInicio = newinicio;
							(*pai)->enderecoFim = newfim;
						}
						resul = 1;
					}
				}else{
					arvoreAVL** paiMaisEsq = raiz;
					arvoreAVL** maisEsq = &((*raiz)->dir);
					while((*maisEsq)->esq != NULL){
						paiMaisEsq = maisEsq;
						maisEsq = &((*maisEsq)->esq) ;
					}
					if((*paiMaisEsq)->status == 'O'){
						maisEsq = paiMaisEsq;
					}		
					if(qtdLocal == qtdBlocos){
						arvoreAVL** paiMaisDir = raiz;
						arvoreAVL** maisDir = &((*raiz)->esq);
						while((*maisDir)->dir != NULL){
							paiMaisDir = maisDir;
							maisDir = &((*maisDir)->dir) ;
						}
						if((*paiMaisDir)->status == 'O'){
							maisDir = paiMaisDir;
						}
						int newinicio = (*maisDir)->enderecoInicio;
						int newfim = (*maisEsq)->enderecoFim;
						(*raiz)->enderecoInicio = newinicio;
						(*raiz)->enderecoFim = newfim;
						(*raiz)->status = 'O';
						free(*maisDir);
						free(*maisEsq);
						*maisDir = NULL;
						*maisEsq = NULL;	
						resul = 1;
					}else if(qtdLocal > qtdBlocos){
						int newfim = (*raiz)->enderecoFim - qtdBlocos;
						int newinicio = (*maisEsq)->enderecoInicio - qtdBlocos;
						(*raiz)->enderecoFim = newfim;
						(*maisEsq)->enderecoInicio = newinicio;
						resul = 1;					
					}else{
						resul = alocar(&(*raiz)->dir,raiz, qtdBlocos, status, fim);
					}	
				}
			}else{
				resul = alocar(&(*raiz)->dir,raiz, qtdBlocos, status, fim);
			}
		}
	}
	return resul;
}

int liberar(arvoreAVL** raiz, arvoreAVL** pai, int qtdBlocos, int* status, int fim){
	int resul = 0;
	if( *raiz != NULL){
		resul = liberar(&(*raiz)->esq,raiz, qtdBlocos, status, fim);
		if(resul == 0){
			if((*raiz)->status == 'O'){
				int qtdLocal = (*raiz)->enderecoFim - (*raiz)->enderecoInicio +1;
				if( pai == NULL && (*raiz)->esq == NULL && (*raiz)->dir == NULL){
					if(qtdLocal == qtdBlocos){
						(*raiz)->status = 'L';
						*status = 1 - (*status);
					}else{
						int newfim = (*raiz)->enderecoFim - qtdBlocos;
						int auxfim = (*raiz)->enderecoFim;
						(*raiz)->enderecoFim = newfim;
						int newinicio = newfim+1;
						inserirAVL(raiz, criarFolha(status, newinicio,auxfim), fim);
					}
					resul = 1;
				}else if((*raiz)->dir == NULL){
					if(qtdLocal == qtdBlocos){
						int newfim = (*pai)->enderecoFim + qtdBlocos;
						(*pai)->enderecoFim = newfim;
						free(*raiz);
						*raiz = NULL;
						resul = 1;
					}else if(qtdLocal > qtdBlocos){
						if((*pai)->esq == *raiz){
							int newfim = (*raiz)->enderecoFim - qtdBlocos;
							int newinicio = (*pai)->enderecoInicio - qtdBlocos;
							(*raiz)->enderecoFim = newfim;
							(*pai)->enderecoInicio = newinicio;
						}else{
							int newinicio = (*raiz)->enderecoInicio + qtdBlocos;
							int newfim = (*pai)->enderecoFim + qtdBlocos;
							(*raiz)->enderecoInicio = newinicio;
							(*pai)->enderecoFim = newfim;
						}
						resul = 1;
					}
				}else{
					arvoreAVL** paiMaisEsq = raiz;
					arvoreAVL** maisEsq = &((*raiz)->dir);
					while((*maisEsq)->esq != NULL){
						paiMaisEsq = maisEsq;
						maisEsq = &((*maisEsq)->esq) ;
					}
					if((*paiMaisEsq)->status == 'L'){
						maisEsq = paiMaisEsq;
					}
					if(qtdLocal == qtdBlocos){
						arvoreAVL** paiMaisDir = raiz;
						arvoreAVL** maisDir = &((*raiz)->esq);
						while((*maisDir)->dir != NULL){
							paiMaisDir = maisDir;
							maisDir = &((*maisDir)->dir) ;
						}
						if((*paiMaisDir)->status == 'L'){
							maisDir = paiMaisDir;
						}
						int newinicio = (*maisDir)->enderecoInicio;
						int newfim = (*maisEsq)->enderecoFim;
						(*raiz)->enderecoInicio = newinicio;
						(*raiz)->enderecoFim = newfim;
						(*raiz)->status = 'L';
						free(*maisDir);
						free(*maisEsq);
						*maisDir = NULL;
						*maisEsq = NULL;	
						resul = 1;
					}else if(qtdLocal > qtdBlocos){
						int newfim = (*raiz)->enderecoFim - qtdBlocos;
						int newinicio = (*maisEsq)->enderecoInicio - qtdBlocos;
						(*raiz)->enderecoFim = newfim;
						(*maisEsq)->enderecoInicio = newinicio;
						resul = 1;
					}else{
						resul = liberar(&(*raiz)->dir,raiz, qtdBlocos, status, fim);
					}	
				}
			}else{
				resul = liberar(&(*raiz)->dir,raiz, qtdBlocos, status, fim);
			}
		}
	}
	return resul;
}

int buscaEspaco(arvoreAVL *raiz, int espaco){
    int flag = 0;
    if(raiz != NULL){
        
        buscaEspaco(raiz->esq, espaco);

        if(raiz->status = 'L'){
            if((raiz->enderecoFim - raiz->enderecoInicio) >= espaco){
                flag = 1;
            }
        }
        
        if(!flag) buscaEspaco(raiz->dir, espaco);   
    }
    return flag;
}

void concatenaNo(arvoreAVL **raiz){
    arvoreAVL * aux = (*raiz)->dir;

    (*raiz)->enderecoFim = aux->enderecoFim;
    (*raiz)->dir = aux->dir;
    free(aux);
}

void mostraArvore(arvoreAVL* raiz){
    if( raiz != NULL){
        mostraArvore(raiz->esq);
        printf("status: %c ! inicio: %d ! fim: %d\n",raiz->status, raiz->enderecoInicio, raiz->enderecoFim);
        mostraArvore(raiz->dir);
    }
}