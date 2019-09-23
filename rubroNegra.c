#include<stdio.h>
#include<stdlib.h>
#define RED 1
#define BLACK 2

typedef struct Tree{
    char *portugues;
    struct text *ingles;
    int cor;
    struct Tree *esq, * dir, *pai;
}Tree;

typedef struct text{
    char palavra[25];
    struct text *prox;
} text;


struct Tree *rotacionaEsquerda(struct Tree * A);
struct Tree *rotacionaDireita(struct Tree * A);
struct Tree *move2EsqRED(struct Tree * H);
struct Tree * balancear(struct Tree * H);
void trocaCor(struct Tree *H);



void main(){

}


struct Tree *rotacionaEsquerda(struct Tree * A){
	struct Tree * B = A->dir;
	A->dir = B->esq;
	B->esq = A;
	B->cor = RED;
	return B;
}

struct Tree *rotacionaDireita(struct Tree * A){
	struct Tree * B = A->esq;
	A->esq = B->dir;
	B->dir = A;
	B->cor = RED;
	return B;
}



struct Tree *move2EsqRED(struct Tree * H){
	trocaCor(H);

	if(H->dir->esq->cor == RED){
		H->dir = rotacionaDireita(H->dir);
		H = rotacionaEsquerda(H);
		trocaCor(H);
	}
	return H;
}


struct Tree * balancear(struct Tree * H){
	if(H->dir->cor == RED)
		H = rotacionaEsquerda(H);

	if(H->esq != NULL && H->dir->cor == RED && H->esq->esq->cor == RED)
		H = rotacionaDireita(H);

	if(H->esq->cor == RED && H->dir->cor == RED)
		trocaCor(H);

	return H;

}

void trocaCor(struct Tree *H){
	if(H->cor == 1)
		H->cor = 0;
	else
		H->cor = 1;

	if(H->dir->cor == 1 || H->dir == NULL )
		H->dir->cor = 0;

	if(H->esq->cor == 1 || H->esq == NULL )
		H->esq->cor = 0;

	if(H->dir->cor == 0 )
		H->dir->cor = 1;

	if(H->esq->cor == 0 )
		H->esq->cor = 1;

}