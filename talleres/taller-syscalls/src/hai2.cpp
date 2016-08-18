#include "hai2.h"

int contador = 0;
int main(int argc, char const *argv[]){
    
    int status;
    pid_t child;

    if(argc <= 1){
        fprintf(stderr, "Uso: %s commando [argumentos ...]\n", argv[0]);
        exit(-1);
    }

    child = fork();
    if(child == 0){
        signal(SIGURG, &handH);
    while(contador < 5){    
    }
    pid_t padre = getppid();
    kill(padre, SIGINT);
    execvp(argv[1], argv+1);
    exit(0);
    }

    signal(SIGINT, &handP);
    for (int i = 0; i < 5; ++i){
    printf("%s\n","sup!");
    kill(child,SIGURG);
    sleep(1);
    }
    
    return 0;
}

void handH(int numerito){
    printf("%s\n", "ya va!");
    contador++;
}

void handP(int numerito){
    int status;     //DUDA
    while(1) {
        if (wait(&status) < 0) { perror("waitpid"); break; }
        if (WIFEXITED(status)) break; /* Proceso terminado */
    }
    exit(0);
}