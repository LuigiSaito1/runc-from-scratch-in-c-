# 🐳 runc-from-scratch-in-c++

Um runtime de containers minimalista implementado do zero em C++, explorando os conceitos fundamentais de isolamento do Linux (namespaces, cgroups, etc).

## 📚 Sobre o Projeto

Este projeto é uma implementação educacional de um runtime de containers, similar ao `runc` (usado pelo Docker), mas construído do zero para entender como containers funcionam internamente.

### 🎯 Objetivos

- Entender como containers funcionam "por baixo dos panos"
- Implementar isolamento usando Linux namespaces
- Aprender sobre syscalls como `clone()`, `unshare()`, `mount()`, etc.
- Criar um container runtime funcional sem dependências externas

## 🏗️ Arquitetura

```
runc-from-scratch-in-c-/
├── include/              # Headers
│   ├── uts_namespace.h   # UTS namespace (hostname)
│   └── pid_namespace.h   # PID namespace (processos)
├── src/                  # Implementações
│   ├── runc.cpp          # Programa principal
│   ├── uts_namespace.cpp # Isolamento de hostname
│   └── pid_namespace.cpp # Isolamento de PIDs
├── build/                # Arquivos compilados (ignorado)
├── rootfs/               # Sistema de arquivos do container (ignorado)
└── CMakeLists.txt        # Configuração de build
```

## ✨ Features Implementadas

### ✅ UTS Namespace (Hostname Isolation)
- Isola o hostname do container
- Cada container pode ter seu próprio hostname
- Implementado com `unshare(CLONE_NEWUTS)`

### ✅ PID Namespace (Process Isolation)
- Isola a árvore de processos
- O primeiro processo no container vê PID 1
- Processos do container não veem processos do host
- Implementado com `clone(CLONE_NEWPID)`

### 🚧 Em Desenvolvimento
- [ ] Mount Namespace (filesystem isolation)
- [ ] Network Namespace (network isolation)
- [ ] User Namespace (user/group isolation)
- [ ] Cgroups (resource limits)
- [ ] Capabilities (security)

## 🚀 Como Usar

### Pré-requisitos

- Linux (kernel 3.8+)
- GCC/G++ com suporte a C++11
- CMake 3.10+
- Privilégios de root (para criar namespaces)

### Compilação

```bash
# Criar diretório de build
mkdir -p build
cd build

# Configurar e compilar
cmake ..
make

# Voltar para o diretório raiz
cd ..
```

### Execução

```bash
# Executar o container (requer sudo)
sudo ./build/runc
```

### Testando dentro do Container

Quando o shell do container iniciar, você pode testar:

```bash
# Ver o PID do shell (deve ser 1 ou 2)
echo $$

# Ver o hostname (deve ser "container")
hostname

# Tentar listar processos (ainda mostra processos do host - normal!)
ps aux

# Sair do container
exit
```

## 🧪 Exemplos de Uso

### Exemplo 1: Verificar Isolamento de Hostname

```bash
# Terminal 1 - Dentro do container
sudo ./build/runc
hostname  # Mostra: container

# Terminal 2 - No host
hostname  # Mostra: seu-hostname-original
```

### Exemplo 2: Verificar Isolamento de PID

```bash
# Dentro do container
echo $$        # Mostra: 1 (ou 2 se for o bash)
echo $PPID     # Mostra: 0 ou 1
```

## 🔬 Conceitos Técnicos

### Linux Namespaces

Namespaces são uma feature do kernel Linux que permite isolar recursos do sistema. Cada namespace fornece uma visão isolada de um recurso específico:

| Namespace | Isola | Syscall Flag |
|-----------|-------|--------------|
| **UTS** | Hostname e domain name | `CLONE_NEWUTS` |
| **PID** | Process IDs | `CLONE_NEWPID` |
| **Mount** | Filesystem mount points | `CLONE_NEWNS` |
| **Network** | Network interfaces, IPs | `CLONE_NEWNET` |
| **IPC** | Inter-process communication | `CLONE_NEWIPC` |
| **User** | User and group IDs | `CLONE_NEWUSER` |
| **Cgroup** | Cgroup hierarchy | `CLONE_NEWCGROUP` |

### Syscalls Utilizadas

- **`clone()`**: Cria um novo processo com namespaces isolados
- **`unshare()`**: Move o processo atual para um novo namespace
- **`sethostname()`**: Define o hostname dentro do namespace UTS
- **`execl()`**: Substitui o processo atual por outro programa
- **`waitpid()`**: Espera um processo filho terminar

## 📖 Recursos de Aprendizado

### Documentação Oficial
- [Linux Namespaces - man page](https://man7.org/linux/man-pages/man7/namespaces.7.html)
- [clone() - man page](https://man7.org/linux/man-pages/man2/clone.2.html)
- [unshare() - man page](https://man7.org/linux/man-pages/man2/unshare.2.html)

### Artigos e Tutoriais
- [Linux Containers in 500 Lines of Code](https://blog.lizzie.io/linux-containers-in-500-loc.html)
- [How Docker Works - Under the Hood](https://www.docker.com/blog/how-docker-works/)

## 🛠️ Desenvolvimento

### Estrutura do Código

#### `runc.cpp` - Programa Principal
```cpp
int main() {
    // Cria o PID namespace e inicia o container
    create_pid_namespace();
}
```

#### `pid_namespace.cpp` - Isolamento de Processos
```cpp
// Função executada dentro do novo namespace
int container_main(void* arg) {
    // Cria outros namespaces (UTS, etc)
    create_uts_namespace("container");
    
    // Executa shell
    execl("/bin/bash", "bash", NULL);
}

// Cria o namespace usando clone()
int create_pid_namespace() {
    clone(container_main, stack_top, CLONE_NEWPID | SIGCHLD, NULL);
}
```

#### `uts_namespace.cpp` - Isolamento de Hostname
```cpp
int create_uts_namespace(const char* hostname) {
    unshare(CLONE_NEWUTS);
    sethostname(hostname, strlen(hostname));
}
```

### Adicionando Novos Namespaces

1. Criar header em `include/nome_namespace.h`
2. Implementar em `src/nome_namespace.cpp`
3. Adicionar ao `CMakeLists.txt`
4. Chamar a função em `container_main()`

## ⚠️ Limitações Conhecidas

- **`ps aux` mostra processos do host**: Normal! Precisa implementar Mount Namespace e montar `/proc` isolado
- **Requer root**: Necessário para criar namespaces (pode ser resolvido com User Namespace)
- **Sem limite de recursos**: Precisa implementar cgroups
- **Sem isolamento de rede**: Precisa implementar Network Namespace

## 🤝 Contribuindo

Este é um projeto educacional! Contribuições são bem-vindas:

1. Fork o projeto
2. Crie uma branch para sua feature (`git checkout -b feature/nova-feature`)
3. Commit suas mudanças (`git commit -m 'Add: nova feature'`)
4. Push para a branch (`git push origin feature/nova-feature`)
5. Abra um Pull Request

## 📝 Roadmap

- [x] UTS Namespace (hostname)
- [x] PID Namespace (processos)
- [ ] Mount Namespace (filesystem)
- [ ] Network Namespace (rede)
- [ ] User Namespace (usuários)
- [ ] Cgroups (limites de recursos)
- [ ] Capabilities (segurança)
- [ ] Overlay filesystem
- [ ] Container images

## 📄 Licença

Este projeto está sob a licença MIT. Veja o arquivo [LICENSE](LICENSE) para mais detalhes.
