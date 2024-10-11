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
        //returns the length based on the first four bits
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
    //start and end of the byte indexes 
    int byte_index_start = codepoint_index_to_byte_index(str,cpi_start);
    int byte_index_end = codepoint_index_to_byte_index(str,cpi_end);
    //end of the substring
    int end = byte_index_end - byte_index_start;
    
    //checks to make sure that cpi start is less than end and both are positive
    if(cpi_start >= 0 && cpi_end >= 0 && cpi_start < cpi_end){
        for (int i = 0; i < end; i++) {
            //rewrites result to be the substring starting from the byteindexstart and adding one
            //as the loop goes
            result[i] = str[byte_index_start + i];
        }
        //ends the char array with the null terminator
        result[end] = '\0';
    }
    else{
        //empty string if the conditions are not met
        result[0] = '\0';
    }
}

int32_t codepoint_at(char str[], int32_t cpi){
    int32_t codepoint = 0;
    int byte_index = codepoint_index_to_byte_index(str,cpi);
    int32_t width = width_from_start_byte(str[byte_index]);

    //if the width is 1 then return the codepoint of just that one
    if(width == 1){
        return str[byte_index];
    }
    else if(width == 2){
        //if the width is 2 then remove the first 3 bits then left shift the bits 6 spaces for the first byte
        //does the same thing for the second byte
        //the bitwise or | combines it into 11 bits 
        codepoint = ((str[byte_index] & 0b00011111) << 6) | (str[byte_index + 1] & 0b00111111);
        return codepoint;
    }
    else if(width == 3){
        codepoint =((str[byte_index] & 0b00001111) << 12) | ((str[byte_index + 1] & 0b00111111) << 6) |  (str[byte_index + 2] & 0b00111111);
        return codepoint;
    }
    else if(width == 4){
        codepoint = ((str[byte_index] & 0b00000111) << 18) |
                            ((str[byte_index + 1] & 0b00111111) << 12) |
                            ((str[byte_index + 2] & 0b00111111) << 6) |
                            (str[byte_index + 3] & 0b00111111);
        return codepoint;
    }
    return -1;
}

char is_animal_emoji_at(char str[], int32_t cpi){
    int32_t codepoint = codepoint_at(str,cpi);
    //if the codepoint is between the rat and chipmunk emoji or crab and dog then return true
    if ((codepoint >= 0x1F400 && codepoint <= 0x1F43E) || (codepoint >= 0x1F980 && codepoint <= 0x1F99F)) {
        return 1;
    }
    //false by default
    return 0;
    
}



int main(){ 
    char get_input_string[] = "Enter a UTF-8 Encoded String: ";
    char input_string[100];
    printf("%s",get_input_string);
    fgets(input_string,100,stdin);
    
    //checks and removes the new line character that is used when code is being typed in terminal
    int8_t length = strlen(input_string);
    if(length > 0 && input_string[length-1] == '\n'){
        input_string[length - 1] ='\0';
    }
    
    //by defualt it is not ascii and if it is then set it to true
    //this changes the number value to a string
    char is_true[6] = "false";

    if(is_ascii(input_string) != 0){
        strcpy(is_true, "true");
    }
    printf("Valid ASCII: %s\n",is_true);


    capitalize_ascii(input_string);
    printf("Uppercased ASCII: %s\n",input_string );

    //byte_length is used a couple times for loops so having it set improves efficiency a little
    int byte_length = strlen(input_string);
    printf("Length in bytes %d\n", byte_length);
    int32_t utf8_string_length = utf8_strlen(input_string);
    printf("Number of Codepoints: %d\n",utf8_strlen(input_string));

    int byte_index = 0;
    printf("Bytes per codepoint: ");
    //iterates throughout each codepoints and and finds the width
    //adds the value of width in case it was a multibyte character
    for (int i = 0; i< utf8_string_length; i++){
        int width = width_from_start_byte(input_string[byte_index]);
        printf("%d ", width);
        byte_index += width;
    }
    printf("\n");

    //creates a substring
    char substring [100];
    //will always get the first 6 codepoints
    utf8_substring(input_string,0,6, substring);
    printf("Substring of the first 6 code points: \"%s\"\n", substring);


    printf("Code Points as decimal numbers: ");
    for (int i = 0; i < utf8_string_length; i++){
        //codepoints are returned as 32 bit int so nothing else is needed
        printf("%d ",codepoint_at(input_string, i));
    }
    printf("\n");

    printf("Animal Emojis: ");
    for (int i = 0; i < utf8_string_length; i++){
        //checks if it is animal emoji
        if(is_animal_emoji_at(input_string,i)){
            //4bits for emoji and last for terminator
            char emojis[5];
            //substring writes the emoji into the emoji[] from the inputstring
            utf8_substring(input_string,i, i+1, emojis);
            printf("%s", emojis);
        }
    }
    printf("\n");
    return 0;
}