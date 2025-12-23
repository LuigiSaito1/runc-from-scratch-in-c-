#include <iostream>
#include <unistd.h>
#include "pid_namespace.h"

int main() {
    std::cout << "=== runc - Runtime Container ===" << std::endl;
    
    char hostname_before[256];
    gethostname(hostname_before, sizeof(hostname_before));

    if (create_pid_namespace() == -1) {
        std::cerr << "[ERRO] Falha ao criar PID namespace" << std::endl;
        return 1;
    }
    
    char hostname_after[256];
    gethostname(hostname_after, sizeof(hostname_after));

    
    return 0;
}