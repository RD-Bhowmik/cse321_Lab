#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>

#define TOTAL_STUDENT 10
#define MAX_CHAIR 3

int wait_count = 0;  
int serve_count = 0;   
int student_count = 0;    

sem_t st_sem;              
sem_t student_sem;        
pthread_mutex_t mutex; 

void* stud_thread(void* arg);
void* st_thread(void* arg);

int main() {
    sem_init(&st_sem, 1, 0);        
    sem_init(&student_sem, 1, 0);   
    pthread_mutex_init(&mutex, NULL);
    
    pthread_t st;
    pthread_t students[TOTAL_STUDENT];
    int student_ids[TOTAL_STUDENT];
    
    pthread_create(&st, NULL, st_thread, NULL);
    
    int i = 0;
    while (i < TOTAL_STUDENT) {
        student_ids[i] = i;
        pthread_create(&students[i], NULL, stud_thread, &student_ids[i]);
        sleep(1);
        i++;
    }
    
    i = 0;
    while (i < TOTAL_STUDENT) {
        pthread_join(students[i], NULL);
        i++;
    }
    
    sem_post(&st_sem);
    
    pthread_join(st, NULL);
    
    sem_destroy(&st_sem);
    sem_destroy(&student_sem);
    pthread_mutex_destroy(&mutex);
    
    return 0;
}

void* stud_thread(void* arg) {
    int id = *((int*)arg);
    
    pthread_mutex_lock(&mutex);
    
    if (wait_count < MAX_CHAIR) {
        wait_count++;
        printf("Student %d started waiting for consultation\n", id);
        
        if (wait_count == 1) {
            sem_post(&st_sem);  
        }
        
        pthread_mutex_unlock(&mutex);
        sem_wait(&student_sem);
        
        printf("Student %d is getting consultation\n", id);
        
        sleep(1);  
        
        printf("Student %d finished getting consultation and left\n", id);
        pthread_mutex_lock(&mutex);

        serve_count++;
        student_count++;
        printf("Number of served students: %d\n", serve_count);
        pthread_mutex_unlock(&mutex);
    } 
    else {
        printf("No chairs remaining in lobby. Student %d Leaving.....\n", id);
        student_count++;
        pthread_mutex_unlock(&mutex);
    }
    
    pthread_exit(NULL);
}

void* st_thread(void* arg) {
    while (student_count < TOTAL_STUDENT) {
        sem_wait(&st_sem);
        
        pthread_mutex_lock(&mutex);
        if (student_count >= TOTAL_STUDENT && wait_count == 0) {
            pthread_mutex_unlock(&mutex);
            break;
        }
        
        if (wait_count > 0) {
            wait_count--;
            printf("A waiting student started getting consultation\n");
            printf("Number of students now waiting: %d\n", wait_count);
            printf("ST giving consultation\n");
            
            sem_post(&student_sem);
        }
        
        pthread_mutex_unlock(&mutex);
        
        pthread_mutex_lock(&mutex);
        if (wait_count > 0) {
            sem_post(&st_sem);  
        }
        pthread_mutex_unlock(&mutex);
    }
    
    pthread_exit(NULL);
}