#include <iostream>
#define N 4

void print_stripe(int* stripe){
    for (int i = 0; i<N*N; i++)
        printf("%d ", stripe[i]);
    printf("\n");
}

void print_mtx(int** mtx) {
    for (int i=0; i<N; i++){
        for(int k=0; k<N; k++)
            printf("%3d ", mtx[i][k]);
        printf("\n");
    }
    printf("\n");
}

int main(void){
    srand(time(0));
    int** mtx = new int*[N];
    for (int i = 0; i < N; i++) {
        mtx[i] = new int[N];
        for (int k = 0; k < N; k++) 
            mtx[i][k] = rand() % 1000;
    }
    int stripe[N*N] = {0};
    int m = 0;
    // rc
    // 00 01 02 03     03; 02 - 13; 01-12-23; 00-11-22-33   
    // 10 11 12 13      
    // 20 21 22 23
    // 30 31 32 33

    // Fill by UL-LR (primary) diagonal
    for (int col = N-1; col >= 0; col--) 
        for(int row = 0; row < N-col; row++){
            stripe[m] = mtx[row][col+row];
            m++;
        }
   
    for (int row = 1; row < N; row++) 
        for(int col = 0; col < N-row; col++){
            stripe[m] = mtx[row+col][col];
            m++;
        }
    print_mtx(mtx);
    printf("\nprimary: ");
    print_stripe(stripe);

    m = 0;
    for (int col = 0; col < N; col++)
        for(int row = 0; row <= col; row++) {
            stripe[m] = mtx[row][col-row];
            m++;
        }
   
    for (int row = 1; row < N; row++)
        for (int col = N-1; col >= row; col--) { // 4 3 2 1    5-
            stripe[m] = mtx[row+(N-col-1)][col];
            m++;
        }
    printf("\nsecondary: ");
    print_stripe(stripe);

    int row1 = 0;
    int col1 = 0;
    int row2 = N-1;
    int col2 = N-1;
    m = 0;
    while(m < (N*N)) {
        for(int i=col1;i<=col2;i++) { 
            stripe[m] = mtx[row1][i];
            m++;
        }
        for(int j=row1+1;j<=row2;j++) {
            stripe[m] = mtx[j][col2];
            m++;
        }
        for(int i=col2-1;i>=col1;i--) {
            stripe[m] = mtx[row2][i];
            m++;
        }
        for(int j=row2-1;j>=row1+1;j--) {
            stripe[m] = mtx[j][col1];
            m++;
        }
        row1++;
        row2--;
        col1++;
        col2--;
    }
    printf("\ninward: ");
    print_stripe(stripe);

    
    // outward
    // 00 01 
    // 10 11 
    int row = N/2;
    int col = N/2;

    if (N%2==0) {
        row--;
        col--;
    }

    m = 0;
    
    
    if(N%2!=0) {
        int distance = 2;
        stripe[0] = mtx[row][col];
        m++;
        col++;
        while (m < N*N ) {
        for (int i = 0; i < distance; i++) {
            stripe[m++] = mtx[row][col];
            row++;
        }
        row--;
        col--;
        for (int i = 0; i < distance; i++) {
            stripe[m++] = mtx[row][col];
            col--;
        }
        col++;
        row--;
        for (int i = 0; i < distance; i++) {
            stripe[m++] = mtx[row][col];
            row--;
        }
        row++;
        col++;
        for (int i = 0; i < distance; i++) {
            stripe[m++] = mtx[row][col];
            col++;
        }
        distance+=2;
        }
    }
    else {
        int distance = 1;
        while (m < N*N) { 
            for(int i = 0; i < distance; i++) {
                stripe[m++] = mtx[row][col];
                row--;
            }
            row++;
            col++;
            for(int i = 0; i < distance; i++) {
                stripe[m++] = mtx[row][col];
                col++;
            }
            col--;
            row++;

            for(int i = 0; i < distance; i++) {
                stripe[m++] = mtx[row][col];
                row++;
            }   
            row--;
            col--;
            for(int i = 0; i < distance; i++) {
                stripe[m++] = mtx[row][col];
                col--;
            }
            distance+=2;
        }
    } 
    printf("\noutward: ");
    print_stripe(stripe);
    return 0;
}