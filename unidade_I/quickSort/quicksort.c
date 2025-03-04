#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>

typedef struct Order
{
    char name[3];
    int num;
    int index;
} Order;

typedef struct Rates
{
    int numElem;
    Order order[6];
} Rates;

void replace(int *V1, int *V2, int *steps)
{
    int aux = *V1;
    *V1 = *V2;
    *V2 = aux;
    *steps += 1;
}

int hoare(int *V, int i, int j, int *steps)
{
    int P = V[i], x = i - 1, y = j + 1;
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
    int P = V[j], x = i - 1;
    for (int y = i; y < j; y++)
    {
        if (V[y] <= P)
        {
            replace(&V[++x], &V[y], steps);
        }
    }
    replace(&V[++x], &V[j], steps);
    return x;
}

int calc_median(int *V, int i, int j)
{
    int n = j - i + 1;
    int v1 = i + n / 4;
    int v2 = i + n / 2;
    int v3 = i + 3 * n / 4;

    if ((V[v1] >= V[v2]) && (V[v1] <= V[v3]) || (V[v1] <= V[v2]) && (V[v1] >= V[v3]))
        return v1;
    else if ((V[v2] >= V[v1]) && (V[v2] <= V[v3]) || (V[v2] <= V[v1]) && (V[v2] >= V[v3]))
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

    return hoare(V, i, j, steps);
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

    return lomuto(V, i, j, steps);
}

int rands(int *V, int i, int j, int type, int *steps)
{
    if (type == 1)
    {
        replace(&V[i], &V[i + (abs(V[i]) % (j - i + 1))], steps);
        return hoare(V, i, j, steps);
    }
    else
    {
        replace(&V[j], &V[i + (abs(V[i]) % (j - i + 1))], steps);
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
            p = hoare(V, i, j, steps);
            quicksort(V, i, p, type, type_rand, steps);
        }
        else if (type == 1)
        {
            p = lomuto(V, i, j, steps);
            quicksort(V, i, p - 1, type, type_rand, steps);
        }
        else if (type == 2)
        {
            p = rands(V, i, j, type_rand, steps);

            if (type_rand == 1)
            {
                quicksort(V, i, p, type, type_rand, steps);
            }
            else
            {
                quicksort(V, i, p - 1, type, type_rand, steps);
            }
        }
        else if (type == 3)
        {
            p = lomuto_median(V, i, j, steps);
            quicksort(V, i, p - 1, type, type_rand, steps);
        }
        else
        {
            p = hoare_median(V, i, j, steps);
            quicksort(V, i, p, type, type_rand, steps);
        }

        quicksort(V, p + 1, j, type, type_rand, steps);
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

    return steps;
}

void sortElements(Order order[], int n)
{
    for (int i = 0; i < n; i++)
    {
        int smaller = i;

        for (int j = i + 1; j < n; j++)
        {
            if (order[smaller].num > order[j].num || (order[smaller].num == order[j].num && order[smaller].index > order[j].index))
            {
                smaller = j;
            }
        }

        Order aux = order[i];
        order[i] = order[smaller];
        order[smaller] = aux;
    }
}

void print(int numElements, Order *order, FILE *output)
{

    fprintf(output, "N(%d)", numElements);

    for (int i = 0; i < 6; i++)
    {
        fprintf(output, ",%s(%d)", order[i].name, order[i].num);
    }
    fprintf(output, "\n");
}

int main(int argc, char *argv[])
{
    // FILE *input = fopen("input.txt", "r");
    // FILE *output = fopen("output.txt", "w");
    FILE *input = fopen(argv[1], "r");
    FILE *output = fopen(argv[2], "w");
    if (input != NULL)
    {
        int numV = 0, numElements = 0;

        Rates rates;
        fscanf(input, "%d", &numV);

        for (int i = 0; i < numV; i++)
        {
            fprintf(output, "%d:", i);
            fscanf(input, "%d", &numElements);
            int vector[numElements];
            rates.numElem = numElements;

            for (int j = 0; j < numElements; j++)
            {
                fscanf(input, "%d", &vector[j]);
            }

            rates.order[0].index = 0;
            rates.order[0].num = order(vector, 0, numElements, 1, 0);
            strcpy(rates.order[0].name, "LP");
            rates.order[1].index = 1;
            rates.order[1].num = order(vector, 0, numElements, 3, 0);
            strcpy(rates.order[1].name, "LM");
            rates.order[2].index = 2;
            rates.order[2].num = order(vector, 0, numElements, 2, 0);
            strcpy(rates.order[2].name, "LA");
            rates.order[3].index = 3;
            rates.order[3].num = order(vector, 0, numElements, 0, 0);
            strcpy(rates.order[3].name, "HP");
            rates.order[4].index = 4;
            rates.order[4].num = order(vector, 0, numElements, 4, 0);
            strcpy(rates.order[4].name, "HM");
            rates.order[5].index = 5;
            rates.order[5].num = order(vector, 0, numElements, 2, 1);
            strcpy(rates.order[5].name, "HA");

            sortElements(rates.order, 6);
            print(numElements, rates.order, output);
        }
    }

    fclose(input);
    fclose(output);
    return 0;
}
