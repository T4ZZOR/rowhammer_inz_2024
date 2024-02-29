#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <unistd.h>

// define console text colors
#define BLACK "\x1b[30m"
#define RED "\x1b[31m"
#define GREEN "\x1b[32m"
#define YELLOW "\x1b[33m"
#define BLUE "\x1b[34m"
#define MAGENTA "\x1b[35m"
#define CYAN "\x1b[36m"
#define WHITE "\x1b[37m"
#define NORMAL "\x1b[m"

// define size of megabytes
#define MB1 (1024 * 1024 * 0.2)
#define MB256 (1024 * 1024 * 256)
#define MB512 (1024 * 1024 * 512)
#define MB768 (1024 * 1024 * 768)
#define MB896 (1024 * 1024 * 896)
#define GB1 (1024 * 1024 * 1024)

int generateRandomData(int * data, size_t size){
    srand(time(NULL));
    long int data_size = 0;

    for(size_t i = 0; i < size / sizeof(int); i++){
        data[i] = rand() % 1000;
        data_size++;
    }

    if (data_size != 0){
        printf("[allocate data by random memory, array size = %ld]\n", data_size);
        return data_size;
    }

    else {
        printf("[ERROR] cannot generate random data\n");
        return -1;
    }

    return 0;
}

int createCopy(int * data, size_t size, int **copy_data){
    *copy_data = (int *) malloc(size);
    
    if (copy_data == NULL){
        printf("[ERROR] cannot alocate [%zu bytes] memory to RAM\n", size);
        return 1;
    }

    memcpy(*copy_data, data, size);

    return 0;
}

int compareData(int * data, size_t size, int * copy_data){
    if (memcmp(data, copy_data, size) == 0){
        printf("\x1b[32m[ok]\x1b[0m \n");
        return 0;
    }

    else {
        printf("\x1b[31m[rowhammer]\x1b[0m \n");
        return 1;
    }
}

void clflush(volatile void *p) {
    asm volatile("clflush (%0)" :: "r"(p));
}

float calculatePercentage(int iterations, int actualIteration){
    float percentage = ((float)actualIteration * 10000) / iterations;

    return percentage;
}

void showInformations(int iterations, int type, float percentage, void* addres1, void* addres2, float attack_time){
    printf("iter. - %s%d%s, ", BLUE, iterations, NORMAL);
    printf("attack [%s] - %s%.2f%%%s, ", type == 0 ? "READ" : "WRITE", BLUE, percentage, NORMAL);
    printf("addres_1: %s%p%s | addres_2: %s%p%s -> ", YELLOW, (void*)addres1, NORMAL, YELLOW, (void*)addres2, NORMAL);
    printf("attack time = %s%0.1f%s ms ", YELLOW, attack_time, NORMAL);
}

int main(){
    // alocate memory
    size_t size = MB1;// size of data in RAM
    int *data = (int *) malloc(size);

    if (data == NULL){
        printf("[ERROR] cannot alocate [%zu bytes] memory to RAM", size);
        return 1;
    }

    // set random values in memory
    int data_elem_size = generateRandomData(data, size);

    // create copy data
    int *copy_data;

    clock_t start = clock();
    int copy = createCopy(data, size, &copy_data);
    clock_t end = clock();
    float seconds = (float)(end - start) / CLOCKS_PER_SEC;
    printf("[create copy data, time = %f]\n", seconds);

    // attack data and compare to detect bit-flips
    int flag = 0;
    int AI = 55;        // nanoseconds
    int RI = 64000;     // miliseconds
    long int iterations = (2L * RI * 190) / AI; // iterations = attack time

    int dataAttackIndex_1 = 0;
    int dataAttackIndex_2 = 2;
    int attack_mode = 0; // 0 - read // 1 - write
    int counter = 0;

    printf("\n[ATTACK MODE READ]\n\n");
    while(flag==0){
        // calculate in percentage attack
        float percentageProgress = calculatePercentage(data_elem_size, counter);

        // attack loop read
        if(attack_mode == 0){
            clock_t start = clock();
            for(long int i = 0; i < iterations; i++){
                int d1 = data[dataAttackIndex_1];
                int d2 = data[dataAttackIndex_2];

                clflush(&data[dataAttackIndex_1]);
                clflush(&data[dataAttackIndex_2]);
            }
            clock_t end = clock();
            double attack_time = ((double)(end - start)) / CLOCKS_PER_SEC * 1000.0;
            showInformations(counter, 0, percentageProgress, &data[dataAttackIndex_1], &data[dataAttackIndex_2], attack_time);
            if (compareData(data, size, copy_data)){
                break;
            }; 
        }
        
        // attack loop write
        else{
            // backup data to restore from attack indexes
            int data_backup_1 = data[dataAttackIndex_1];
            int data_backup_2 = data[dataAttackIndex_2];

            clock_t start = clock();
            for(long int i = 0; i < iterations; i++){
                data[dataAttackIndex_1] = 0xFFFFFFFF;
                data[dataAttackIndex_2] = 0xFFFFFFFF;

                clflush(&data[dataAttackIndex_1]);
                clflush(&data[dataAttackIndex_2]);
            }
            clock_t end = clock();
            double attack_time = ((double)(end - start)) / CLOCKS_PER_SEC * 1000.0;
            showInformations(counter, 1, percentageProgress, &data[dataAttackIndex_1], &data[dataAttackIndex_2], attack_time);

            data[dataAttackIndex_1] = data_backup_1;
            data[dataAttackIndex_2] = data_backup_2;

            if (compareData(data, size, copy_data)){
                break;
            };  
        }

        dataAttackIndex_1 += 100;
        dataAttackIndex_2 += 100;
        counter++;

        if(dataAttackIndex_1 >= data_elem_size || dataAttackIndex_2 >= data_elem_size){
            if(attack_mode == 1){
                flag = 1;
            }
            attack_mode = 1;

            printf("\n[ATTACK MODE WRITE]\n\n");

            dataAttackIndex_1 = 0;
            dataAttackIndex_2 = 2;
            counter = 0;        
        }
    }
    free(data);
    free(copy_data);

    return 0;
}