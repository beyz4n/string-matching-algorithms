#include <string.h>
#include <stdio.h>
#define max(x,y) ((x>y) ? x : y)

// occurence counter for horspool's algorithm
int horspoolsOccurence;
// this method creates a shift table for a given pattern
void createShiftTable(int *shiftTable, char pattern[]){
    // maximum 500 unique characters, they all have a position here, 0 if no shift can be given using pattern
    // we shouldn't include the last character so -1 - 1
    int currentPt = strlen(pattern) - 2;
    // while we don't exceed the pattern starting from the right to left
    while(currentPt >= 0){
        // if we didn't fill this place before by checking if it is found or not
        if( shiftTable[pattern[currentPt]] == 0)
            shiftTable[pattern[currentPt]] = strlen(pattern) - currentPt - 1;
        // move to left
        currentPt--;
    }
}
// this method takes input arguments as text and pattern as a char array
void horspools(char text[],char pattern[]){
    int patternLen = strlen(pattern);
    // in Horspool's Algorithm we start from the pattern's right hand side and align it
    int currentPt = patternLen - 1;
    // create the shift table here
    // TODO: Change the 500 to 128
    int shiftTable[128] = {0};
    createShiftTable(shiftTable, pattern);
    // while we don't exceeed the text
    while(currentPt < strlen(text)){
        // checking the pattern and text's character is identical or not
        if(text[currentPt] == pattern[currentPt % patternLen]){
            // if it is the pattern's last element (if  the pattern index is 0)
            if(currentPt % patternLen == 0){
                // then we increment the occurence counter
                horspoolsOccurence++;
                //mark
                currentPt += patternLen + 1;
            }
            // whether if there's a total match or a partial match we increment the current pointer
            currentPt--;
        }
        // if it doesn't match then we look at the shift table that we created for this pattern and shift accordingly
        else{
            // find the shift amount using shift table and the text's mismatched character, if it is not zero then we can use that shift amount
            int shift = shiftTable[text[currentPt+1]] != 0 ? shiftTable[text[currentPt]] : patternLen;
            currentPt += (shift != 6 ? (shift + (patternLen - (currentPt % patternLen) + 1)) : shift);
        }
    }

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

    /*
    printf("Good suffix table \n");
    for(int i = 0; i < patternLength; i++){
        printf("%d ", goodSuffixTable[i]);
    }
    printf("\n");
     */
}

int Boyer_Moore_Alg(char* pattern, char* text){
    int d1;
    int d2;
    int patterLength = (int) strlen(pattern);
    int textIndex = patterLength - 1;
    int numberOfMatch;
    char currentCh = text[textIndex];
    int goodSuffixTable[patterLength];
    GoodSuffixGenerator(goodSuffixTable ,pattern);
    int badSymbolTable[128] = {0};
    createShiftTable(badSymbolTable, pattern);
    int indexInBadSymbol;
    int found = 0;
    int count = 0;
/*
    printf("Good suffix table \n");
    for(int i = 0; i < strlen(pattern); i++){
        printf("%d ", goodSuffixTable[i]);
    }
    printf("\n");
*/

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
    printf("number of count %d\n", count);
    return count;
}



int main(){
    //horspools("BARD LOVED BANANAS", "BAOBAB");
    // horspools("GCATCGCAGAGAGTATACAGTACG", "GCAGAGAG");
    int count = Boyer_Moore_Alg("baubabab", "baubababhdshsdhbaubababdjsbausdbauubab");
    printf("count %d", count);

    //horspools("Hello we are trynaingining somethingnaingining.", "naingining");
    //horspools("HelloHelloHello", "Hello");

    /*
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
printf("%d", horspoolsOccurence);
    return 1;
    */
}
