#include <math.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>

typedef struct Genes
{
    char gene[1002];
    int qtdInit;
    int qtd;
    int porcent;
} Genes;

typedef struct Disease
{
    int index;
    char code[30];
    int totalPorcent;
    int qtdGenes;
    Genes *genes;
} Disease;

void calc_table(int *K, char *P)
{
    int p_size = strlen(P);
    for (int i = 1, j = -1; i < p_size; i++)
    {
        while (j >= 1 && P[j + 1] != P[i])
            j = K[j];

        if (P[j + 1] == P[i])
            j++;

        K[i] = j;
    }
}

void insert(int *R, int m, int index)
{
    R[index] = m;
}

int overwrite_gene(char *str, int n) {
    int len = strlen(str);
    if (n >= len) {
        str[0] = '\0';  
        return -1;
    }
 
    for (int i = 0; i < len - n; i++) {
        str[i] = str[i + n];
    }
    str[len - n] = '\0';  

    return 1;
}

void KMP(char *DNA, Disease *disease, int subSize)
{
    for (int d = 0; d < disease->qtdGenes; d++)
    {
        int m = strlen(DNA), n = strlen(disease->genes[d].gene), index = 0, parcialV = 0;
        int *K = malloc(n * sizeof(int));
        memset(K, 0, n * sizeof(int));
        calc_table(K, disease->genes[d].gene);

        for (int i = 0, j = 0; i < m; i++)
        {
            while (j >= 1 && disease->genes[d].gene[j + 1] != DNA[i])
            {
                if (j >= subSize - 1)
                {
                    parcialV += j + 1;
                    disease->genes[d].qtd += parcialV;
                    parcialV = 0;
                    int go = overwrite_gene(disease->genes[d].gene, j + 1);
                    calc_table(K, disease->genes[d].gene);
                    int size = strlen(disease->genes[d].gene);

                    if (go == -1 || subSize > size)
                    {
                        j = -1;
                        i = m;
                        break;
                    }
                    j = -1;
                }
                else
                {
                    j = K[j];
                }
            }

            if (j == n - 1)
            {
                break;
            }

            if (disease->genes[d].gene[j + 1] == DNA[i])
            {
                j++;
            }

            if (j == m - 1)
            {
                parcialV += j + 1;
                index++;
                j = K[j];
                i = m;
                disease->genes[d].qtd = parcialV;
            }
        }
        parcialV = 0;
        disease->genes[d]
            .porcent = (disease->genes[d].qtd * 100) / disease->genes[d].qtdInit;
    }
}

int calc_percent(Disease *disease)
{
    if (disease->qtdGenes == 1)
    {
        return (disease->genes[0].porcent >= 90 ? 100 : disease->genes[0].porcent);
    }
    else
    {
        int counter = 0;
        for (int i = 0; i < disease->qtdGenes; i++)
        {
            if (disease->genes[i].porcent >= 90)
                counter++;
        }
        float porcent = ((float)counter * 100) / disease->qtdGenes;
        return (int)round(porcent) >= 90 ? 100 : (int)(round)(porcent);
    }
}

void sortElements(Disease *D, int n, FILE *output)
{
    for (int i = 0; i < n; i++)
    {
        int smaller = i;

        for (int j = i + 1; j < n; j++)
        {
            if (D[smaller].totalPorcent < D[j].totalPorcent || (D[smaller].totalPorcent == D[j].totalPorcent && D[smaller].index > D[j].index))
            {
                smaller = j;
            }
        }

        Disease aux = D[i];
        D[i] = D[smaller];
        D[smaller] = aux;
        fprintf(output, "%s->%d%%\n", D[i].code, D[i].totalPorcent);
    }
}

int main(int argc, char *argv[])
{
     clock_t start = clock();
    // FILE *input = fopen("input.txt", "r");
    // FILE *output = fopen("output.txt", "w");
    FILE *input = fopen(argv[1], "r");
    FILE *output = fopen(argv[2], "w");
    if (input != NULL)
    {
        int subSize = 0, num_disease = 0;
        char DNA[40001];
        fscanf(input, "%d %s %d", &subSize, DNA, &num_disease);

        Disease *arrDisease = malloc(num_disease * sizeof(Disease));
        for (int i = 0; i < num_disease; i++)
        {
            Disease disease;

            fscanf(input, "%s %d", disease.code, &disease.qtdGenes);

            disease.genes = malloc(disease.qtdGenes * sizeof(Genes));
            for (int j = 0; j < disease.qtdGenes; j++)
            {
                fscanf(input, "%s", disease.genes[j].gene);

                if (strcmp(disease.genes[j].gene, " ") != 0)
                {
                    disease.genes[j].qtd = 0;
                    disease.genes[j].qtdInit = strlen(disease.genes[j].gene);
                    disease.genes[j].porcent = 0;
                }
            }

            KMP(DNA, &disease, subSize);
            arrDisease[i].totalPorcent = calc_percent(&disease);
            arrDisease[i].index = i;
            strcpy(arrDisease[i].code, disease.code);

            free(disease.genes);
        }
        sortElements(arrDisease, num_disease, output);
        free(arrDisease);
    }

    fclose(input);
    fclose(output);
     clock_t end = clock();
    double elapsed_time = ((double)(end - start)) / CLOCKS_PER_SEC;

    printf("Tempo de execução: %.3f segundos\n", elapsed_time);
    return 0;
}