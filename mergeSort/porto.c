#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>

typedef struct Container
{
    char code[15];
    char cnpj[20];
    int weight;
    int index;
} Container;

typedef struct WrongContainerData
{
    char code[15];
    char cnpj[20];
    char wrongCnpj[20];
    int weightDifference;
    int percentage;
    int index;
    int priority;
} WrongContainerData;

void copy(WrongContainerData *dest, WrongContainerData *src, int count)
{
    for (int i = 0; i < count; i++)
    {
        dest[i] = src[i];
    }
}

void intercalar(WrongContainerData *inputContainers, WrongContainerData *outputContainers, int i, int m, int j)
{
    int i1 = i, i2 = m + 1, k = i;

    while (i1 <= m && i2 <= j)
    {
        if (inputContainers[i1].priority == inputContainers[i2].priority)
        {
            if (inputContainers[i1].priority == 0)
            {
                if (inputContainers[i1].percentage > inputContainers[i2].percentage ||
                    (inputContainers[i1].percentage == inputContainers[i2].percentage && inputContainers[i1].index < inputContainers[i2].index))
                {
                    outputContainers[k++] = inputContainers[i1++];
                }
                else
                {
                    outputContainers[k++] = inputContainers[i2++];
                }
            }
            else
            {
                if (inputContainers[i1].index < inputContainers[i2].index)
                {
                    outputContainers[k++] = inputContainers[i1++];
                }
                else
                {
                    outputContainers[k++] = inputContainers[i2++];
                }
            }
        }
        else if (inputContainers[i1].priority > inputContainers[i2].priority)
        {
            outputContainers[k++] = inputContainers[i1++];
        }
        else
        {
            outputContainers[k++] = inputContainers[i2++];
        }
    }

    while (i1 <= m)
    {
        outputContainers[k++] = inputContainers[i1++];
    }

    while (i2 <= j)
    {
        outputContainers[k++] = inputContainers[i2++];
    }

    copy(&inputContainers[i], &outputContainers[i], j - i + 1);
}

void print(WrongContainerData *containers, int index, FILE *output)
{
    for (int i = 0; i < index; i++)
    {
        if (containers[i].priority == 0)
        {
            fprintf(output, "%s:%d(%d%%)\n", containers[i].code, containers[i].weightDifference, containers[i].percentage);
        }
        else
        {
            fprintf(output, "%s:%s<->%s\n", containers[i].code, containers[i].cnpj, containers[i].wrongCnpj);
        }
    }
}

void mergesort(WrongContainerData *containers, WrongContainerData *containersAUX, int i, int j)
{
    if (i < j)
    {
        int m = i + (j - i) / 2;

        mergesort(containers, containersAUX, i, m);
        mergesort(containers, containersAUX, m + 1, j);

        intercalar(containers, containersAUX, i, m, j);
    }
}

void selectContainers(Container *sample, Container *allContainers, int numContainer, int numSample, FILE *output)
{
    WrongContainerData *wrongContainers = malloc(numSample * sizeof(WrongContainerData));
    int index = 0;

    for (int i = 0; i < numContainer; i++)
    {
        for (int j = 0; j < numSample; j++)
        {
            if (strcmp(allContainers[i].code, sample[j].code) == 0)
            {
                if (strcmp(allContainers[i].cnpj, sample[j].cnpj) != 0)
                {
                    strcpy(wrongContainers[index].code, allContainers[i].code);
                    strcpy(wrongContainers[index].cnpj, allContainers[i].cnpj);
                    strcpy(wrongContainers[index].wrongCnpj, sample[j].cnpj);
                    wrongContainers[index].priority = 1;
                    wrongContainers[index].index = allContainers[i].index;
                    index++;
                }
                else
                {
                    int difference = abs(sample[j].weight - allContainers[i].weight);
                    float percentage10 = ((float)allContainers[i].weight * 10) / 100;
                    float percentage = (difference * 10) / percentage10;
                    int per = (int)round(percentage);
                    if (per > 10)
                    {
                        strcpy(wrongContainers[index].code, allContainers[i].code);
                        wrongContainers[index].weightDifference = difference;
                        wrongContainers[index].percentage = per;
                        wrongContainers[index].priority = 0;
                        wrongContainers[index].index = allContainers[i].index;
                        index++;
                    }
                }
            }
        }
    }

    WrongContainerData *aux = malloc(index * sizeof(WrongContainerData));

    mergesort(wrongContainers, aux, 0, index - 1);

    print(wrongContainers, index, output);

    free(wrongContainers);
    free(aux);
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

        for (int i = 0; i < numContainers; i++)
        {
            fscanf(input, "%s %s %d", allContainers[i].code, allContainers[i].cnpj, &allContainers[i].weight);
            allContainers[i].index = i;
        }

        fscanf(input, "%d", &numSample);
        Container *sample = malloc(numSample * sizeof(Container));

        for (int i = 0; i < numSample; i++)
        {
            fscanf(input, "%s %s %d", sample[i].code, sample[i].cnpj, &sample[i].weight);
            sample[i].index = i;
        }

        selectContainers(sample, allContainers, numContainers, numSample, output);

        free(allContainers);
        free(sample);
    }

    fclose(input);
    fclose(output);
    return 0;
}
