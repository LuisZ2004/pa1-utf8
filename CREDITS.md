https://en.wikipedia.org/wiki/List_of_emojis wikipedia for the unicode for emojis

https://stackoverflow.com/questions/5736402/how-can-i-overwrite-an-array-of-chars-aka-a-string-with-a-new-array-of-chars how to change the true/false char array

i used the firstfour function from class in this assignment

for my codepoint_at function i used some code from https://www.geeksforgeeks.org/c-program-to-determine-the-unicode-code-point-at-a-given-index/

however it only worked for single byte characters, I asked chatgpt why it was not working for special charcters and it gave me the code I used in the assignement.

I also asked chatgpt to explain how the left shifting works and how the code would read 🐱 and know what the codepoint was

it removes the identifier bits, reads the important part and then saves the value by left shifting the bits into codepoint which is a 32 bit int and then repeats it for the following bytes.

also for my capitalize ascii function i had written it almost exactly how it is now except when I ran it it did not work. I asked chatgpt and it told me to use unsigned ints and this is because ascii characters are unsigned
