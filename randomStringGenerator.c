#include <stdio.h>
#include <stdlib.h>
int main(){
    
    FILE *outputFile = fopen("input2.html", "w");
    FILE *inputFile = fopen("inputOptions.txt", "r");

    if(inputFile == NULL){
        printf("input file could not be found");
        exit(1);
    }
    if(outputFile == NULL){
        printf("input file could not be found");
        exit(1);
    }
    char pattern[250];
    long long length = 0;

    fgets(pattern, 250, inputFile);
    printf("enter the length of the file ");
    scanf("%d", &length);
    printf("\n");
    if (length<0){
        printf("enter a valid length");
        exit(2);
    }

    int occurenceCounter = 0;
    int random = 32 + rand()%95;
    for(int i = 0; i<length; ){
        if(rand()%10){
            
            if (random != 60 && random != 62){
                putc( random , outputFile );
                random = 32 + rand()%95;
                i++;
            }
            else{
                random = 32 + rand()%95;
                i++;
            }
            
        }
        else{
            for(int j = 0; pattern[j] != NULL; j++){
                putc(pattern[j], outputFile);
            }
            occurenceCounter++;
            i++;
        }
        
    }
    printf("number of occurences of the pattern are %d\n", occurenceCounter);

}