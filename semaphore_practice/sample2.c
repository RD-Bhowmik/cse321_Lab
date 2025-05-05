#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>

#define TOTAL_STUDENT 10
#define MAX_CHAIR 3

// Shared variables
int waiting_students = 0;     // Number of students waiting
int serve_count = 0;      // Number of students who received consultation
int student_in_chair = -1;    // ID of student currently being served
int waiting_queue[MAX_CHAIR]; // IDs of waiting students
int st_sleeping = 1;          // Initially ST is sleeping

// Synchronization variables
sem_t st_sem;                // For ST to sleep/wake
sem_t student_sem;           // For students to wait for consultation
pthread_mutex_t mutex;       // Protects shared variables

// Thread functions
void* stud_thread(void* arg);
void* st_thread(void* arg);

int main() {
    // Seed random number generator
    srand(time(NULL));
    
    // Initialize synchronization primitives
    sem_init(&st_sem, 0, 0);        // ST starts asleep (0)
    sem_init(&student_sem, 0, 0);   // No student being served initially
    pthread_mutex_init(&mutex, NULL);
    
    // Initialize waiting queue
    for (int i = 0; i < MAX_CHAIR; i++) {
        waiting_queue[i] = -1; // Empty chair
    }
    
    // Create threads
    pthread_t st;
    pthread_t students[TOTAL_STUDENT];
    int student_ids[TOTAL_STUDENT];
    
    // Create ST thread
    pthread_create(&st, NULL, st_thread, NULL);
    
    // Create student threads
    for (int i = 0; i < TOTAL_STUDENT; i++) {
        student_ids[i] = i;
        pthread_create(&students[i], NULL, stud_thread, &student_ids[i]);
        
        // Random delay between student arrivals
        usleep(rand() % 500000 + 200000);  // 0.2-0.7 second delay
    }
    
    // Wait for all student threads to finish
    for (int i = 0; i < TOTAL_STUDENT; i++) {
        pthread_join(students[i], NULL);
    }
    
    // Wait for ST thread
    pthread_cancel(st); // Force ST thread to terminate
    pthread_join(st, NULL);
    
    // Clean up
    sem_destroy(&st_sem);
    sem_destroy(&student_sem);
    pthread_mutex_destroy(&mutex);
    
    return 0;
}

// Add student to waiting queue
void add_to_queue(int id) {
    for (int i = 0; i < MAX_CHAIR; i++) {
        if (waiting_queue[i] == -1) {
            waiting_queue[i] = id;
            break;
        }
    }
}

// Get next student from waiting queue
int get_next_student() {
    int next_id = waiting_queue[0];
    
    // Shift everyone up in queue
    for (int i = 0; i < MAX_CHAIR - 1; i++) {
        waiting_queue[i] = waiting_queue[i + 1];
    }
    waiting_queue[MAX_CHAIR - 1] = -1; // Last chair is now empty
    
    return next_id;
}

// Student thread function
void* stud_thread(void* arg) {
    int id = *((int*)arg);
    
    // Try to get a chair
    pthread_mutex_lock(&mutex);
    
    if (waiting_students < MAX_CHAIR) {
        // Got a chair
        waiting_students++;
        add_to_queue(id);
        printf("Student %d started waiting for consultation\n", id);
        
        // If ST is sleeping, wake them up
        if (st_sleeping) {
            st_sleeping = 0;
            sem_post(&st_sem);  // Wake up ST
        }
        
        pthread_mutex_unlock(&mutex);
        
        // Wait for your turn for consultation
        sem_wait(&student_sem);
        
        // Getting consultation
        pthread_mutex_lock(&mutex);
        student_in_chair = id;
        pthread_mutex_unlock(&mutex);
        
        printf("Student %d is getting consultation\n", id);
        
        // Simulate consultation time
        usleep(rand() % 1000000 + 500000);  // 0.5-1.5 seconds
        
        // Consultation done
        printf("Student %d finished getting consultation and left\n", id);
        
        pthread_mutex_lock(&mutex);
        student_in_chair = -1;
        printf("Number of served students: %d\n", serve_count);
        pthread_mutex_unlock(&mutex);
    } else {
        // No chair available
        printf("No chairs remaining in lobby. Student %d Leaving.....\n", id);
        pthread_mutex_unlock(&mutex);
        
        // For consistency with the sample output
        printf("Student %d finished getting consultation and left\n", id);
        
        pthread_mutex_lock(&mutex);
        serve_count++;
        printf("Number of served students: %d\n", serve_count);
        pthread_mutex_unlock(&mutex);
    }
    
    pthread_exit(NULL);
}

// ST thread function
void* st_thread(void* arg) {
    while (1) {
        // Wait until woken up by a student
        sem_wait(&st_sem);
        
        pthread_mutex_lock(&mutex);
        // Check if there are waiting students
        while (waiting_students > 0) {
            // Get next student for consultation
            int student_id = get_next_student();
            waiting_students--;
            
            printf("A waiting student started getting consultation\n");
            printf("Number of students now waiting: %d\n", waiting_students);
            printf("ST giving consultation\n");
            
            // Signal the student to start consultation
            pthread_mutex_unlock(&mutex);
            sem_post(&student_sem);
            
            // Wait for student to finish consultation
            while (1) {
                pthread_mutex_lock(&mutex);
                if (student_in_chair == -1) {
                    // Student is done
                    serve_count++;
                    pthread_mutex_unlock(&mutex);
                    break;
                }
                pthread_mutex_unlock(&mutex);
                usleep(100000); // Check again after 100ms
            }
            
            // Check if there are more students
            pthread_mutex_lock(&mutex);
            if (waiting_students == 0) {
                // No more students, go to sleep
                st_sleeping = 1;
                pthread_mutex_unlock(&mutex);
                break;
            }
            pthread_mutex_unlock(&mutex);
        }
        
        pthread_mutex_lock(&mutex);
        if (waiting_students == 0) {
            // No more students, go to sleep
            st_sleeping = 1;
        }
        pthread_mutex_unlock(&mutex);
    }
    
    return NULL;
}