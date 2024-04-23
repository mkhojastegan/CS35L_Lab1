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


	if(argc == 3){
        int fd[2];
		// Pipe error
        if(pipe(fd) == -1){
			exit(EXIT_FAILURE);
		}
        pid_t pid1 = fork();
		// Fork error
		if(pid1 == -1){
			exit(EXIT_FAILURE);
		}
        if(pid1 == 0){
			// dup2 Error, if that's even possible
            if(dup2(fd[1], STDOUT_FILENO) == -1 ){
				exit(EXIT_FAILURE);
			}
            close(fd[0]);
            close(fd[1]);
			// Error opening first program
            if(execlp(argv[1], argv[1], NULL) == -1){
                exit(2);
            }
            exit(0);
        } else {
            pid_t pid2 = fork();
			if(pid2 == -1){
				exit(EXIT_FAILURE);
			}
            if(pid2 == 0){
                if(dup2(fd[0], STDIN_FILENO) == -1){
					exit(EXIT_FAILURE);
				}
                close(fd[0]);
                close(fd[1]);
                if(execlp(argv[2], argv[2], NULL) == -1){
                    exit(2);
                }
                exit(0);
            } else {
                close(fd[0]);
                close(fd[1]);
				int status;
                wait(&status);
                if(WIFEXITED(status) && WEXITSTATUS(status) != 0){
					exit(WEXITSTATUS(status));
				}
				wait(&status);
				if(WIFEXITED(status) && WEXITSTATUS(status) != 0){
					exit(WEXITSTATUS(status));
				}
            }
        }
	}

	// for(int i = 1; i < argc; i++){
	// 	pid_t pid = fork();
	// 	if(pid == 0){
	// 		if(execlp(argv[i], argv[i], NULL) == -1){
	// 			// TODO: Check for bogus programs inserted
	// 			// Exits from the child with 2
	// 			exit(2);
	// 		}
	// 		exit(0);
	// 	} else {
	// 		int status;
	// 		wait(&status);
	// 		if(WIFEXITED(status) && WEXITSTATUS(status) != 0){
	// 			// We want to return the status of WEXITSTATUS
	// 			// in the final program
	// 			exit(WEXITSTATUS(status));
	// 		}
	// 	}
	// }
	return 0;
}
