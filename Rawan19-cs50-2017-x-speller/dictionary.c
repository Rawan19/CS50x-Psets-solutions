#include <stdbool.h>
#include "dictionary.h"

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

 //define a nodeof type struct globally
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
 int n=0;

 //for calculating the size
 int loaded;

 bool load( const char *dictionary)
{
    //open the dictionary file
    FILE *fp = fopen(dictionary, "r");
    if (fp==NULL)
    printf("couldn't filethe text file");

    //for every word that we scan,in the file, line by line? it could be made with fgets though
    while (fscanf(fp, "%s", word)!=EOF){

        //malloc a node pointer(of type:node) for it
        node*new_node=malloc(sizeof(node));

        //if the heap is full or I can't get this pointer
        if(new_node==NULL){
            //free it
            unload();
            return false;
        }
        else{

        //copy word from file into the variable"word", located in the node of our linked list
        strcpy(new_node->word,word);

        }
        // call the hash function to get the integer hash value(the index of a bucket in the # table)
        int index=hash_it(new_node->word);

        if(hashTable[index]==NULL){
           hashTable[index]=new_node;
           new_node->next=NULL;
           // printf("%s : %d\n",hashTable[index]->word, index); worked!
       }
       else{
        //a # table is an array of linked lists, each element(hashTable[index]) ia a pointer to the first node, in thelinked list
        //insert the node into the linked list..
        new_node->next=hashTable[index];
        hashTable[index]=new_node;
       }
       loaded+=1;

    }
    fclose(fp);
    return true;
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
    int n=strlen(word);
    int i;
    char temp[n];
    strcpy(temp,word);

    //tolower can't be used with const char, so copy word to alter it
    for(i=0;i<n;i++){
        if(isupper(temp[i])){
            temp[i]=tolower(temp[i]);
        }
    }
    int index=hash_it(temp);
    node*cursor=hashTable[index];

    while(cursor!=NULL){

    //means: go to hashtable[Ex:12], look at the value there, it's of type node, so you'll find :word & node*next, get me the value of word.
    //compare the word from the txt file with the word that exists in the # table linked list;
    if(strcasecmp(temp, cursor->word)==0){

      return true;

    }
    //if you didn't find it, move the cursor to the next node and compare again
    cursor=cursor->next;

    }
    return false;
}

//size
unsigned int size(void){


    return loaded;
}

bool unload(void){

    int i;

    //iterating through the hashtable elements
    for(i=0;i<HASHTABLE_SIZE;i++){



        //and checking if there's a value in each index
        while(hashTable[i]!=NULL){

            //for each element(linked list) in # table: free the linked list, node by node
            node*tmp=hashTable[i];
            hashTable[i]=hashTable[i]->next;
            free(tmp);
        }
    }

    return true;
}




