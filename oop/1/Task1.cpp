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
        
int main() {
    srand(time(NULL));
    while (1) {

        int size = rand() % 10;
        int maxValue = 100;
        int* arr = genRandArray(size, maxValue);
        print(arr);
        // очистка выделенной памяти
        free(arr);

        int c = getchar();
        if (c == 120) // X key
            break;
    }
    return 0;
}
