#include <sys/ptrace.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/reg.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <syscall.h>

int main(int argc, char* argv[]) {
	int status;
	pid_t child;

	if (argc <= 1) {
		fprintf(stderr, "Uso: %s commando [argumentos ...]\n", argv[0]);
		exit(1);
	}

	/* Fork en dos procesos */
	child = fork();
	if (child == -1) { perror("ERROR fork"); return 1; }
	if (child == 0) {
		if (ptrace(PTRACE_TRACEME, 0, NULL, NULL)) {
			perror("ERROR child ptrace(PTRACE TRACEME, ...)"); 
			exit(1);
		}
		/* Solo se ejecuta en el hijo */
		execvp(argv[1], argv+1);
		/* Si vuelve de exec() hubo un error */
		perror("ERROR child exec(...)"); exit(1);
	} else {
		/* Solo se ejecuta en el padre */
		while(1) {
			if (wait(&status) < 0) { perror("waitpid"); break; }

			if (WIFEXITED(status)) break; /* Proceso terminado */

			int sysno = ptrace(PTRACE_PEEKUSER, child, 8 * ORIG_RAX, NULL);
			//printf("%d\n",sysno);
			if(sysno == SYS_kill){
				ptrace(PTRACE_KILL, child, NULL, NULL);
				printf("%s\n","Se ha hecho justicia" );
				exit(0);
			}

			ptrace(PTRACE_SYSCALL, child, NULL, NULL); /* Continua */
		}
		ptrace(PTRACE_DETACH, child, NULL, NULL);
	}
	return 0;
}