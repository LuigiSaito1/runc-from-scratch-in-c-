#define _GNU_SOURCE
#include "pid_namespace.h"
#include "uts_namespace.h"
#include <sched.h>
#include <unistd.h>
#include <sys/wait.h>
#include <iostream>
#include <cstring>
#include <cerrno>

#define STACK_SIZE (1024 * 1024)

int container_main(void* arg) {
    std::cout << "[CONTAINER] Iniciando no novo PID namespace..." << std::endl;

    if (create_uts_namespace("container") == -1) {
        std::cerr << "[ERRO] Falha ao criar UTS namespace" << std::endl;
        return 1;
    }
    
    char hostname[256];
    gethostname(hostname, sizeof(hostname));
    std::cout << "[CONTAINER] Hostname: " << hostname << std::endl;
    
    std::cout << "\n[CONTAINER] Iniciando shell... (digite 'exit' para sair)\n" << std::endl;
    std::cout << "[CONTAINER] Tente rodar: echo $$, ps aux, hostname\n" << std::endl;
    
    execl("/bin/bash", "bash", NULL);
    
    std::cerr << "execl failed: " << strerror(errno) << std::endl;
    return 1;
}

int create_pid_namespace() {
    std::cout << "\n[PID] Criando novo PID namespace..." << std::endl;
    
    char* stack = new char[STACK_SIZE];
    if (!stack) {
        std::cerr << "[ERRO] Falha ao alocar stack" << std::endl;
        return -1;
    }
    
    char* stack_top = stack + STACK_SIZE;
    
    pid_t pid = clone(container_main,
                      stack_top,
                      CLONE_NEWPID | SIGCHLD,
                      NULL);
    
    if (pid == -1) {
        std::cerr << "clone failed: " << strerror(errno) << std::endl;
        delete[] stack;
        return -1;
    }
    
    std::cout << "[HOST] Processo filho criado com PID: " << pid << std::endl;
    
    int status;
    waitpid(pid, &status, 0);
    
    delete[] stack;
    
    std::cout << "\n[HOST] Container finalizado" << std::endl;
    
    return 0;
}