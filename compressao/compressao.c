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

typedef struct Table
{
    char *tagNum;
    int tagHex;
} Table;

typedef struct Node
{
    int freq;
    int S;
    struct Node *R;
    struct Node *L;
} Node;

typedef struct Heap
{
    int size;
    int capacity;
    Node **nodes;
} Heap;

typedef struct CompressHUF
{
    float porcent;
    uint64_t compress;
    int count;
    int type;
} CompressHUF;

typedef struct CompressRLE
{
    int *V;
    float porcent;
    int index;
} CompressRLE;

int *get_freq(Data *data, int *freq)
{
    for (int i = 0; i < data->qtd; i++)
    {
        freq[data->S[i]] += 1;
    }
    return freq;
}

Heap *build_heap(Data *data)
{
    Heap *heap = malloc(sizeof(Heap));
    heap->capacity = data->qtd;
    heap->size = 0;
    heap->nodes = malloc(data->qtd * sizeof(Node *));
    return heap;
}

void heapify_up(Heap *heap, int index)
{
    if (index == 0)
        return;

    int parent = (index - 1) / 2;

    if (heap->nodes[parent]->freq > heap->nodes[index]->freq)
    {
        Node *temp = heap->nodes[parent];
        heap->nodes[parent] = heap->nodes[index];
        heap->nodes[index] = temp;
        heapify_up(heap, parent);
    }
}

void insert(Heap *heap, int histogram, int i, Node *L, Node *R)
{
    Node *newNode = malloc(sizeof(Node));
    newNode->freq = histogram;
    newNode->L = L;
    newNode->R = R;
    newNode->S = i;

    heap->nodes[heap->size] = newNode;
    heapify_up(heap, heap->size);
    heap->size++;
}

void heapify_down(Heap *heap, int index)
{
    int left = 2 * index + 1;
    int right = 2 * index + 2;
    int smallest = index;

    if (left < heap->size && heap->nodes[left]->freq < heap->nodes[smallest]->freq)
    {
        smallest = left;
    }

    if (right < heap->size && heap->nodes[right]->freq < heap->nodes[smallest]->freq)
    {
        smallest = right;
    }

    if (smallest != index)
    {
        Node *temp = heap->nodes[index];
        heap->nodes[index] = heap->nodes[smallest];
        heap->nodes[smallest] = temp;

        heapify_down(heap, smallest);
    }
}

Node *extract_min(Heap *heap)
{
    if (heap->size == 0)
        return NULL;

    Node *min = heap->nodes[0];
    heap->nodes[0] = heap->nodes[heap->size - 1];
    heap->size--;

    heapify_down(heap, 0);
    return min;
}

Node *build_tree(Data *data)
{
    int freq[MAX_HEX] = {0};
    int *histogram = get_freq(data, freq);

    Heap *heap = build_heap(data);

    for (int i = 0; i < MAX_HEX; i++)
    {
        if (histogram[i])
        {
            insert(heap, histogram[i], i, NULL, NULL);
        }
    }

    while (heap->size > 1)
    {
        Node *x = extract_min(heap);
        Node *y = extract_min(heap);
        insert(heap, x->freq + y->freq, -1, x, y);
    }

    return extract_min(heap);
}

void create_table(Node *head, char *way, int deep, Table *tableHex, int *rate)
{
    if (head == NULL)
        return;

    if (head->L == NULL && head->R == NULL)
    {
        way[deep] = '\0';
        tableHex[head->S].tagNum = strdup(way);
        tableHex[head->S].tagHex = head->S;

        if (*rate < deep)
        {
            *rate = deep;
        }
        return;
    }

    way[deep] = '0';
    create_table(head->L, way, deep + 1, tableHex, rate);

    way[deep] = '1';
    create_table(head->R, way, deep + 1, tableHex, rate);
}

void compact(Data *data, Table *table, int *rate, CompressHUF *compressHUF)
{
    if (!(*rate))
    {
        int qtd = data->qtd;
        int count = 0;
        while (qtd > 0)
        {
            count++;
            qtd -= 8;
        }

        float porcent = ((float)(count * 2) / (data->qtd * 2)) * 100;
        compressHUF->porcent = porcent;
        compressHUF->count = count;
        compressHUF->type = 0;
    }
    else
    {

        char *C = malloc((data->qtd * 8 + 1) * sizeof(char));
        C[0] = '\0';

        for (int i = 0; i < data->qtd; i++)
        {
            strcat(C, table[data->S[i]].tagNum);
        }

        while (strlen(C) % 8 != 0)
        {
            strcat(C, "0");
        }

        compressHUF->type = 1;
        float porcent = ((float)strlen(C) / 4 / (data->qtd * 2)) * 100;
        compressHUF->porcent = porcent;

        uint64_t num = strtoull(C, NULL, 2);
        compressHUF->compress = num;
        compressHUF->count = (int)(strlen(C) / 4);
        free(C);
    }
}

void free_tree(Node *node)
{
    if (node == NULL)
        return;
    free_tree(node->L);
    free_tree(node->R);
    free(node);
}

CompressHUF *HUF(Data *data, FILE *output)
{
    Node *root = build_tree(data);
    char way[MAX_HEX];
    Table *tableHex = calloc(MAX_HEX, sizeof(Table));
    int rate = 0;
    create_table(root, way, 0, tableHex, &rate);

    CompressHUF *compressHUF = malloc(sizeof(CompressHUF));
    compact(data, tableHex, &rate, compressHUF);

    for (int i = 0; i < MAX_HEX; i++)
    {
        if (tableHex[i].tagNum != NULL)
        {
            free(tableHex[i].tagNum);
        }
    }
    free(tableHex);
    free_tree(root);

    return compressHUF;
}

CompressRLE *RLE(Data *data)
{
    CompressRLE *compressRLE = malloc(sizeof(CompressRLE));
    compressRLE->V = malloc((data->qtd * 2) * sizeof(int));
    int count = 1, index = 0;

    for (int i = 0; i < data->qtd; i++)
    {
        int current = data->S[i];
        while (current == data->S[i + 1])
        {
            count += 1;
            i++;
        }

        compressRLE->V[index] = count;
        compressRLE->V[index + 1] = data->S[i];
        index += 2;
        count = 1;
    }

    float rate = (index * 2);
    float porcent = (rate / (data->qtd * 2));
    compressRLE->porcent = porcent * 100;
    compressRLE->index = index;

    return compressRLE;
}

void printHUF(CompressHUF *compressHUF, FILE *output)
{
    fprintf(output, "HUF(%.2f%%)=", compressHUF->porcent);
    if (compressHUF->type)
    {
        fprintf(output, "%0*lX\n", compressHUF->count, compressHUF->compress);
    }
    else
    {
        for (int i = 0; i < compressHUF->count; i++)
        {
            fprintf(output, "00");
        }
        fprintf(output, "\n");
    }
}

void printRLE(CompressRLE *compressRLE, FILE *output)
{
    fprintf(output, "RLE(%.2f%%)=", compressRLE->porcent);

    for (int i = 0; i < compressRLE->index; i += 2)
    {
        fprintf(output, "%02X", compressRLE->V[i]);
        fprintf(output, "%02X", compressRLE->V[i + 1]);
    }
    fprintf(output, "\n");
}

int main(int argc, char *argv[])
{
    // FILE *input = fopen("input.txt", "r");
    // FILE *output = fopen("output.txt", "w");
    FILE *input = fopen(argv[1], "r");
    FILE *output = fopen(argv[2], "w");

    if (input == NULL)
    {
        printf("Erro ao abrir input.txt\n");
        return 1;
    }

    int qtdSeq;
    fscanf(input, "%d", &qtdSeq);

    for (int i = 0; i < qtdSeq; i++)
    {

        Data currentData;
        fscanf(input, "%x", &currentData.qtd);

        currentData.S = malloc(currentData.qtd * sizeof(int));
        for (int j = 0; j < currentData.qtd; j++)
        {
            fscanf(input, "%x", &currentData.S[j]);
        }

        CompressHUF *compressHUF = HUF(&currentData, output);
        CompressRLE *compressRLE = RLE(&currentData);

        fprintf(output, "%d->", i);
        if (compressHUF->porcent < compressRLE->porcent)
        {

            printHUF(compressHUF, output);
        }
        else if (compressHUF->porcent > compressRLE->porcent)
        {
            printRLE(compressRLE, output);
        }
        else
        {
            printHUF(compressHUF, output);
            fprintf(output, "%d->", i);
            printRLE(compressRLE, output);
        }
        free(currentData.S);
        free(compressHUF);
        free(compressRLE);
    }

    fclose(input);
    fclose(output);
    return 0;
}
