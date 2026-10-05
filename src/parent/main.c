#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>
#include <string.h>
#include <signal.h>
#include <errno.h>


int main(void) {

	char fileName[256];
	if (!fgets(fileName, sizeof(fileName), stdin)) {
		fprintf(stderr, "Error of read file name\n");
		return 1;
	}
	fileName[strcspn(fileName, "\n")] = '\0';

	int pipefd1[2], pipefd2[2];
	if (pipe(pipefd1) == -1) {
		perror("pipefd1");
		return 1;
	}
	if (pipe(pipefd2) == -1) {
		perror("pipefd2");
		return 1;
	}

	pid_t pid = fork();

	if (pid == -1) {
		perror("fork");
		return 1;
	}

	if (pid == 0) {
		close(pipefd1[1]);
		close(pipefd2[0]);
		if (dup2(pipefd1[0], 0) == -1) {
			perror("dup2 stdin");
			_exit(1);
		}
		if (pipefd1[0] != 0) {
			close(pipefd1[0]);
		}
		if (dup2(pipefd2[1], 1) == -1) {
			perror("dup2 stdout");
			_exit(1);
		}
		if (pipefd2[1] != 1) {
			close(pipefd2[1]);
		}
		char* argv[] = {"./child", fileName, NULL};
		execve("./child", argv, NULL);
		perror("execve");
		fflush(stderr);
		_exit(1);
	}

	signal(SIGPIPE, SIG_IGN);

	close(pipefd1[0]);
	close(pipefd2[1]);
	
	char cin[256];
	while (fgets(cin, sizeof(cin), stdin)) { 
		if (write(pipefd1[1], cin, strlen(cin)) == -1) {
			if (errno == EPIPE) break;
			perror("write to pipe1");
			break;
		}
	}

	close(pipefd1[1]);

	char buf[256];
	ssize_t n;
	while ((n = read(pipefd2[0], buf, sizeof(buf))) > 0) {
		fwrite(buf, 1, n, stdout);
	}
	close(pipefd2[0]);

	int status;
	if (waitpid(pid, &status, 0) == -1) {
		perror("waitpid");
		return 1;
	}

	if (WIFEXITED(status) && WEXITSTATUS(status) == 2) {
		return 2;
	}

	return 0;
}
