#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <sys/time.h>
#define max(x,y) ((x>y) ? x : y)
#define ARRAY_SIZE 500

long long bruteForceComparison;
long long horspoolComparison;
long long boyerComparison;
int shiftTable[128];

typedef struct indexNode{
    //We need to store the occurance indices dynamically and we don't have ArrayLists. So, we have to implement a basic LL.
    long long index;
    struct indexNode* next;
} indexNode;

indexNode* head = NULL;
indexNode* lastNode = NULL;

//linked list insertion to the last element
void insertIndex(long long data){
    indexNode* newIndex = malloc(sizeof(indexNode));
    newIndex->index = data;
    newIndex->next = NULL;

    if (head == NULL){
        head = newIndex;
        lastNode = newIndex;
    }else{
        lastNode->next = newIndex;
        lastNode = newIndex;
    }
}



// this method creates a shift table for a given pattern
void createShiftTable(int *shiftTable, char pattern[]){
    // maximum 128 unique ascii characters, they all have a position here, 0 if no shift can be given using pattern
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

void markFile(indexNode* head,int patternlen, FILE* file, FILE* output){
    indexNode* currentNode = head; //begin with the head of the linked list
    long long previousIndex = 0;

    while (currentNode != NULL){
        long long printedCount = currentNode->index - previousIndex;
        //print the characters before the mark
        for(; !feof(file)&& printedCount; printedCount--){
            fputc(fgetc(file),output); // taking input to an array
        }
        //if you are at the last node, break the loop
        if (currentNode->next == NULL)
            break;

        //mark the pattern
        fputs("<mark>",output);
        indexNode* nextNode = currentNode->next;
        while (nextNode != NULL)
        {   
            //if there is a overlapping occurance in the text print overlapping characters.
            if ((nextNode->index)-(currentNode->index)<patternlen)
            {
                printedCount+=(nextNode->index)-(currentNode->index);
                 currentNode = nextNode;
                 nextNode = nextNode->next;
                for(; !feof(file) && printedCount; printedCount--){
                    fputc(fgetc(file),output);
                }
            }else{
                //if there is no overlap until the next ocurrance print the pattern
                printedCount = patternlen;
                for(; !feof(file) && printedCount; printedCount--){
                    fputc(fgetc(file),output); 
                }
                break;
            }  
        }
        fputs("</mark>",output);
        //Save the previous marked point
        previousIndex = currentNode->index+patternlen;
        //go to next node
        currentNode = nextNode;
    }
    
    if(head != NULL){
    //mark the last pattern here
    fputs("<mark>",output);
    int printedCount = patternlen;
    for(; !feof(file) && printedCount; printedCount--){
        fputc(fgetc(file),output); 
    }
    fputs("</mark>",output);
    }

    //print the remaining chars
    while(!feof(file)){
        char ch  =fgetc(file);
        if (ch >0)
        fputc(ch,output);

    }
}

// this method takes input arguments as text and pattern as a char array and finds the occurence number using Horspool's Algorithm
int horspools(char text[],char pattern[]){
    // occurence counter for horspool's algorithm
    int horspoolsOccurence = 0;
    int patternLen = strlen(pattern);
    int strLen = strlen(text);
    // in Horspool's Algorithm we start from the pattern's right hand side and align it
    int currentPt = patternLen - 1;
    int patternPt = patternLen - 1;
    // create the shift table here
    // while we don't exceeed the text
    while(currentPt < strLen && currentPt >= 0){
        // checking the pattern and text's character is identical or not
        if(text[currentPt] == pattern[patternPt]){
            horspoolComparison++;
            // if it is the pattern's last element (if  the pattern index is 0)
            if(patternPt == 0){
                // then we increment the occurence counter
                horspoolsOccurence++;
                // find the shift amount using shift table and the text's mismatched character, if it is not zero then we can use that shift amount  
                int shift = shiftTable[text[currentPt + patternLen - patternPt - 1]];
                // if the amount is 0 then we have the full pattern length
                if(shift == 0) 
                    shift = patternLen;
                // increment the text pointer accordingly and reset the pattern pointer 
                currentPt += (shift + patternLen - patternPt - 1) + 1;
                patternPt =  patternLen;
            }
            // whether if there's a total match or a partial match we increment the current pointer
            currentPt--;
            patternPt--;
        }
        // if it doesn't match then we look at the shift table that we created for this pattern and shift accordingly
        else{
            horspoolComparison++;
            // find the shift amount using shift table and the text's last character aligned with pattern's last character
            // if it is not zero then we can use that shift amount  
            int shift = shiftTable[text[currentPt + patternLen - patternPt - 1]];
            // if the amount is 0 then we have the full pattern length
            if(shift == 0) 
                shift = patternLen;
            // increment the text pointer accordingly and reset the pattern pointer 
            currentPt += (shift + patternLen - patternPt - 1);
            patternPt =  patternLen - 1;
        }
    }    
    // return the occurence number
    return horspoolsOccurence;
}

//Simple brute force algorithm
int bruteForce(char* string, char* pattern){
    int str_len = strlen(string);
    int pattern_len = strlen(pattern);
    int occurence = 0;

    //Iterate the every element in the string
    for (int i = 0; i < str_len-pattern_len+1; i++)
    {
        int j = 0;
        for (j = 0; j < pattern_len; j++)
        {
            bruteForceComparison++;
            //if there is a mismatch, shift the pattern by 1 
            if (pattern[j] != string[i+j])
            break;
        }
        //if all matched, increment the occurence
        if (j == pattern_len)
            occurence++;
        
    }
    return occurence;
}

int bruteMarker(char* string, char* pattern,int occurIndex){
    int str_len = strlen(string);
    int pattern_len = strlen(pattern);
    for (int i = 0; i < str_len-pattern_len+1; i++)
    {
        int j = 0;
        for (j = 0; j < pattern_len; j++)
        {
            if (pattern[j] != string[i+j])
            break;
        }
        if (j == pattern_len){
            insertIndex(i+((ARRAY_SIZE-pattern_len)*occurIndex));// inserting the next occurance index to mark the file.
        }
    }

    return 1;
}

// Function to generate good suffix table
void goodSuffixGenerator(int* goodSuffixTable, char* pattern){
    int patternLength = strlen(pattern); // variable to keep pattern length
    // Since for  match = 0, we will not use good suffix table, so initialize first index with zero.
    goodSuffixTable[0] = 0;
    int check; // variable to check if there is another occurrence for the match part in the pattern
    int index;
    int shiftNumber = patternLength; // if no match, shift amount is pattern length

    for(int match = 1; match <= patternLength; match++){ // for each match number
        for(int i = patternLength - 2 ; i >= 0; i--){ // to find matched part in pattern
            check = 0; // check is 0 for now

            if(pattern[i] == pattern[patternLength - 1]){ // if pattern contains match part again before match at the end
                check = 1; // check is 1 since we found another occurrence of first character of match
                index = i; // index variable to keep copy of index of beginning of the second match part

                for(int j = 1; j <= match; j++){ // check if there is another occurrence of match part
                    if(pattern[index] != pattern[patternLength - j]){
                        check = 0;
                    }
                    index--;
                    if(index == -1){ // if index is -1 no need to check anymore
                        break;
                    }
                }
            }
            // If there is another occurrence of match part and
            // if the character before match repeats itself again before the match then
            // we skip that match and try to find another match without incorrect character
            if(check && pattern[patternLength - match - 1] != pattern[i - match] ){
                shiftNumber = patternLength - i - 1; // calculate shift amount
                break;
            }
        }
        goodSuffixTable[match] = shiftNumber; // add shift amount to table
        // to access match = 1 value of good suffix => goodSuffixTable[1]
        shiftNumber = patternLength; // assign pattern length to shift number again
    }
}

// Boyer moore algorithm
int Boyer_Moore_Alg(char* pattern, char* text, int* goodSuffixTable){
    int d1; // shift amount according to bad symbol table
    int d2; // shift amount according to good suffix table
    int patternLength = (int) strlen(pattern); // length of given pattern
    int textLength = strlen(text); // length of text
    int textIndex = patternLength - 1;
    int numberOfMatch; // number of character match at each trial
    char currentCh;
    int count = 0; // counter for full match
    int shift; // shift amount

    while(textIndex < textLength){
        numberOfMatch = 0;
        currentCh = text[textIndex]; // update current char

        // count number of character match at each trial
        for(int i = textIndex, patternIndex = patternLength - 1 ; patternIndex >= 0 ; i--, patternIndex--){
            boyerComparison++; // increment number of comparison for boyer moore
            if(pattern[patternIndex] == text[i]){
                numberOfMatch++;
            }
            else
                break;
        }

        // If number of match is equal to pattern length: pattern is found
        if(numberOfMatch == patternLength){
            count++; // since pattern is found, increment full-match counter
        }

        // To find d1 value
        shift = shiftTable[text[textIndex - numberOfMatch]] == 0 ? patternLength: shiftTable[currentCh];
        d1 = max( shift - numberOfMatch, 1);

        if(numberOfMatch == 0){ // shift for not match
            textIndex += d1;
        }
        else{ // shift for match
            d2 = goodSuffixTable[numberOfMatch];
            textIndex += max(d1, d2);
        }
    }
    return count;
}

// this is the main function where we integrate all the functions above to search for a given pattern in a given input
// using the brute force algorithm, the horspool algorithm and the boyer-moore algorithm. we mark the pattern in the given input, time the algorithms,
// find the number of comparisons made and find the total number of occurences and print these.
int main(){

    char pattern[250]; // array to hold the pattern
    char filePath[250]; // array to hold the input file path
    FILE *fileOptions = fopen("inputOptions.txt","r"); // creating a FILE for the input options to take the pattern and input file path

    if(fileOptions == NULL){ // checking if the file exists
        printf("fileOptions file could not be found"); // printing error if not
        exit(1);
    }

    fgets(pattern, 250, fileOptions); // taking the pattern from input options
    fgets(filePath, 250, fileOptions); // taking the input file path from input options 
    fclose(fileOptions); // closing the file because we won't need it anymore
    printf("%s%s\n", pattern, filePath); // printing the pattern to be searched and the file path of the input file 
    // checking whether a pattern was entered to the program or not
    if(pattern[1] == '\0'){
        printf("pattern could not be found"); // if not, printing error message and exiting the program
        exit(1);
    }
    // checking wether the pattern length exceeds the maximum length of 248
    if(strlen(pattern) == 249 && pattern[249] == '\0' && pattern[248] != 10 ){
        printf("pattern length exceeds the maximum of 248"); // if it exceeds the maximum, printing error message and exiting the program
        exit(1);
    }
    // checking wheter a file path was entered to the program or not
    if(filePath[1] == '\0'){
        printf("filePath could not be found"); // if not, printing error message and exiting program
        exit(1);
    }
    // checking wether the file Path length exceeds the maximum length of 248
    if(strlen(filePath) == 249 && filePath[249] == '\0' && filePath[248] != 10 ){
        printf("file Path length exceeds the maximum of 248"); // if it exceeds the maximum, printing error message and exiting the program
        exit(1);
    }
    

    pattern[(strlen(pattern)-1)] = '\0'; // setting the last element of the pattern to the end of text char

    FILE *file = fopen(filePath,"r"); // creating a FILE for the input file
    FILE *output = fopen("output.html", "w"); // creating a FILE for the marked output

    if(file == NULL){ // checking if the input file exists
        printf("input file could not be found"); // pringting error if not so
        exit(1);
    }

    int piece = 0; // this keeps the information of which section are we in in the file 
    char input[ARRAY_SIZE]; // creating array to hold input
    char temp[ARRAY_SIZE]; // creating temporary array to hold the last patternlength -1 number of elements to add them to the new array(more on this comes later)
    *temp = '\0'; // setting the temporary array to 0
    int bruteForceOccurence = 0; // variable to hold the number of occurences for the brute force algorithm
    int boyerOccurence = 0; // variable to hold the number of occurences for the boyer-moore algorithm
    int horspoolsOccurence = 0; // variable to hold the number of occurences for the horspool algorithm
    double bruteForceTime = 0.0; // variable to hold the time for the brute force algorithm
    double boyerTime = 0.0; // variable to hold the time for the boyer-moore algorithm
    double horspoolTime = 0.0; // variable to hold the time for the horspool algorithm
    struct timeval timer1, timer2; // variables to hold the time before and after a function call to calculate the difference and by doing so finding the time it took to execute that function
    int patternLength = strlen(pattern); // setting the length of the pattern to a variable
    int tempLength = 0; // initilazing a variable to hold the temporary array length
    // here we take the time before and after calling the algorithm to find out the time it took
    // after that we add that time to the corresponding algorithm(s) time counter
    gettimeofday(&timer1, NULL);
    createShiftTable(shiftTable, pattern);
    gettimeofday(&timer2, NULL);
    // adding the time it took to generate the bad shift table to the horspool algorithm time counter
    horspoolTime += ((timer2.tv_sec-timer1.tv_sec) * 1000000) + timer2.tv_usec - timer1.tv_usec; // using seconds and nanoseconds to determine the time it took
    // adding the time it took to generate the bad shift table to the boyer-moore algorithm time counter
    boyerTime += ((timer2.tv_sec-timer1.tv_sec) * 1000000) + timer2.tv_usec - timer1.tv_usec;
    // here we are doing the same for the good suffix table as above with the bad shift table
    gettimeofday(&timer1, NULL);
    int goodSuffixTable[patternLength + 1];
    goodSuffixGenerator(goodSuffixTable ,pattern);
    gettimeofday(&timer2, NULL);
    // adding the time it took to generate the good suffix table to the boyer-moore algorithm time counter
    boyerTime += ((timer2.tv_sec-timer1.tv_sec) * 1000000) + timer2.tv_usec - timer1.tv_usec;
    // printing the good suffix table using a for loop
    printf("Good suffix table: \n");
    for(int i = 1; i <= strlen(pattern); i++){
        printf("k = %d - > %d\n", i , goodSuffixTable[i]);
    }  
    // printing the bad symbol table using a for loop  
    printf("Bad symbol table: \n");
    for(int i = 0; i < 128; i++){
        if(shiftTable[i])
            printf("%c - > %d\n", i, shiftTable[i]);
    }
    printf("other characters - > %d\n", patternLength);
    // in this while loop we divide the input string into, at max, 500 byte arrays and call each algortihm to find the string while countin their time
    while(!feof(file)){ // taking input until the end of the file
        int i = 0;
        tempLength = strlen(temp); // using a temporary array to carry the last patternlength -1 elements into the new one so we don't miss a pattern because of splitting it
        // checking whether there are elements to copy to the new array
        if( 0<tempLength ){
            // using for loop to copy the last patternlength -1  elements from previous array to the new array
            for(int j = 0; j< tempLength ;j++){
                input[j] = temp[j]; 
            }
            i += patternLength-1; // adjusting the index accordingly
            *temp = '\0'; // resetting the temporary array
        }
        // taking input one char at a time until our array reaches a max length of 499 chars
        for(; !feof(file) && (i<ARRAY_SIZE-1) ; i++){
            input[i] = fgetc(file); // taking input to an array
        }
        input[i] = '\0'; // setting the end of text file for the array

        // here we are calling our 3 different algorithms to search the given pattern in the given array
        // we also time the different algorithms and add their time to their counter respectively
        gettimeofday(&timer1, NULL);// taking time before the algorithm start
        bruteForceOccurence += bruteForce(input,  pattern); // calling the algorithm
        gettimeofday(&timer2, NULL); // taking the time after the algorithm finishes
        // substracting the time before and after the algorithm and adding it to it's time counter
        bruteForceTime += ((timer2.tv_sec-timer1.tv_sec) * 1000000) + timer2.tv_usec - timer1.tv_usec; // using seconds and nanoseconds to find the time passed
        // here we do the same as above with a different algorithm
        gettimeofday(&timer1, NULL);
        horspoolsOccurence += horspools(input, pattern);
        gettimeofday(&timer2, NULL);
        horspoolTime += ((timer2.tv_sec-timer1.tv_sec) * 1000000) + timer2.tv_usec - timer1.tv_usec;
        // here we do the same as above with a different algorithm
        gettimeofday(&timer1, NULL);
        boyerOccurence += Boyer_Moore_Alg(pattern, input, goodSuffixTable);
        gettimeofday(&timer2, NULL);
        boyerTime += ((timer2.tv_sec-timer1.tv_sec) * 1000000) + timer2.tv_usec - timer1.tv_usec;
        // here we call our marker to mark the pattern
        bruteMarker(input,pattern,piece++);
        // here we store the last patternlength -1 elements of the array on a temporary array so that we don't skip a pattern while dividing the input
        if(!feof(file)){
            i -= patternLength-1;
            for(int j = 0; i<ARRAY_SIZE ; i++, j++){ // copying to the temp array
                temp[j] = input[i];
            }
            input[--i] = '\0'; // setting the end of text char
        }

    }
    fclose(file);
    
    // here we print the occurence, number of comparisons and times of each algorithm
    printf("Horspool occurence: %d Number of comparisons: %lli Time(ms): %.6f\n", horspoolsOccurence, horspoolComparison, (horspoolTime/1000.0) );
    printf("Brute force occurence: %d Number of comparisons: %lld Time(ms): %.6f\n", bruteForceOccurence,bruteForceComparison, (bruteForceTime/1000.0) );
    printf("Boyer-Moore algorithm occurence: %d Number of comparisons: %lld Time(ms): %.6f\n", boyerOccurence, boyerComparison, (boyerTime/1000.0) );
    // Opening a new file to read from the beginning
    FILE *markedFile = fopen(filePath,"r");
    markFile(head,patternLength,markedFile,output);

    return 1;
    
}
