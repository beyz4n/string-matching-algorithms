#include <string.h>
#include <stdio.h>
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
            shiftTable[pattern[currentPt]] = strlen(pattern) - currentPt + 1;
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

int bruteForce(char* string, char* pattern,FILE* file){

    int str_len = strlen(string);
    int pattern_len = strlen(pattern);

    int occurance = 0;

    for (int i = 0; i < str_len-pattern_len; i++)
    {
        int j = 0;
        for (j = 0; j < pattern_len; j++)
        {
            if (pattern[j] != string[i+j])
            break;
        }

        if (j == pattern_len)
            occurance++;
        
        
    }
    return occurance;

}

int main(){
    //horspools("BARD LOVED BANANAS", "BAOBAB");
    horspools("GCATCGCAGAGAGTATACAGTACG", "GCAGAGAG");
    //horspools("Hello we are trynaingining somethingnaingining.", "naingining");
    //horspools("HelloHelloHello", "Hello");
    char pattern[250];
    char filePath[250];

    
    return 1;
}
