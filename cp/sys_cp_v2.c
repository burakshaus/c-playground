#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <sys/stat.h>
#include <stdlib.h>
#define BUFFER_SIZE 4096
int main(int argc, char *argv[]){
int inputFd, outputFd;
ssize_t numRead;
char buffer[BUFFER_SIZE];
struct stat sourceMetadata;
if (argc != 3){
	printf("Usage: %s <source> <destination>", argv[0]);
	exit(EXIT_FAILURE);
}

inputFd = open(argv[1],O_RDONLY);
if(inputFd == -1){
perror("Error: source file couldn't opened");
exit(EXIT_FAILURE);
}

if(fstat(inputFd,&sourceMetadata) == -1){
	perror("Error: file attributes couldn't read");
exit(EXIT_FAILURE);
}
outputFd = open(argv[2], O_WRONLY | O_CREAT | O_TRUNC, 0666);
if(outputFd == -1){
perror("Error: destination file couldn't be created");
exit(EXIT_FAILURE);
}
while((numRead = read(inputFd, buffer, BUFFER_SIZE)) > 0){
if(write(outputFd,buffer,numRead) != numRead){
perror("Error: writing error");
close(inputFd);
close(outputFd);
exit(EXIT_FAILURE);
}
}
#ifndef _WIN32
if(fchmod(outputFd, sourceMetadata.st_mode) == -1){
perror("Error: permissions couldn't edited");
#endif
}
close(inputFd);
close(outputFd);
printf("Copying and permissions transfer is code\n");
return 0;
}
