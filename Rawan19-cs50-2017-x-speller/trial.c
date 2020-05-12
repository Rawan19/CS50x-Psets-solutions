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


/**
 * Loads dictionary into memory. Returns true if successful else false.
 */
 //define a nodeof type struct
    typedef struct node{
        //each node contains a word
        char word[LENGTH+1]; 
        //and a pointer to the next node
        struct node*next; 
        //and just call it node for short
    }node;
    
    //create the dictionary, an array of nodes(of type node),  can I put number of words in the dic instead of HASHTABLE_SIZE??
    node*hashTable[HASHTABLE_SIZE];
    
 int hash_it(const char* needs_hashing) ;
  bool check(const char *word);
 char word[LENGTH+1];
 
int main(){
    char*dictionary="small.txt";
    bool a= load( char *dictionary);
    return 0;}
   bool load( char *dictionary)
{
     //an array that holds the hash codes generated
   // int hashCode[3];
   // int i=0;
     //define a nodeof type struct
    typedef struct node{
        //each node contains a word
        char word[LENGTH+1]; 
        //and a pointer to the next node
        struct node*next; 
        //and just call it node for short
    }node;
    
    //create the dictionary, an array of nodes(of type node),  can I put number of words in the dic instead of HASHTABLE_SIZE??
    node*hashTable[HASHTABLE_SIZE]; 
    
    //open the dictionary file
    FILE *fp = fopen("dictionaries/small", "r");
    if (fp==NULL)
    printf("couldn't filethe text file");
    
    //for every word that we scan,in the file, line by line? it could be made with fgets though
    while (fscanf(fp, "%s", word)!=EOF){
        
        //malloc a node pointer for it
        node*new_node=malloc(sizeof(node));
        
        //if the heap is full or I can't get this pointer
        if(new_node==NULL){
            //free it
            unload();
            return false;
        }
        else{
        
        //copy word from file into the varibale"word", located in the node of our linked list
        strcpy(new_node->word,word);
        
        }
        // call the hash function to get the integer hash value(the index of a bucket in the # table)
        int index=hash_it(new_node->word);
        
        //store the hash code in the array
        //hashCode[i]=index;
       // i++;
        
        node*head=hashTable[index]; //the name of the array is a pointer to the first elemnet in it "or just =new_node->word"
        
        //insert the node into the linked list..
        new_node->next=head;
        head=new_node;
        printf("%s : %d\n",head->word, index); //maybe the node isn't put correctly in the table???
        
    return true;
}
//means: go to hashtable[Ex:12], look at the value there, it's of type node, so you'll find :word &node*next, get me the value of word.

//now to check:

char *wordiz=argv[1];
printf("%s\n",wordiz);


bool misspelled = check(wordiz);
if(misspelled==true)
printf("word is found!");
else
printf("wrong dict!");
    return 0;
}
/**
 * Hash function via reddit user delipity:
 * https://www.reddit.com/r/cs50/comments/1x6vc8/pset6_trie_vs_hashtable/cf9nlkn
 */
 
 //hash function, takes a string as an input
int hash_it(const char* needs_hashing) 
{
    unsigned int hash = 0;
    for (int i=0, n=strlen(needs_hashing); i<n; i++)
        hash = (hash << 2) ^ needs_hashing[i];
    //and returns a hash code for each string(word)
    return hash % HASHTABLE_SIZE;
}

 bool check(const char *word)
{
    int index=hash_it(word);
    int i;
    
    node*head=hashTable[index]; //the name of the array is a pointer to the first elemnet in it 
    node*cursor=head; 
    for(i=0;i<strlen(word);i++){
        
        //preserve case
        if(isupper(word[i]))
        return false;
    }
       
    while(cursor!=NULL){
    
    //compare the word the user entered with the word that exists in the # table
    if(strcasecmp(word, hashTable[index]->word)==0) //fix here!!
    
    return true;
    else
    return false;
    
    cursor=cursor->next;
    }
    return false;
}

