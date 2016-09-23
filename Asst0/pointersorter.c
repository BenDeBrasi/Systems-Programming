#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<ctype.h>

/*Prototyping 3 functions I will be using. CreateSubstring exists to store each component string into an array of strings and reallocate space to the 2D as needed. 
 * Swap simply takes two values in the array and switches their location. insertionSort runs the insertion on the array and prints each string in ascii order. */


char **createSubString(int amtofSubstring, char **outputString, char* tmpSubString, char* argvPointer, int countLetters);
char **swap(char** outputString, int currIndex, int smallest);
void insertionSort(char** outputString, int amtofSubstring);
	
int main( int argc, char *argv[]){

	/*Ensures the user enters in 2 arguments only, the first being the program name and the second the string they wish to be outputted in ascii order. */
	if(argc != 2){
		printf("Please restart the program and enter 2 arguments!\n");
		return 0;
	}

	// Variable to store the total size of the string entered by the user. Is later used as a condition for the for loop which analyzes each letter in the user
	// input string. 
	
	const size_t size = strlen(argv[1]);
	
	//Pointer to char pointer where each individual string containing letters from the users command line entered string will be stored. 

	char **outputString = (char**) malloc(sizeof(char*) * 100);

	//Counts the amount of letters encountered in a contigious part of the user's command line entered string. Will be used to allow for strncpy to work and to allocate memory exactingly. 
	int countLetters = 0;

	//Keeps track of the amount of substrings with letters is present in the user's command line string. Will be used to allocate memeory and to sort and print through each letter-only substring found. 
	int amtofSubstring = 0;

	//Keeps track of the index of the first letter that appears in any portion of the user entered string.
	int startWord = 0;	

	//Keeps track of the ending index of any letter based substring in the user entered string. Also, is used as the incrementing variable that goes through the user entered string.
	size_t endWord = 0;
	
	//Stores each letter based string from the user entered string. Allows for exact and adjustable memory allocation for the potentially differing substring lengths as well as a 
	//convinent way to place each substring into the 2d array mentioned and declared earlier. "abc" is used as a filler value.
	char *tmpSubString = "abc";

	//Keeps track of the amount of times the array has been expanded. Serves as a mutliple for the intitial array size so the array grows by 100 each time it needs to be expanded. 
	int extensions = 1;

	//For loop to sort through each char in the user entered input. 
	for(endWord = 0; endWord < size; endWord++){
		
		//Checks if a char is a letter. If not it continues to go back to the start of the for loop, increment end word and start analyzing the next char.
		if( !isalpha(argv[1][endWord]) ){
			continue;
		}

		//Is entered once a letter has been found only.
		else{
			
			countLetters++;
			//If this is the first letter found then startWord stores that index so when the word is ready to be stored the program  know where the string begins.
			if(countLetters == 1)
				startWord = endWord;
			
			//The way to ensure not overrunning the user string, but also get all letter based strings is once a letter is found to check the next char as well. If the next char is then 
			//the loop is continued. If it is not then we know we've reached the end of the letter based substring and we will increment amtofSubstring, call createSubstring to store it
			//and then resume analyzing the chars. If the current letter we are on is the last letter in the user entered string then we know that this must be stored and nothing else analyzed
			//thus amtofSubstring is incremeted and createSubstring is called and then the this loop is break-ed out of to ensure no over running.
			if(endWord + 1 == size){
				//append last string
				amtofSubstring++;
				
				//Expands the array by 100 every time room runs out.
				if( (extensions * 100) == amtofSubstring){
					extensions++;
					outputString = (char**) realloc(outputString, (extensions * 100) * sizeof(char*));
				}
	
				
				outputString = createSubString(amtofSubstring, outputString, tmpSubString, &argv[1][startWord], countLetters);
				break;
			}
			
			//If the next char is a letter in the user entered string analyze the next letter.
			if(isalpha(argv[1][endWord + 1])){
				continue;
			}
			//If the next letter is not a letter and if it exists within the bounds of the length of the user input then store it into outputString.
			else{
				amtofSubstring++;
				
				//Expands the array by 100 every time room runs out.
				if( (extensions * 100) == amtofSubstring){
					extensions++;
					outputString = (char**) realloc(outputString, (extensions * 100) * sizeof(char*));
				}
					
				
				outputString = createSubString(amtofSubstring, outputString, tmpSubString, &argv[1][startWord], countLetters);
			}

		}

		//Right after a string is stored into outputString, countLetters is returned to 0 so that if another letter is found start letter will be assigned to it properly.
		countLetters = 0;
	}
	
	//If there are are no letter based substrings a statement is printed informing the user and immediately exiting the program as to not use computer resources uselessly.
	if(amtofSubstring == 0){
		printf("There are no letters!\n");
		free(outputString);
		return 0;
	}

	//Calls insertionSort. Printing and sorting are taken care of by that function.
	insertionSort(outputString, amtofSubstring);
	printf("\n");

	//Frees all dynamically allocated memory used and exits the program.
	int f = 0;
	for(; f < amtofSubstring; f++){
		
		free(outputString[f]);
	}

	free(outputString);

	return 0;
}

//A function to take a letter based substring from the user's input and store it into an array. It also will expand the array if there is not enough room.
char **createSubString(int amtofSubstring, char **outputString, char* tmpSubString, char* argvPointer, int countLetters){

	//Allocates memeory to a char pointer based on the the length of the letter based substring in user's input. This amount is varibale thus the need to track and pass it along.
	//Adds +1 to manually add the null terminating character to the end of each substring.
	tmpSubString = (char*) malloc(sizeof(char) * (countLetters + 1));
	
	//Copies the substring of countLetters size to tmpSubString from the user input.
	strncpy(tmpSubString, argvPointer, countLetters);
	
	//Adds null terminating character to the temporary char pointer
	tmpSubString[countLetters]= '\0';
	
	//Stores the char pointer just made into the right index of outputString
	outputString[amtofSubstring-1] = tmpSubString;
	
	return outputString; 
}
	
//Function for an ordinary inserstion sort.
void insertionSort(char** outputString, int amtofSubstring){
	
	//2 variable needs for the double loop. I will go from outputString[1] to outputString[amtofSubstring int order to check every letter. 0 is not included because j will take that value every time
	//in the worst case and because an array with 1 element is considered sorted thus by setting i to 1 it will not go to the loops if a single element array is presented to it. ]
	int i  = 1;
	int j = 0;

	for(; i < amtofSubstring ;i++){
		//Sets j = i so that we can compare i to all members before it if need be.
		j = i;
		//Compares outputString[j] to outputString[j-1] until either outputString[j-1] element's ascii value is less than outputString[j] 
		//or until the 0th index is reached the the j element is now the smallest element in the sorted part of the array.
	
		for( j=i; j > 0 && (strcmp(outputString[j-1], outputString[j]) >= 0); j--){
		swap(outputString, j-1, j);
		}
		
		
	}

	//Prints out the sorted array contents in ascii order starting from 'A' words and ending at 'z' words.
	int r =0;
	for(;r<amtofSubstring;r++){
		printf("%s\n", outputString[r]);
	}

}

//Basic swap function that takes an array two index values and swaps the data between them.
char **swap(char** outputString, int currIndex, int smallest){
	char *tmp = outputString[smallest];
	outputString[smallest] = outputString[currIndex];
	outputString[currIndex] = tmp;
	return outputString;
}
