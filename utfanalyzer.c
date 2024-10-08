#include <stdint.h>
#include <stdio.h>
#include <string.h>

int32_t is_ascii(char str[]){
    //gives num the value of the length of the string for the loop
    int num = strlen(str);
    //iterates over the string
    for (int i = 0; i < num; i++){
        //if the ascii value is greater than or equal to 128 then return false
        if((uint8_t)str[i] >= 128){
            return 0;
        }
    }
    //returns true if the loop never indicates false
    return 1;
}

int32_t capitalize_ascii(char str[]){
    int num = strlen(str);
    int charsCapped = 0;
    for (int i = 0; i < num; i++){
        //if the ascii value is between 96 and 123 (lowercase letters)
        //then it subtracts 32 to capitalize
        //also adds one if the letter was capitalized
        if((uint8_t)str[i] < 123 && (uint8_t)str[i] > 96 ){
            str[i] -= 32;
            charsCapped +=1;
        }
    }
    //prints the string that is now capitalized
    printf("%s", str);
    //returns the number of characters capitalized
    return charsCapped;
}



int main(){

    //all from the assignment used to test code
    printf("Is abcd ASCII? %d\n", is_ascii("abcd"));
    printf("Is 🔥 ASCII? %d\n", is_ascii("🔥"));

    int32_t ret = 0;
    char str[] = "abcd";
    ret = capitalize_ascii(str);
    printf("Capitalized String: %s\nCharacters updated: %d\n", str, ret);


    return 0;
}