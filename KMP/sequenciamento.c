#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>

typedef struct Genes {
    char *gene;
} Genes;

typedef struct Doenca {
    char *code;
    int qtdGenes;
    Genes *genes;
} Doenca;

int main(int argc, char *argv[])
{
    FILE *input = fopen("input.txt", "r");
    FILE *output = fopen("output.txt", "w");
    //FILE *input = fopen(argv[1], "r");
    //FILE *output = fopen(argv[2], "w");
    if (input != NULL){
        int subSize = 0, doencas = 0;
        char DNA[35];
        fscanf(input, "%d", &subSize);
        printf("%d\n",subSize);
        fscanf(input, "%s", DNA);
        printf("%s\n",DNA);
        fscanf(input, "%d", &doencas);
        printf("%d\n",doencas);

        for(int i = 0; i < doencas; i++){
            Doenca *doenca = malloc(sizeof(Doenca));

            fscanf(input, "%s %d", doenca->code, &doenca->qtdGenes);
            printf("%s %d\n", doenca->code, doenca->qtdGenes);

            doenca->genes = malloc(doenca->qtdGenes * sizeof(Genes));
            for(int j = 0; j < doenca->qtdGenes; j++){

                fscanf(input, "%s", doenca->genes[j]);
                printf("%s ", doenca->genes[j]);
            }
            printf("\n");

            free(doenca);
        }

    }

    fclose(input);
    fclose(output);
    return 0;
}