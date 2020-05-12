import nltk
from nltk.tokenize import TweetTokenizer

class Analyzer():
    """Implements sentiment analysis."""

    def __init__(self, positives, negatives):
        """Initialize Analyzer."""
        
        self.positives=[]
        self.negatives=[]
        
        with open("positive-words.txt","r") as lines:
            for line in lines:
               #add each word to the set in memory, omiting white spaces
               self.positives.append(line.strip())
            
        with open("negative-words.txt","r")as lines2:
            for line in lines2:
               self.negatives.append(line.strip())
            
        lines.close()
        lines2.close()
        #now the 2 files are successfuly loaded to memory
        

        # TODO

    def analyze(self, text):
        """Analyze text for sentiment, returning its score."""
        score=0
        
        #split each tweet into single words(tokens)
        tokenizer = nltk.tokenize.TweetTokenizer()
        tokens = tokenizer.tokenize(text)
        
        for word in tokens:
            if word.lower() in self.positives:
                score+=1
            elif word.lower() in self.negatives:
                score-=1
            else:
                score+=0
         # TODO
        return score
