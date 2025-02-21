#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>

#define MAX_HEX 256

typedef struct Data
{
    int qtd;
    int *S;
} Data;

typedef struct Node
{
    int freq;
    Data data;
    int S;
    Node *R;
    Node *L;
} Node;

typedef struct Heap
{
    int size;
    int capacity;
    Node *nodes;
} Heap;

int *get_freq(Data *data)
{
    int freq[MAX_HEX] = {0};

    // Conta a frequência de cada byte (caractere)
    for (int i = 0; i < data->qtd; i++)
    {
        freq[data->S[i]] += 1;
    }

    return freq;
}

Heap *build_heap(int capacity)
{
    Heap *heap = malloc(sizeof(heap));

    heap->capacity = capacity;
    heap->size = 0;
    heap->nodes = malloc(capacity * sizeof(Node));

    return heap;
}

void insert(Heap *heap, int freq, int i, Node *L, Node *R)
{
    Node newNode;
    newNode.freq = freq;
    newNode.L = L;
    newNode.R = R;
    newNode.S = i;

    heap->nodes[heap->size++] = newNode;
}

Node *extract_min(Heap *heap)
{
    Node min = heap->nodes[0];
    heap->nodes[0] = heap->nodes[heap->size - 1];
    heap->size--;

    return &min;
}

Node *build_tree(Data *data)
{
    int *freq = get_freq(data);

    Heap *heap = build_heap(data->qtd);

    for (int i = 0; i < MAX_HEX; i++)
    {
        if (freq[i])
            insert(heap, freq[i], i, NULL, NULL);
    }

    while (heap->size > 1)
    {
        Node *x = extract_min(heap);
        Node *y = extract_min(heap);
        insert(heap, x->freq + y->freq, -1, x, y);
    }

    return extract_min(heap);
}

void compact(Data *data)
{
    int C[data->qtd];

    for (int i = 0; i < data->qtd; i++)
    {
    }
}

void HUF(Data *data)
{
    Node *min = build_tree(data);
    float rate = (min->S * 2);

    printf("HUF(%d%%)=%d", (rate / (data->qtd * 2)) * 100, min->S);
}

void RLE(Data *data)
{
    int V[data->qtd * 2];
    int count = 1, index = 0;

    for (int i = 0; i < data->qtd; i++)
    {
        int current = data->S[i];
        while (current == data->S[i + 1])
        {
            count += 1;
            i++;
        }

        V[index] = count;
        V[index + 1] = data->S[i];
        index += 2;
        count = 1;
    }

    float rate = (index * 2);

    printf("RLE(%.2f%%)=", (rate / (data->qtd * 2)) * 100);

    for (int i = 0; i < index; i += 2)
    {
        printf("%02X", V[i]);
        printf("%02X", V[i + 1]);
    }
    printf("\n");
}

int main(int argc, char *argv[])
{
    FILE *input = fopen("input.txt", "r");
    FILE *output = fopen("output.txt", "w");
    // FILE *input = fopen(argv[1], "r");
    // FILE *output = fopen(argv[2], "w");
    if (input != NULL)
    {
        int qtdSeq = 0;
        fscanf(input, "%d", &qtdSeq);

        for (int i = 0; i < qtdSeq; i++)
        {
            printf("%d->", i);
            Data currentData;
            fscanf(input, "%d", &currentData.qtd);

            currentData.S = malloc(currentData.qtd * sizeof(int));
            for (int j = 0; j < currentData.qtd; j++)
            {
                fscanf(input, "%x ", &currentData.S[j]);
            }

            // RLE(&currentData);
        }
    }

    fclose(input);
    fclose(output);
    return 0;
}