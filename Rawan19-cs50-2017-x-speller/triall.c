#include <stdbool.h>
#include<stdio.h>
#include<string.h>
#include<cs50.h>
#include <stdbool.h>
#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<strings.h>
#include<ctype.h>
#define LENGTH 45

#define HASHTABLE_SIZE 65536 //2^16
#define DICTIONARY "dictionaries/small"

  bool load( const char *dictionary);
int hash_it(const char* needs_hashing);
bool check(const char *word);
unsigned int size(void);
bool unload(void);

char word[LENGTH+1];

//for calculating the size
 unsigned int loaded;
//define a nodeof type struct
    typedef struct node{
        //each node contains a word
        char word[LENGTH+1]; 
        //and a pointer to the next node
        struct node*next; 
        //and just call it node for short
    }node;
    
    //create the dictionary, an array of nodes(of type node),  can I put number of words in the dic instead of HASHTABLE_SIZE??
    node *hashTable[HASHTABLE_SIZE];
    //define all variables globally...
    
    //int hashCodes[5];
    //int j=0;
    
int main(){
    
    bool a= load( DICTIONARY);
    printf("%d\n",a);
    
    //check
    char*wordd="ACM";
    bool b=check(wordd);
    printf("%d\n",b);
    
    //size
    unsigned int n=size();
    printf("%d\n",n);
    
    
    
    return 0;
}

    
   bool load( const char *dictionary)
{
     //an array that holds the hash codes generated
   // int hashCode[3];
   // int i=0;
     
    //open the dictionary file
    FILE *fp = fopen(dictionary, "r");
    if (fp==NULL)
    printf("couldn't filethe text file");
    
    //define all your variables outside the while loop
      int index;
    
    node*head=malloc(sizeof(node)); 
    //for every word that we scan,in the file, line by line? it could be made with fgets though
    while (fscanf(fp, "%s", word)!=EOF){
        
        //malloc a node pointer for it
        node*new_node=malloc(sizeof(node));
        
        //if the heap is full or I can't get this pointer
        if(new_node==NULL){
            //free it
            printf("NULL!");
            unload();
            return false;
        }
        else{
        
        //copy word from file into the varibale"word", located in the node of our linked list
        strcpy(new_node->word,word);
        
        }
        // call the hash function to get the integer hash value(the index of a bucket in the # table)
         index=hash_it(new_node->word);
        
        //store the hash code in the array
        //hashCode[i]=index;
       // i++;
        
        node*head=hashTable[index]; //the name of the array is a pointer to the first elemnet in it "or just =new_node->word"
        
        //insert the node into the linked list..
        new_node->next=head;
        head=new_node;
        
        //inserting the node in the # table
        hashTable[index]=head;
      
        printf("%s : %d\n",head->word, index); //maybe the node isn't put correctly in the table???NO, it is correct
        loaded+=1;
        
        free(head);
     
    }


        printf("%s : %d\n",head->word, index); //maybe the node isn't put correctly in the table???
    return true;
}

//hash function, takes a string as an input
int hash_it( const char* needs_hashing) 
{
    unsigned int hash = 0;
    for (int i=0, n=strlen(needs_hashing); i<n; i++)
        hash = (hash << 2) ^ needs_hashing[i];
    //and returns a hash code for each string(word)
    return hash % HASHTABLE_SIZE;
}

bool check(const char *word)
{
    //passing the word by address
    int index=hash_it(word);
      node*head=malloc(sizeof(node));
      if(head==NULL)
      printf("NULL");
      
      head=hashTable[index]; //the name of the array is a pointer to the first elemnet in it ??
      
    node*cursor=head; 
    
 
 while(cursor!=NULL){
    
    //compare the word the user entered with the word that exists in the # table
    if(strcasecmp(word, hashTable[index]->word)==0){ //compare each word in the dic with the given word{
   
    return true;                          
    
    }
    else
    ////if you didn't find it, move it to the next node and conpare again(traversing the linked list)
    cursor=cursor->next;
    
    }
    //free(head);
    return false ;
}


//size
unsigned int size(void){
    

    return loaded;
    

}

bool unload(void){
   // int h=hashCode[j];
     //node*head=hashTable[h]; //the name of the array is a pointer to the first elemnet in it "or just =new_node->word"
     
     int i;
     
     //iterating through the hashtable 
     for(i=0;i<HASHTABLE_SIZE;i++){
         
         
         node*head=hashTable[i];
         
         //and checking if there's a value in each index
         while(head!=NULL){
             
             //free the linked list, node by node
             node*tmp=head;
             head=head->next;
            free(tmp);
         }
     }
     
    return true;
}