#include <iostream>
#include <cuda_runtime.h>

__global__ void gpu_matMul(int *a, int *b, int *c, 
                        int ra, int ca, int rb, int cb) {
        // int a[ra][ca]
        // int b[rb][cb]
        // int c[ra][cb]

        int i = blockIdx.x * blockDim.x + threadIdx.x;
        int j = blockIdx.y * blockDim.y + threadIdx.y;
        for(int k = 0; k < ca; k++) {
            c[i * cb + j] += a[i * ca + k] * b[k * cb + j];
        }
}

int main() {
    int ra = 3, ca = 4, rb = 4, cb = 2; 
    int *aa = (int*) malloc(ra * ca * sizeof(int));
    int *bb = (int*) malloc(rb * cb * sizeof(int));
    int *cc = (int*) malloc(ra * cb * sizeof(int)); 

    for(int i=0;i<ra;i++) {
        for(int j=0;j<ca;j++) {
            aa[i * ca + j] = 1;
        }
    }

    for(int i=0;i<rb;i++) {
        for(int j=0;j<cb;j++) {
            bb[i * cb + j] = 2;
        }
    }

    for(int i=0;i<ra;i++) {
        for(int j=0;j<cb;j++) {
            cc[i * cb + j] = 0;
        }
    }

    int *ga, *gb, *gc;
    cudaMalloc(&ga, ra * ca * sizeof(int));
    cudaMalloc(&gb, rb * cb * sizeof(int));   
    cudaMalloc(&gc, ra * cb * sizeof(int));

    cudaMemcpy(ga, aa, ra*ca*sizeof(int), cudaMemcpyHostToDevice);
    cudaMemcpy(gb, bb, rb*cb*sizeof(int), cudaMemcpyHostToDevice);
    cudaMemcpy(gc, cc, ra*cb*sizeof(int), cudaMemcpyHostToDevice);

    dim3 blocksize(1);
    dim3 threadsize(ra, cb);

    std::cout << "mat mul using gpu\n";

    gpu_matMul<<<blocksize, threadsize>>>(ga, gb, gc, ra, ca, rb, cb);

    cudaMemcpy(cc, gc, ra*cb*sizeof(int), cudaMemcpyDeviceToHost);

    for(int i=0;i<ra;i++) {
        for(int j=0;j<cb;j++) {
            std::cout << cc[i * cb + j] << " ";
        }
        std::cout << std::endl;
    }
}
