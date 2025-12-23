#define _GNU_SOURCE
#include "uts_namespace.h"
#include <sched.h>
#include <unistd.h>
#include <string.h>
#include <iostream>
#include <cerrno>

int create_uts_namespace(const char* hostname) {
    if (unshare(CLONE_NEWUTS) == -1) {
        std::cerr << "unshare(CLONE_NEWUTS) failed: " << strerror(errno) << std::endl;
        return -1;
    }

    std::cout << "[UTS] Novo UTS namespace criado\n";

    if (sethostname(hostname, strlen(hostname)) == -1) {
        std::cerr << "sethostname failed: " << strerror(errno) << std::endl;
        return -1;
    }

    std::cout << "[UTS] Hostname definido para: " << hostname << "\n";
    
    return 0;
}