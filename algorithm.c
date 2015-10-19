
/* 
Description: The ANSI C code of the SOS Algorithm
Programmer:  Ruan Pablo Medeiros
E-mail:      pm.ruan@gmail.com
Date:	     04/09/2014
Lisence:     Free
*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <math.h>
#include <time.h>
#include "mersenne.h"
#include "sos.h"

#define FAIL    0
#define min(x,y) ((x<y) ? x : y)
#define COND FUNCTION==9 || FUNCTION==10 || FUNCTION==11 || FUNCTION==12 || FUNCTION==13 || FUNCTION==14 || FUNCTION==15


void AllocArrays();

/*Main program of the search algorithm*/
int main(int argc, char **argv){
    int i, k, r;
    double avg;
    double stdDev;
    double *var;
    double mediafo=0;
    double *mediaBfo;
    double *mediaM;
    
    srand(time(NULL));

    if (GetParameters(argv) == -1){	//read input file
        return 0;
    }
    showParameters(FUNCTION, RUN, MAX_ITER, POP_SIZE, DIM);

    switch(FUNCTION){
        case 9:
            rest=1;
            break;
        case 10:
            rest=2;
            break;
        case 11:
            rest=7;
            break;
        case 12: //Pressure Vessel
            rest=4;
            break;
        case 13: //Tension/compression string
            rest=4;
            break;
        case 14: // Speed Reducer(Gear Train)
            rest=11;
            break;
        case 15: // 10-Bar Truss
            rest=21;
            break;
    }

    //Alloc arrays
    mediaBfo=(double*)malloc(MAX_ITER*sizeof(double));
    mediaM=(double*)malloc(MAX_ITER*sizeof(double));
    for(i=0;i<MAX_ITER;i++){
        mediaBfo[i]=0;
        mediaM[i]=0;
    }

    var=(double*)malloc(RUN*sizeof(double));

    AllocArrays();	

    prepararObjFunc();

    for (r=0;r<RUN;r++){	
        //Init population
        initPop();
        bestfo = 10000000;

        for(i=0;i<POP_SIZE;i++){
            if(fo[i]<=bestfo){
                best_index=i;
                bestfo=fo[i];
            }
        }

        for(i=0;i<DIM;i++)best[i]=pop[best_index][i];

        num_fit_eval=0;
        num_iter=0;
        mediafo=0;
        
        while(num_iter<MAX_ITER){
            num_iter++;
            sos_iter();
            for(i=0;i<POP_SIZE;i++){
                mediafo+=fo[i];
            }
            
            mediafo=mediafo/POP_SIZE;
            mediaM[num_iter]+=mediafo;//sum of all mediafo in the num_iter position
            mediaBfo[num_iter]+=bestfo;//sum of all bestfo	in the num_iter position
        }

        //Loop de Iterações.
        printf("RUN: %d\n",r);
        printf("Best solution: ");
        for (k=0; k<DIM;k++){//variables
          printf("%g ",best[k]);
        }
        printf(" Fo:");
        printf("%g \n",bestfo);
        if(r==0)bestfoRUN=bestfo;
        bestfoRUN=min(bestfo, bestfoRUN);
        printf("MIN: %g\n",bestfoRUN);
        printf("bestfo: %g\n", bestfo);
        printf("bestfoRUN: %g\n", bestfoRUN);

        showConst(var,r);
        printf("N_fit_eval:");
        printf("%i \n\n",num_fit_eval);
        //
        
    }//end FOR RUN
    bestfo=var[0];
    best_index=0;
    for (i = 0; i < POP_SIZE; i++){
        if(var[i]<=bestfo && var[i]!=2147483646){
            bestfo=var[i];
            best_index=i;
       }
    }
    int nfeasible = AvgStdDev(&avg,&stdDev,var);
    printf("====================\n");
    printf("Best Fo: ");
    printf("%g\n",bestfoRUN);
    printf("Avg: ");
    printf("%g\n",avg);
    printf("StdDev: ");
    printf("%g\n",stdDev);
    printf("Feasible: ");
    printf("%i\n",nfeasible);
    printf("====================\n");
    freeArrays();
    
    return 0;
}
