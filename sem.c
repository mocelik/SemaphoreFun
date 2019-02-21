#include <stdio.h>
#include <stddef.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <semaphore.h>

void run();
sem_t *sem;

int main(int argc, char** argv){
	sem = sem_open("/semaphoreTesting", O_CREAT, 0644,1);	
	run();
	sem_close(sem);
	sem_unlink("/semaphoreTesting");
}

void run(){
	char output = '-';
	pid_t pid = fork();
	
	if (pid==0){ // child process
		output = 'X';
		for (int i=0; i<5; i++){
			
			useResourceSynchronized(output, sem);
			sleep(2);
		}
		printf("\n");
		exit(EXIT_SUCCESS);
		
	} else if (pid>0){ // parent process
		output = 'O';
		for (int i=0; i<5; i++){
			sem_wait(sem);
			useResource(output);
			sem_post(sem);
			sleep(2);
		}
		printf("\n");
		return;
		
	} else { // error, couldn't fork
		perror("\tChild process could not be forked. Exiting...\n");
		exit(EXIT_FAILURE);
	}

	return;
}

void useResource(char ch, sem_t *sem){
	
	sem_wait(sem); // wait turn
	
	// critical section beginning
	sleep(1);
	printf("%c",ch); 
	fflush(stdout); // flush to print
	// critical section end
	
	sem_post(sem);
}
