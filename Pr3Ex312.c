/*
Marco Rondón, 1794984
Mario Roda Sevilla, 1792216
*/
#include <stdio.h>
#include <stdlib.h>

#define MAXAR 7
#define MAXARNO 5

typedef struct{
    unsigned n1,n2,estat;
}aresta;
typedef struct
{
    unsigned nodes[MAXAR+1];
    unsigned arvis[MAXAR];
    int naresvisit;
}estructuracami;

unsigned arestesdelnode(unsigned , unsigned ,  aresta[] , unsigned []);
int arestaaLallista(unsigned aresta, unsigned llistap[], unsigned npos);
void ValenciaNodes(aresta lArestes[], unsigned numArestes, unsigned numNodes, unsigned *lValenciaNodes, unsigned *posNodeMax);


int main(){
    unsigned *llistaValenciaNodes, *posNodeMax;
    unsigned i,arestespos[MAXARNO],npos=0,seguir=0, opcio, valida;
    estructuracami cami;
    //Tenim 7 arestes [0-6] y el node més gran es el 3 (tenim 4 nodes [0-3]).
    unsigned MAXNODE = 3;
    aresta larestes[]={{0,1,0},{0,2,0},{0,2,0},{0,3,0},{0,3,0},
    {1,2,0},{1,3,0}};
    // Reservem memoria
    if ((posNodeMax = (unsigned *)malloc(sizeof(unsigned)))==NULL){{printf("No hi ha espai a la memoria suficient");return -1;}}
    if ((llistaValenciaNodes = (unsigned *)malloc(sizeof(unsigned)*4))==NULL){printf("No hi ha espai a la memoria suficient");return -1;};
    ValenciaNodes(larestes, 7, 4, llistaValenciaNodes, posNodeMax);
    printf("El node amb maxima valencia es el node %d\n",*posNodeMax);
    for (i=0;i<4;i++){
        printf("El node %d te valencia %d\n",i,llistaValenciaNodes[i]);
    }
    cami.naresvisit=0;

    for(i=0;i<MAXAR;i++){
        printf("Aresta %d -> [%d,%d]\n",i,larestes[i].n1,larestes[i].n2);
    }
    //bucle per comprovar el node d'inici
    do{
        printf("Node Inici (0-%u): ", MAXNODE);
        scanf("%u",&cami.nodes[0]);
        if ((cami.nodes[0]>MAXNODE)){
            printf("Node incorrecte. Introdueix un node entre 0 i %u.\n", MAXNODE);
        }
    } while (cami.nodes[0] > MAXNODE);
    npos=arestesdelnode(cami.nodes[0], MAXAR, larestes, arestespos);
    seguir=1;
    do{
        if(npos > 0){
            printf("Teniu %d arestes per triar: \n",npos);
            for(i=0;i<npos;i++){
                printf("%d -> [%d,%d]\n",arestespos[i],larestes[arestespos[i]].n1,
                       larestes[arestespos[i]].n2);
            }
            //Bucle que comprova que l'aresta introduïda sigui disponible
            do {
                printf("Trieu: ");
                scanf("%u", &opcio);
                valida = arestaaLallista(opcio, arestespos, npos);
                if (!valida){
                    printf("Aresta no disponible. Trieu una de la llista,\n");
                }
            } while (!valida);
            cami.arvis[cami.naresvisit] = opcio;
            larestes[cami.arvis[(cami.naresvisit)]].estat=1;
            (cami.naresvisit)++;
            if(larestes[cami.arvis[(cami.naresvisit)-1]].n1==cami.nodes[(cami.naresvisit)-1]){
                cami.nodes[cami.naresvisit]=larestes[cami.arvis[(cami.naresvisit)-1]].n2;
            }
            else{
                cami.nodes[cami.naresvisit]=larestes[cami.arvis[(cami.naresvisit)-1]].n1;
            }
            for(i=0; i<cami.naresvisit; i++){
                printf("%d -(%d)-> ",cami.nodes[i],cami.arvis[i]);
            }
            printf("%d\n",cami.nodes[cami.naresvisit]);
            npos=arestesdelnode(cami.nodes[cami.naresvisit] , MAXAR , larestes , arestespos);
        }
        else{
            printf("No queden arestes disponibles.\n");
            printf("Arestes visitades (%d):\n",cami.naresvisit);
            for(i=0;i<cami.naresvisit;i++){
                printf("%d ",cami.arvis[i]);
            }
            seguir=0;
        }
    }while(seguir);
    printf("\n");

    // Alliberem memoria
    free(llistaValenciaNodes);
    free(posNodeMax);
    return 0;
}

unsigned arestesdelnode(unsigned nn, unsigned NAR, aresta larestes[], unsigned llistap[]){
    unsigned i,nombre=0;
    for(i=0;i<NAR;i++){
        if(larestes[i].estat==0 && (larestes[i].n1==nn||larestes[i].n2==nn)){
            llistap[nombre]=i;
            nombre++;
        }
    }
    return nombre;
}
/* 
La funció recorre la llista d'arestes disponibles mirant si una aresta
concreta està en aquesta.
*/
int arestaaLallista(unsigned ar, unsigned llistap[], unsigned npos){
    unsigned i;
    for (i=0; i<npos; i++){
        if (llistap[i] == ar){
            return 1;
        }
    }
    return 0;
}

void ValenciaNodes(aresta lArestes[], unsigned numArestes, unsigned numNodes, unsigned *lValenciaNodes, unsigned *posNodeMax){
    unsigned i;
    // Posem a zero el contador de la valencia de cada node
    for (i=0;i<numNodes;i++){
        lValenciaNodes[i]=0;
    }
    // Contem el numero de vegades que apareix cada node (la seva valencia)
    for (i=0;i<numArestes;i++){
        lValenciaNodes[lArestes[i].n1] += 1;
        lValenciaNodes[lArestes[i].n2] += 1;
    }
    // Mirem quin es el node amb mes valencia de tots.
    *posNodeMax = 0;
    for (i=0;i<numNodes;i++){
        if (*posNodeMax > lValenciaNodes[i]){
            *posNodeMax = i;
        }
    }
}