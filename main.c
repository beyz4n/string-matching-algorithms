#include <string.h>
// occurence counter for horspool's algorithm
int horspoolsOccurence;
// this method creates a shift table for a given pattern
int * createShiftTable(char pattern[]){
    // maximum 500 unique characters, they all have a position here, 0 if no shift can be given using pattern
    int shiftTable[500] = {0};
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
    // in Horspool's Algorithm we start from the pattern's right hand side and align it
    int currentPt = strlen(pattern) - 1;
    // create the shift table here
    int * shiftTable = createShiftTable(pattern);
    // while we don't exceeed the text
    while(currentPt < strlen(text)){
        // checking the pattern and text's character is identical or not
        if(text[currentPt] == pattern[currentPt % strlen(pattern)]){
            // if it is the pattern's last element (if  the pattern index is 0)
            if(currentPt % strlen(pattern) == 0){
                // then we increment the occurence counter
                horspoolsOccurence++;
            }
            // whether if there's a total match or a partial match we increment the current pointer
            currentPt++;
        }
        // if it doesn't match then we look at the shift table that we created for this pattern and shift accordingly
        else{
            // find the shift amount using shift table and the text's mismatched character, if it is not zero then we can use that shift amount
            int shift = shiftTable[text[currentPt]] != 0 ? shiftTable[text[currentPt]] : strlen(pattern);
            currentPt += shift;
        }
    }

}
int main(){

    
    return 1;
}