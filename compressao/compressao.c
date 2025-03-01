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

void create_table(Node *head, char *way, int deep, Table *tableHex)
{
    if (head == NULL)
        return;

    if (head->L == NULL && head->R == NULL)
    {
        if (!deep)
        {
            int bits = head->freq;
            int count = 0;
            while (bits > 0)
            {
                bits -= 8;
                count++;
            }
            int index = 0;
            while (count > 0)
            {
                way[index] = '00';
                index++;
                count--;
            }
            way[index] = '\0';
            tableHex[head->S].tagNum = strdup(way);
            tableHex[head->S].tagHex = head->S;

            return;
        }

        way[deep] = '\0';
        tableHex[head->S].tagNum = strdup(way);
        tableHex[head->S].tagHex = head->S;
        return;
    }

    way[deep] = '0';
    create_table(head->L, way, deep + 1, tableHex);

    way[deep] = '1';
    create_table(head->R, way, deep + 1, tableHex);
}

void compact(Data *data, Table *table)
{
    int total_size = 0;

    for (int i = 0; i < data->qtd; i++)
    {
        total_size += strlen(table[data->S[i]].tagNum);
    }

    char *C = malloc((total_size + 1) * sizeof(char));
    C[0] = '\0';

    for (int i = 0; i < data->qtd; i++)
    {
        strcat(C, table[data->S[i]].tagNum);
    }

    C[total_size] = '\0';
    // printf("HUF(%.2f%%)=", ((float)(rate * 2) / (data->qtd * 2)) * 100);
    int num = strtol(C, NULL, 2);
    printf("%X\n", num);
    free(C);
}

void free_tree(Node *node)
{
    if (node == NULL)
        return;
    free_tree(node->L);
    free_tree(node->R);
    free(node);
}

void HUF(Data *data)
{
    Node *root = build_tree(data);
    char way[MAX_HEX];
    Table *tableHex = calloc(MAX_HEX, sizeof(Table));
    way[0] = '\0';
    create_table(root, way, 0, tableHex);

    compact(data, tableHex);

    for (int i = 0; i < MAX_HEX; i++)
    {
        if (tableHex[i].tagNum != NULL)
        {
            free(tableHex[i].tagNum);
        }
    }
    free(tableHex);
    free_tree(root);
}

int main(int argc, char *argv[])
{
    FILE *input = fopen("input.txt", "r");
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

        HUF(&currentData);
        free(currentData.S);
    }

    fclose(input);
    return 0;
}
