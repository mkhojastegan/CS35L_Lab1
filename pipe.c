#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <errno.h>

int main(int argc, char *argv[])
{
	// TODO: it's all yours
	if(argc == 1){
		errno = EINVAL;
		//ferror();
		exit(EXIT_FAILURE);
	}

	printf("Number arguments: %d\n", argc);
	for(int i = 0; i < argc; i++){
		printf("argv[%d] = %s\n", i, argv[i]);
	}

	// Multiple programs
	// For loop, fork every time, execlp
	// fork(program1)
	// fork(program2)
	return 0;
}
