#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct no {
    int posicao;
    struct no *prox;
} No;

typedef struct cabecalho {
    No *inicio;
    No *fim;
} Cabecalho;

Cabecalho *criaCabecalho(){
    Cabecalho *c = malloc(sizeof(Cabecalho));
    c->inicio = c->fim = NULL;
    return c;
}

No *criaNo(int pos){
    No *n = malloc(sizeof(No));
    n->posicao = pos;
    n->prox = NULL;
    return n;
}

void liberaLista(Cabecalho *c){
    No *atual = c->inicio;
    while (atual != NULL) {
        No *t = atual->prox; 
        free(atual); 
        atual = t;   
    }
    free(c);
    c = NULL;
}

char *lerArquivo(const char *nomeArquivo, int *tamanhoArquivo) {
    FILE *arquivo = fopen(nomeArquivo, "r");

    if (arquivo == NULL) {
        printf("Erro ao abrir o arquivo %s.\n", nomeArquivo);
        return NULL;
    }

    fseek(arquivo, 0, SEEK_END);
    int tamanho = ftell(arquivo);
    fseek(arquivo, 0, SEEK_SET);

    char *conteudo = malloc(sizeof(char) * (tamanho + 1));
    *tamanhoArquivo = tamanho;

    if (conteudo == NULL) {
        fclose(arquivo);
        return NULL;
    }

    fread(conteudo, 1, tamanho, arquivo);

    conteudo[tamanho] = '\0';

    fclose(arquivo);

    return conteudo;
}

int *prefixFunction(const char *p, int m) {    
    
    int *pi = malloc(sizeof(int) * m);
    int k = -1;
    pi[0] = k;

    for (int q = 1; q < m; q++) {
        while (k > -1 && p[k + 1] != p[q]) {
            k = pi[k];
        }

        if (p[q] == p[k + 1]) {
            k = k + 1;
        }

        pi[q] = k;
    }
    return pi;
}

/*
t = base de dados
n = tamanho da base de dados
p = padrões de vírus
m = tamanho de padrões
*/
void KMP(const char *t, const char *p,int *pi, int n, int m) {
    int contador=0,flag=0;
    int q = -1;
    int i,j;

    Cabecalho *cab = criaCabecalho();

    for (i = 0; i < n; i++) {
        
        if(n - i ==6){
            liberaLista(cab);
            return;
        }

        if (t[i] != '\n' && t[i] != '\r' && flag==1) {
            printf("%c", t[i]);
        }

        if(t[i]=='>'){
            flag=1;
            contador=0;
            printf("[" );
        }

        if(t[i]=='\n'){
            flag = 0;
        }

        if(t[i]=='\n' && t[i+1]=='>'){
            printf("] no. de ocorrencias: %d  posicoes: ",contador);
            No *aux = cab->inicio;
            while(aux!=NULL){
                printf("%d ", aux->posicao);
                aux = aux->prox;
            }
            printf("\n");
            liberaLista(cab);
            cab = criaCabecalho();
            contador=0;
        }

        while (q > -1 && p[q + 1] != t[i]) {
            q = pi[q];
        }

        if (p[q + 1] == t[i]) {
            q = q + 1;
        }

        if (q == m - 1) {
            if(cab->inicio == NULL){
                No *novoNo = criaNo(i);
                cab->inicio = cab->fim = novoNo;
            }else{
                No *novoNo = criaNo(i);
                cab->fim->prox = novoNo;
                cab->fim = novoNo;
            }
            contador++;
            q = pi[q];
        }
    }
}

char *buscaPadrao(const char *arq,int *inicioBusca,int *tamanhoNovoPadrao,int *flag,int finalArq){

    //coloca um ponteiro para a ultima posicao do nome do padrao
    while (*inicioBusca < finalArq && arq[*inicioBusca] != '\n') {
        if(arq[*inicioBusca] != '>'){
            printf("%c",arq[*inicioBusca]);
        }
        *inicioBusca = *inicioBusca+1;
    }
    printf("\n");

    //Seleciona a linha do padrao
    *inicioBusca = *inicioBusca+1;
    int inicio = *inicioBusca;
    int tamanhoPadrao = 0;
    
    //conta o tamanho do novo vetor
    while (*inicioBusca < finalArq && arq[*inicioBusca] != '\n') {
            tamanhoPadrao++;
            *inicioBusca = *inicioBusca+1;
    }

    //aloca novo vetor
    char *teste = malloc(sizeof(char)*(tamanhoPadrao-1));
    
    //escreve no novo vetor
    int i=0;
    *tamanhoNovoPadrao = tamanhoPadrao-1;
    while(i<tamanhoPadrao-1){
        teste[i]=arq[inicio];
        i++;
        inicio++;
    }

    //coloca no proximo ponto de padrão
    *inicioBusca = *inicioBusca +1;
    
    //flag para dizer se esta ou não no inicio do arquivo
    if(finalArq - *inicioBusca == 4){
        *flag = 0;
    }

    return teste;
}

int main() {
    const char nomeArquivo1[] = "BaseDadosDNA.txt"; // t - texto
    const char nomeArquivo2[] = "PadroesVirus.txt"; // p - padrao
    int tamanho1 = -1;
    int tamanho2 = -1;

    char *arq1 = lerArquivo(nomeArquivo1, &tamanho1);
    char *arq2 = lerArquivo(nomeArquivo2, &tamanho2);

    int inicioBusca = 0;
    int tamanhoNovo = 0;
    int flag=1;
    
    printf("\n\n");
    
    while(flag==1){
        char *padrao = buscaPadrao(arq2,&inicioBusca,&tamanhoNovo,&flag,tamanho2);
        int *pi = prefixFunction(padrao, tamanhoNovo);
        KMP(arq1, padrao, pi, tamanho1, tamanhoNovo);
        free(pi);
        free(padrao);
        printf("\n\n");
    }

    free(arq1);
    free(arq2);

    return 0;
}
