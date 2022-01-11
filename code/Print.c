int PrintRegister(KEY fileData)
{
    FILE* indexFile;
    KEY* readKey;
    
    if(access("indexPrimaryResult.bin", F_OK ) == 0 ) {
	    indexFile = fopen("indexPrimaryResult.bin", "r+b");
	} else {
	    indexFile = fopen("indexPrimaryResult.bin", "w+b");
	}

    fseek(indexFile, 0, SEEK_SET);

    while(fread(readKey, sizeof(KEY), 1, indexFile)) {
        if(readKey->ClientId == fileData.ClientId && readKey->MovieId == fileData.MovieId) {
            printf("Achou");
            return 1;
        } else {
            fseek(indexFile, sizeof(int), SEEK_CUR);
        }
    }

    fclose(indexFile);
    return 0;
}