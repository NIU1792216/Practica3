/*
Marco Rondón, 1794984
Mario Roda Sevilla, 1792216
*/
#include <stdio.h>
#include <stdlib.h>

typedef struct{
    unsigned n1,n2,estat;
}aresta;
typedef struct
{
    unsigned *llistaNodesVist;
    unsigned *llistaArestesVist;
    int numArestesVist;
}estructuracami;

unsigned arestesDelNode(unsigned , unsigned ,  aresta[] , unsigned []);
int arestaaLallista(unsigned aresta, unsigned llistap[], unsigned numVPossibles);
void ValenciaNodes(aresta lArestes[], unsigned numArestes, unsigned numNodes, unsigned *lValenciaNodes, unsigned *posNodeMax);
void CalcularCaminDonatOrigen(unsigned origen, unsigned numArestes, aresta lArestes[], estructuracami *camiPossible, unsigned *numCaminsTrobats);
void mostrarCami(estructuracami *cami);


int main(){
    FILE *fgraf;
    unsigned *llistaValenciaNodes, *posNodeMax, numVertex, numArestes;
    unsigned i, origen=-1, *numCaminsDisponibles;
    estructuracami *cami;
    aresta *larestes;

    // Obrim el fitxer.
    if ((fgraf = fopen("Konigs.txt", "r")) == NULL){
        printf("No es pot obrir el fitxer\n");
        return 1;
    }
    fscanf(fgraf, "%u\n", &numVertex); //nº nodes
    fscanf(fgraf, "%u\n", &numArestes);// nº arestes

    if ((larestes = (aresta *)malloc(sizeof(aresta)*numArestes)) == NULL){
        printf("No hi ha suficient memoria per carregar les arestes\n");
        return 2;
    }
    // Creem la llista d'arestes del fitxer donat.
    for (i=0; i <numArestes; i++){
        fscanf(fgraf, "%u %u\n", &larestes[i].n1, &larestes[i].n2);
        larestes[i].estat = 0;
    }
    printf("Fi de la lectura.\n");
    fclose(fgraf);
    // Reservem memoria
    if ((cami = (estructuracami *)malloc(sizeof(estructuracami))) == NULL){
        printf("No hi ha espai suficient a la memoria");
        return 2;
    }
    if ((cami->llistaNodesVist = (unsigned *)malloc(sizeof(unsigned)*(numArestes + 1))) == NULL){
        printf("No hi ha espai suficient a la memoria");
        return 2;
    }
    if ((cami->llistaArestesVist = (unsigned *)malloc(sizeof(unsigned)*(numArestes))) == NULL){
        printf("No hi ha espai suficient a la memoria");
        return 2;
    }
    if ((posNodeMax = (unsigned *)malloc(sizeof(unsigned)))==NULL){
        printf("No hi ha espai suficient a la memoria");
        return 2;
    }
    if ((llistaValenciaNodes = (unsigned *)malloc(sizeof(unsigned)*numVertex))==NULL){
        printf("No hi ha espai suficient a la memoria");
        return 2;
    }
    if ((numCaminsDisponibles = (unsigned *)malloc(sizeof(unsigned))) == NULL){
        printf("No hi ha espai suficient a la memoria\n");
        return 2;
    }
    //Calculem les valencies de cada node i les mostrem.
    ValenciaNodes(larestes, numArestes, numVertex, llistaValenciaNodes, posNodeMax);
    printf("El node amb maxima valencia es el node %d\n",*posNodeMax);
    for (i=0;i<numVertex;i++){
        printf("El node %d te valencia %d\n",i,llistaValenciaNodes[i]);
    }
    // Inicialitzem el nombre d'arestes visitades a zero.
    cami->numArestesVist=0;
    for(i=0;i<numArestes;i++){
        printf("Aresta %d -> [%d,%d]\n",i,larestes[i].n1,larestes[i].n2);
    }
    // Bucle per comprovar que el node d'inici sigui correcte
    do{
        printf("Node Inici (0-%u): ", numVertex - 1);
        scanf("%u",&origen);
        if ((origen>numVertex - 1)){
            printf("Node incorrecte. Introdueix un node entre 0 i %u.\n", numVertex - 1);
        }
    } while (origen > numVertex - 1);

    cami->llistaNodesVist[0] = origen;
    (*numCaminsDisponibles) = 0;
    CalcularCaminDonatOrigen(origen, numArestes, larestes, cami, numCaminsDisponibles);

    printf("La funcio ha trobat %u camins disponibles a partir del node %u", *numCaminsDisponibles, origen);
    printf("\n");

    // Alliberem memoria
    free(larestes);
    free(llistaValenciaNodes);
    free(posNodeMax);
    free(numCaminsDisponibles);
    free(cami->llistaNodesVist);
    free(cami->llistaArestesVist);
    free(cami);
    return 0;
}

unsigned arestesDelNode(unsigned Node, unsigned numA, aresta larestes[], unsigned APosibles[]){
    unsigned i,nombre=0;
    for(i=0;i<numA;i++){
        if(larestes[i].estat==0 && (larestes[i].n1==Node||larestes[i].n2==Node)){
            APosibles[nombre]=i;
            nombre++;
        }
    }
    return nombre;
}
/* 
La funció recorre la llista d'arestes disponibles mirant si una aresta
concreta està en aquesta.
*/
int arestaaLallista(unsigned ar, unsigned llistap[], unsigned numVPossibles){
    unsigned i;
    for (i=0; i<numVPossibles; i++){
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
        if (lValenciaNodes[*posNodeMax] < lValenciaNodes[i]){
            *posNodeMax = i;
        }
    }
}

void CalcularCaminDonatOrigen(unsigned origen, unsigned numArestes, aresta lArestes[], estructuracami *camiPossible, unsigned *numCaminsTrobats){
    unsigned i, lAPossibles[numArestes], numAPossibles = arestesDelNode(origen, numArestes, lArestes, lAPossibles);

    if (numAPossibles == 0){
        mostrarCami(camiPossible);
        (*numCaminsTrobats)++;
        return;
    }

    for (i=0;i<numAPossibles;i++){
        unsigned AEscollida = lAPossibles[i], VDesti;

        lArestes[AEscollida].estat = 1;
        camiPossible->llistaArestesVist[camiPossible->numArestesVist] = AEscollida;
        
        
        if (lArestes[AEscollida].n1 == origen){
            VDesti = lArestes[AEscollida].n2;
        }
        else{
            VDesti = lArestes[AEscollida].n1;
        }

        (camiPossible->numArestesVist)++;
        camiPossible->llistaNodesVist[camiPossible->numArestesVist] = VDesti;

        CalcularCaminDonatOrigen(VDesti, numArestes, lArestes, camiPossible, numCaminsTrobats);

        (camiPossible->numArestesVist)--;
        lArestes[AEscollida].estat = 0;
    }
}

void mostrarCami(estructuracami *cami){
    unsigned i;
    for (i=0;i<cami->numArestesVist;i++){
        printf("%u -%u-> ",cami->llistaNodesVist[i],cami->llistaArestesVist[i]);
    }
    printf("%u\n",cami->llistaNodesVist[cami->numArestesVist]);
}
