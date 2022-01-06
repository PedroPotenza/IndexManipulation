#include "../header.h"

/*-------------------------------------- Utils --------------------------------------*/
FILE * fileOpenRead(char * filename) {
	FILE *file = fopen(filename, "rb");
	
	if(file == NULL) {
		printf("The file %s cannot be open.", filename);
		exit(1);
	}
	
	return file;	
}

FILE * readPositions() {
	char * filename = "position.bin";
	short int ZERO = 0;
	
	if(access(filename, F_OK ) == 0)
		return fopen(filename, "r+b");
		
	FILE * file = fopen(filename, "w+b");

	fwrite(&ZERO, sizeof(short int), 3, file);
	
	return file;
}

void increasePosition(VETOR_POSITION vetor) {

    char * filename = "position.bin";
	
	FILE * file = fopen(filename, "w+b");

    //fazer essa lógica ainda

	fwrite(&ZERO, sizeof(short int), 1, file);
	
	return file;

}

