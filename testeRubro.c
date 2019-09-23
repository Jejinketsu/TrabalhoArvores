#include <stdio.h>
#include <stdlib.h>
#include <time.h>

typedef struct node{
    int info;
    int isRed;
    struct node* left;
    struct node* right;
} node;

typedef struct ocorrencia{
    int num, casos;
    struct ocorrencia *prox;
}ocorrencia;

node* insert(node* no, int valor);
node* insertLLRN(node* no, int valor);
int cor(node* no);
node* rotacionaEsquerda(node* no);
node* rotacionaDireita(node* no);
node* troca_cor(node* no);
void mostra_arvore(node* raiz);

ocorrencia * addNum(ocorrencia *lista, int new);
ocorrencia * contain(ocorrencia *lista, int num);
void mostrarLista(ocorrencia *lista);
void addArv(node *raiz, node *new);
node * freeArv(node *raiz);
int nos(node *no);
int ramos(node *no);
int profundidade(node *no, int valor);
int descendentes(node *no);
int altura(node *no, int valor, int maior);

node * busca(node *no, int valor);

void main(void){

    srand(time(NULL));
    float tempoTotal = 0;
    ocorrencia *lista = malloc(sizeof(ocorrencia));
    node* raiz = NULL;
    // raiz = insert(raiz, 5);
    // mostra_arvore(raiz);


    for(int i = 0; i < 30; i++){
        node *raiz = malloc(sizeof(node));
        raiz->info = rand()%100;

        clock_t inicio = clock();

        for(int i = 1; i < 30000; i++){
            node *arv = malloc(sizeof(node));
            arv->info = rand()%100;
            addArv(raiz, arv);
        }

        clock_t fim = clock();

        float tempo = (fim-inicio)*1000 / CLOCKS_PER_SEC;
        tempoTotal += tempo;

        inicio = clock();
        busca(raiz, 50);
        fim = clock();

        float tempoBusca = (fim-inicio)*1000 / CLOCKS_PER_SEC;

        int maiorP = altura(raiz, raiz->info, 1);
        int menorP = altura(raiz, raiz->info, 0);

        //freeArv(raiz);

        int diferenca = maiorP-menorP;
        contain(lista, diferenca);

        printf("----------Teste %d-------------\n", i+1);
        printf("Maior profundidade: %d\n", maiorP);
        printf("Menor profundidade: %d\n", menorP);
        mostrarLista(lista);
        printf("\nTempo de insercao: %.5fms\n", tempo);
        printf("Tempo de busca: %.5fms\n\n", tempoBusca);

    }

}


node* insert(node* no, int valor){
    no = insertLLRN(no, valor);
    no->isRed = 0;
    return no;
}

node* insertLLRN(node* no, int valor){
    node* novo;

    if(no == NULL){
        novo = (node*)malloc(sizeof(node));
        novo->info = valor;
        novo->left = NULL;
        novo->right = NULL;
        novo->isRed = 1;
        return novo;
    }

    if(valor < no->info)
        no->left = insertLLRN(no->left, valor);
    else
        no->right = insertLLRN(no->right, valor);


    if(cor(no->left) == 0 && cor(no->right) == 1)
        no = rotacionaEsquerda(no);

    if(cor(no->left) == 1 && cor(no->left->left) == 1)
        no = rotacionaDireita(no);

    if(cor(no->left) == 1 && cor(no->right) == 1){
        no = troca_cor(no);
    }

    return no;
}

int cor(node* no){
    return (no == NULL) ? 0 : no->isRed;
}

node* rotacionaEsquerda(node* no){
    node* novo;
    novo = (node*)malloc(sizeof(node));

    novo = no->right;
    no->right = novo->left;
    novo->left = no;
    novo->isRed = no->isRed;
    no->isRed = 1;
    return novo;
}

node* rotacionaDireita(node* no){
    node* novo;
    novo = (node*)malloc(sizeof(node));

    novo = no->left;
    no->left = novo->right;
    novo->right = no;
    novo->isRed = no->isRed;
    no->isRed = 1;
    return novo;
}

node* troca_cor(node* no){
    no->isRed = 1;
    no->left->isRed = 0;
    no->right->isRed = 0;
    return no;
}

void mostra_arvore(node* no){
    if(no){
        printf("(%d%c) ", no->info, (no->isRed == 1) ? 'V' : 'P');
        mostra_arvore(no->left);
        mostra_arvore(no->right);
    }
}


// -----------------------------------------------------------

ocorrencia * addNum(ocorrencia *lista, int new){
    ocorrencia *novo = (ocorrencia *) malloc(sizeof(ocorrencia));
    novo->num = new;
    novo->casos = 1;
    novo->prox = NULL;
    return novo;
}

ocorrencia * contain(ocorrencia *lista, int num){
    if(lista == NULL){ 
        lista = addNum(lista, num);
    } else {
        ocorrencia *aux;
        for(aux = lista; aux != NULL; aux = aux->prox){
            if(aux->num == num){
                aux->casos += 1;
                break;
            }
            if(aux->prox == NULL) break;
        }
        if(aux->prox == NULL) aux->prox = addNum(lista, num);  
    }
    return lista;
}

void mostrarLista(ocorrencia *lista){
    if(lista != NULL){
        printf("%d: %d, ", lista->num, lista->casos);
        mostrarLista(lista->prox);
    }
}

void addArv(node *raiz, node *new){
    if(raiz != NULL){
        if(raiz->info < new->info ){
            if(raiz->right == NULL) raiz->right = new;
            else addArv(raiz->right, new);
        } else { 
            if(raiz->left == NULL) raiz->left = new;
            else addArv(raiz->left, new);
        }
    }
}

node * freeArv(node *raiz){
    if(raiz != NULL){
        freeArv(raiz->right);
        freeArv(raiz->left);
        free(raiz);
    }
    return NULL;
}

int altura(node *no, int valor, int maior){
    int Altura = 0;
    node *aux = malloc(sizeof(node));

    aux = busca(no, valor);

    int interna(node *no){
        int left = 0, right = 0;

        if(no->left != NULL) left += interna(no->left) + 1;
        if(no->right != NULL) right += interna(no->right) + 1;

        if(maior) return left > right ? left : right;
        else return left < right ? left : right;
        
    }

    if(aux != NULL) Altura = interna(aux);

    return Altura;
}

node * busca(node *no, int valor){
    node *aux = malloc(sizeof(node));
    if(no != NULL){
        if(no->info != valor){
            if(no->info >= valor) aux = busca(no->left, valor);
            else aux = busca(no->right, valor);
        } else {
            aux = no;
        }
    }
    return aux;
}

