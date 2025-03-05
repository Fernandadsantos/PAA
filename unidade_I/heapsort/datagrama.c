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

int current_pack = 0, current_heap_index, flag = 0;

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
    if (E < T && V[E].index < V[P].index)
    {
        P = E;
    }

    if (D < T && V[D].index < V[P].index)
    {
        P = D;
    }

    if (P != i)
    {
        replaceNodes(V, P, i);
        heapify(V, T, P);
    }
}

int find_dad(int i) { return (i - 1) / 2; }

void build_heap(Package V[])
{
    for (int i = find_dad(current_heap_index); i >= 0; i--)
        heapify(V, current_heap_index, i);
}

void printPackages(Package packages, FILE *output)
{
    fprintf(output, "|");
    for (int k = 0; k < packages.numElem; k++)
    {
        fprintf(output, "%02X", packages.packageContent[k]);
        if (k + 1 != packages.numElem)
        {
            fprintf(output, ",");
        }
    }
}

void heapsort(Package V[], int total_packs, FILE *output)
{
    build_heap(V);

    for (int i = current_heap_index - 1; i >= 0; i--)
    {
        if (V[0].index == current_pack)
        {
            printPackages(V[0], output);

            replace(&V[0], &V[current_heap_index - 1]);
            current_pack++;
            current_heap_index--;
            heapify(V, current_heap_index, 0);
            flag = 1;
        }
    }
    if (flag && total_packs != 1)
    {
        fprintf(output, "|\n");
        flag = 0;
    }
}

int main(int argc, char *argv[])
{
    FILE *input = fopen(argv[1], "r");
    FILE *output = fopen(argv[2], "w");
    // FILE *input = fopen("input.txt", "r");
    // FILE *output = fopen("output.txt", "w");

    if (input != NULL)
    {
        int numV = 0, numPack = 0;
        fscanf(input, "%d %d", &numV, &numPack);
        Package *packages = malloc(numV * sizeof(Package));
        current_heap_index = 0;
        for (int i = 0; i < numV; i++)
        {
            int total_packs = 0;
            for (int j = 0; j < numPack; j++)
            {
                if (fscanf(input, "%d %d ", &packages[current_heap_index].index, &packages[current_heap_index].numElem) != EOF)
                {
                    packages[current_heap_index].packageContent = malloc(packages[current_heap_index].numElem * sizeof(int));

                    for (int k = 0; k < packages[current_heap_index].numElem; k++)
                    {
                        fscanf(input, "%X ", &packages[current_heap_index].packageContent[k]);
                    }
                    current_heap_index++;
                    total_packs++;
                }
            }
            heapsort(packages, total_packs, output);
        }
    }

    fclose(input);
    fclose(output);
    return 0;
}
