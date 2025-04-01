#include <iostream>
#include <cuda_runtime.h>

__global__ void matAdd(int* a, int* b, int* c) {
    int idx = threadIdx.x;
    *(c + idx) = *(a + idx) + *(b + idx);
}
int main() {
    std::cout << "hello world from CPU\n";

    int *ca = (int*) malloc(100 * sizeof(int));
    int *cb = (int*) malloc(100 * sizeof(int));
    int *cc = (int*) malloc(100 * sizeof(int));

    for(int i = 0; i < 100; i++) {
        *(ca + i) = 1;
        *(cb + i) = 1;
    }

    int* ga;
    int* gb;
    int* gc;
    cudaMalloc(&ga, 100 * sizeof(int));
    cudaMalloc(&gb, 100 * sizeof(int));
    cudaMalloc(&gc, 100 * sizeof(int));

    cudaMemcpy(ga, ca, 100 * sizeof(int), cudaMemcpyHostToDevice);
    cudaMemcpy(gb, cb, 100 * sizeof(int), cudaMemcpyHostToDevice);

    dim3 blocksize(1);
    dim3 threadsize(100);
    matAdd<<<blocksize, threadsize>>>(ga, gb,gc);

    cudaMemcpy(cc, gc, 100 * sizeof(int), cudaMemcpyDeviceToHost);

    for(int i = 0; i < 100; i++) {
        std::cout << *(cc + i) << std::endl;
    }

}
