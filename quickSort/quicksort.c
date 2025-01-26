#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>

typedef struct Rates
{
    int numElem;
    int ordem[6];
} Rates;

void replace(int *V1, int *V2, int *steps)
{
    int aux = *V1;
    *V1 = *V2;
    *V2 = aux;
    *steps += 1;
    printf("trocar: %d \n", *steps);
}

int hoare(int *V, int i, int j, int *steps)
{
    // Declaração do pivô e índices
    int P = V[i], x = i - 1, y = j + 1;
    // Particionando o vetor pelo pivô
    while (1)
    {
        while (V[--y] > P)
            ;
        while (V[++x] < P)
            ;
        if (x < y)
        {
            replace(&V[x], &V[y], steps);
        }

        else
            return y;
    }
}

int lomuto(int *V, int i, int j, int *steps)
{
    int P = V[j], x = i - 1, y = i;

    for (y = i; y < j; y++)
    {
        if (V[y] <= P)
        {
            replace(&V[++x], &V[y], steps);
        }
    }

    replace(&V[++x], &V[j], steps);
    return x;
}

// V1 = V[n/4], V2 = V[n/2], V3 = V[3n/4]
int calc_median(int *V, int i, int j)
{
    int n = j - i + 1;
    int v1 = V[i + n / 4];
    int v2 = V[i + n / 2];
    int v3 = V[i + 3 * n / 4];

    if ((v1 > v2) != (v1 > v3))
        return v1;
    else if ((v2 > v1) != (v2 > v3))
        return v2;
    else
        return v3;
}

int hoare_median(int *V, int i, int j, int *steps)
{
    int P = calc_median(V, i, j);

    int pivot_index = i;
    for (int k = i; k <= j; k++)
    {
        if (V[k] == P)
        {
            pivot_index = k;
            break;
        }
    }
    replace(&V[pivot_index], &V[i], steps);

    int x = i - 1;
    int y = j + 1;

    while (1)
    {
        do
        {
            y--;
        } while (V[y] > P);

        do
        {
            x++;
        } while (V[x] < P);

        if (x >= y)
            return y;

        replace(&V[x], &V[y], steps);
    }
}

int lomuto_median(int *V, int i, int j, int *steps)
{
    int P = calc_median(V, i, j);

    int pivot_index = i;
    for (int k = i; k <= j; k++)
    {
        if (V[k] == P)
        {
            pivot_index = k;
            break;
        }
    }
    replace(&V[pivot_index], &V[j], steps);

    int x = i - 1;

    for (int y = i; y < j; y++)
    {
        if (V[y] <= P)
        {
            x++;
            replace(&V[x], &V[y], steps);
        }
    }

    replace(&V[x + 1], &V[j], steps);

    return x + 1;
}

int rands(int *V, int i, int j, int type, int *steps)
{
    // rand_hoare
    if (type == 1)
    {
        // Troca do pivô por aleatório
        replace(&V[i], &V[i + (abs(V[i]) % (j + 1))], steps);
        // Chamada do particionamento
        return hoare(V, i, j, steps);
    }
    // rand_lomuto
    else
    {
        // Troca do pivô por aleatório
        replace(&V[j], &V[i + (abs(V[i]) % (j + 1))], steps);
        // Chamada do particionamento
        return lomuto(V, i, j, steps);
    }
}

void quicksort(int *V, int i, int j, int type, int type_rand, int *steps)
{
    
    *steps += 1; 
    if (i < j)
    {
        int p = 0;

        if (type == 0)
        {
            printf("HOARE\n");
            p = hoare(V, i, j, steps);
            quicksort(V, i, p, type, type_rand, steps);
        }
        else if (type == 1)
        {
            printf("LOMUTO\n");
            p = lomuto(V, i, j, steps);
            quicksort(V, i, p - 1, type, type_rand, steps);
        }
        else if (type == 2)
        {
            p = rands(V, i, j, type_rand, steps);

            if(type_rand == 1){ 
                printf("RAND_HOARE\n");
                quicksort(V, i, p, type, type_rand, steps);
            }
            else{
                printf("RAND_LOMUTO\n");
                quicksort(V, i, p-1, type, type_rand, steps);
            }
        }
        else if (type == 3)
        {
            printf("LOMUTO_MEDIAN\n");
            p = lomuto_median(V, i, j, steps);
            quicksort(V, i, p - 1, type, type_rand, steps);
        }
        else
        {
            printf("HOARE_MEDIAN\n");
            p = hoare_median(V, i, j, steps);
            quicksort(V, i, p, type, type_rand, steps);
        }

        quicksort(V, p + 1, j, type, type_rand, steps);
    }
}

void print(int numV, int numElements)
{
    for (int i = 0; i < numV; i++)
    {
        printf("%d:N(%d),", i, numElements);

        printf("\n");
    }
}

int order(int *V, int ini, int numElements, int type, int type_rand)
{
    int aux[numElements];
    int steps = 0;

    for (int i = 0; i < numElements; i++)
    {
        aux[i] = V[i];
    }

    quicksort(aux, ini, numElements - 1, type, type_rand, &steps);
    
    if(type == 2){
        printf("%d \n", type_rand);
        for (int j = 0; j < numElements; j++) { 
                printf("%d ", aux[j]);
        }
        printf("\n");

    }

    return steps;
}

int main(int argc, char *argv[])
{
    FILE *input = fopen("input.txt", "r");
    FILE *output = fopen("output.txt", "w");
    // FILE *input = fopen(argv[1], "r");
    // FILE *output = fopen(argv[2], "w");
    if (input != NULL)
    {
        int numV = 0, numElements = 0;

        Rates rates;
        fscanf(input, "%d", &numV);

        for (int i = 0; i < numV; i++)
        {
            fscanf(input, "%d", &numElements);
            int *vector = malloc(numElements * sizeof(int));
            rates.numElem = numElements;

            printf("VETOR ");
            for (int j = 0; j < numElements; j++)
            {
                fscanf(input, "%d", &vector[j]);
                printf("%d ", vector[j]);
            }
                printf("\n");

            rates.ordem[0] = order(vector, 0, numElements, 0, 0); // hoare
            rates.ordem[1] = order(vector, 0, numElements, 1, 0); // lomuto
            rates.ordem[2] = order(vector, 0, numElements, 2, 0); // rand lomuto
            rates.ordem[3] = order(vector, 0, numElements, 2, 1); // rand hoare
            rates.ordem[4] = order(vector, 0, numElements, 3, 0); // lomuto median
            rates.ordem[5] = order(vector, 0, numElements, 4, 0); // hoare median

            

            printf("%d:N(%d),HP(%d),LP(%d),LA(%d),HA(%d),LM(%d),HM(%d)\n", 0, numElements, rates.ordem[0], rates.ordem[1], rates.ordem[2], rates.ordem[3], rates.ordem[4], rates.ordem[5]);

            // print(numV, rates.numElem);
        }
    }

    fclose(input);
    fclose(output);
    return 0;
}
