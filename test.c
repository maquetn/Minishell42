#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "minishell.h"

int ft_strslen(char **strs) {
    int i = 0;
    while (strs[i]) {
        i++;
    }
    return i;
}

void ft_arg(char **strs, int i) {
    int max_len = ft_strslen(strs);
    
    if (i >= max_len || i < 0) {
        printf("Invalid index\n");
        return;
    }

    int j = strlen(strs[i]);
    if (j <= 2) {
        printf("String length is less than or equal to 2\n");
        return;
    }

    int new_len = j - 2;
    if (new_len <= 0) {
        printf("Invalid new string length\n");
        return;
    }

    char* new_str = malloc((new_len + 1) * sizeof(char));
    if (new_str == NULL) {
        printf("Memory allocation failed\n");
        return;
    }

    strlcpy(new_str, strs[i] + 2, new_len); // Copy the substring excluding the first two characters
    new_str[new_len] = '\0'; // Null-terminate the new string

    free(strs[i]); // Free the original string

    strs[i] = new_str; // Assign the new string to the array
}

int main() {
    char *str = "hello>>world";
    char **strs;
    int i = 0;
    int j = 0;

    strs = ft_split(str, ' '); // Assuming ft_split is properly defined elsewhere

    while (strs[i]) {
        while (strs[i][j]) {
            if ((strs[i][j] == '>' && strs[i][j + 1] == '>') || 
                (strs[i][j] == '<' && strs[i][j + 1] == '<')) {
                ft_arg(strs, i);
                j++; // Increment j if modification occurs to skip the second character in >> or <<
            }
            j++;
        }
        j = 0;
        i++;
    }

    i = 0;
    while (strs[i]) {
        printf("%s\n", strs[i]);
        i++;
    }

    // Free allocated memory for strs and its elements
    i = 0;
    while (strs[i]) {
        free(strs[i]);
        i++;
    }
    free(strs);

    return 0;
}
