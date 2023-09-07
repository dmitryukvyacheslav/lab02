#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define RAND_MAX INT_MAX

int* genRandArray(int size, int maxValue) {
    
    int* arrayptr = (int*) malloc((size+1) * sizeof(int));
    if (!arrayptr) return NULL;
    arrayptr[0] = size;
    for (int i = 1; i <= size; i++) 
        arrayptr[i] = rand() % (maxValue+1);
    return arrayptr;
}   

void print(int* arr) {
    printf("%d: ", arr[0]);
    for (int i = 1; i <= arr[0]; i++)
        printf("%d ", arr[i]);
}
      
int** genRandMatrix(int size, int maxValue) {
    int** arrayptr = (int**)malloc(size * sizeof(int*));
    if (!arrayptr) return NULL;
    for (int i = 0; i < size; i++)
        arrayptr[i] = genRandArray(rand() % 10, maxValue);
    return arrayptr;
}

void printMatrix(int** matrix, int size) {
    printf("%d\n", size);
    for (int i = 0; i < size; i++) {
        print(matrix[i]);
        putchar('\n');
    }
}

int main() {
    srand(time(NULL));
    int size = rand() % 10;
    int maxValue = 100;
    int* arr = genRandArray(size, maxValue);
    print(arr);
    // очистка выделенной памяти
    free(arr); 
    getchar();
    return 0;
}
