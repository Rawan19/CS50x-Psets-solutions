#include<cs50.h>
#include <stdio.h>
#include<math.h>

void mario();

int main(void){
    
    mario();  
    return 0;
}
    
void mario(){
    
    int height,i,j,k; 
    
    //an initial value to the number of "#" 
    int raw=1;         
    
    printf("Height:");
    
    //get the height of the pyramid from the user
    height=get_int();  
    
    //an initial value for d, which is the distance moved towards the right to make an alignment.
    int d= height-1;  
    
    if(0<height && height<=23){
    
        for(i=0;i<height;i++){
            
            //the number of "#" always increments by 1, when starting a new line
             raw+=1;   
    
            for (k=d; k>=1; k--){
            
               //the distances needed to right align the pyramid 
               printf(" ");  
     }
     
            for(j=1;j<=raw;j++){ 
                printf("#");
     }
       
            printf("\n");
        
            //at the beginning of each line, the distance decreases by a "1"
            d-=1;          
     }
     }
    else{
        printf("Oops! looks like you entered an invalid number!\n");
     //call mario function
     mario();                  
     }   }