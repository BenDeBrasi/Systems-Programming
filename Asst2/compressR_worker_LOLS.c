
#include<stdio.h>
#include<string.h>
#include<unistd.h>
#include<stdlib.h>
#include<ctype.h>
#include <sys/types.h>
#include <sys/wait.h>

int main(int argc, char *argv[]){
	
	int processID = atoi(argv[1]);
	int startingPoint = atoi(argv[2]);
	int numberOfCharsToRead = atoi(argv[3]);
	int amtOfProcess = atoi(argv[4]);
	char *fileName = argv[5];
	char *extension = argv[6];
	char *file = argv[7];
	int numberOfCharsInFile = atoi(argv[8]);
		
	FILE *fp = fopen(fileName, "r");
	
	int i; 
       
	char stringOfWholeLine[numberOfCharsInFile];
	
	fgets(stringOfWholeLine, numberOfCharsInFile + 1, fp);

	//creating array to store files characters
    char str[numberOfCharsToRead];
	
	int x=0;  
    	
	//goes through file and stores the characters into str
    for(i = startingPoint; i <  (startingPoint + numberOfCharsToRead); i++){
			str[x] =  stringOfWholeLine[i];
			x++;
	}
	str[x] = '\0';
	    
    //allocating memory in heap to store the compressed string portion the thread compresses
    char outputString[numberOfCharsToRead+10];
    
	int k;
	
	int letterCount = 1;
	
	outputString[0] = '\0';
	
	//loops through the portion of the file the process is assigned to compress
    for(k = 0; k < numberOfCharsToRead; k++){
		//if character is not an alphabet....skip it
		if(!isalpha(str[k])){}
		
		//it character is an alphabet....
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
	    
    //If the number of threads = 1 we use the naming scheme without the number at the end
    char newFileName[100] = {'\0'};
	if(amtOfProcess == 1){
		char ext[2];
		char l[] = "_LOLS";
		ext[0]= '_';
		ext[1]= '\0';
		strcat(newFileName, file);
		strcat(newFileName, ext);
		strcat(newFileName, extension);
		strcat(newFileName, l);
    }
	
	//If the number of process > 1 we use the naming scheme with the number at the end
	if(amtOfProcess > 1){
		char ext[2];
		char l[] = "_LOLS";
		ext[0]= '_';
		ext[1]= '\0';
		char chr[5]; 
		sprintf(chr, "%d",processID);
		strcat(newFileName,file);
		strcat(newFileName,ext);
		strcat(newFileName,extension);
		strcat(newFileName,l);
		strcat(newFileName,chr);
    }
		
	FILE *fp2 = fopen(newFileName, "w");
		
	fprintf(fp2, outputString);
	
	fclose(fp2);
		
	fclose(fp);
	
	return 0;
}
