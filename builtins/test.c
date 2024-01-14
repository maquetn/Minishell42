#include <stdio.h>
#include <string.h>

int main() {
    // Input string
    const char *inputString = "Hello, World!";
    
    // Character to exclude
    char charToExclude = 'o';

    // Find the position of the character in the string
    char *charPosition = strchr(inputString, charToExclude);

    // Copy the string without the character at the found position
    char outputString[256];  // You may adjust the size as needed
    if (charPosition != NULL) {
        strncpy(outputString, inputString, charPosition - inputString);
        outputString[charPosition - inputString] = '\0';  // Null-terminate the output string
        strcat(outputString, charPosition + 1);  // Concatenate the rest of the string
    } else {
        strcpy(outputString, inputString);  // If character not found, copy the whole string
    }

    // Print the result
    printf("Original String: %s\n", inputString);
    printf("String without '%c' at strchr position: %s\n", charToExclude, outputString);

    return 0;
}