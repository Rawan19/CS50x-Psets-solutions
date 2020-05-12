#include <cs50.h>
#include <stdio.h>
#include <ctype.h>
#include<string.h>

int main(void){


    int i;
    int s=0;                        //s is a counter for spaces

    char *name=get_string();
    char *initials=(char*)malloc(5); //initial memory, size of char is one byte

    if(name==NULL)
       return 1;
    else{
     int count=0;
     for(i=0; i<=strlen(name);i++){
        if ( isalpha(name[i]) || name[i]==' ' )        //making sure the user only enters characters and spaces
            count++;

        if(name[i]==' ')
        s+=1;

                }

     if(count==strlen(name) && s!=0 ){            //s, for making sure the user entered more than 1 name(at least 1 space)
        initials[0]=toupper(name[0]);
        int j=1;                    //j representsthe number of the initialized letters,starting from 1, the second character
        for(i=1; i<strlen(name);i++){
            if(name[i]==' '){
                initials[j]=toupper(name[i+1]);
                j++;
                }}

        for(j=0;j<strlen(initials);j++)
               printf("%c", initials[j]);

            printf("\n");
                }

        else {
                printf("Please enter a valid name: Only characters and spaces!");
                name=get_string();
        }
        free(initials);  //returning the borrowed heap memory back to the O.S

    }
    return 0;
}