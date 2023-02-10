#include<stdio.h>
#include<stdlib.h>


typedef struct no{
    int valor;
    struct no *esquerdo, *direito;
    short altura;
}No;

No* novoNo(int x){
    No *novo = malloc(sizeof(No));

    if(novo){
        novo->valor = x;
        novo->esquerdo = NULL;
        novo->direito = NULL;
        novo->altura = 0;
    }
    else{
        printf("\nErro ao colocar novo no! \n");
    }
    return novo;
}

//Retorna o maior dentre dois valores
short maior(short a, short b){
    return(a > b)? a: b;
}

//Retorna a altura de um nó ou -1 caso ele seja null
short alturaNo(No *no){
    if(no == NULL){
        return -1;
    }
    else{
        return no-> altura;
    }
}

//Calcula e retorna o valor de balaceamento de um nó 

short fatorBalanceamento(No *no){
    if(no){
        return(alturaNo(no->esquerdo) - alturaNo(no->direito));
    }
    else{
        return 0;
    }
}

//Funcao para rotacao à esquerda
No* rotacaoEsquerda(No *r){
    No *y, *f;

    y = r->direito; //y aponta para a asubarore direit da raiz r
    f = y->esquerdo; //f aponta para o filho esquerdo de y

    y->esquerdo = r; //o filho esquerdo de y passa a ser a raiz r
    r->direito = f; //o filho direito de r passa a ser f

    //recalcula a altura dos nos que foram movimentados
    r->altura = maior(alturaNo(r->esquerdo), alturaNo(r->direito)) + 1;
    y->altura = maior(alturaNo(y->esquerdo), alturaNo(y->direito)) + 1;

    return y;
}

//Funcao rotacao para à direita
No *rotacaoDireita(No *r){
    No *y, *f;

    y = r->esquerdo;
    f = y->direito;

    y->direito = r;
    r->esquerdo = f;

    r->altura = maior(alturaNo(r->esquerdo), alturaNo(r->direito)) + 1;
    y->altura = maior(alturaNo(y->esquerdo), alturaNo(y->direito)) + 1;

    return y;
}

//Funcoes para rotacoes duplas
No * rotacaoEsquerdaDireita(No *r){
    r->esquerdo = rotacaoEsquerda(r->esquerdo);
    return rotacaoDireita(r);
}

No* rotacaoDireitaEsquerda(No *r){
    r->direito = rotacaoDireita(r->direito);
    return rotacaoEsquerda(r);
}

//Funcao para realizar o balanceamento da árvore após uma inserção ou remoção
//Recebe o nó que está balanceado e retorna a nova raiz após o balanceamento
No* balancear(No *raiz){
    short fb = fatorBalanceamento(raiz);

    //rotacao à esquerda
    if(fb < -1 && fatorBalanceamento(raiz->direito) <= 0)
    {
        raiz = rotacaoEsquerda(raiz);
    }
    //rotacao à direita
    else if(fb > 1 && fatorBalanceamento(raiz->esquerdo) >= 0)
    {
        raiz = rotacaoDireita(raiz);
    }
    //rotacao dupla à esquerda
    else if(fb > 1 && fatorBalanceamento(raiz->esquerdo) >= 0)
    {
        rotacaoEsquerdaDireita(raiz);
    }
    //rotacao dupla à direita
    else if(fb < -1 && fatorBalanceamento(raiz->direito) > 0)
    {
        raiz = rotacaoDireitaEsquerda(raiz);
    }

    return raiz;
}

//Insere um novo no na arvore e retorna o endereco do novo nó ou nova raiz 
//apos o balanceamento

No* inserir(No *raiz, int x){
    if(raiz == NULL){  //arvore vazia
        return novoNo(x);
    }
    else{
        if(x < raiz->valor)
        {
            raiz->esquerdo = inserir(raiz->esquerdo, x);
        }
        else if(x > raiz -> valor)
        {
            raiz->direito = inserir(raiz->direito, x);
        }
        else{
            printf("\nInsercao nao realizada! \nO elemento %d a existe.\n", x);
        }
    }

    //recalcula a altura de todos os nos entre a raiz e o novo no inserido
    raiz->altura = maior(alturaNo(raiz->esquerdo), alturaNo(raiz->direito) + 1);

    //verifica a necessidade de rebalancear a árvore
    raiz = balancear(raiz);

    return raiz;
}

//Funcao remover
No *remover(No *raiz, int chave){
    if(raiz == NULL){
        printf("Valor nao encontrado.\n");
        return NULL;
    }
    //Procura o nó a remover
    else{
        if(raiz->valor == chave)
        {   
            //remover nos folhas
            if(raiz->esquerdo == NULL && raiz->direito == NULL){
                free(raiz);
                printf("Elemento folha removido: %d.", chave);
                return NULL;
            }
            else{
                //remover nós que possuem dois filhos
                if(raiz->esquerdo != NULL && raiz-> direito != NULL){
                    No *aux = raiz->esquerdo;
                    while(aux->direito != NULL){
                        aux = aux->direito;
                    }
                    raiz->valor = aux->valor;
                    aux->valor = chave;
                    printf("Elemento trocado: %d\n", chave);
                    raiz->esquerdo = remover(raiz->esquerdo, chave);
                    return raiz;
                }
                else{
                    //remover nós que possuem um filho
                    No *aux;
                    if(raiz->esquerdo != NULL){
                        aux = raiz->esquerdo;
                    }
                    else{
                        aux = raiz->direito;
                    }
                    free(aux);
                    printf("Elemento com 1 filho removido: %d", chave);
                    return aux;
                }
            }
        }
        //Se o numero for menor que a raiz
        else{
            if(chave < raiz->valor)
            {
                //Se a chave for menor que a raiz da sub arvore a esquerda
                //Como é uma recursao ele vai ate achar o valor ou achar NULL
                raiz->esquerdo = remover(raiz->esquerdo, chave);
            }
            else
            {
                //Se a chave for maior que a raiz da sub arvore a direita
                raiz->direito = remover(raiz->direito, chave);
            }
        }
        //Recalcula a altura de todos os nós 
        raiz->altura = maior(alturaNo(raiz->esquerdo), alturaNo(raiz->direito)) + 1;

        //Rebalancear a arvore
        raiz = balancear(raiz);

        return raiz;
    }

}

void imprimir(No *raiz, int nivel){
    int i;
    //se a raiz for direfente de nula
    if(raiz){
        imprimir(raiz->direito, nivel + 1);
        printf("\n\n");

        for(i = 0; i < nivel; i++){
            printf("\t");
        }

        printf("[%d]", raiz->valor);
        imprimir(raiz->esquerdo, nivel + 1);
    }

}

int main(){
    int opc, valor;
    No *raiz = NULL;

    do{
        printf("\n1- Inserir\n2-Remover\n3-Imprimir\n4-Sair\n");
        scanf("%d", &opc);
        
        switch (opc)
        {
        case 1:
            printf("Informe o valor a ser inserido: ");
            scanf("%d", &valor);
            raiz = inserir(raiz, valor);
            system("pause");
            system("cls");
            break;
        
        case 2:
            printf("Informe o valor a ser removido: ");
            scanf("%d", &valor);
            raiz = remover(raiz, valor);
            system("pause");
            system("cls");
            break;

        case 3:
            imprimir(raiz, 1);

            break;
        
        default:
            break;
        }

    }while(opc != 4);
}