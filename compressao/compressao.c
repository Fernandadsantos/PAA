#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>

typedef struct Data
{
    int qtd;
    int *data;
} Data;

// typedef struct Node
// {
//     uint32_t F;
//     int S;
//     Node *R;
//     Node *L;
// } Node;

typedef struct Queue
{
    Data *Q;
} Queue;

// void insert(Queue *queue, int h, int i, Node *R, Node *L)
// {
// }

// int size_queue(Queue *queue)
// {
//     // retorna o tamnho da fila
//     return -1;
// }

// Queue *create_min_queue()
// {
//     // cria e retorna a fila de prioridade minima(ordem crescente de repetições)
// }

// Node *built_tree(uint32_t H[], uint32_t n)
// {
//     Queue *queue = create_min_queue();

//     for (int i = 0; i < n; i++)
//     {
//         // insere os símbolos na fila
//         if (H[i])
//             insert(queue, H[i], i, NULL, NULL);

//         // combina os nós com menor frequência
//         while (size_queue(queue) > 1)
//         {
//             Node *x = extract_min(queue);
//             Node *y = extract_min(queue);
//             insert(queue, x->F + y->F, "\0", x, y);
//         }
//     }

//     // retorna a raiz
//     return extract_min(queue);
// }

void RLE(Data *data)
{
    int V[data->qtd * 2];
    int count = 1, index = 0;

    for (int i = 0; i < data->qtd; i++)
    {
        int current = data->data[i];
        while (current == data->data[i + 1])
        {
            count += 1;
            i++;
        }

        V[index] = count;
        V[index + 1] = data->data[i];
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

            currentData.data = malloc(currentData.qtd * sizeof(int));
            for (int j = 0; j < currentData.qtd; j++)
            {
                fscanf(input, "%x ", &currentData.data[j]);
            }

            RLE(&currentData);
        }
    }

    fclose(input);
    fclose(output);
    return 0;
}