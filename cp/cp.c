#include <stdio.h>
#include <stdlib.h>
#define BUFFER_SIZE 4096
int main(int argc, char *argv[]){
	FILE *sourceile;
	FILE *destFile;
	unsigned char buffer[BUFFER_SIZE];
	size_t bytesRead;

	if(argc != 3){
		printf("Usage: %s <source_file> <destination_file>\n", argv[0]);
		return 1;
	}
	FILE *sourceFile = fopen(argv[1],"rb");
	if(sourceFile == NULL){
		perror("Error opening source file");
		return 1;
	}

	destFile = fopen(argv[2],"wb");
	


	while((bytesRead = fread(buffer,1,BUFFER_SIZE, sourceFile)) > 0){
		fwrite(buffer,1,bytesRead,destFile);
	}
	printf("File copied successfully. \n");
	fclose(sourceFile);
	fclose(destFile);
	return 0;
	
}
