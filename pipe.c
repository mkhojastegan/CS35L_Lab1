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

	if(argc == 2){
		pid_t pid = fork();
		if(pid == 0){
			if(execlp(argv[1], argv[1], NULL) == -1){
				exit(EINVAL);
			}
			exit(0);
		} else {
			int status;
			wait(&status);
			if(WIFEXITED(status) && WEXITSTATUS(status) != 0){
				// We want to return the status of WEXITSTATUS
				// in the final program
				exit(WEXITSTATUS(status));
			}
		}
	}

	// Two program edge case
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
			// Should be EINVAL?
            if(execlp(argv[1], argv[1], NULL) == -1){
                exit(EINVAL);
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
				// If pid1 or pid2 exit with failure
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
	if(argc == 3){
		exit(EXIT_SUCCESS);
	}

	if(argc > 3){
        int fd[2];
		// input
        int in = 0;
        pid_t pid;

        for(int i = 1; i < argc; i++){
            if(pipe(fd) == -1){
				exit(EXIT_FAILURE);
			}

            pid = fork();
			// Fork error
			if(pid == -1){
				exit(EXIT_FAILURE);
			}

            if(pid == 0){
				// Take in other command's pipe
                dup2(in, STDIN_FILENO);

                if(i != argc-1){
					// Everything except last command
                    dup2(fd[1], STDOUT_FILENO);
                }
                close(fd[0]);
                if(execlp(argv[i], argv[i], NULL) == -1){
					// Bogus program
                    exit(EINVAL);
                }
                exit(0);
            } else {
				// Wait for child, make output of current command the next input
                wait(NULL);
                close(fd[1]);
                in = fd[0];
            }
        }
        int status;
		// Last process
        waitpid(pid, &status, 0);
        if(WIFEXITED(status) && WEXITSTATUS(status) != 0){
            exit(WEXITSTATUS(status));
        }
    }

	return 0;
}
