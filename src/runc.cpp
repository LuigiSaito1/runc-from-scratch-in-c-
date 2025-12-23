#include <iostream>
#include <unistd.h>
#include <sys/wait.h>
#include "uts_namespace.h"

int main() {
    std::cout << "=== runc - Runtime Container ===" << std::endl;
    
    char hostname_before[256];
    gethostname(hostname_before, sizeof(hostname_before));
    std::cout << "[HOST] Hostname antes: " << hostname_before << std::endl;
    
    pid_t pid = fork();
    
    if (pid == -1) {
        perror("fork failed");
        return 1;
    }
    
    if (pid == 0) {
        std::cout << "\n[CONTAINER] Iniciando container...\n" << std::endl;
        
        if (create_uts_namespace("container") == -1) {
            std::cerr << "[ERRO] Falha ao criar UTS namespace" << std::endl;
            return 1;
        }
        
        char hostname_inside[256];
        gethostname(hostname_inside, sizeof(hostname_inside));
        std::cout << "[CONTAINER] Hostname dentro do container: " << hostname_inside << std::endl;
        
        std::cout << "\n[CONTAINER] Iniciando shell... (digite 'exit' para sair)\n" << std::endl;
        execl("/bin/bash", "bash", NULL);
        
        perror("execl failed");
        return 1;
    } else {
        int status;
        waitpid(pid, &status, 0);
        
        char hostname_after[256];
        gethostname(hostname_after, sizeof(hostname_after));
        std::cout << "\n[HOST] Hostname depois: " << hostname_after << std::endl;
        std::cout << "[HOST] Hostname do host não foi afetado!" << std::endl;
    }
    
    return 0;
}