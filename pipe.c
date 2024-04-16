#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <errno.h>

int main(int argc, char *argv[])
{
	// TODO: it's all yours

	// Edge case - no arguments passed
	if(argc == 1){
		exit(EINVAL);
	}

	// printf("Number arguments: %d\n", argc);
	// for(int i = 0; i < argc; i++){
	// 	printf("argv[%d] = %s\n", i, argv[i]);
	// }

	// Multiple programs
	// For loop, fork every time, execlp
	// fork(program1)
	// fork(program2)

	for(int i = 1; i < argc; i++){
		pid_t pid = fork();
		if(pid == 0){
			if(execlp(argv[i], argv[i], NULL) == -1){
				exit(1);
			}
			exit(0);
		} else {
			int status;
			wait(&status);
			if(WIFEXITED(status) && WEXITSTATUS(status) != 0){
				exit(1);
			}
		}
	}
	return 0;
}
