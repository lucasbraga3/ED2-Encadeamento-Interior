#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <stdbool.h>
#include <math.h>
#include <unistd.h>
#define MAX_NOME 100
#define MAX_SIZE 7
#define MAX_SIZEAUX 5

struct cliente{
    char nome[MAX_NOME];
    int id;
    int status;
    int proximo;
};typedef struct cliente Client;

struct lista{
    Client cliente;
    struct lista *prox;
};typedef struct lista Lista;

Lista * tabelaHash[MAX_SIZE];
//int quant_elems = 4;
int hash(int id){
    return id % MAX_SIZE;
}
int removtracker = 0;

void init_tabela_hash(){
    for(int i = 0; i < MAX_SIZE; i++){
        tabelaHash[i] = NULL;
    }
}

Lista* cria_lista(Client *a){
    Lista *Novalista = (Lista*)malloc(sizeof(Lista));
   // Novalista->cliente = malloc(sizeof(Client*));
    Novalista->cliente.id = (*a).id;
    Novalista->cliente.status = (*a).status;
    Novalista->cliente.proximo = (*a).proximo;
    strcpy(Novalista->cliente.nome,(*a).nome);
    //Novalista->cliente = a;
    Novalista->prox = NULL;
    return Novalista;
}

void add_lista(Client *a, Lista *list){
    Lista **aux = &list;
    while((*aux)->prox != NULL){
        aux = &(*aux)->prox;
    }
    Lista *newnode = cria_lista(a);
    (*aux)->cliente.proximo = (*a).id;
    (*aux)->prox = newnode;
}
int c = 0;
void insere_hash(Client *c){
   // int i;
   // quant_elems++;
    int index = c;
    if(tabelaHash[index] != NULL){
        add_lista(c,tabelaHash[index]);
    }
    else{
        Lista *newnode = cria_lista(c);
        tabelaHash[index] = newnode;
        int tempid = c->id;
        tabelaHash[index]->cliente.id = tempid;
        printf("\nTEST = id: %d ", tabelaHash[index]->cliente.id);
        printf(" TEST = nome: %s\n", tabelaHash[index]->cliente.nome);
    }
    c++;
}

//fwrite(addressData, sizeData, numbersData, pointerToFile);
void clientesarqmod(){
    FILE *arquivo;
    arquivo = fopen("clientesnovos.dat","wb");
    for(int i = 0; i<MAX_SIZE; i++){
        if(tabelaHash[i] != NULL){
            Lista *aux = tabelaHash[i];
            fwrite(&(tabelaHash[i]->cliente.id), sizeof(int), 1, arquivo);
            fwrite(tabelaHash[i]->cliente.nome,sizeof(char)*MAX_NOME,1,arquivo);
            fwrite(&(tabelaHash[i]->cliente.proximo),sizeof(int),1,arquivo);
            fwrite(&(tabelaHash[i]->cliente.status),sizeof(int),1,arquivo);
            while (aux->prox != NULL)
            {  
                aux = aux->prox;
                fwrite(&(aux->cliente.id),sizeof(int),1,arquivo);
                fwrite(aux->cliente.nome,sizeof(char)*MAX_NOME,1,arquivo);
                fwrite(&(aux->cliente.proximo),sizeof(int),1,arquivo);
                fwrite(&(aux->cliente.status),sizeof(int),1,arquivo);
            }
        }
    }
    fclose(arquivo);
}

void clientesarqmod2(){
    FILE *arquivo;
    //int id = -1;
    //char *nome = malloc(sizeof(char)*MAX_NOME);
    arquivo = fopen("clientesnovos.dat","wb");
    for(int i = 0; i<MAX_SIZE; i++){
        if(tabelaHash[i] != NULL){
            Lista *aux = tabelaHash[i];
            fwrite(&(tabelaHash[i]->cliente.id), sizeof(int), 1, arquivo);
            fwrite(tabelaHash[i]->cliente.nome,sizeof(char)*MAX_NOME,1,arquivo);
            fwrite(&(tabelaHash[i]->cliente.proximo),sizeof(int),1,arquivo);
            fwrite(&(tabelaHash[i]->cliente.status),sizeof(int),1,arquivo);
            while (aux->prox != NULL)
            {  
                aux = aux->prox;
                fwrite(&(aux->cliente.id),sizeof(int),1,arquivo);
                fwrite(aux->cliente.nome,sizeof(char)*MAX_NOME,1,arquivo);
                fwrite(&(aux->cliente.proximo),sizeof(int),1,arquivo);
                fwrite(&(aux->cliente.status),sizeof(int),1,arquivo);
            }
        }
    }
    fclose(arquivo);
}
//fread(addressData, sizeData, numbersData, pointerToFile);
void clientarqinit(){
    FILE *arquivo;
    arquivo = fopen("clientesnovos.dat","rb");
    //Client *c = malloc(sizeof(Client*));
    int id;
    int prox;
    char *nome = malloc(sizeof(char)*MAX_NOME);
    //char *status = malloc(sizeof(char)*6);
    int statusint;
    int i = 0;
   // strcpy(status,"false");
   while(i<MAX_SIZE){
        fread(&id,sizeof(int),1,arquivo); //esse tava uma bosta tbm ai mudei
        for (int j = 0; j < MAX_NOME; j++)
        {
            char temp;
            fread(&temp, sizeof(char), 1, arquivo);
            nome[j] = temp;
        }
        //strcpy(c->nome,nome);
        
        fread(&prox,sizeof(int),1,arquivo); //por algum misterio da vida, esse funciona e o de cima nao
        fread(&statusint,sizeof(int),1,arquivo);
        if(statusint != 0){
        printf("nome: %s \n",nome);
        printf("Codigo do Cliente: %d \n",id);
        printf("proximo: %d \n",prox);
        printf("status: %d \n",statusint);
        printf("\n");
        }
        i++;
        /*if (nome[0] != '\0'){
            if (statusint == 0){
                strcpy(status,"false");
            
            }
            else{
                strcpy(status,"true");
            }
            
        }*/
        }
    fclose(arquivo);
}

void fixprox(int idprox, int adress){
    FILE *arquivo;
    int compid;
    arquivo = fopen("clientesnovos.dat","r+b");
    int i = 0;
    while(i<MAX_SIZE){
        fread(&compid,sizeof(int),1,arquivo);
        if(compid == idprox){
            fseek(arquivo,sizeof(char)*MAX_NOME,SEEK_CUR);
            fwrite(&adress,sizeof(int),1,arquivo);
            i = MAX_SIZE;
        }
        fseek(arquivo,sizeof(char)*MAX_NOME,SEEK_CUR);
        fseek(arquivo,sizeof(int),SEEK_CUR);
        fseek(arquivo,sizeof(int),SEEK_CUR);
        i++;
    }
    fclose(arquivo);
}

void otherprox(int id){
    FILE *arquivo;
    int compid;
    int prox;
    int status;
    int flag = 0;
    arquivo = fopen("clientesnovos.dat","r+b");
    int i = 0;
    while(i<MAX_SIZE){
        fread(&compid,sizeof(int),1,arquivo);
        if(compid == id){
            flag = 1;
        }
        if(flag == 0){
        fseek(arquivo,sizeof(char)*MAX_NOME,SEEK_CUR);
        fseek(arquivo,sizeof(int),SEEK_CUR);
        fseek(arquivo,sizeof(int),SEEK_CUR);
        }
        else if(flag == 1){
            fseek(arquivo,sizeof(char)*MAX_NOME,SEEK_CUR);
            fread(&prox,sizeof(int),1,arquivo);
            fread(&status,sizeof(int),1,arquivo);
            if(status != 0 && compid != id && hash(compid) == hash(id)){
                fixprox(id,i);
                i = MAX_SIZE;
            }
        }
        i++;
    }
    fclose(arquivo);
}

void addclientediretoarq(Client *a){
    FILE *arquivo;
   // FILE *arquivotable;
    int index = hash(a->id);
    a->status = 1;
    int arqid = -1;
    int arqstatus = 0;
    int arqprox = 0;
    int filledotherflag = 0;
    int endflag = 0;
    int flagid;
    int flagprox = -1;
    int adress = -1;
    char *arqnome = malloc(sizeof(char)*MAX_NOME);
    char temp;
    arquivo = fopen("clientesnovos.dat","r+b");
    //arquivotable = fopen("tabelanova.dat","r+b");
    //int comparador = index - 1;
    int i = 0;
    while(i<MAX_SIZE){
        fread(&arqid,sizeof(int),1,arquivo);
        for (int j = 0; j < MAX_NOME; j++)
        {
            fread(&temp, sizeof(char), 1, arquivo);
            arqnome[j] = temp;
        }
        fread(&arqprox,sizeof(int),1,arquivo); 
        fread(&arqstatus,sizeof(int),1,arquivo);
        if(hash(arqid) == index && arqid != -1 && arqprox>=flagprox && arqstatus == 1){
            flagid = arqid;
            flagprox = arqprox;
        }
        if(arqstatus == 0 && hash(i) == index){
            fseek(arquivo,-sizeof(int),SEEK_CUR);
            fseek(arquivo,-sizeof(int),SEEK_CUR);
            fseek(arquivo,-sizeof(char)*MAX_NOME,SEEK_CUR);
            fseek(arquivo,-sizeof(int),SEEK_CUR);
            fwrite(&a->id,sizeof(int),1,arquivo);
            fwrite(a->nome,sizeof(char)*MAX_NOME,1,arquivo);
            fwrite(&i, sizeof(int),1,arquivo);
            fwrite(&a->status,sizeof(int),1,arquivo);
            endflag = 1;
            adress = i;
        }
        if(arqstatus!= 0 && hash(i) == index){
            filledotherflag = 1;
        }
        if(arqstatus == 0 && filledotherflag == 1){
            fseek(arquivo,-sizeof(int),SEEK_CUR);
            fseek(arquivo,-sizeof(int),SEEK_CUR);
            fseek(arquivo,-sizeof(char)*MAX_NOME,SEEK_CUR);
            fseek(arquivo,-sizeof(int),SEEK_CUR);
            fwrite(&a->id,sizeof(int),1,arquivo);
            fwrite(a->nome,sizeof(char)*MAX_NOME,1,arquivo);
            fwrite(&i, sizeof(int),1,arquivo);//fwrite(&index,sizeof(int),1,arquivo);
            fwrite(&a->status,sizeof(int),1,arquivo);
            endflag = 1;     
            adress = i; 
            i = MAX_SIZE;      
        }
        i++;
    }
    fclose(arquivo);
    if(endflag == 1){
    if (flagprox != 7){
       fixprox(flagid, adress);
    }
    otherprox(a->id);
    }
    else{
        printf("Nao foi possivel adicionar o cliente devido a motivos de overflow \n");
    }
}

void removeclientdiretodoarq(int id){
    FILE *arquivo;
    int arqid;
    int saveid = -1;
    int savepos;
    int saveposant;
    int removed_status = 0;
    int arqprox;
   // char *arqnome = malloc(sizeof(char)*MAX_NOME);
   // char temp;
    arquivo = fopen("clientesnovos.dat","r+b");
    int i = 0;
    while (i<MAX_SIZE)
    {
        fread(&arqid,sizeof(int),1,arquivo);
        if(arqid == id){
            fseek(arquivo,sizeof(char)*MAX_NOME,SEEK_CUR);
            fread(&arqprox,sizeof(int),1,arquivo);
            fwrite(&removed_status,sizeof(int),1,arquivo);
            savepos = i;
            i = MAX_SIZE;
            break;  
        }
        if(hash(arqid) == hash(id)){
            saveid = arqid;
            saveposant = i;
        }
        fseek(arquivo,sizeof(char)*MAX_NOME,SEEK_CUR);
        fseek(arquivo,sizeof(int),SEEK_CUR);
        fseek(arquivo,sizeof(int),SEEK_CUR);
        i++;
    }   
    fclose(arquivo);
    if(hash(arqid) != arqprox && saveid != -1 && saveid != id){
        if(savepos != arqprox){
            fixprox(saveid,arqprox);
        }
        else {
            fixprox(saveid,hash(saveposant));
        }
    }
}

void tabelarqinit(){
    FILE *arquivo;
    int indice = 0;
    arquivo = fopen("tabelanova.dat","wb");
    for(int i = 0; i<MAX_SIZE;i++){
        if (tabelaHash[i] != NULL){
            fwrite(&indice,sizeof(int),1,arquivo);
            indice++;
        }
        else{
            int nada = -1;
            fwrite(&nada,sizeof(int),1,arquivo);
        }
    }
    fclose(arquivo);
}

void tablearqread(){
    FILE *arquivo;
    arquivo = fopen("tabelanova.dat","rb");
    int *indice = malloc(sizeof(int));
    for(int i = 0; i<MAX_SIZE;i++){
        fread(indice,sizeof(int),1,arquivo);
        printf("indice %d contem: %d \n",i, *indice);
    }
    fclose(arquivo);
}

void invoke_tables(){
    clientesarqmod();
    clientarqinit();
    tabelarqinit();
    tablearqread();
}

void remove_hash(int id){
    //quant_elems--;
    int index = hash(id);
    if(tabelaHash[index] != NULL){
        Lista *aux = tabelaHash[index];
        if(tabelaHash[index]->cliente.id == id){
            tabelaHash[index] = tabelaHash[index]->prox;
            printf("Cliente removido \n");
            free(aux);
            return;
        }
        else{
            while(aux->prox != NULL){
                if(aux->prox->cliente.id == id){
                    Lista *aux2 = aux->prox;
                    aux->prox = aux2->prox;
                    aux->cliente.proximo = aux2->cliente.proximo;
                    free(aux2);
                    return;
                }
                aux = aux->prox;
            }
            printf("Cliente nao existe \n");
            //quant_elems++;
        }
    }
    else{
        printf("Cliente nao existe \n");
       // quant_elems++;
    }
    invoke_tables();
}

void add_cliente(){
    Client *new = (Client*)malloc(sizeof(Client));
    printf("Digite o nome do cliente: ");
    scanf("%s",new->nome);
    printf("Digite o codigo do cliente: ");
    scanf("%d",&new->id);
    new->status = 0;
    new->proximo = -1;
    insere_hash(new);
    invoke_tables();
}

void add_cliente2(){
    Client *new = (Client*)malloc(sizeof(Client));
    printf("Digite o nome do cliente: ");
    scanf("%s",new->nome);
    printf("Digite o codigo do cliente: ");
    scanf("%d",&new->id);
    new->status = 1;
    new->proximo = -1;
    addclientediretoarq(new);
    clientarqinit();
}

void delet_client2(){
    printf("Digite o codigo do cliente a ser removido: ");
    int id;
    scanf("%d",&id);
    removeclientdiretodoarq(id);
    clientarqinit();
}

void print_hash(){
    for(int i = 0; i<MAX_SIZE; i++){
        if(tabelaHash[i] != NULL){
            Lista *aux = tabelaHash[i];
            printf("Indice %d: \n",i);
            printf("Nome: %s \n",tabelaHash[i]->cliente.nome);
            printf("Codigo do Cliente: %d \n",tabelaHash[i]->cliente.id);
            printf("Proximo: %d \n",tabelaHash[i]->cliente.proximo);
            printf("Status: %d \n",tabelaHash[i]->cliente.status);
            while (aux->prox != NULL)
            {  
                aux = aux->prox;
                printf("Nome: %s \n",aux->cliente.nome);
                printf("Codigo do Cliente: %d \n",aux->cliente.id);
                printf("Proximo: %d \n",aux->cliente.proximo);
                printf("Status: %d \n",aux->cliente.status);
            }
        }
        else{
            printf("Indice %d: \n",i);
            printf("Vazio \n");
        }
    }
}

int main(){
    /*DESCOMENTE PARA RESETAR TABELA EM CASO DE BUG VIOLENTO!!!!*/ /*init_tabela_hash();
    int i;
    for(i = 0;i<MAX_SIZE;i++){
        Client *new = (Client*)malloc(sizeof(Client));
        strcpy(new->nome,"\0");
        new->id = -1;
        new->proximo = i;
        new->status = 0;
        insere_hash(new);
    }
    invoke_tables();*/
    clientarqinit();
    //add_cliente2();
    int opcao = 0;
    while (opcao != 3)
    {
        //invoke_tables();
        printf("1 - Adicionar cliente \n");
        printf("2 - Remover cliente \n");
        printf("3 - Sair \n");
        scanf("%d",&opcao);
        switch (opcao)
        {
        case 1:
            add_cliente2();
            break;
        case 2:
            delet_client2();
            break;
        case 3:
            break;
        default:
            break;
        }
    }
    return 0;
}