#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

typedef struct Container
{
    char code[15];
    char cnpj[20];
    int weight;
} Container;

void initContainer(Container *containers, int numContainer)
{
    for (int i = 0; i < numContainer; i++)
    {
        *containers[i].cnpj = NULL;
        *containers[i].code = NULL;
        containers[i].weight = 0;
    }
}

int main(int argc, char *argv[])
{
    // FILE *input = fopen(argv[1], "r");
    // FILE *output = fopen(argv[2], "w");
    FILE *input = fopen("input.txt", "r");
    FILE *output = fopen("output.txt", "w");
    int numContainers = 0;

    fscanf(input, "%d", &numContainers);
    Container *containers = malloc(numContainers * sizeof(Container));
    initContainer(containers, numContainers);

    int index = 0;
    int counter = numContainers;

    while (counter > 0)
    {
        fscanf(input, "%s %s %d", containers[index].code, containers[index].cnpj, &containers[index].weight);
        printf("%s %s %d\n", containers[index].code, containers[index].cnpj, containers[index].weight);
        index++;
        counter--;
    }

    fclose(input);
    fclose(output);
    free(containers);
    return 0;
}