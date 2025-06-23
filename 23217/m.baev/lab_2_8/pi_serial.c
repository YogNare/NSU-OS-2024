#include <stdio.h>
#include <stdlib.h>

#define num_steps 200000000
#define num_threads 4

int main(int argc, char** argv) {
    
    double pi = 0;

    double pi_list[num_threads];

    for (int j = 0; j < num_threads; j ++) {
        for (int i = num_steps/num_threads * j; i < num_steps/num_threads * (j + 1) ; i++) {
            
            pi_list[j] += 1.0/(i*4.0 + 1.0);
            pi_list[j] -= 1.0/(i*4.0 + 3.0);
        }
    }

    for (int i = 0; i < num_threads; i ++) pi += pi_list[i];
    
    pi = pi * 4.0;
    printf("pi done - %.15g \n", pi);    
    
    return (EXIT_SUCCESS);
}

