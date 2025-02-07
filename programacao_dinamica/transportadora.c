#include <stdio.h>
#include <stdlib.h>

typedef struct Package
{
    char code[15];
    int volume;
    int weight;
    float value;
} Package;

typedef struct Vehicle
{
    char plate[8];
    int weight;
    int volume;
    float totalValue;
    int totalVolume;
    int totalWeight;
    Package *packages;
} Vehicle;

typedef struct Carrier
{
    Vehicle *vehicles;
    Package *packages;
    int numV;
    int numP;
} Carrier;

int currentNumP = 0, currentCapacity = 0;

void readInput(Carrier *carrier, FILE *input)
{
    fscanf(input, "%d", &carrier->numV);
    carrier->vehicles = malloc(carrier->numV * sizeof(Vehicle));
    for (int i = 0; i < carrier->numV; i++)
    {
        fscanf(input, "%s %d %d", carrier->vehicles[i].plate, &carrier->vehicles[i].weight, &carrier->vehicles[i].volume);
    }

    fscanf(input, "%d", &carrier->numP);
    carrier->packages = malloc(carrier->numP * sizeof(Package));

    for (int i = 0; i < carrier->numP; i++)
    {
        fscanf(input, "%s %f %d %d", carrier->packages[i].code, &carrier->packages[i].value, &carrier->packages[i].weight, &carrier->packages[i].volume);
    }
}

int maxValue(float value1, float value2)
{
    if (value1 > value2 || value1 == value2)
    {
        return value1;
    }

    return value2;
}

void initTable(float table[currentNumP][currentCapacity])
{
    for (int i = 0; i < currentNumP + 1; i++)
    {
        for (int j = 0; j < currentCapacity + 1; j++)
        {
            table[i][j] = 0;
        }
    }
}

void distributePackages(Vehicle *vehicles, Package *packages, int numP)
{
    currentNumP = numP;
    currentCapacity = vehicles->weight;
    int currentVolume = 0;
    float table[currentNumP][currentCapacity];
    initTable(table);

    for (int i = 1; i < numP; i++)
    {
        for (int j = 1; j < currentCapacity; j++)
        {
            if (packages[i].volume <= vehicles->volume && packages[i].weight <= vehicles->weight)
            {
                float currentvalue = maxValue(table[i - 1][j], table[i - 1][j - packages[i].weight] + packages[i].value);
            }
            else
            {
                table[i][j] = table[i - 1][j];
            }
        }
    }
}

int main(int argc, char *argv[])
{

    FILE *input = fopen("input.txt", "r");
    FILE *output = fopen("output.txt", "w");

    if (input != NULL)
    {

        Carrier carrier;

        readInput(&carrier, input);

        for (int i = 0; i < carrier.numV; i++)
        {
            distributePackages(&carrier.vehicles[i], &carrier.packages, carrier.numP);
        }

        fclose(input);
        fclose(output);
    }

    return 0;
}
