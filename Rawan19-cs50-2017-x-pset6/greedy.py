import cs50

def main():
    
    count=getCoins()
    print("{}".format(count))
    
def getCoins():
    count=0
    #the user tells us how much change we owe them
    print("How much change is owed?")
    
    #let's say he enters .4123 $
    dollars= cs50.get_float()
    
    if dollars>0:
        #convert it to cents 41.23
        cents= dollars * 100
        
        #round it:41.00
        cents=round(cents)
        
        #start by the largest coin possible, can we use a quarter? 41-25=16, so, yes.
        while cents-25>=0:
            count+=1
            cents-=25
            
        #can we use a dime? 16-10=6>0: yes.
        while cents-10>=0:
            count+=1
            cents-=10
            
        #can we use a nickel? 6-5=1. yes
        while cents-5>=0:
            count+=1
            cents-=5
            
        #can we use a penny? 1-1=0: yes
        while cents-1>=0:
            count+=1
            cents-=1
    
    else:
        print("please enter a positive number!")
        #using recursion toallow the user to enter a valid number
        getCoins()
    
    return count
            
            
        
            
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
if __name__=="__main__":
    main()