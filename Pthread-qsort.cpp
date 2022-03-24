//include the io library
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <sys/time.h>
#include <iostream>
#include <cstdlib>
using namespace std;

//define the variables
#define RANGE_LOW 1
#define RANGE_HIGH 1000000
#define THREAD_NUM 4
#define NUM 7500
#define PER_THREAD NUM / THREAD_NUM
#define OFFSET NUM % THREAD_NUM

//declare functions created
void quickSort(int[], int, int);
int generate_random_number(unsigned int, unsigned int);
void* threadAssign (void*);
void merge();

//declare arrays
int num[NUM];
int tmp_num[NUM];
int arr[NUM];

//main function
int main() {
    float time_use=0;
    struct timeval start;
    struct timeval end;

    //generate random integers
    for (int i = 0; i < NUM; i++) {
        arr[i] = generate_random_number(RANGE_LOW, RANGE_HIGH);
    }
    
    pthread_t threads[THREAD_NUM];

    //start recording time
    gettimeofday(&start,NULL);
    
    //create threads
    for (long i = 0; i < THREAD_NUM; i++) {
        int rc = pthread_create(&threads[i], NULL, threadAssign, (void *)i);
        if (rc){
            printf("ERROR; return code from pthread_create() is %d\n", rc);
            exit(-1);
        }
    }

    //join threads
    for(long i = 0; i < THREAD_NUM; i++) {
        pthread_join(threads[i], NULL);
    }
    
    //merge the sorted arry sections
    merge();

    //stop recording time and print
    gettimeofday(&end,NULL);
    time_use=(end.tv_sec-start.tv_sec)*1000000+(end.tv_usec-start.tv_usec);//微秒
    printf("%d threads, time_use is %.10f microseconds\n",THREAD_NUM, time_use);
    
    //check if the array is sorted correctly
    for (int i = 0; i < NUM - 2; i++) {
        if (tmp_num[i] > tmp_num[i + 1]) {
            cout << "wrong at " << tmp_num[i] << " and " << tmp_num[i + 1] << endl;
            break;
        }
    }
    cout << "done\n";
    return 0;
}

//function to generate random integer
int generate_random_number(unsigned int lower_limit, unsigned int upper_limit) {
    return rand() % RANGE_HIGH + 1;
}


//function used to pass task to threads
void* threadAssign (void* arg) {
    int tid = (long)arg;
    int start = tid * PER_THREAD;
    int end = (tid + 1) * PER_THREAD - 1;
    if (tid == THREAD_NUM - 1) {
        end += OFFSET;
    }
    //call quick sort to implement sort in each thread
    quickSort(arr, start, end);
    pthread_exit(NULL);
}

//function to merge the sorted array sections from each thread
void merge () {
    //record start index of array from each thread
    long index[THREAD_NUM];
    for (int i = 0; i < THREAD_NUM; ++i) {
        index[i] = i * PER_THREAD;
    }

    //compare multiple numbers to pick the smallest a time
    for (long i = 0; i < NUM; ++i) {
        long minInd, minNum = RANGE_HIGH;
        for (int j = 0; j < THREAD_NUM; ++j) {
            if ((index[j] < (j + 1) * PER_THREAD) && (arr[index[j]] < minNum)) {
                minInd = j;
                minNum = arr[index[j]];
            }
        }
        tmp_num[i] = arr[index[minInd]];
        index[minInd]++;
    }
}



//function to sort the student array by grade, called in the the task functions above
//implement quick sort recursively
void quickSort(int arr[], int start, int end) {
    //return when start index meets end index
    if (start >= end) {
        return;
    }
    //inite 2 integers left and right, equal to start and end respectively
    int left = start;
    int right = end;
    //inite a pivot, which is the grade at middle index
    int pivot = arr[(start + end) / 2];

    //while loop before left index meets right index
    while (left <= right) {
        //if the grade at left index is greater than pivot,
        //do not need to do anything, move the left index to right
        while (left <= right && arr[left] < pivot) {
            left++;
        } 
        //if the grade at right index is samller than pivot,
        //do not need to do anything, move the right index to left
        while (left <= right && arr[right] > pivot) {
            right--;
        }
        //at this point, the grades at left index and right index
        //need to be moved to correct positions
        if (left <= right) {
            //the left index grade is smaller than pivot 
            //the right index grade is greater than pivot
            //these 2 elements are swapped 
            
            int temp = arr[left];
            //swap both the name and grade in the structs 
            arr[left] = arr[right];
           
            arr[right] = temp;
            

            //after swapping a pair of elements,
            //continue move left to right, right to left
            left++;
            right--;
        }

    }

    //call quickSort to recursively sort the left part of the array
    //and the right part of the array
    quickSort(arr, start, right);
    quickSort(arr, left, end);
}

