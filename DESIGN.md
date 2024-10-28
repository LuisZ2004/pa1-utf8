
/////////////////////////////old questions////////////////////////////////////
If all characters in UTF-32 are 4 bytes then characters that do not need all 4 bytes are going to be using memory for charcters that do not require the amount allocated. For example 1 could just be 00000001 (1 byte) in utf-8 or but it would be 00000000 000000000 00000000 00000001 in utf-32. However it may be easier to read utf-32 because if all characters are 32 bits then there is no need to have identifier bits, they would all be from the same library. Another benefit is that the number of characters would also increase because the bits previously used as idenifier bits could now be used as different characters.

The lack of the leading 10 would make it harder to identify where in the string you were. You could be on a single character or in the middle of a multibyte character. I think that the leading one is useful for recognizing this and also recognizing that it is infact a charcter and not a number. 
//////////////////////////////////////////////////////////////////////////////


New Questions:

Consider the 3-byte sequence 11100000 10000000 10100001. Answer the following questions:

What code point does it encode in UTF-8, and what character is that?
Breaking this 3 bte sequence down, 11100000, has a leading 1110 which means that this is a 3 byte character, the following two bytes lead with 10 which means that are continutation bits. Removing the identifiers, the significant bits are 0000, 000000 and 100001 respectively. combining these bits 0000000000100001, after removing the leading 0's, gives us 0x21 which is the character '!'. 

What are the three other ways to encode that character?
The first byte, 11100000, can be read as E0, the second, 10000000 as 80, and the last A1. Using the information from above, 0000000000100001 are the significant bits, converting this to a two byte character, a leading 110 is required for the first byte of a two byte character. A leading 10 is required for a continutation character. This leaves us with 11 bits for the two byte character.
 Therefore the two byte representation would be 11000000 10100001 or 0xc0a1. Repeating this for the 4 byte representation, the bytes would be 11110000 100000000 100000000 10100001 which would be 0xf08080a1. The original and 3 other ways of encoding this character are 0x21, 0xc0a1 ,0xe080a1, 0xf08080a1.

Give an example of a character that has exactly three encodings (but not four, like the one in the previous example does)




What are some problems with having these multiple encodings, especially for ASCII characters? A web search for “overlong UTF-8 encoding” may be useful here.
