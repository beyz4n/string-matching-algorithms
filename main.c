#include<stdio.h> 
#include<stdlib.h>
#include<string.h>
#include<Math.h>


int main(){

    char pattern[250];
    char filePath[250];

    printf("enter the pattern to be searched: ");
    gets(pattern);
    printf("\nenter the html file name: ");
    gets(filePath);
    
    FILE *file = fopen(filePath,"r"); 

     if(file == NULL){
        printf("input file could not be found");
        exit(1);
    }

    int arraySize = strlen(pattern)*30;
    if (arraySize >= 500){
        arraySize = 500;
    }

    char input[arraySize];
    while(!feof(file)){
        
        for(int i = 0 ; !feof(file) && (i<arraySize) ; i++){
            fscanf(file, "%c", input[i]);
        }
        // call functions
    }

    return 1;
}
