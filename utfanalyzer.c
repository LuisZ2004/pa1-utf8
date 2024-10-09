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
//firstfour function from class
char firstfour( char c){
	return c & 0b11110000;
}


int32_t width_from_start_byte(char start_byte){
    int c = firstfour(start_byte);
    //uses the first four function from class to check if the
    //first four characters are the identifiers
    if(c == 0b11000000){
        return 2;
    }
    else if(c == 0b11100000){
        return 3;
    }
    else if(c == 0b11110000){
        return 4;
    }
    else if((start_byte & 0b10000000) == 0){
        return 1;
    }

    return -1;
}

int32_t utf8_strlen(char str[]){
    
}



int main(){

    //all from the assignment used to test code
    printf("Is abcd ASCII? %d\n", is_ascii("abcd"));
    printf("Is 🔥 ASCII? %d\n", is_ascii("🔥"));

    int32_t ret = 0;
    char str[] = "abcd";
    ret = capitalize_ascii(str);
    printf("Capitalized String: %s\nCharacters updated: %d\n", str, ret);

    char s[] = "Héy"; // same as { 'H', 0xC3, 0xA9, 'y', 0 },   é is start byte + 1 cont. byte
    printf("Width: %d bytes\n", width_from_start_byte(s[1])); // start byte 0xC3 indicates 2-byte sequence


    return 0;
}