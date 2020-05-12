#include <stdio.h>
#include <stdlib.h>
#include <cs50.h>
#include <stdint.h>
#include<inttypes.h>

int main(int argc, char *argv[])
{
    if(argc!=2){
        
        fprintf(stderr, "Usage: ./copy infile outfile\n");
        return 1;
    }
    char*card=argv[1];
     //allocate 512 ,of data type:, bytes from memory, each time, I'll read 512 bytes into an array called buffer 
    uint8_t buffer[512];
   
   int a=0;
    
    //keep track of the number of JPEG's you find
    int count=0;
    
    //open card file
    FILE *cardRaw=fopen(card,"r");
    
    if(cardRaw==NULL){
    fprintf(stderr, "Couldn't open card/raw\n");
    return 2;
    }
    
    //do I need this line of code?
    //unsigned char *buffer;
    
    
    
    char *filename=malloc(8);
    
    //define a pointer to the JPEG img file
    FILE*img=NULL;
    
    //!feof(cardRaw)
    while(fread(buffer, 512, 1,cardRaw)==1){  
         
        
        //if it's the beginning of a new JPEG
        if(buffer[0]==0xff && buffer[1]==0xd8 && buffer[2]==0xff && (buffer[3] & 0xf0)==0xe0){ 
            
        //if a file is opened,if it's not the first JPEG
         if (img!=NULL){ 
            fclose(img);
            
         }
                      
            sprintf(filename,"%03d.jpg",count);
           // printf("%s",filename);
            count+=1;
            
            
            //open a new JPEG file
            img=fopen (filename,"w");
            
            fwrite(buffer, 512,1,img);
            //indicating you found a JPEG
            a=1;
          
            
        //keep writing to the new imgfile untill you find the next JPEG
       
        
         } //if those 512 bytes are not the beginning of a JPEG
        else {
            if(a==1){
                
                //then it belongs to the last opened img,;filename is a global variable, so this loop can see it and append to the file
         // img=fopen (filename,"a");
          //write those 512 bytes to the JPEG file
          fwrite(buffer, 512,1,img); 
            }
             
        }
        
    }
    
    
     
    free(filename);
    fclose(cardRaw);
    fclose(img);
    
    return 0;
}
