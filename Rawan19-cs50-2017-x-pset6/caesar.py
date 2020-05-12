import cs50
import sys

def  main():
    #turn key into integer
    key=int(sys.argv[1])
    
    #create an empty list
    ciphered=[]
    
    if len(sys.argv)!=2 or key<0:
        print("make sure you entered a positive key number, and only 2 command line arguments!")
        sys.exit("Usage: ./caesar word")
        exit(1)
        
    else:
        #prompt for plain text
        print("plaintext:", end="")
        plainText= cs50.get_string()
        
        #iterate over each char
        #for i in range(len(plainText)):
            #for each char inthe plainText
        for c in plainText:
            if c.isalpha():
                if c.isupper():
                    #convert ASCII to Alpha index
                    alphaLetter=ord(c)
                    alphaLetter=alphaLetter-65
                        
                    #caesar
                    a=(alphaLetter + key)%26
                        
                    #convert that alpha to ASCII
                    a=a+65
                    a=chr(a)
                    ciphered.append(a)
                     
                #if it's a lower case   
                else:
                    #convert ASCII to Alpha index
                    alphaLetter=ord(c)
                    alphaLette=alphaLetter-97
                        
                    #caesar
                    a=(alphaLette + key)%26
                        
                    #convert that alpha to ASCII
                    a=a+97
                    a=chr(a)
                    ciphered.append(a)
                        
            else:
                #if the input isn't a letter "!, etc", print it as it is
                ciphered.append(c)
                        
        print("ciphertext: ", end="")
    
        #for i in range(len(ciphered)):
        for c in ciphered:
            print("{}".format(c), end="")
                    
    print()
    
if __name__=="__main__":
    main()
    
    
   
                
        