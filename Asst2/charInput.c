/*#include <stdio.h>
#include <string.h>
#include <stdlib.h>

int main()
{
  char* productValue = (char*)malloc(sizeof(char)*3);
  char* outputString = (char*)malloc(sizeof(char)*3);
  //char outputString;
  int i, x, number,result;
  printf("Please enter number\n");
  //scanf("%d", &number);
  //result = number * 2;
  result = 10;
  char * value = "Hi";
  sprintf(productValue, "%d", result);
  strcat(outputString, productValue);
  strcat(outputString, value);

  
  printf("%s\n", outputString);
  return 0;

}
void * multiply(void* threadid){
	int tid = (int)threadid;
	int x = 2;
	int y = 4;
	printf("thread: %d string =  %s start: %d end: %d \n", thread_id, string, start, end);

  pthread_exit(NULL);
}
*/

#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <string.h>

#define NUMTHREADS 3
typedef struct thread_data {
        int tid;
        int ;
        int id;
}thread_data;

void *compress(void* str)
{
  /*char *string = "helloworld";
  int thread_id = (int)tid;
  int length = strlen(string);
  int chunk = length/NUMTHREADS;
  int start = thread_id * chunk;
  int end;
  if (thread_id == NUMTHREADS-1)
	end = length-1;
  else
  	end = start+chunk;*/
  
  /*int thread_id = (int)tid;
  int x = (int)tid;
  int y = x+1;
  int z = x + y;
  printf("thread: %d  x: %d + y %d = %d  \n", thread_id, x,  y, z);*/
  //int thread_id = (int)tid;
  char * strng = (char*)malloc(sizeof(char)*sizeof(str));
  //int i=0, k = 0, size = 100;
  /*char * str = (char*)malloc(sizeof(char)*size);
  printf("Enter String to be compressed, Tid %d\n", thread_id);
  scanf("%99s",str);*/
  printf(" string = %s\n", strng);
  
  pthread_exit(NULL);
  
}
int main(){
    unsigned int len_max = 128;
    unsigned int current_size = 0;

    char *pStr = malloc(len_max);
    current_size = len_max;
    printf("\nEnter a very very very long String value:");
	
    if(pStr != NULL){
		int c = EOF;
		unsigned int i =0;
        //accept user input until hit enter or end of file
		while (( c = getchar() ) != '\n' && c != EOF)
		{
			pStr[i++]=(char)c;
			//if i reached maximize size then realloc size
			if(i == current_size)
			{
				current_size = i+len_max;
				pStr = realloc(pStr, current_size);
			}
		}

		pStr[i] = '\0';
	}
    pthread_t threads[NUMTHREADS];
	int length = strlen(pStr);
    int chunk = length/NUMTHREADS;
    // master thread will create 2 threads and call compress function
	for (int j = 0; j < NUMTHREADS; j++)
	{
		//creates a variable that holds the return value of the the thread creat function
		int rc = pthread_create(&threads[j],NULL,compress,pStr);
		//if return value is anything but 0, show an error message stating thread was not successfully created
		if (rc)
		{
			printf("error: in pthread_create() \n");
			exit(-1);
		}
	}
	pthread_exit(NULL);
	return 0;
}