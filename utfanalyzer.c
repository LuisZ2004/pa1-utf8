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
    int chars_capped = 0;
    for (int i = 0; i < num; i++){
        //if the ascii value is between 96 and 123 (lowercase letters)
        //then it subtracts 32 to capitalize
        //also adds one if the letter was capitalized
        if((uint8_t)str[i] < 123 && (uint8_t)str[i] > 96 ){
            str[i] -= 32;
            chars_capped +=1;
        }
    }
    //prints the string that is now capitalized
    printf("%s", str);
    //returns the number of characters capitalized
    return chars_capped;
}
//firstfour function from class
int32_t firstfour( char c){
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
    //finds the length of the string
    int c = strlen(str);
    //get the number of codepoints
    int num_code_points = 0;
    for (int i=0; i < c; i++){
        int character = firstfour(str[i]);
        //checks if the it is a standalone byte
        if ((str[i] & 0b10000000) == 0) {
            num_code_points += 1;
        }
        //checks if its a 2 byte char
        else if(character == 0b11000000){
            num_code_points +=1;
            //adds one to i so it skips the next one
            i += 1;
        }
        //checks if its a 3 byte char
        else if(character == 0b11100000){
            num_code_points +=1;
            //skips the next 2
            i += 2;
        }
        //checks if its a 4 byte char
        else if(character == 0b11110000){
            num_code_points +=1;
            //skips the next 3
            i += 3;
        }
        //if it is not recognized it returns -1
        else{
            return -1;
        }
    }
    return num_code_points;
}

int32_t codepoint_index_to_byte_index(char str[], int32_t cpi){
    int byte_index = 0;
    for(int i = 0; i < cpi; i++){
        int character = firstfour(str[i]);
        //checks if the it is a standalone byte
        if ((str[i] & 0b10000000) == 0) {
            byte_index += 1;
        }
        //checks if its a 2 byte char and adds 2 to the byte index
        else if(character == 0b11000000){
            byte_index +=2;
            //adds one to i so it skips the next one
            i += 1;
        }
        //checks if its a 3 byte char and adds 3 to the index
        else if(character == 0b11100000){
            byte_index +=3;
            //skips the next 2
            i += 2;
        }
        //checks if its a 4 byte char
        else if(character == 0b11110000){
            byte_index +=4;
            //skips the next 3
            i += 3;
        }
        //if it is not recognized it returns -1
        else{
            return -1;
        }
    }
    
    return byte_index;
}


void utf8_substring(char str[], int32_t cpi_start, int32_t cpi_end, char result[]){
    int byte_index_start = codepoint_index_to_byte_index(str,cpi_start);
    int byte_index_end = codepoint_index_to_byte_index(str,cpi_end);
    int end = byte_index_end - byte_index_start;
    if(cpi_start >= 0 && cpi_end >= 0 && cpi_start < cpi_end){
        for (int i = 0; i < end; i++) {
            result[i] = str[byte_index_start + i];
        }
        result[end] = '\0';
    }
    else{
        result[0] = '\0';
    }
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
    char stri[] = "Joséph";
    printf("Length of string %s is %d\n", stri, utf8_strlen(stri));  // 6 codepoints, (even though 7 bytes)

    char strig[] = "Joséph";
    int32_t idx = 4;
    printf("Codepoint index %d is byte index %d\n", idx, codepoint_index_to_byte_index("Joséph", idx));

    char result[17];
    utf8_substring("🦀🦮🦮🦀🦀🦮🦮", 3, 7, result);
    printf("Substring: %s\n", result); // these emoji are 4 bytes long

    return 0;
}