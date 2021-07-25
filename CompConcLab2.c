#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include "timer.h"

int NTHREADS;
float *matrix1;
float *matrix2;
float *productmatrix; //resultado do produto entre matrizes

typedef struct {

int tamanho; //dimensao
int identificador; //id da thread

}infomatrix;

//funcao da thread de multiplicação de matrizes
void *threadzada(void *arg){
    infomatrix *info=(infomatrix*) arg;
    printf("Thread %d\n",info->identificador);

    float l;
    int i,j,k;//variaveis para auxiliar nos for durante a multiplicacao de matrizes

    for(i=info->identificador;i<info->tamanho;i+=NTHREADS){
        for(j=0;j<info->tamanho;j++){
           l=0;
            for(k=0;k<(info->tamanho);k++){
            l+=matrix1[j*(info->tamanho)+k] * matrix2[k*(info->tamanho)+i];}
            productmatrix[j*(info->tamanho)+i]=l;}}
        pthread_exit(NULL);}




int main(int argc, char* argv[])
{
    int tamanho;
    double tinicial, tfinal, ttotal, tthread, tdata, tmemoria; //tempos
    infomatrix *info;
    pthread_t *tid;

    GET_TIME(tinicial)
    {
        if(argc<3){
            printf("Digite: %s <numero de threads> <dimensao da matriz>\n", argv[0]);
            return 1;
        }
    }

    NTHREADS=atoi(argv[1]);  //argumentos
    tamanho=atoi(argv[2]);


    if(NTHREADS>tamanho){
        printf("o numero de threads nao deveria ser maior que o tamanho\n\n");
        return 2;
    }

//utilizando malloc para alocar espaço em cada uma das 3 matrizes e conferindo se há algum erro
    productmatrix=malloc(sizeof(float) * tamanho * tamanho);
    if(productmatrix==NULL){
        printf("erro no malloc");
        return 1;
    }

    matrix1=malloc(sizeof(float) * tamanho * tamanho);
    if(matrix1==NULL){
        printf("erro no malloc");
        return 1;
    }

    matrix2=malloc(sizeof(float) * tamanho * tamanho);
    if(matrix2==NULL){
        printf("erro no malloc");
        return 1;
    }

    int i,j;
    for(i=0;i<tamanho;i++){
        for(j=0;j<tamanho;j++){
            matrix1[i*tamanho+j]=5;  //valor colocado como exemplo
            matrix2[i*tamanho+j]=5;
            productmatrix[i*tamanho+j]=0;
        }}

    GET_TIME(tfinal);
    tdata=tfinal-tinicial;
    GET_TIME(tinicial);
    tid = (pthread_t*) malloc(sizeof(pthread_t)*NTHREADS);
    if (tid==NULL) {
        printf("erro no malloc");
        return 1;
  }
    info=(infomatrix*)malloc(sizeof(infomatrix)*NTHREADS);
    if(info==NULL){
        printf("erro no malloc");
        return 1;
    }
    for(i=0;i<NTHREADS;i++){ //criando thread
        (info+i)->identificador=i;
        (info+i)->tamanho=tamanho;
        if(pthread_create(tid+i,NULL,threadzada,(void*) (info+i))){
            printf("erro no pthread_create");
            return 4;        }
            ;
    }
    for(i=0;i<NTHREADS;i++){ //funcao pthread join
        if(pthread_join(*(tid+i), NULL)) {
            printf("erro no pthread_join");
      return 5;
    }}

    GET_TIME(tfinal);
    tthread=tfinal-tinicial;

    for(i=0;i<tamanho;i++){ //validando a matriz
        for(j=0;j<tamanho;j++){
            if(productmatrix[i*tamanho+j]!=25*tamanho){//25 pra ser de acordo com as matrizes preenchidas com 5
                printf("erro no tamanho da matriz");
                return 6;}}}

    GET_TIME(tinicial);

    //liberando memoria
    free(info);
    free(matrix1);
    free(matrix2);
    free(tid);
    free(productmatrix);

    GET_TIME(tfinal);

    ttotal=tinicial+tfinal+tthread+tmemoria; //calculando tempo

    printf("O tempo inicial durou %e segundos\nO tempo final durou %e segundos\nA liberação de memoria durou %e segundos\nO tempo total durou %e segundos\n",tinicial,tfinal,tmemoria,ttotal);

    pthread_exit(NULL); //finalizando para evitar erros

    return 0;
}


