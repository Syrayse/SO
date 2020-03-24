# Sistemas Operativos

Repositório a ser utilizado para o desenvolvimento do Trabalho Prático na âmbito da Unidade Curricular de Sistemas Operativos @ Universidade do Minho 2019/20.

## Estrutura de Ficheiros

De forma a permitir uma maior organização e agilização do processo de desenvolvido, separou-se os ficheiros numa estrutura adequada, posteriormente consumida pela `Makefile` presente na diretória `src/`.

### Ficheiros `.h`

Os contratos, denominados de *header files*, devem ser colocados na pasta `include/`.

### Ficheiros `.c`

Os ficheiros de código, referentes diretamente ao projecto a ser desenvolvido, devem ser colocados na pasta `src/`.

### Estruturas Auxiliares

Estruturas auxiliares utilizadas (i.e.: *Stacks*, *Queues*, ...), devem ser colocadas na pasta `src/lib/`.

### Compilação

Após compilado, o output `.o` é automaticamente colocado na pasta `src/out/`, ignorada pelo *git*.