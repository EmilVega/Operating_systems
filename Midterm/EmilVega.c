// Student: Emil Darío Vega Gualán

#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/wait.h>

pid_t crea_proceso(void)
{
    pid_t pid;
    do {
    	pid = fork();
    } while ((pid == -1) && (errno== EAGAIN));
    	return pid;
}
void proceso_hijo(char *arg[])
{
    
    //if (execv("/usr/bin/baobab", arg) == -1) {
    //   	perror("execlp");
    //	exit(EXIT_FAILURE);
    //}
    
     execlp("baobab","baobab",NULL,NULL);
}
void proceso_padre(pid_t pid)
{
    printf("\n¡Si este texto se imprime, han resuelto el examen de medio curso! ¡Felicidades!\n");
}
int main(int argc, char *argv[])
{
    char *arg[] = { "baobab", argv[1], NULL };
    pid_t pid = crea_proceso();
    switch (pid) {
    	case -1:
    		perror("fork");
    		return EXIT_FAILURE;
    		break;
   
    	case 0:
    		proceso_hijo(arg);
    		break;
    	default:
    		proceso_padre(pid);
    		break;
    }
    return EXIT_SUCCESS;
}
