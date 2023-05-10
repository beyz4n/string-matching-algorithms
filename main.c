#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#define max(x,y) ((x>y) ? x : y)

int call = 0;
// this method creates a shift table for a given pattern
void createShiftTable(int *shiftTable, char pattern[]){
    // maximum 256 unique ascii characters, they all have a position here, 0 if no shift can be given using pattern
    // we shouldn't include the last character so -1 - 1
    int currentPt = strlen(pattern) - 2;
    // while we don't exceed the pattern starting from the right to left
    while(currentPt >= 0){
        // if we didn't fill this place before by checking if it is found or not - if 0 then it is empty
        if( shiftTable[pattern[currentPt]] == 0)
            // fill it in
            shiftTable[pattern[currentPt]] = strlen(pattern) - currentPt - 1;
        // move to left
        currentPt--;
    }
}

// this method takes input arguments as text and pattern as a char array
int horspools(char text[],char pattern[], FILE *output){
    // occurence counter for horspool's algorithm
    int horspoolsOccurence = 0;
    int patternLen = strlen(pattern);
    // in Horspool's Algorithm we start from the pattern's right hand side and align it
    int currentPt = patternLen - 1;
    int patternPt = patternLen - 1;
    // create the shift table here
    int shiftTable[256] = {0};
    createShiftTable(shiftTable, pattern);
    // while we don't exceeed the text
    while(currentPt < strlen(text)){
        // checking the pattern and text's character is identical or not
        if(text[currentPt] == pattern[patternPt]){
            // if it is the pattern's last element (if  the pattern index is 0)
            if(patternPt == 0){
                // then we increment the occurence counter
                horspoolsOccurence++;
                // set to next location
                currentPt += patternLen + 1;
                patternPt =  patternLen - 1;
                //mark
            }
            // whether if there's a total match or a partial match we increment the current pointer
            currentPt--;
            patternPt--;
        }
        // if it doesn't match then we look at the shift table that we created for this pattern and shift accordingly
        else{
            // find the shift amount using shift table and the text's mismatched character, if it is not zero then we can use that shift amount  
            int shift = (patternPt == (patternLen - 1 )) ? shiftTable[text[currentPt]] : shiftTable[text[currentPt+1]];
            // if the amount is 0 then we have the full pattern length
            if(shift == 0) 
                shift = patternLen;
            // increment the text pointer accordingly and reset the pattern pointer 
            currentPt += (shift + patternLen - patternPt - 1);
            patternPt =  patternLen - 1;
        }
    }
    return horspoolsOccurence;
}

int bruteForce(char* string, char* pattern,FILE* output){
    int str_len = strlen(string);
    int pattern_len = strlen(pattern);
    int occurence = 0;
    for (int i = 0; i < str_len-pattern_len+1; i++)
    {
        int j = 0;
        for (j = 0; j < pattern_len; j++)
        {
            if (pattern[j] != string[i+j])
            break;
        }
        if (j == pattern_len)
            occurence++;
    }
        return occurence;
}

// Function to generate good suffix table
void GoodSuffixGenerator(int* goodSuffixTable, char* pattern){
    int patternLength = strlen(pattern);
    goodSuffixTable[0] = 0;
    int check = 0;
    int index;
    int shiftNumber = patternLength;

    for(int match = 1; match < patternLength; match++){ // for each match number
        for(int i = patternLength - 2 ; i >= 0; i--){ // to find matched part in pattern
            check = 0;
            if(pattern[i] == pattern[patternLength - 1]){
                check = 1;
                index = i;
                for(int j = 1; j <= match; j++){
                    if(pattern[index] != pattern[patternLength - j]){
                        check = 0;
                    }
                    index--;
                    if(index == -1){
                        break;
                    }
                }
            }
            if(check && pattern[patternLength - match - 1] != pattern[i - match] ){
                shiftNumber = patternLength - i - 1;
                break;
            }
        }
        goodSuffixTable[match] = shiftNumber;
        shiftNumber = patternLength;
    }
}

int Boyer_Moore_Alg(char* pattern, char* text, FILE *output){
    int d1;
    int d2;
    int patternLength = (int) strlen(pattern);
    int textIndex = patternLength - 1;
    int numberOfMatch;
    char currentCh = text[textIndex];
    int goodSuffixTable[patternLength];
    GoodSuffixGenerator(goodSuffixTable ,pattern);
    int badSymbolTable[256] = {0};
    createShiftTable(badSymbolTable, pattern);
    if(!call){
        
        printf("Good suffix table \n");
        for(int i = 0; i < strlen(pattern); i++){
            printf("%d ", goodSuffixTable[i]);
        }
        printf("\n");
        
        printf("bad symbol table: \n");
        for(int i = 0; i < 256; i++){
            int shift = badSymbolTable[i] == 0 ? patternLength : badSymbolTable[i];
            printf("%c - > %d\n", i, shift);
        }
        call++;
    }
    int indexInBadSymbol;
    int found = 0;
    int count = 0;
    while(textIndex < strlen(text)){
        numberOfMatch = 0;
        // count number of match
        for(int i = textIndex, patternIndex = strlen(pattern) - 1 ; patternIndex >= 0 ; i--, patternIndex--){
            if(pattern[patternIndex] == text[i]){
                numberOfMatch++;
            }
            else
                break;
        }

        // If number of match is equal to pattern length: pattern is found
        if(numberOfMatch == strlen(pattern)){
            textIndex += strlen(pattern); // shift by pattern length
            currentCh = text[textIndex]; // update current char
            count++;
            continue;
        }

        // To find d1 value
        d1 = max(badSymbolTable[currentCh] - numberOfMatch, 1);

        if(numberOfMatch == 0){ // find shift value from bad symbol table
            textIndex += d1;
            currentCh = text[textIndex];
        }
        else{ // find shift value from good suffix table
            d2 = goodSuffixTable[numberOfMatch];
            textIndex += max(d1, d2);
            currentCh = text[textIndex];
        }
    }
    return count;
}

int main(){
    //horspools("BARD LOVED BANANAS", "BAOBAB");
    // horspools("GCATCGCAGAGAGTATACAGTACG", "GCAGAGAG");
    //int count = Boyer_Moore_Alg("baubabab", "baubababhdshsdhbaubababdjsbausdbauubab");
    //printf("count %d", count);

    //horspools("Hello we are trynaingining somethingnaingining.", "naingining");
    //horspools("HelloHelloHello", "Hello");
    //printf("brute force test ocurrance: %d", bruteForce("GCATCGCAGAGAGTATACAGTACG","GCAGAGAG",NULL));


    
    char pattern[250];
    char filePath[250];
    FILE *fileOptions = fopen("inputOptions.txt","r");

    if(fileOptions == NULL){
        printf("fileOptions file could not be found");
        exit(1);
    }

    fgets(pattern, 250, fileOptions);
    fgets(filePath, 250, fileOptions);
    fclose(fileOptions);
    printf("%s%s\n", pattern, filePath);
    
    pattern[(strlen(pattern)-1)] = '\0';

    FILE *file = fopen(filePath,"r");
    FILE *output = fopen("output.html", "w");

     if(file == NULL){
        printf("input file could not be found");
        exit(1);
    }

    int arraySize = strlen(pattern)*30;
    if (arraySize >= 500){
        arraySize = 500;
    }

    char input[arraySize];
    char temp[arraySize];
    *temp = NULL;
    int bruteForceOccurence = 0;
    int boyerOccurence = 0;
    int horspoolsOccurence = 0;

    while(!feof(file)){
        int i = 0;

        if( 0<strlen(temp) ){
            // copy the pattern length of last elements from previous array to new array
            for(int j = 0; j< strlen(temp);j++){
                input[j] = temp[j]; 
            }
            i += strlen(pattern)-1;
            *temp = NULL;
        }
        
        for(; !feof(file) && (i<arraySize-1) ; i++){
            input[i] = fgetc(file);
        }
        input[i] = '\0';

        // call functions
        
        bruteForceOccurence += bruteForce(input,  pattern, output);
        horspoolsOccurence += horspools(input, pattern, output);
        boyerOccurence += Boyer_Moore_Alg(pattern, input, output);

        if(!feof(file)){
            i -= strlen(pattern)-1;
            for(int j = 0; i<arraySize ; i++, j++){
                temp[j] = input[i];
            }
            input[--i] = '\0';
        }
    }
    
    printf("horspool occurence: %d\n", horspoolsOccurence);
    printf("Brute force occurence: %d\n", bruteForceOccurence);
    printf("Boyer moore algorith occurence: %d\n", boyerOccurence);
    return 1;
    
}
