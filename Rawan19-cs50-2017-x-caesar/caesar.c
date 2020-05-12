#include <cs50.h>
#include <stdio.h>
#include <stdlib.h>
#include<string.h>
#include<ctype.h>

int main(int argc, string argv[]){

int key=atoi(argv[1]) ;     //turn key into integer
int alphaLetter,i;
int ciphered[30];           //the ciphered text
if(argc!=2 || key<0){
    printf("make sure you entered a positive key number, and only 2 command line arguments!");
    return 1;
}
else {
printf("plaintext:");        //prompt for plain text
string plainText= get_string();

for(i=0;i<strlen(plainText);i++){
    if (isalpha(plainText[i])){
        if(isupper(plainText[i])){
           alphaLetter= plainText[i]-65;        //convert ASCII to Alpha index
           ciphered[i]= (alphaLetter + key)%26; //ceaser
           ciphered[i]=ciphered[i] +65;         // convert Alpha back to ASCII
     }
        else{
            alphaLetter= plainText[i]-97;        //convert ASCII to Alpha
            ciphered[i]= (alphaLetter + key)%26; //ceaser
            ciphered[i]=ciphered[i] +97;         // convert Alpha back to ASCI
        }}
    else{
        ciphered[i]=plainText[i];            //if the input isn't a letter "!, etc", print it as it is
        }
    }
printf("ciphertext: ");
for(i=0;i<strlen(plainText);i++){
     printf("%c", ciphered[i]);
}

}

printf("\n");
return 0;
}