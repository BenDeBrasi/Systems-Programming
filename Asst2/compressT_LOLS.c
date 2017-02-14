#include<stdio.h>
#include<string.h>
#include<pthread.h>
#include<unistd.h>
#include<stdlib.h>
#include<ctype.h>

//Prototyping compressT_LOLS defined by the project and compress for multithread
void *compress(void *arg);
void *compressT_LOLS(char *fileName, int parts);

//data struct which will be used by each thread for compression
typedef struct data{
    int numberOfCharsToRead;
    int threadID;
    int startingPoint;
    int amtOfThreads;
	char *fileName;
	char *extension;
	char *file;
}data;

//Made global to make appending the file name easier. 
int numberOfCharsInFile = 0;

//main function which inputs filename, and number of threads, then passes the two values into compressLOLS
int main(int argc, char **argv){
   
	
    //If the user did not enter two arguments call an error and end the program
    if(argc != 3){
        printf("Error: There are not 2 arguments.\n");
        return -1;
    }

    //Atoi failure error check
    int parts;
    if((parts = atoi(argv[2])) == 0){
        printf("Error: User didn't enter a number for parts or it was 0.\n");
        return -1;
     }

	//Calling function which will check for errors and create multithreads
    compressT_LOLS(argv[1],parts);
    
	return 0;
}

void *compressT_LOLS(char *fileName, int parts){
 	
	//Checks if the number of threads user wants is equal to or less than 0.
	if(parts <= 0){
        printf("Error: Parts <= 0.\n");
        return NULL;
    }
   
    //Check file has read access & exists in current directory
    if( (access(fileName, F_OK) && access(fileName, R_OK)) != -1){
        //File exists
		printf("Past the file access and readibility of the uncompressed file\n");
    }
    else{
        //File doesn't exist
        printf("Error: File doesn't exist or you don't have read access.\n");
        return NULL;
    } 
    
	//opening the file
	FILE *fp = fopen(fileName, "r");
	
	char c;

    /* for as long as we can get characters... */
	while((c=fgetc(fp)) != EOF) {
		numberOfCharsInFile++;
	}
	
	//-1 to account for the incrementation that will occur due to reaching the EOF value
	numberOfCharsInFile--;
	   
    //Check if there is at least one character in the file
    if(numberOfCharsInFile == 0){
        printf("The file is empty.\n");
        return NULL;
    }

	//Checks if the the number of files to be compressed into is less than the number of characters in the file
    if(parts > numberOfCharsInFile){
        printf("The number of files to compress the original string exceeds the length of the original string.\n");
        return NULL;
    }

	//Create an array with each index corresponding to the number of char's that thread needs to read
    int i;
    
	int arrayNumChars[parts];
    for(i = 0; i < parts; i++){
        arrayNumChars[i] =  numberOfCharsInFile / parts ;
    }
    
    // If the amount of files the user wants the compression to be stored in does not divide evenly into the number of chars we spread out the remainder starting from the 1st file to the n-1 file
    for (i = 0; i <= (int)(numberOfCharsInFile % parts) - 1 && (int)(numberOfCharsInFile % parts) != 0;i++){
        arrayNumChars[i] += 1;
    }
   
    //array which holds all the starting points for each thread to compress
    int arrayStart[parts];
   
	//The first file will always start at the first index of the string
	arrayStart[0] = 0;
   
    //Intializing the starting points for each thread into an array
    for(i = 1; i < parts; i++){
        arrayStart[i] = arrayStart[i - 1] + arrayNumChars[i - 1];
    }

	char fName[strlen(fileName)];
	
	strcpy(fName, fileName);
	
	const char delim[] = ".";
	
	//index variable for putting tokenized words into 2D array
    	char *token;
	
	token = strtok(fName, delim);
	
	int strIndex = 0;

    	//declaring a double pointer 2D array of type char
   	char **strArr;

    //making the array strArr have a return type of type char**
    strArr = (char**)malloc(sizeof(char*));

    //allocating space in memory for the first tokenized word in the 2D array
    strArr[strIndex] = (char*)malloc(sizeof(char)*strlen(token));

    //copying the address of the tokenized word into the arrays index
    strArr[strIndex]=token;
     
    //iterating the token to go to the next word
    token = strtok(NULL,delim);
	while(token!=NULL ){
        //increment through the 2D array
        strIndex++;

        //allocating space in memory for the next tokenized word in the 2D array
        strArr[strIndex]=(char*)malloc(sizeof(char)*strlen(token));

        //copying the address of the tokenized word into the arrays index
        strArr[strIndex]=token;
        
        token = strtok(NULL,delim);
    }




    //creating an array of threads
    pthread_t arrThreadObj[parts]; 
    
	//declaring an array of structs which will hold all necessary data for each thread to use during its portion of compression
    data arg[parts];
    
	int j;
    
	//Each index represents a thread that will be created. This loop intializes the proper values for each thread to compress
    for(j = 0; j < parts; j++){     

        arg[j].threadID = j;
        arg[j].startingPoint = arrayStart[j];
        arg[j].numberOfCharsToRead = arrayNumChars[j];  
        arg[j].amtOfThreads = parts;
		arg[j].fileName = fileName;
		arg[j].extension = strArr[1];
		arg[j].file = strArr[0];
	}
	
	//This is the file name of the compressed file
	
	//The following code creates the compressed file name to be used to check if the file has already been compressed into 1 part or more than 1 part. If either is true, the program returns.

    int curr;    
	int rc;
    
	//Creates threads passing in compress as function and the struct as its argument to use for compression
    for(curr = 0; curr < parts; curr++){    
        rc = pthread_create(&arrThreadObj[curr],NULL, compress, &arg[curr]);
        //Checks if thread is created
        if (rc){
            printf("Error: Failed thread creation. \n");
            exit(-1);
        }
    

	printf("Right before joining threads in main\n");
    
    //joins all the threads to ensure they dont run unpredictably
    for(curr = 0; curr < parts; curr++){
        pthread_join(arrThreadObj[curr], NULL);
    }

    return NULL; 
}

void *compress(void *arg){
   
	//casting arg into a pointer to a data struct 
    data *argReal = (data*) arg;

    FILE *fp =fopen(argReal->fileName,"r");
	
	//Will be used as a counter
    int i; 
       
	char stringOfWholeLine[numberOfCharsInFile];
	
	//Gets the entire string from the file	
	fgets(stringOfWholeLine, numberOfCharsInFile + 1, fp);

	//creating array to store files characters that will be compressed in this particular thread
    char str[argReal->numberOfCharsToRead];
	
	int x=0;  
    	
	//goes through file and stores the characters into str
    for(i = argReal->startingPoint; i <  (argReal->startingPoint + argReal->numberOfCharsToRead); i++){
			str[x] =  stringOfWholeLine[i];
			x++;
	}
	str[x] = '\0';
	    
    //allocating memory in heap to store the compressed string portion the thread compresses
    char outputString[argReal->numberOfCharsToRead+10];
    
	int k;
	
	int letterCount = 1;
	
	outputString[0] = '\0';
	
	//This loop applies the compression algorithm to this threads substring of the string in the file
    for(k = 0; k < argReal->numberOfCharsToRead; k++){
		if(!isalpha(str[k])){}
		
		else{
			if (k == strlen(str)-1){
				if(letterCount ==2){
					char cToStr[3];
					cToStr[2] = '\0';
					cToStr[0] = str[k];
					cToStr[1] = str[k];
					strcat(outputString, cToStr);
				}
				else if(letterCount <2){
					char cToStr[2];
					cToStr[1] = '\0';
					cToStr[0] = str[k];
					strcat(outputString, cToStr);
				}
				else if(letterCount>2){
					char cToStr[4], chr[2];
					sprintf(cToStr, "%d", letterCount);
					chr[0] = str[k];
					chr[1] = '\0';
					strcat(outputString, cToStr);
					strcat(outputString, chr);
				}
				break;
			}
			if (str[k] == str[k+1])
				letterCount++;	
			else if(str[k]!= str[k+1] && letterCount == 2){ 
				char cToStr[3];
				cToStr[2] = '\0';
				cToStr[0] = str[k];
				cToStr[1] = str[k];
				strcat(outputString, cToStr);
				letterCount = 1; 
			}    
			else if(str[k]!= str[k+1] && letterCount <2){
				char cToStr[2];
				cToStr[1] = '\0';
				cToStr[0] = str[k];
				strcat(outputString, cToStr);
				letterCount = 1;
			}
			else if(str[k]!= str[k+1] && letterCount >2){
				char cToStr[4], chr[2];
				sprintf(cToStr, "%d", letterCount);
				chr[0] = str[k];
				chr[1] = '\0';
				strcat(outputString, cToStr);
				strcat(outputString, chr);
				letterCount = 1;
			}
		}
    }

	printf("After the compression for thread#%d compressed is: %s\n", argReal->threadID, outputString);
    
    //Creates the name of the compressed file for each thread with a series of concatinations
	char newFileName[100];

	if(argReal->amtOfThreads == 1){
		char ext[2];
		char l[] = "_LOLS";
		ext[0]= '_';
		ext[1]= '\0';
		char chr[2];
		strcat(newFileName, argReal->file);
		strcat(newFileName, ext);
		strcat(newFileName, argReal->extension);
		strcat(newFileName, l);
		strcat(newFileName, chr);
    }
	
	if(argReal->amtOfThreads  > 1){
		char ext[2];
		char l[] = "_LOLS";
		ext[0]= '_';
		ext[1]= '\0';
		char chr[5]; 
		sprintf(chr, "%d",argReal->threadID);
		strcat(newFileName, argReal->file);
		strcat(newFileName, ext);
		strcat(newFileName, argReal->extension);
		strcat(newFileName, l);
		strcat(newFileName, chr);
    }

	FILE *fp2 = fopen(newFileName, "w");

	fprintf(fp2, outputString);
	fclose(fp2);
	fclose(fp);
	
    pthread_exit(NULL);
}
