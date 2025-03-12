#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <math.h>
#include <time.h>

int check_inside(labrint, X, Y){

}


void read_input(FILE *input, FILE * output){
    int qtd_labrints = 0, lines = 0, columns = 0;
    fscanf(input, "%d", &qtd_labrints);

    for(int q = 0; q < qtd_labrints; q++){
        printf("L%d:\n", q);
        fscanf(input, "%d %d", &columns, &lines);

        char labrint[lines][columns  + 1];
        int startX = 0, startY = 0;
        for(int i = 0; i < lines; i++){ 
            for(int j = 0; j < columns; j++){
                fscanf(input, " %c", &labrint[i][j]); 
                if(  labrint[i][j] == 'X'  ){
                    printf("INICIO@%d,%d\n", i, j);
                    startX = i;
                    startY = j;
                }
            } 
        }

        int inside = 1, X = startX, Y = startY, last_moviment = 0;

        while(inside){

            inside = check_sideout(labrint, X, Y);

            if(labrint[X][Y +1] != NULL || labrint[X][Y +1] != '1'){
                printf("F@%d,%d->%d,%d\n", X, Y, X, Y+1);
                Y +=1;
                continue;
            }

            if(labrint[X+1][Y] != NULL || labrint[X+1][Y] != '1'){
                printf("D@%d,%d->%d,%d\n", X, Y, X+1, Y );
                X +=1;
                continue;
            }

            if(labrint[X][Y-1] != NULL || labrint[X][Y-1] != '1'){
                printf("E@%d,%d->%d,%d\n", X, Y, X, Y-1 );
                Y -=1;
                continue;
            }

            if(labrint[X-1][Y] != NULL || labrint[X-1][Y] != '1'){
                printf("T@%d,%d->%d,%d\n", X, Y, X-1, Y );
                X -=1;
                continue;
            }

            
        }
 

        for(int i = 0; i < lines; i++){
            for(int j = 0; j < columns; j++){
                fprintf(output, "%c ", labrint[i][j] ); 
            }
            fprintf(output, "\n");
        }
 
    }

}

int main(int argc, char *argv[])
{
    FILE *input = fopen("input.txt", "r");
    FILE *output = fopen("output.txt", "w");
    // FILE *input = fopen(argv[1], "r");
    // FILE *output = fopen(argv[2], "w");

    if (input != NULL)
    { 
        read_input(input, output);

        fclose(input);
        fclose(output);
    }
    return 0;
}