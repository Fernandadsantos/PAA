#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>

typedef struct Gene
{
    char gene[1000];
    int percent;
} Gene;

typedef struct Illness
{
    char code[100];
    int qtdGenes;
    Gene genes[10];
} Illness;

void calc_table(long *K, char *subString)
{
    for (int i = 1, j = -1; i < strlen(subString); i++)
    {
        while (j >= 0 && subString[j + 1] != subString[i])
        {
            j = K[j];
        }

        if (subString[j + 1] == subString[i])
        {
            j++;
        }

        K[i] = j;
    }
}

void calc_table(long *K, char *subString)
{
    for (int i = 1, j = -1; i < strlen(subString); i++)
    {
        while (j >= 0 && subString[j + 1] != subString[i])
        {
            j = K[j];
        }

        if (subString[j + 1] == subString[i])
        {
            j++;
        }

        K[i] = j;
    }
}

void inserir(int32_t *V, int32_t i, int32_t *d)
{
    V[*d] = i;
    (*d)++;
}

void KMP(int32_t *K, long *R, char *T, char *P)
{
    int n = strlen(T), m = strlen(P), d = 0;
    calc_table(K, P);

    for (int i = 0, j = -1; i < n; i++)
    {
        while (j >= 0 && P[j + 1] != T[i])
            j = K[j];

        if (P[j + 1] == T[i])
            j++;

        if (j == m - 1)
        {
            inserir(R, i - m + 1, &d);
            j = K[j];
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
        int subSize = 0, illnessNum = 0;
        char DNA[35];
        fscanf(input, "%d %s %d", &subSize, DNA, &illnessNum);

        Illness *illnesses = malloc(illnessNum * sizeof(Illness));

        for (int i = 0; i < illnessNum; i++)
        {
            fscanf(input, "%s %d", illnesses[i].code, &illnesses[i].qtdGenes);

            for (int j = 0; j < illnesses[i].qtdGenes; j++)
            {
                fscanf(input, "%s ", illnesses[i].genes[j].gene);

                int32_t K[strlen(illnesses[i].genes[j].gene)];
                int32_t R[strlen(DNA)];

                KMP(&K, &R, DNA, illnesses[i].genes[j].gene);
            }
        }
        free(illnesses);
    }

    fclose(input);
    fclose(output);
    return 0;
}