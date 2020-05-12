from flask import Flask, redirect, render_template, request, url_for

import helpers
import sys
import os
import html
import plotly
import socket

from analyzer import Analyzer
from termcolor import colored

app = Flask(__name__)

@app.route("/")
def index():
    return render_template("index.html")

@app.route("/search")
def search():

    # validate(get) screen_name from GET request
    screen_name = request.args.get("screen_name", "")
    if not screen_name:
        return redirect(url_for("index"))

    # get screen_name's tweets
    tweets = helpers.get_user_timeline(screen_name,100)

    # TODO
    pos=0.0
    neg=0.0
    neu=0.0
    
    if not tweets:
        return redirect(url_for("index"))
        
    # absolute paths to lists
    positives = os.path.join(sys.path[0], "positive-words.txt")
    negatives = os.path.join(sys.path[0], "negative-words.txt")

    # instantiate analyzer
    analyzer = Analyzer(positives, negatives)
    
     # analyze tweet
    for tweet in tweets:
        score = analyzer.analyze(tweet)
        
        #if it's a positive tweet
        if score > 0.0:
           pos+=1.0
        elif score < 0.0:
            neg+=1.0
        else:
            neu+=1.0
        
    positive, negative, neutral = pos, neg, neu

    # generate chart
    chart = helpers.chart(positive, negative, neutral)

    # render results
    return render_template("search.html", chart=chart, screen_name=screen_name)
