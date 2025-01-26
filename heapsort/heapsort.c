#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

typedef struct Package
{
    int index;
    int numElem;
    int *packageContent;
} Package;

void replace(Package *P1, Package *P2)
{
    Package aux = *P1;
    *P1 = *P2;
    *P2 = aux;
}

void replaceNodes(Package V[], int P, int i)
{
    Package aux = V[P];
    V[P] = V[i];
    V[i] = aux;
}

int left(int i)
{
    return 2 * i + 1;
}

int right(int i)
{
    return 2 * i + 2;
}

void heapify(Package V[], int T, int i)
{
    int P = i, E = left(i), D = right(i);
    if (E < T && V[E].index > V[P].index)
    {
        P = E;
    }

    if (D < T && V[D].index > V[P].index)
    {
        P = D;
    }

    if (P != i)
    {
        replaceNodes(V, P, i);
        heapify(V, T, P);
    }
}

void build_heap(Package V[], int n)
{
    for (int i = n; i >= 0; i--)
        heapify(V, n, i);
}

void heapsort(Package V[], int n)
{
    build_heap(V, n);

    for (int i = n - 1; i > 0; i--)
    {
        replace(&V[0], &V[i]);

        heapify(V, i, 0);
    }
}

int main(int argc, char *argv[])
{
    // FILE *input = fopen(argv[1], "r");
    // FILE *output = fopen(argv[2], "w");
    FILE *input = fopen("input.txt", "r");
    FILE *output = fopen("output.txt", "w");

    if (input != NULL)
    {
        int numV = 0, numPack = 0;
        fscanf(input, "%d %d", &numV, &numPack);
        Package *packages = malloc(numV * sizeof(Package));

        for (int i = 0; i < numV; i++)
        {
            fscanf(input, "%d %d ", &packages[i].index, &packages[i].numElem);

            packages[i].packageContent = malloc(packages[i].numElem * sizeof(packages[i].packageContent));

            for (int j = 0; j < packages[i].numElem; j++)
            {
                fscanf(input, "%X ", &packages[i].packageContent[j]);
            }
        }

        heapsort(packages, numV);

        for (int j = 0; j < numV; j++)
        {
            printf("%d ", packages[j].index);
            for (int k = 0; k < packages[j].numElem; k++)
            {
                printf("%02X ", packages[j].packageContent[k]);
            }
            printf("\n");
        }
    }

    fclose(input);
    fclose(output);
    return 0;
}
