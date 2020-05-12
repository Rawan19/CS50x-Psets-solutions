#include<cs50.h>
#include <stdio.h>
#include<math.h>
//let's get greedy!
int getCoins();
int main(void){
    
    int count= getCoins();
    
    printf("%d \n", count);      //print the final (least possible) number of coins, that we used
}

int getCoins(){
    double cents=0;
    int count=0;
printf("How much change is owed?");               //the user tells us how much change we owe them

float dollars= get_float();  //let's say he enters .4123 $
    
if(dollars>0){
    cents= dollars * 100;    //convert it to cents 41.23
    cents=round(cents);      //round it:41.00
    
//printf ("%.2f \n", cents);
    
    cents= (int)cents;           //Typecasting double into int: 41
    
    while (cents-25>=0){          //start by the largest coin possible, can we use a quarter? 41-25=16, so, yes.
    count ++;                    //count=1
     cents-=25;
}
while(cents-10>=0){               //can we use a dime? 16-10=6>0: yes.
    count ++;                    //count=2
     cents-=10;
}
while(cents-5>=0){               //can we use a nickel? 6-5=1. yes
    count ++;                   //count=3
     cents-=5;
}
while(cents-1>=0){              //can we use a penny? 1-1=0: yes.
    count ++;                   //count=4
     cents-=1;
}
    
   }
else{
    printf("please enter a positive number!"); 
    getCoins();                                //using recursion toallow the user to enter a valid number
}
return count;
}
    
   