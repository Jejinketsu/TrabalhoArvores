#include <stdio.h>
#include <stdlib.h>
#include <time.h>

typedef struct arv arvAVL;

arvAVL* criarArv();
arvAVL* criarFolha(int *status, int nInicio, int nFinal,int endInicio, int endFim);
int ehFolha(arvAVL *no);
int maiorNum(int a, int b);
void showArv(arvAVL* raiz);
int altarvAVL(arvAVL* raiz);
void rotacaoRR(arvAVL** raiz);
int inverterStatus(int status);
void inserirAVL(arvAVL** raiz, arvAVL* no);
int alocar(arvAVL** raiz, arvAVL** pai, int qtdBlocos, int *status);
int liberar(arvAVL** raiz, arvAVL** pai, int qtdBlocos, int* status);
int BuscaLivre(arvAVL* raiz, int Valor);
int BuscaOcupado(arvAVL* raiz, int Valor);

struct arv{
    char status;
    int nInicio, nFinal;
    int endInicio, endFim;
    arvAVL *esq, *dir;
};

int main(){

	arvAVL* raiz = criarArv();
	int  status = 0, op = 1;

	clock_t tempoBuscaI, tempoBuscaF;
	clock_t tempoAlterarI, tempoAlterarF;
	float tempoBuscaDecorrido, tempoAlterarDecorrido;

	while(op != 0){
		printf("1. Criar Arvore\n2. Mostrar Arvore\n3. Alocar Blocos\n4. Liberar Blocos\n0. Sair\n");
		scanf("%d", &op);

		switch(op){
			case 1: {

				free(raiz);
				raiz = NULL;

				printf("O status do primeiro bloco eh 1-Ocupado ou 0-Livre: \n");
				scanf("%d", &status);

				int Inicio, Fim;
				printf("Insira o endereço incial e final: ");
				scanf("%d %d", &Inicio, &Fim);

				int auxInicio=0, auxFim=0, cont=0;

				while(auxFim < Fim){
					printf("Insira o endereço incial e final do %d\xa7 elemento: ", cont++);
					scanf("%d %d", &auxInicio, &auxFim);
					inserirAVL(&raiz, criarFolha(&status, Inicio, Fim, auxInicio, auxFim));
				}
				break;
			}

			case 2:
				showArv(raiz);
				printf("------------\n");
				break;

			case 3: {
			
				int auxalocar;
				printf("Quantidade de blocos para Alocar: ");
				scanf("%d", &auxalocar);


				tempoBuscaI = clock();
				int encontradoAloc = BuscaLivre(raiz, auxalocar);
				tempoBuscaF = clock();
				tempoBuscaDecorrido = ((tempoBuscaF- tempoBuscaI)*1000)/CLOCKS_PER_SEC;
				printf("Tempo gasto BUSCAR: %lf \n", tempoBuscaDecorrido);

				
				tempoAlterarI = clock();
				if( alocar(&raiz,NULL, auxalocar, &status) == 0)
					printf("Espaço insuficiente [%d].\n", encontradoAloc);
				else
					printf("Alocação bem sucedida [%d].\n", encontradoAloc);
				
				tempoAlterarF = clock();
				tempoAlterarDecorrido = ((tempoAlterarF-tempoAlterarI)*1000)/CLOCKS_PER_SEC;
				printf("Tempo gasto ALTERAR: %lf \n", tempoBuscaDecorrido);

				break;
			}

			case 4: {

				int auxliberar;
				printf("Quantidade de blocos para liberar: ");
				scanf("%d", &auxliberar);

				tempoBuscaI = clock();
				
				int encontradoLib = BuscaOcupado(raiz, auxliberar);

				tempoBuscaF = clock();
				tempoBuscaDecorrido = ((tempoBuscaF- tempoBuscaI)*1000)/CLOCKS_PER_SEC;
				printf("Tempo gasto BUSCAR: %lf \n", tempoBuscaDecorrido);
				

				tempoAlterarI = clock();
				if( liberar(&raiz,NULL, auxliberar, &status) == 0)
					printf("Espaço insuficiente [%d].\n", encontradoLib);
				else
					printf("Liberação bem sucedida [%d].\n", encontradoLib);
				
				tempoAlterarF = clock();
				tempoAlterarDecorrido = ((tempoAlterarF-tempoAlterarI)*1000)/CLOCKS_PER_SEC;
				printf("Tempo gasto ALTERAR: %lf \n", tempoBuscaDecorrido);
				break;
			}
			default:
				break;
		}
	}
    return 0;
}

int inverterStatus(int status){
	return status == 1 ? 0 : 1;
}


arvAVL* criarArv(){
	return NULL;
}

arvAVL* criarFolha(int *status, int nInicio, int nFinal,int endInicio, int endFim){
	arvAVL* novoNo = (arvAVL*) malloc(sizeof(arvAVL));
	novoNo->esq = NULL;
	novoNo->dir = NULL;
	novoNo->nInicio = nInicio;
	novoNo->nFinal = nFinal;
	novoNo->endInicio = endInicio;
	novoNo->endFim = endFim;

	if(*status == 0) novoNo->status = 'L';
	else novoNo->status = 'O';

	*status = inverterStatus(*status);
	return novoNo;
}

int ehFolha(arvAVL *no){
	return no->dir == NULL && no->esq == NULL ? 1 : 0;
}

int maiorNum(int a, int b){
	return a > b ? a : b;
}

void showArv(arvAVL* raiz){
	if(raiz != NULL){
		showArv(raiz->esq);
		printf("%c | %d-%d | %d-%d\n",raiz->status, raiz->nInicio, raiz->nFinal, raiz->endInicio, raiz->endFim);
		showArv(raiz->dir);
	}
}

int altarvAVL(arvAVL* raiz){
	int n = 0;

	if(raiz == NULL)n = -1;
	else n = maiorNum(altarvAVL(raiz->esq), altarvAVL(raiz->dir))+1;

	return n;
}

void rotacaoRR(arvAVL** raiz){
	arvAVL *aux;
	aux = (*raiz)->dir;
	(*raiz)->dir = aux->esq;
	aux->esq = *raiz;
	*raiz = aux;
}

void inserirAVL(arvAVL** raiz, arvAVL* no){
	if(*raiz==NULL){
		*raiz = no;
	}else{
		if( (*raiz)->nFinal >= no->endInicio){
			if(no->endInicio > (*raiz)->endFim){
				inserirAVL(&(*raiz)->dir, no);
				if(abs(altarvAVL((*raiz)->esq) - altarvAVL((*raiz)->dir)) == 2)
					rotacaoRR(raiz);
			}
		}
	}
}

int alocar(arvAVL** raiz, arvAVL** pai, int qtdBlocos, int *status){

	int resul = 0;

	if( *raiz != NULL){
		resul = alocar(&(*raiz)->esq,raiz, qtdBlocos, status);

		if(resul == 0){
			
			if((*raiz)->status == 'L'){
				int qtdLocal = (*raiz)->endFim - (*raiz)->endInicio + 1;
				
				if(pai == NULL && (*raiz)->esq == NULL && (*raiz)->dir == NULL){
					
					if(qtdLocal == qtdBlocos){
						(*raiz)->status = 'O';
					}else{
						int newFim = (*raiz)->endFim - qtdBlocos;
						int auxFim = (*raiz)->endFim;
						(*raiz)->endFim = newFim;

						int newInicio = newFim+1;

						inserirAVL(raiz, criarFolha(status, (*raiz)->nInicio, (*raiz)->nFinal, newInicio, auxFim));

					}
					resul = 1;
				}else if((*raiz)->esq == NULL && (*raiz)->dir == NULL){
					if(qtdLocal == qtdBlocos){

						int newFim = (*pai)->endFim + qtdBlocos;
						(*pai)->endFim = newFim;
						free(*raiz);
						*raiz = NULL;
						resul = 1;

					}else if(qtdLocal > qtdBlocos){
						if((*pai)->esq == *raiz){
							int newFim = (*raiz)->endFim - qtdBlocos;
							int newInicio = (*pai)->endInicio - qtdBlocos;
							(*raiz)->endFim = newFim;
							(*pai)->endInicio = newInicio;
						}else{
							int newInicio = (*raiz)->endInicio + qtdBlocos;
							int newFim = (*pai)->endFim + qtdBlocos;
							(*raiz)->endInicio = newInicio;
							(*pai)->endFim = newFim;
						}
						resul = 1;
					}
				}else{
			
					arvAVL** paiMaisEsq = raiz;
					arvAVL** maisEsq;
					if((*raiz)->dir == NULL){
						maisEsq = raiz;
					}else{
						maisEsq = &((*raiz)->dir);
					}
					while((*maisEsq)->esq != NULL){
						paiMaisEsq = maisEsq;
						maisEsq = &((*maisEsq)->esq) ;
					}
					if((*paiMaisEsq)->status == 'O'){
						maisEsq = paiMaisEsq;
					}	

					if(qtdLocal == qtdBlocos){

						arvAVL** paiMaisDir = raiz;
						arvAVL** maisDir;;
						if((*raiz)->esq == NULL ){
							maisDir = raiz;
						}else{
							maisDir = &((*raiz)->esq);	
						}
						while((*maisDir)->dir != NULL){
							paiMaisDir = maisDir;
							maisDir = &((*maisDir)->dir) ;
						}
						if((*paiMaisDir)->status == 'L'){
							maisDir = paiMaisDir;
						}
												
						int newInicio = (*maisDir)->endInicio;
						int newFim = (*maisEsq)->endFim;
						(*raiz)->endInicio = newInicio;
						(*raiz)->endFim = newFim;
						(*raiz)->status = 'O';

						if(paiMaisEsq == raiz){
							(*raiz)->dir = ((*maisEsq)->dir);
						}else{
							(*paiMaisEsq)->esq = ((*maisEsq)->dir);
						}

						if(paiMaisDir == raiz){
							(*raiz)->esq = ((*maisDir)->esq);
						}else {
							(*paiMaisDir)->dir = ((*maisDir)->esq);
						}
						
						free(*maisDir);
						free(*maisEsq);
						*maisDir = NULL;
						*maisEsq = NULL;	
						resul = 1;
					}else if(qtdLocal > qtdBlocos){
						int newFim = (*raiz)->endFim - qtdBlocos;
						int newInicio = (*maisEsq)->endInicio - qtdBlocos;
						(*raiz)->endFim = newFim;
						(*maisEsq)->endInicio = newInicio;
						resul = 1;
						
					}else{
						resul = alocar(&(*raiz)->dir, raiz, qtdBlocos, status);
					}	
				}
			}else{
				resul = alocar(&(*raiz)->dir, raiz, qtdBlocos, status);
			}
		}
	}
	return resul;
}

int liberar(arvAVL** raiz, arvAVL** pai, int qtdBlocos, int* status){

	int resul = 0;

	if( *raiz != NULL){
		resul = liberar(&(*raiz)->esq, raiz, qtdBlocos, status);
	
		if(resul == 0){
				
			if((*raiz)->status == 'O'){
				int qtdLocal = (*raiz)->endFim - (*raiz)->endInicio + 1;
				
				if(pai == NULL && ehFolha(*raiz)){
					
					if(qtdLocal == qtdBlocos){
						(*raiz)->status = 'L';
					}else{
						int newFim = (*raiz)->endFim - qtdBlocos;
						int auxFim = (*raiz)->endFim;
						(*raiz)->endFim = newFim;

						int newInicio = newFim+1;

						inserirAVL(raiz, criarFolha(status, (*raiz)->nInicio,(*raiz)->nFinal,newInicio,auxFim));

					}
					resul = 1;
				}else if((*raiz)->dir == NULL){
					if(qtdLocal == qtdBlocos){
						//Buscar os mais interarvAVLs para unir;
						int newFim = (*pai)->endFim + qtdBlocos;
						(*pai)->endFim = newFim;
						free(*raiz);
						*raiz = NULL;
						resul = 1;
					}else if(qtdLocal > qtdBlocos){
						if((*pai)->esq == *raiz){
							int newFim = (*raiz)->endFim - qtdBlocos;
							int newInicio = (*pai)->endInicio - qtdBlocos;
							(*raiz)->endFim = newFim;
							(*pai)->endInicio = newInicio;
						}else{
							int newInicio = (*raiz)->endInicio + qtdBlocos;
							int newFim = (*pai)->endFim + qtdBlocos;
							(*raiz)->endInicio = newInicio;
							(*pai)->endFim = newFim;
						}
						resul = 1;
					}
				}else{
			
					//Buscando o mais a esquerda Ocupado.
					arvAVL** paiMaisEsq = raiz;
					arvAVL** maisEsq;
					if((*raiz)->dir == NULL){
						maisEsq = raiz;
					}else{
						maisEsq = &((*raiz)->dir);
					}
					while((*maisEsq)->esq != NULL){
						paiMaisEsq = maisEsq;
						maisEsq = &((*maisEsq)->esq) ;
					}
					if((*paiMaisEsq)->status == 'L'){
						maisEsq = paiMaisEsq;
					}
					//free(*paiMaisEsq);		
					
					if(qtdLocal == qtdBlocos){

						//Buscando o mais a direita Ocupado.
						arvAVL** paiMaisDir = raiz;
						arvAVL** maisDir;
						if((*raiz)->esq == NULL ){
							maisDir = raiz;
						}else{
							maisDir = &((*raiz)->esq);	
						}

						int newInicio = (*maisDir)->endInicio;
						int newFim = (*maisEsq)->endFim;
						(*raiz)->endInicio = newInicio;
						(*raiz)->endFim = newFim;
						(*raiz)->status = 'L';

						while((*maisDir)->dir != NULL){
							paiMaisDir = maisDir;
							maisDir = &((*maisDir)->dir) ;
						}
						if((*paiMaisDir)->status == 'O'){
							maisDir = paiMaisDir;
						}
						
						printf("ERRO[%d][%d]\n", newInicio, newFim);

						if(paiMaisEsq == raiz){
							(*raiz)->dir = ((*maisEsq)->dir);
						}else{
							(*paiMaisEsq)->esq = ((*maisEsq)->dir);
						}

						if(paiMaisDir == raiz){
							(*raiz)->esq = ((*maisDir)->esq);
						}else {
							(*paiMaisDir)->dir = ((*maisDir)->esq);
						}

						free(*maisDir);
						free(*maisEsq);
						*maisDir = NULL;
						*maisEsq = NULL;	
						resul = 1;
					}else if(qtdLocal > qtdBlocos){
						int newFim = (*raiz)->endFim - qtdBlocos;
						int newInicio = (*maisEsq)->endInicio - qtdBlocos;
						(*raiz)->endFim = newFim;
						(*maisEsq)->endInicio = newInicio;
						resul = 1;
					}else{
						resul = liberar(&(*raiz)->dir, raiz, qtdBlocos, status);
					}	
				}
			}else{
				resul = liberar(&(*raiz)->dir, raiz, qtdBlocos, status);
			}
		}
	}
	return resul;
}

int BuscaLivre(arvAVL* raiz, int Valor){
	int encontrado = 0;

	if(raiz != NULL){
		encontrado = BuscaLivre(raiz->esq, Valor);
		if((raiz)->status == 'L'){
			int qtdLocal = (raiz)->endFim - (raiz)->endInicio + 1;
			if(Valor <= qtdLocal){
				encontrado = 1;
			}
		}
		if(encontrado==0){
			encontrado = BuscaLivre(raiz->dir, Valor);
		}
	}
	return encontrado;
}

int BuscaOcupado(arvAVL* raiz, int Valor){
	int encontrado = 0;

	if(raiz != NULL){
		encontrado = BuscaOcupado(raiz->esq, Valor);
		if((raiz)->status == 'L'){
			int qtdLocal = (raiz)->endFim - (raiz)->endInicio +1;
			if(Valor <= qtdLocal){
				encontrado = 1;
			}
		}
		if(encontrado==0){
			encontrado = BuscaOcupado(raiz->dir, Valor);
		}
	}
	return encontrado;
}