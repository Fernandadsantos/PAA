#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>

typedef struct Container
{
    char code[15];
    char cnpj[20];
    char wrongCnpj[20];
    int weight;
    int index;
    int weightDifference;
    int priority;
    int percentage;
} Container;

void copy(Container *dest, Container *src, int count)
{
    for (int i = 0; i < count; i++)
    {
        dest[i] = src[i];
    }
}

void intercalar(Container *allContainers, Container *aux, int i, int m, int j, int type)
{
    int i1 = i, i2 = m + 1, k = i;

    while (i1 <= m && i2 <= j)
    {
        // type == 1 comparação de CNPJ pelo index
        if (type == 1)
        {
            if (allContainers[i1].index < allContainers[i2].index)
            {
                aux[k++] = allContainers[i1++];
            }
            else
            {
                aux[k++] = allContainers[i2++];
            }
        } // type == -1 comparação de porcentagem, se igual com index menor ou por porcentagem maior
        else if (type == -1)
        {
            if (allContainers[i1].percentage > allContainers[i2].percentage)
            {
                aux[k++] = allContainers[i1++];
            }
            else if (allContainers[i1].percentage == allContainers[i2].percentage)
            {
                if (allContainers[i1].index < allContainers[i2].index)
                {
                    aux[k++] = allContainers[i1++];
                }
                else
                {
                    aux[k++] = allContainers[i2++];
                }
            }
            else
            {
                aux[k++] = allContainers[i2++];
            }
        }
        else
        {
            if (strcmp(allContainers[i1].code, allContainers[i2].code) < 0)
            {
                aux[k++] = allContainers[i1++];
            }
            else
            {
                aux[k++] = allContainers[i2++];
            }
        }
    }

    while (i1 <= m)
    {
        aux[k++] = allContainers[i1++];
    }

    while (i2 <= j)
    {
        aux[k++] = allContainers[i2++];
    }

    copy(&allContainers[i], &aux[i], j - i + 1);
}

void mergesort(Container *containers, Container *containersAUX, int i, int j, int type)
{
    if (i < j)
    {
        int m = i + (j - i) / 2;

        mergesort(containers, containersAUX, i, m, type);
        mergesort(containers, containersAUX, m + 1, j, type);

        intercalar(containers, containersAUX, i, m, j, type);
    }
}

int binarySearch(int start, int end, Container *allContainers, char *code)
{
    if (end < start)
    {
        return -1;
    }

    int index = start + (end - start) / 2;

    if (strcmp(allContainers[index].code, code) == 0)
    {
        return index;
    }
    else if (strcmp(allContainers[index].code, code) < 0)
    {
        return binarySearch((index + 1), end, allContainers, code);
    }
    else
    {
        return binarySearch(start, index - 1, allContainers, code);
    }
}

void printContainers(Container *containers, int index, FILE *output, int type)
{
    for (int i = 0; i < index; i++)
    {
        if (!type)
        {
            fprintf(output, "%s:%dkg(%d%%)\n", containers[i].code, containers[i].weightDifference, containers[i].percentage);
        }
        else
        {
            fprintf(output, "%s:%s<->%s\n", containers[i].code, containers[i].cnpj, containers[i].wrongCnpj);
        }
    }
}

void selectContainers(Container *sample, Container *allContainers, int numContainer, int numSample, FILE *output)
{
    Container *ContainersCNPJ = malloc(numSample * (sizeof(Container)));
    Container *ContainersWeight = malloc(numSample * (sizeof(Container)));
    int indexCNPJ = 0, indexW = 0;

    for (int i = 0; i < numSample; i++)
    {
        int index = binarySearch(0, numContainer, allContainers, sample[i].code);
        if (index != -1)
        {
            if (strcmp(allContainers[index].cnpj, sample[i].cnpj) != 0)
            {
                strcpy(ContainersCNPJ[indexCNPJ].code, sample[i].code);
                strcpy(ContainersCNPJ[indexCNPJ].wrongCnpj, sample[i].cnpj);
                strcpy(ContainersCNPJ[indexCNPJ].cnpj, allContainers[index].cnpj);
                ContainersCNPJ[indexCNPJ].index = allContainers[index].index;
                indexCNPJ++;
            }
            else
            {
                int difference = abs(sample[i].weight - allContainers[index].weight);
                float percentage10 = ((float)allContainers[index].weight * 10) / 100;
                float percentage = (difference * 10) / percentage10;
                int per = (int)round(percentage);

                if (per > 10)
                {
                    strcpy(ContainersWeight[indexW].code, allContainers[index].code);
                    ContainersWeight[indexW].weightDifference = difference;
                    ContainersWeight[indexW].percentage = per;
                    ContainersWeight[indexW].index = allContainers[index].index;
                    indexW++;
                }
            }
        }
    }

    Container *aux1 = malloc(indexCNPJ * (sizeof(Container)));
    Container *aux2 = malloc(indexW * (sizeof(Container)));

    mergesort(ContainersCNPJ, aux1, 0, indexCNPJ - 1, 1);
    printContainers(ContainersCNPJ, indexCNPJ, output, 1);

    mergesort(ContainersWeight, aux2, 0, indexW - 1, -1);
    printContainers(ContainersWeight, indexW, output, 0);

    free(ContainersCNPJ);
    free(ContainersWeight);
    free(aux1);
    free(aux2);
}

void readContainers(Container *containers, int numContainers, FILE *input)
{
    for (int i = 0; i < numContainers; i++)
    {
        fscanf(input, "%s %s %d", containers[i].code, containers[i].cnpj, &containers[i].weight);
        containers[i].index = i;
    }
}

int main(int argc, char *argv[])
{
    // FILE *input = fopen("input.txt", "r");
    // FILE *output = fopen("output.txt", "w");
    FILE *input = fopen(argv[1], "r");
    FILE *output = fopen(argv[2], "w");
    if (input != NULL)
    {
        int numContainers = 0;
        int numSample = 0;

        fscanf(input, "%d", &numContainers);
        Container *allContainers = malloc(numContainers * sizeof(Container));
        Container *aux = malloc(numContainers * sizeof(Container));

        readContainers(allContainers, numContainers, input);

        mergesort(allContainers, aux, 0, numContainers - 1, 0);

        fscanf(input, "%d", &numSample);
        Container *sample = malloc(numSample * sizeof(Container));

        readContainers(sample, numSample, input);

        selectContainers(sample, allContainers, numContainers, numSample, output);

        free(allContainers);
        free(sample);
        free(aux);
    }

    fclose(input);
    fclose(output);
    return 0;
}
