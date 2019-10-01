#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#define tam 10000
#define repeat 30

typedef struct arv arvoreAVL;

struct arv{
    arvoreAVL *dir ,*esq;
    int valor;
};

arvoreAVL* nova_arvore();
arvoreAVL* criarFolha(int valor);
int maior(int a, int b);
void rotacaoRR(arvoreAVL** raiz);
void rotacaoLL(arvoreAVL** raiz);
void rotacaoLR(arvoreAVL** raiz);
void rotacaoRL(arvoreAVL** raiz);
int altura_do_no(arvoreAVL* raiz);
int inserirAVL(arvoreAVL** raiz, arvoreAVL* NO);
void mostraArvore(arvoreAVL* raiz);
int buscarvalor(arvoreAVL* raiz, int valor);
int maior_folha(arvoreAVL* raiz);
int menor_folha(arvoreAVL* raiz);
int* numeroaleatorio(int qtd);

#define TESTES 30

void main(){
    
    int casos[]={0 , 0};

    float tempoTotal = 0;
    int vetMaiorP[TESTES];
    int vetMenosP[TESTES];
    int vetDif[TESTES];
    float tempoIn[TESTES];
    float tempoBusc[TESTES];
    
    for(int i = 0; i<TESTES; i++){
        arvoreAVL* raiz = nova_arvore();
       
        int *aleatorio = numeroaleatorio(tam);

        clock_t inicio = clock();

        arvoreAVL* folha = NULL;
        for(int x = 0; x<100000; x++){
            folha = criarFolha(aleatorio[x]);
            inserirAVL(&raiz, folha);
        }        

        clock_t fim = clock();

        float tempo = (fim-inicio)*1000/CLOCKS_PER_SEC;
        tempoIn[i] = tempo;

        tempoTotal += tempo;

        int menorP = menor_folha(raiz);
        int maiorP = maior_folha(raiz); 
        
        vetMaiorP[i] = maiorP;
        vetMenosP[i] = menorP;
        
        int diferenca = abs(menorP - maiorP);
        vetDif[i] = diferenca;
        if(diferenca == 0)
            casos[0]++;
        else
            casos[1]++;

        inicio = clock();
        int find = buscarvalor(raiz,aleatorio[5]);    
        fim = clock();

        float tempoBusca = (inicio-fim)*1000/CLOCKS_PER_SEC;
        tempoBusc[i] = tempoBusca;

        /*
        printf("Caso %d\n",i+1);
        printf("Tempo gasto INSERIR: %lf \n", tempo);
        printf("Menor Nivel: %d\n", maiorP);
        printf("Maior Nivel: %d\n", maiorP);
        printf("Tempo de Busca: %lf \n", tempoBusca);
        printf("-------------\n");
        */
    }
    
    printf("vetMaiorP\n");
    for(int i = 0; i < TESTES; i++){
        printf("%d\n", vetMaiorP[i]);
    }

    printf("\nvetMenosP\n");
    for(int i = 0; i < TESTES; i++){
        printf("%d\n", vetMenosP[i]);
    }

    printf("\nvetDif\n");
    for(int i = 0; i < TESTES; i++){
        printf("%d\n", vetDif[i]);    
    }

    printf("\nDifNoRepeat\n");
    printf("0\n");
    printf("1\n");

    printf("\nDifOcorrencia\n");
    printf("%d\n",casos[0]);
    printf("%d\n",casos[1]);

    printf("\ntempoIn\n");
    for(int i = 0; i < TESTES; i++){
        printf("%.0f\n", tempoIn[i]);  
    }

    printf("\ntempoBusc\n");
    for(int i = 0; i < TESTES; i++){
        printf("%.0f\n", tempoBusc[i]);
    }

    printf("\nTempo total: %.0f\n", tempoTotal);

}


arvoreAVL* nova_arvore(){
    
    return NULL;
}

arvoreAVL* criarFolha(int valor){
    arvoreAVL* novoNo = (arvoreAVL*) malloc(sizeof(arvoreAVL));
    novoNo->esq = NULL;
    novoNo->dir = NULL;
    novoNo->valor = valor;
    return novoNo;
}

int maior(int a, int b){
    if(a>b)
        return a;
    return b;
}

void rotacaoRR(arvoreAVL** raiz){
    arvoreAVL *aux;
    aux = (*raiz)->dir;
    (*raiz)->dir = aux->esq;
    aux->esq = *raiz;
    *raiz = aux;
}

void rotacaoLL(arvoreAVL** raiz){
    arvoreAVL *aux;
    aux = (*raiz)->esq;
    (*raiz)->esq = aux->dir;
    aux->dir = *raiz;
    *raiz = aux;
}

void rotacaoLR(arvoreAVL** raiz){
    rotacaoRR(&((*raiz)->esq));
    rotacaoLL(raiz);
}

void rotacaoRL(arvoreAVL** raiz){
    rotacaoLL(&((*raiz)->dir));
    rotacaoRR(raiz);
}

int altura_do_no(arvoreAVL* raiz){
    int n = 0;
    if(raiz == NULL)
        n = -1;
    else
        n = maior(altura_do_no(raiz->esq), altura_do_no(raiz->dir))+1;
    return n;
}

int inserirAVL(arvoreAVL** raiz, arvoreAVL* NO){    
    int inseriu = 1;
    if(*raiz==NULL){
        *raiz = NO;
    }else{
        if(NO->valor < (*raiz)->valor){
            if(inserirAVL(&((*raiz)->esq),NO)==1){
                if( abs(altura_do_no((*raiz)->esq) - altura_do_no((*raiz)->dir) ) == 2){
                    if(NO->valor < ((*raiz)->esq)->valor){
                        //printf("LL\n");
                        rotacaoLL(raiz);
                    }else{
                        rotacaoLR(raiz);
                        //printf("LR\n");
                        
                    }
                }
            }
        }else{
            if(NO->valor > (*raiz)->valor){
                if( inserirAVL(&(*raiz)->dir,NO)==1){
                    if( abs(altura_do_no((*raiz)->esq) - altura_do_no((*raiz)->dir) ) == 2){
                        if(NO->valor > ((*raiz)->dir)->valor){
                            //printf("RR\n");
                            rotacaoRR(raiz);
                        }else{
                            //printf("RL\n");
                            rotacaoRL(raiz);
                        }
                    }
                }
            }else
                inseriu = 0;
        }
    }

    return inseriu;
}

void mostraArvore(arvoreAVL* raiz){
    if( raiz != NULL){
        mostraArvore(raiz->esq);
        printf("%d %d\n",raiz->valor, altura_do_no(raiz));
        mostraArvore(raiz->dir);
    }
}

int buscarvalor(arvoreAVL* raiz, int valor){
    int find = -1;

    if(raiz != NULL){
        if(raiz->valor == valor)
            find = valor;
        else if(raiz->valor > valor)
            find = buscarvalor(raiz->esq, valor);
        else
            find = buscarvalor(raiz->dir, valor);
    }
    return find;
}

int maior_folha(arvoreAVL* raiz){
    int maxD = 0, maxE = 0,depth = -1;

    if(raiz != NULL){
        maxE = maior_folha(raiz->esq)+1;
        maxD = maior_folha(raiz->dir)+1;

        if(maxD > maxE) 
            depth = maxD;
        else
            depth = maxE;
    }
    return depth;
}

int menor_folha(arvoreAVL* raiz){
    int maxD = 0, maxE = 0,depth = -1;

    if(raiz != NULL){
        maxE = maior_folha(raiz->esq)+1;
        maxD = maior_folha(raiz->dir)+1;

        if(maxD < maxE) 
            depth = maxD;
        else
            depth = maxE;
    }
    return depth;
}


int* numeroaleatorio(int qtd){
    int* vetor = (int*) malloc(qtd*sizeof(int));
    for(int x = 0; x<qtd; x++)
        vetor[x] = rand()%tam;
    return vetor;
}