#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>  
#include <time.h>
#define MAX_HEX 256

typedef struct Data
{
    int qtd;
    int *S;
} Data;

typedef struct Node
{
    int freq;
    int S;
    struct Node *R;
    struct Node *L;
} Node;
  
typedef struct CompressHUF
{
    float porcent;
    char * compress;
    int count;
    int type; 
} CompressHUF;

typedef struct CompressRLE
{
    int *V;
    float porcent;
    int index;
} CompressRLE;

int size = 0;

void get_freq(Data *data, int *freq)
{
    for (int i = 0; i < data->qtd; i++)
    {
        freq[data->S[i]]++;
    }
}
  
void heapfy(Node **heap, int i) {

  int parent = (i - 1)/2;

  if (i > 0 && heap[parent]->freq  > heap[i]->freq ) {
    Node *temp = heap[parent];
    heap[parent] = heap[i];
    heap[i] = temp;
    heapfy(heap, parent);
  }
}

void heapfy_min(Node **heap, int i) {

      int L = 2 * i + 1;
      int R = 2 * i + 2;
      int min = i;

      if (L < size && heap[L]->freq < heap[min]->freq) {
        min = L;
      }

      if (R < size && heap[R]->freq < heap[min]->freq) {
        min = R;
      }

      if (min != i) {
        Node *temp = heap[i];
        heap[i] = heap[min];
        heap[min] = temp;

        heapfy_min(heap, min);
      }
}
 

void insert(Node **heap, int freq, int symbol, Node *R, Node *L){ 
    Node *new_node = (Node *)malloc(sizeof(Node));
    new_node->freq = freq;
    new_node->S = symbol;
    new_node->R = R;
    new_node->L = L; 
    heap[size] = new_node;
    
    heapfy(heap, size );
    size++;
}

Node *extract_min(Node **heap){
    if(size == 0)
        return NULL;

    Node *min = heap[0];
    heap[0] = heap[size - 1];
    size--;

    heapfy_min(heap, 0);
    return min;
}

Node * build_heap(int *freq){
    Node **heap = (Node **)malloc(MAX_HEX * sizeof(Node*)); 
      
    for(int i = 0; i < MAX_HEX ; i++){
        if(freq[i]){
            insert(heap, freq[i], i, NULL, NULL);
        }

    } 
 
    while(size > 1){
        Node * x = extract_min(heap);
        Node * y = extract_min(heap); 
        insert(heap, x->freq + y->freq, '\0', x, y); 
    }

    return extract_min(heap);
}

void insert_table(Node *node, char **table, char* way, int deep){

    if(!node) return;

    if(!node->L && !node->R){
        way[deep] = '\0';
        table[node->S] = (char *)malloc((deep + 1) * sizeof(char));
        
        for (int i = 0; i < deep; i++) {
            table[node->S][i] = way[i];  
        }

        table[node->S][deep] = '\0';

        return;
    }

    way[deep] = '1';
    insert_table(node->L, table, way, deep + 1);

    way[deep] = '0';
    insert_table(node->R, table, way, deep + 1);
}

 
 
void compact(Data *data, char ** table, char **C ){ 
    *C = malloc((data->qtd * 8 + 1) * sizeof(char));
 
    int index = 0;

    for (int i = 0; i < data->qtd; i++) {

        char *code = table[data->S[i]];
      
        while (*code) {
            (*C)[index++] = *code++;  
        }
    }

    (*C)[index] = '\0';

    while (strlen(*C) % 8 != 0) {
        // (*C)[index++] = '0';
        strcat(*C, "0");
    }
    // (*C)[index] = '\0';

}


void printHUF(CompressHUF *compressHUF, FILE *output, int index) {
    fprintf(output, "%d->HUF(%.2f%%)=", index,compressHUF->porcent);
    
    int binary = strlen(compressHUF->compress);
    int numBytes = (binary + 7) / 8;
    unsigned char *bytes = malloc(numBytes * sizeof(unsigned char)); 
 
    for (int i = 0; i < binary; i++) { 
        bytes[ i/8] = (bytes[i/8] << 1 | (compressHUF->compress[i] - '0'));
    }

    char hexBuffer[numBytes * 2 + 1];
    char *hexPtr = hexBuffer;
  
    static const char hexDigits[] = "0123456789ABCDEF";
    for (int i = 0; i < numBytes; i++) {
        *hexPtr++ = hexDigits[(bytes[i] >> 4) & 0xF]; 
        *hexPtr++ = hexDigits[bytes[i] & 0xF];        
    }
    *hexPtr = '\0';
 
    fprintf(output, "%s\n", hexBuffer);
     
    free(bytes);
}

CompressRLE *RLE(Data *data) {
    CompressRLE *compressRLE = malloc(sizeof(CompressRLE));
    compressRLE->V = malloc((data->qtd * 2) * sizeof(int));
    compressRLE->index = 0;

    int count = 1;
    for (int i = 0; i < data->qtd; i++) {
        
        if(i + 1 < data->qtd && data->S[i] == data->S[i + 1]) {
            count++; 
        }
        else{ 
            compressRLE->V[compressRLE->index++] = count;
            compressRLE->V[compressRLE->index++] = data->S[i];
            count = 1;
        }
    }

    compressRLE->porcent = ((compressRLE->index * 2.0) / (data->qtd * 2)) * 100;
    return compressRLE;
}
 

void printRLE(CompressRLE *compressRLE, FILE *output, int index)
{

    fprintf(output, "%d->RLE(%.2f%%)=", index, compressRLE->porcent);

    for (int i = 0; i < compressRLE->index; i+=2) {
        fprintf(output, "%02X%02X", compressRLE->V[i], compressRLE->V[i+1]); 
    }

  fprintf(output, "\n");
  
}

CompressHUF * HUF(Data *data ){

    int freq[MAX_HEX] = {0};
    get_freq(data, freq);

    Node *queue = build_heap(freq);

    char **table = (char **)malloc(MAX_HEX * sizeof(char*));
    char way[MAX_HEX];
    insert_table(queue, table, way, 0);
 
    CompressHUF *compressHUF = malloc(sizeof(CompressHUF));
    compact(data, table, &compressHUF->compress );   
    compressHUF->porcent = (((float)strlen(compressHUF->compress)/4) /(data->qtd * 2)) * 100;
   
    free(table);  
    return compressHUF;
} 

void load_input(FILE *input, FILE *output)
{ 
    int qtd_str;
    fscanf(input, "%d", &qtd_str);
    Data currentData; 
    CompressHUF *compressHUF = NULL;
    CompressRLE *compressRLE = NULL;
    for (int i = 0; i < qtd_str; i++)
    { 
        fscanf(input, "%d", &currentData.qtd);

        currentData.S = (int *)malloc(currentData.qtd * sizeof(int));
        for (int j = 0; j < currentData.qtd; j++)
        {
            fscanf(input, "%x", &currentData.S[j]);
        }

        compressHUF = HUF(&currentData);
        compressRLE = RLE(&currentData); 

        if (compressHUF->porcent < compressRLE->porcent)
        { 
            printHUF(compressHUF, output, i);
        }
        else if (compressHUF->porcent > compressRLE->porcent)
        {
            printRLE(compressRLE, output, i);
        }
        else
        {
            printHUF(compressHUF, output, i); 
            printRLE(compressRLE, output, i);
        }
    }
    free(currentData.S); 
    free(compressRLE);
    free(compressHUF);
}


int main(int argc, char *argv[])
{  
    //clock_t start = clock();
    //FILE *input = fopen("input.txt", "r");
    //FILE *output = fopen("output.txt", "w");
    FILE *input = fopen(argv[1], "r");
    FILE *output = fopen(argv[2], "w"); 

    if (input != NULL)
    {   
        load_input(input, output); 
        fclose(input);
        fclose(output);
    }
    //clock_t end = clock();
    //double elapsed_time = ((double)(end - start)) / CLOCKS_PER_SEC;

    //printf("Tempo de execução: %.3f segundos\n", elapsed_time);  
    return 0;
}

