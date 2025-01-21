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

int lomuto(int *V, int i, int j)
{
    // Declaração do pivô e índices
    int P = V[j], x = i - 1, y = i;
    // Particionando o vetor pelo pivô
    for (y = i; y < j; y++)
        if (V[y] <= P)
            trocar(V[++x], V[y]);
    // Posicionando o pivô no vetor
    trocar(V[++x], V[j]);
    // Retornando índice do pivô
    return x;
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

// int median(int *V, int i, int j, int k, int type)
// {

//     // Caso base
//     if (i == j)
//         return i;
//     // Recorrência
//     else
//     {
//         int m = 0;
//         if (type == 1)
//         {
//             m = hoare(V, i, j);
//         }
//         else
//         {
//             m = lomuto(V, i, j);
//         }

//         if (m - i + 1 >= k)
//             median(V, i, m, k, type);
//         else
//             median(V, m + 1, j, k - (m - i + 1), type);
//     }
// }

// void rands(int *V, int i, int j, int type)
// {
//     // rand_hoare
//     if (type == 1)
//     {
//         // Troca do pivô por aleatório
//         trocar(&V[i], &V[i + (rand() % (j - i + 1))]);
//         // Chamada do particionamento
//         return hoare(V, i, j);
//     }
//     // rand_lomuto
//     else
//     {
//         // Troca do pivô por aleatório
//         trocar(&V[j], &V[i + (rand() % (j - i + 1))]);
//         // Chamada do particionamento
//         return lomuto(V, i, j);
//     }
// }

void quicksort(int *V, int i, int j, int type)
{
    if (i < j)
    {
        int p = 0;

        if (type == 0)
        {
            p = hoare(V, i, j);
        }
        else
        {
            p = lomuto(V, i, j);
        }

        quicksort(V, i, p, type);
        quicksort(V, p + 1, j, type);
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

            quicksort(vector, 0, numElements - 1, 0);

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
