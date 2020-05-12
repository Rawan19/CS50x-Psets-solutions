#include <cs50.h>
#include <stdio.h>
#include <stdlib.h>
#include<string.h>
#include<ctype.h>

int main(int argc, string argv[]){
    string key=argv[1];
    int alphaLetter;
    int ciphered[32];           //the ciphered text
    int i,j=0;
    for(i=0;i<strlen(key);i++){
        if(!isalpha(key[i]))  {   //key is a string
        printf("Please make sure you entered an alphabetical key");
        return 1;
    }}

    if(argc!=2 ||strlen(key)<1 ){
    printf("make sure you typed 2 command line arguments, and a string key!");
    return 1;
}
 else {
printf("plainText:");        //prompt for plain text
string plainText= get_string();

int keyy=0;

  for(i=0;i<strlen(plainText);i++){
    if (isalpha(plainText[i])){
        if(isupper(plainText[i])){
           if(j==strlen(key))                 //if you reach the last letter, back to the start, and so on(wraparound)
           j=0;
           alphaLetter= plainText[i]-65;        //convert ASCII to Alpha
           if(isupper(key[j])){
            keyy=key[j]-65;                    //convert key ASCII into alpha
           }else{
            keyy=key[j]-97;
           }ciphered[i]= (alphaLetter + keyy)%26; //ceaser
           ciphered[i]=ciphered[i] +65;         // convert Alpha back to ASCII
           j++;
     }
        else{
            if(j==strlen(key))                 //if you reach the last letter, back to the start, and so on(wraparound)
            j=0;
            alphaLetter= plainText[i]-97;        //convert ASCII to Alpha
           if(isupper(key[j])){
            keyy=key[j]-65;                    //convert key ASCII into alpha
           }else{
            keyy=key[j]-97;
            }ciphered[i]= (alphaLetter + keyy)%26; //ceaser
            ciphered[i]=ciphered[i] +97;         // convert Alpha back to ASCI
            j++;
        }}
    else{
        ciphered[i]=plainText[i];            //if the input isn't a letter "!, etc", print it as it is
        }
    }
printf("ciphertext:");
for(i=0;i<strlen(plainText);i++){
     printf("%c", ciphered[i]);
}
printf("\n");
}
return 0;
}