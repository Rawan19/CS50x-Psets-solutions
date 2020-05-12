/**
 * helpers.c
 *
 * Helper functions for Problem Set 3.
 */
 
#include <cs50.h>
#include<math.h>
#include "helpers.h"

/**
 * Returns true if value is in array of n values, else false.
 */
 
 //the problem is in the search function, pin and paper and do it
bool search(int value, int values[], int n)
{
    // implement a searching algorithm
    int newArray[n];
    int i,j=0;
    //int m=0;
    
    //in case of an array with an odd number of elements
    int r=round((n)/2);
    
    //if r "the new number of elements in the array" is even(considering that we start counting from index 0)
    if(r%2==0)         
    r=r-1;
    
    //before you start binary search, make sure you don't create an array of size 0; that r isn't rounded to 0(if n=1), to avoid seg. fault
    if(r>0){
        
    //the index of the number in the middle
    int mid=r;      
    if(n<0)
    return false;
    if(values[mid]==value)
        return true;
        
    //Binary search
    else if(value>values[mid]){     
         
        for(i=mid;i<n;i++){
            newArray[j]=values[i];
            j++;
        }
        
        //if I end up with an array of 1 element"needle"
        if(newArray[0]==value)  
           return true;
        else{
        
            n=n-r;
            return search( value,  newArray, n);
        }}
    else if(value<values[mid]){
                j=0;
                
                for(i=0;i<mid;i++){
                    newArray[j]=values[i];
                    j++;
        }
                //if I end up with an array of 1 element"needle"
                if(newArray[0]==value)  
                   return true;
                 else {
                    n=n-r;
                     //recursion
                    return search( value,  newArray, n);
        }
    }          
               
    }
    
    return false;       
    
}

/**
 * Sorts array of n values.
 */
void sort(int values[], int n)
{
    // implement a sorting algorithm
    int min=values[0];
    int i,j;
    
     //selection sort, for i from 0 to n-1
    for(i=0;i<n;i++){   
         //nested for loop,compare each element with all the other elements
            for(j=i;j<n;j++){             
            
            //find the smallest element
                if( values[i]> values[j+1]){ 
            //swap
            min= values[j+1];                  
            values[j+1]= values[i];
            values[i]=min;
            }
        }
    }
   
}
