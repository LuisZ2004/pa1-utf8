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


int32_t width_from_start_byte(char start_byte) {
    //checks the identifier bits of the byte and returns the width
    //changed from previous where it was only called once for debugging purposes
    if ((start_byte & 0b10000000) == 0) {
        return 1;
    } else if ((start_byte & 0b11100000) == 0b11000000) {
        return 2;
    } else if ((start_byte & 0b11110000) == 0b11100000) {
        return 3;
    } else if ((start_byte & 0b11111000) == 0b11110000) {
        return 4;
    }
    //if it was not recognized return -1
    return -1; 
}

int32_t utf8_strlen(char str[]){
    //finds the length of the string
    int byte_length = strlen(str);
    //get the number of codepoints
    int num_code_points = 0;
    for (int i=0; i < byte_length; i++){
        int width = width_from_start_byte(str[i]);
        if(width > 0){
            //if width is greater than 0 then adds one to the number of codepoints
            // also adds width -1 to i to iterate and skip some of the helper bytes
            num_code_points ++;
            i += (width -1);
        }
    }
    return num_code_points;
}

int32_t codepoint_index_to_byte_index(char str[], int32_t cpi) {
    int byte_index = 0;
    for (int i = 0; i < cpi; i++) {
        int width = width_from_start_byte(str[byte_index]);
        if (width < 1){
            return -1;
        }
        //moves to the next codepoint
        byte_index += width; 
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
        int i =0;
        for(int j =byte_index_start; j < byte_index_end; j++){
            result[i] = str[j];
            i++;
        }
        result[i] = '\0';
    }
    else{
        result[0] = '\0';
    }
}

int32_t codepoint_at(char str[], int32_t cpi){
    int32_t codepoint = 0;
    int byte_index = codepoint_index_to_byte_index(str,cpi);
    int32_t width = width_from_start_byte(str[byte_index]);

    if(width == 1){
        return str[byte_index];
    }
    else if(width == 2){
        codepoint = codepoint = ((str[byte_index] & 0b00011111) << 6) | (str[byte_index + 1] & 0b00111111);
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

int8_t is_animal_emoji_at(char str[], int32_t cpi){
    int32_t codepoint = codepoint_at(str,cpi);
    //if the codepoint is between the rat and chipmunk emoji or crab and dog then return true
    if ((codepoint >= 0x1F400 && codepoint <= 0x1F43E) || (codepoint >= 0x1F980 && codepoint <= 0x1F99F)) {
        return 1;
    }
    //false by default
    return 0;
    
}



int main(){ 
    char get_input_string[] = "Enter a UTF-8 encoded string: ";
    char input_string[100];
    printf("%s",get_input_string);
    fgets(input_string,100,stdin);
    
    //checks and removes the new line character that is used when code is being typed in terminal
    int8_t length = strlen(input_string);
    if(length > 0 && input_string[length-1] == '\n'){
        input_string[length - 1] ='\0';
    }

    char capped_string[100];
    strcpy(capped_string, input_string);
    
    //by defualt it is not ascii and if it is then set it to true
    //this changes the number value to a string
    char is_true[6] = "false";

    if(is_ascii(input_string) != 0){
        strcpy(is_true, "true");
    }
    printf("\nValid ASCII: %s\n",is_true);


    capitalize_ascii(capped_string);
    printf("Uppercased ASCII: \"%s\"\n",capped_string );

    //byte_length is used a couple times for loops so having it set improves efficiency a little
    int byte_length = strlen(input_string);
    printf("Length in bytes: %d\n", byte_length);
    int32_t utf8_string_length = utf8_strlen(input_string);
    printf("Number of code points: %d\n",utf8_strlen(input_string));

    int byte_index = 0;
    printf("Bytes per code point: ");
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


    printf("Code points as decimal numbers: ");
    for (int i = 0; i < utf8_string_length; i++){
        //codepoints are returned as 32 bit int so nothing else is needed
        printf("%d ",codepoint_at(input_string, i));
    }
    printf("\n");

    printf("Animal emojis: ");
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