import cs50

def main():
    mario()
    exit(0)
    
def mario():
    
    print("{}".format(c))
    #an initial value to the number of "#" 
    raw=1
    
    print("Height: ", end="")
    
    #get the height of the pyramid from the user
    height= cs50.get_int();
    
    #an initial value for d, which is the distance moved towards the right to make an alignment.
    d= height-1
     
    if 0<height and height<=23:
        for i in range(height):
            
            #the number of "#" always increments by 1, when starting a new line
            raw+=1
            
            for i in range(d,0,-1):
             #the distances needed to right align the pyramid
                print(" ", end="")
            
            for j in range(raw):
                print("#", end="")
            
            print()
        
             #at the beginning of each line, the distance decreases by a "1"
            d-=1
        
    else:
        print("Oops! looks like you entered an invalid number!")
        #call mario function
        mario()
        
if __name__=="__main__":
    main()