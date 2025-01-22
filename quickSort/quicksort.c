#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>

void trocar(int *V1, int *V2)
{
    int aux = *V1;
    *V1 = *V2;
    *V2 = aux;
}

int hoare(int *V, int i, int j)
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
            trocar(&V[x], &V[y]);
        }

        else
            return y;
    }
}

int lomuto(int *V, int i, int j)
{
    int P = V[j], x = i - 1, y = i;

    for (y = i; y < j; y++)
    {
        if (V[y] <= P)
        {
            trocar(&V[++x], &V[y]);
        }
    }

    trocar(&V[++x], &V[j]);
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

int hoare_median(int *V, int i, int j)
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
    trocar(&V[pivot_index], &V[i]);

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

        trocar(&V[x], &V[y]);
    }
}

int lomuto_median(int *V, int i, int j)
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
    trocar(&V[pivot_index], &V[j]);

    int x = i - 1;

    for (int y = i; y < j; y++)
    {
        if (V[y] <= P)
        {
            x++;
            trocar(&V[x], &V[y]);
        }
    }

    trocar(&V[x + 1], &V[j]);

    return x + 1;
}

int rands(int *V, int i, int j, int type)
{
    // rand_hoare
    if (type == 1)
    {
        // Troca do pivô por aleatório
        printf("rand: %d\n", V[i + (abs(V[i]) % (j + 1))]);
        trocar(&V[i], &V[i + (abs(V[i]) % (j + 1))]);
        // Chamada do particionamento
        return hoare(V, i, j);
    }
    // rand_lomuto
    else
    {
        // Troca do pivô por aleatório
        printf("rand: %d\n", V[i + (abs(V[i]) % (j + 1))]);
        trocar(&V[j], &V[i + (abs(V[i]) % (j + 1))]);
        // Chamada do particionamento
        return lomuto(V, i, j);
    }
}

void quicksort(int *V, int i, int j, int type, int type_rand)
{
    if (i < j)
    {
        int p = 0;

        if (type == 0)
        {
            p = hoare(V, i, j);
            quicksort(V, i, p, type, type_rand);
        }
        else if (type == 1)
        {
            p = lomuto(V, i, j);
            quicksort(V, i, p - 1, type, type_rand);
        }
        else if (type == 2)
        {
            p = rands(V, i, j, type_rand);
            quicksort(V, i, p - 1, type, type_rand);
        }
        else if (type == 3)
        {
            p = lomuto_median(V, i, j);
            quicksort(V, i, p - 1, type, type_rand);
        }
        else
        {
            p = hoare_median(V, i, j);
            quicksort(V, i, p, type, type_rand);
        }

        quicksort(V, p + 1, j, type, type_rand);
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
        int numV = 0, numElements = 0;

        fscanf(input, "%d", &numV);
        for (int i = 0; i < numV; i++)
        {
            fscanf(input, "%d", &numElements);
            int *vector = malloc(numElements * sizeof(int));

            for (int j = 0; j < numElements; j++)
            {
                fscanf(input, "%d", &vector[j]);
            }

            quicksort(vector, 0, numElements - 1, 4, 0);

            for (int k = 0; k < numElements; k++)
            {
                printf("%d ", vector[k]);
            }
            printf("\n");
        }
    }

    fclose(input);
    fclose(output);
    return 0;
}
