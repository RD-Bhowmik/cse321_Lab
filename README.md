# CSE321 Lab - Operating Systems Laboratory

A comprehensive collection of operating systems laboratory assignments, projects, and implementations covering fundamental OS concepts including process management, threading, synchronization, inter-process communication, and file system operations. This lab coursework demonstrates practical implementation of core operating system principles using C programming and Linux system calls.

## 🌟 Course Overview

CSE321 Operating Systems Laboratory provides hands-on experience with fundamental operating system concepts through practical programming assignments. The coursework covers process creation, thread management, synchronization mechanisms, inter-process communication, and file system operations using C programming and Linux system calls.

## 📚 Lab Assignments

### Lab Assignment 1: Basic C Programming and File Operations

- **Objective**: Introduction to C programming and file I/O operations
- **Skills**: File handling, string manipulation, basic C programming
- **Topics**: File reading/writing, user input processing, data validation
- **Files**: `task1.c`, `task2.c`, `task3.c`, `task4.c`, `task5.c`, `oddeven.c`, `sort.c`

### Lab Assignment 2: Process Creation and Management

- **Objective**: Understanding process creation using fork() system call
- **Skills**: Process management, fork() system call, process hierarchy
- **Topics**: Process creation, parent-child relationships, process termination
- **Files**: `task1.c`, `task2.c`

### Lab Assignment 3: Advanced Process Operations

- **Objective**: Advanced process management and system calls
- **Skills**: Process control, signal handling, process synchronization
- **Topics**: Process states, signal processing, process communication
- **Files**: `task1.c`, `task2.c`

### Lab Assignment 4: System Calls and Process Control

- **Objective**: Implementation of system calls and process control mechanisms
- **Skills**: System call implementation, process control, error handling
- **Topics**: System call interface, process control blocks, error handling
- **Files**: `task1.c`

### Lab 5: Threading and Synchronization

- **Objective**: Multi-threading and synchronization mechanisms
- **Skills**: Pthread programming, thread synchronization, mutual exclusion
- **Topics**: Thread creation, thread synchronization, mutex, semaphores
- **Files**: `p1.c`, `p2.c`, `p3.c`, `p4.c`, `p5.c`, `p6.c`

### Lab 6: Inter-Process Communication (IPC)

- **Objective**: Implementation of various IPC mechanisms
- **Skills**: Message passing, shared memory, pipes, semaphores
- **Topics**: Message queues, shared memory segments, pipe communication
- **Files**: `send1.c`, `rec1.c`, `send2.c`, `rec2.c`, `send3.c`, `rec3.c`

## 🛠️ Technical Skills Demonstrated

### Process Management

- **Process Creation**: fork() system call implementation
- **Process Control**: Process states and lifecycle management
- **Process Synchronization**: Parent-child process coordination
- **Signal Handling**: Signal processing and interrupt handling

### Threading and Concurrency

- **Thread Creation**: pthread_create() and thread management
- **Thread Synchronization**: Mutex locks and semaphores
- **Critical Sections**: Mutual exclusion and race condition prevention
- **Thread Communication**: Inter-thread data sharing

### Inter-Process Communication

- **Message Passing**: System V message queues
- **Shared Memory**: Shared memory segments and synchronization
- **Pipes**: Named and unnamed pipe communication
- **Semaphores**: Binary and counting semaphores

### File System Operations

- **File I/O**: File reading, writing, and manipulation
- **System Calls**: Low-level file operations
- **File System Analysis**: File system structure validation
- **Block Management**: Data block tracking and validation

## 📁 Project Structure

```
cse321_Lab/
├── Lab Assignments/
│   ├── 24141083_CSE321_Lab Assignment1/     # Basic C Programming
│   ├── 24141083_CSE321_Lab Assignment2/     # Process Creation
│   ├── 24141083_CSE321_Lab Assignment3/     # Advanced Process Operations
│   └── 24141083_CSE321_Lab Assignment4/     # System Calls
├── Lab 5_ Thread & Synchronization/
│   ├── 01_Threads/                          # Threading Implementation
│   └── 02_Synchronization/                  # Synchronization Mechanisms
├── Lab_6_Inter_Process_Communication/
│   ├── Message Passing/                     # Message Queue Implementation
│   ├── Shared_Memory/                       # Shared Memory Implementation
│   └── Pipes/                               # Pipe Communication
├── Term Projects/
│   ├── 21_Group 6_Term-Project 1/          # Shell Implementation
│   └── 21_Group6_Term-Project 2/           # File System Checker
├── Practice Files/
│   ├── fork/                                # Process Creation Practice
│   ├── thread_practice/                     # Threading Practice
│   └── semaphore_practice/                  # Synchronization Practice
└── Lab Documents/
    ├── Lab 1_Linux_Shell_Commands.pdf
    ├── Lab 2_Introducing_C_Programming_1-1.pdf
    ├── Lab 3_Introducing_C_Programming_2.pdf
    ├── Lab 4_System Calls, Process Creation.pdf
    └── Lab 5_ Thread & Synchronization/
```

## 🔧 Key Implementations

### Shell Implementation (Term Project 1)

- **Custom Shell**: Complete shell implementation with command parsing
- **Features**: Command execution, I/O redirection, piping, history
- **Advanced Features**: Background processes, signal handling, command chaining
- **File**: `shell.c`

### File System Checker (Term Project 2)

- **File System Analysis**: Comprehensive file system validation tool
- **Validation Checks**: Superblock, inode, data bitmap consistency
- **Error Detection**: Duplicate blocks, bad blocks, corruption detection
- **File**: `term2.c`

### Threading and Synchronization

- **Multi-threading**: Parallel processing with pthread library
- **Synchronization**: Mutex locks and semaphores for thread safety
- **Critical Sections**: Protected shared resource access
- **Race Condition Prevention**: Proper synchronization mechanisms

### Inter-Process Communication

- **Message Queues**: System V message passing implementation
- **Shared Memory**: Inter-process data sharing mechanisms
- **Pipes**: Process communication through pipes
- **Synchronization**: IPC synchronization and coordination

## 📊 Lab Assignment Details

### Lab 1: Basic C Programming

- **File Operations**: Reading and writing files
- **Data Processing**: Sorting, searching, and manipulation
- **User Input**: Interactive input processing
- **Error Handling**: Basic error checking and validation

### Lab 2: Process Creation

- **Fork System Call**: Process creation and management
- **Process Hierarchy**: Parent-child relationships
- **Process Termination**: Proper process cleanup
- **Process States**: Understanding process lifecycle

### Lab 3: Advanced Process Operations

- **System Calls**: Low-level system call implementation
- **Process Control**: Advanced process management
- **Signal Handling**: Signal processing and handling
- **Process Communication**: Basic IPC mechanisms

### Lab 4: System Calls and Control

- **System Call Interface**: Direct system call usage
- **Process Control Blocks**: Process state management
- **Error Handling**: Comprehensive error checking
- **Resource Management**: Process resource allocation

### Lab 5: Threading and Synchronization

- **Thread Creation**: pthread_create() implementation
- **Thread Synchronization**: Mutex and semaphore usage
- **Critical Sections**: Protected shared resources
- **Thread Communication**: Inter-thread data sharing

### Lab 6: Inter-Process Communication

- **Message Passing**: System V message queues
- **Shared Memory**: Inter-process data sharing
- **Pipes**: Process communication mechanisms
- **Synchronization**: IPC coordination and control

## 🎯Summary

### Project Title

**CSE321 Lab - Operating Systems Laboratory & System Programming**

### Project Description

Completed comprehensive operating systems laboratory coursework including 6 major lab assignments, 2 term projects, and extensive practice implementations covering process management, threading, synchronization, inter-process communication, and file system operations. Implemented a custom shell and file system checker using C programming and Linux system calls.

### Key Achievements

- **Shell Implementation**: Built complete custom shell with command parsing, I/O redirection, piping, and history
- **File System Checker**: Developed comprehensive file system validation tool with corruption detection
- **Process Management**: Implemented process creation, control, and synchronization mechanisms
- **Threading**: Created multi-threaded applications with proper synchronization
- **IPC Implementation**: Built various inter-process communication mechanisms
- **System Programming**: Demonstrated low-level system call usage and implementation
- **Error Handling**: Implemented robust error checking and validation systems

### Technical Skills Demonstrated

- **System Programming**: C programming, Linux system calls, low-level programming
- **Process Management**: fork(), exec(), wait(), process lifecycle management
- **Threading**: pthread library, thread creation, synchronization, mutual exclusion
- **Inter-Process Communication**: Message queues, shared memory, pipes, semaphores
- **File System Operations**: File I/O, system calls, file system analysis
- **Synchronization**: Mutex locks, semaphores, critical sections, race condition prevention
- **Memory Management**: Dynamic memory allocation, memory mapping, resource management
- **Error Handling**: System call error checking, validation, robust programming

### Technologies Used

**Programming**: C, Linux system calls, pthread library
**Tools**: GCC compiler, GDB debugger, Linux terminal
**Concepts**: Process management, threading, synchronization, IPC, file systems

## 📈 Learning Outcomes

### Theoretical Knowledge

- **Operating System Concepts**: Process management, memory management, file systems
- **Synchronization**: Mutual exclusion, deadlock prevention, race conditions
- **Inter-Process Communication**: Message passing, shared memory, synchronization
- **System Programming**: Low-level programming, system call interface

### Practical Skills

- **C Programming**: Advanced C programming with system calls
- **Linux Programming**: Linux-specific programming and system calls
- **Debugging**: System-level debugging and error handling
- **Performance Optimization**: Efficient system programming techniques

## 🔧 Code Examples

### Process Creation

```c
#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>

int main() {
    pid_t pid = fork();

    if (pid == 0) {
        // Child process
        printf("Child process: PID = %d\n", getpid());
    } else if (pid > 0) {
        // Parent process
        printf("Parent process: PID = %d\n", getpid());
        wait(NULL);
    } else {
        perror("fork failed");
        return 1;
    }
    return 0;
}
```

### Threading with Synchronization

```c
#include <pthread.h>
#include <semaphore.h>

pthread_mutex_t mutex;
sem_t semaphore;

void* thread_function(void* arg) {
    sem_wait(&semaphore);
    pthread_mutex_lock(&mutex);

    // Critical section
    printf("Thread executing critical section\n");

    pthread_mutex_unlock(&mutex);
    sem_post(&semaphore);
    return NULL;
}
```

### Message Passing

```c
#include <sys/msg.h>

struct msg_buffer {
    long msg_type;
    char msg_text[200];
};

int main() {
    int msg_id = msgget((key_t)1234, 0666 | IPC_CREAT);
    struct msg_buffer message;

    message.msg_type = 1;
    strcpy(message.msg_text, "Hello from sender");
    msgsnd(msg_id, &message, sizeof(message), 0);

    return 0;
}
```

## 📈 Future Applications

- [ ] Advanced operating system concepts
- [ ] Kernel programming and device drivers
- [ ] Distributed systems and networking
- [ ] Real-time systems programming
- [ ] System performance optimization
- [ ] Security and access control
- [ ] Virtualization and containerization
- [ ] Embedded systems programming

## 👨‍💻 Author

**RD-Bhowmik**

- Student ID: 24141083
- Course: CSE321 Operating Systems Laboratory
- Institution: BRAC University

---
