from cs50 import SQL
from flask import Flask, flash, redirect, render_template, request, session, url_for
from flask_session import Session
#This CryptContext object is provided for new python applications to quickly and easily add password hashing support
from passlib.apps import custom_app_context as pwd_context
from tempfile import mkdtemp

from helpers import *

# configure application
app = Flask(__name__)

# ensure responses aren't cached
if app.config["DEBUG"]:
    @app.after_request
    def after_request(response):
        response.headers["Cache-Control"] = "no-cache, no-store, must-revalidate"
        response.headers["Expires"] = 0
        response.headers["Pragma"] = "no-cache"
        return response

# custom filter
app.jinja_env.filters["usd"] = usd

# configure session to use filesystem (instead of signed cookies)
app.config["SESSION_FILE_DIR"] = mkdtemp()
app.config["SESSION_PERMANENT"] = False
app.config["SESSION_TYPE"] = "filesystem"
Session(app)

# configure CS50 Library to use SQLite database
db = SQL("sqlite:///finance.db")

@app.route("/")
@login_required
def index():
    #get stocks' info from portfolio table
    rows=db.execute("SELECT stockSymbol, SUM(howMany) AS sum1, price, ROUND(SUM(totalPrice),4) AS sum2 FROM portfolio WHERE id= :id GROUP BY stockSymbol", id=session["user_id"])

    cash=db.execute("SELECT cash from users WHERE id= :id", id=session["user_id"])

    #previous line returns a list
    cash=cash[0]["cash"]
    cash=round(cash,4)
    total=float(cash)
    cash=usd(cash)
    #for row in rows:
    #result[row]=lookup(row["StockSymbol"])

    for row in rows:
       total+=row["sum2"]
       row["price"]=usd(row["price"])
       row["sum2"]=usd(row["sum2"])

    total=round(total,4)


    return render_template("index.html",stocks=rows,current=cash,grand=usd(total))

@app.route("/buy", methods=["GET", "POST"])
@login_required
def buy():
    """Buy shares of stock."""
    if request.method=="POST":

        #if it's blank
        if not request.form.get("symbol"):
            return apology("You must enter a symbol")

        symbol=request.form.get("symbol")
        num=int(request.form.get("number"))
        if symbol.isalpha()== False:
            return apology("Make sure you enetered a valid symbol!")
        elif num <=0:
            return apology("Please enter a positive number!")

        #get the stock info from lookup function
        result=lookup(request.form.get("symbol"))
        if not result:
            return apology("Please enter a valid symbol")

        #can user afford that stock?
        total=result["price"] * num
        #returns a list
        cash = db.execute("SELECT cash from users WHERE id=:id ", id=session["user_id"] )
        cash=cash[0]["cash"]
        if(cash<= total):
            return apology("make sure you have enough cash for this stock")
        else:
            total=result["price"] * num
            #update cash
            db.execute("UPDATE users SET cash=:cash WHERE id= :id ",cash=cash-total, id=session["user_id"] )
            db.execute("INSERT INTO portfolio (id, stockSymbol, howMany, price, totalPrice) VALUES (:id, :stock, :howMany, :price, :total)"
            ,id= session["user_id"], stock=result["name"], howMany=num, price=result["price"] ,total= total )
            return redirect(url_for("index"))

    #if you reached the page(route) via a GET request
    else:
        return render_template("buy.html")


@app.route("/history")
@login_required
def history():

    """Show history of transactions."""
    rows=db.execute("SELECT * FROM portfolio WHERE id=:id ",id=session["user_id"])
    for row in rows:
        if row["howMany"]<0:
            row["BS"]="sold"

    #convert to usd format
    for row in rows:
       row["totalPrice"]=usd(row["totalPrice"])
       row["price"]=usd(row["price"])

    return render_template("history.html",stocks=rows)

@app.route("/login", methods=["GET", "POST"])
def login():
    """Log user in."""

    # forget any user_id
    session.clear()

    # if user reached route via POST (as by submitting a form via POST)
    if request.method == "POST":

        # ensure username was submitted
        if not request.form.get("username"):
            return apology("must provide username")

        # ensure password was submitted
        elif not request.form.get("password"):
            return apology("must provide password")

        # query database for username, remember: .execute returns your table as a dict.(rows;key, columns: value)
        rows = db.execute("SELECT * FROM users WHERE username = :username", username=request.form.get("username"))

        # ensure username exists and password is correct, it's one user, so there's only1 row, and 4 columns
        if len(rows) != 1 or not pwd_context.verify(request.form.get("password"), rows[0]["hash"]):
            return apology("invalid username and/or password")

        # remember which user has logged in
        session["user_id"] = rows[0]["id"]

        # redirect user to home page
        return redirect(url_for("index"))

    # else if user reached route via GET (as by clicking a link or via redirect)
    else:
        return render_template("login.html")

@app.route("/logout")
def logout():
    """Log user out."""

    # forget any user_id
    session.clear()

    # redirect user to login form
    return redirect(url_for("login"))

@app.route("/quote", methods=["GET", "POST"])
@login_required
def quote():
    """Get stock quote."""
    if request.method == "POST":
        if not request.form.get("quote"):
            return apology("You must enter a stock symbol")

        #make sure he entered a string
        quote=request.form.get("quote")
        if quote.isalpha() ==False:
            return apology("Please enter a valid symbol")

        #call lookup function to retrieve stock qoute
        result=lookup (request.form.get("quote"))

        if not result:
            return apology("Please enter a valid symbol")

        #send the price you got from lookup function to qouted.html to display it
        return render_template("quoted.html",name=usd(result["price"]))

    # else if user reached route(this page) via GET (as by clicking a link, a button or via redirect)
    else:
        return render_template("quote.html")

@app.route("/register", methods=["GET", "POST"])
def register():
    """Register user."""
     # if user reached route via POST (as by submitting a form via POST)
    if request.method == "POST":
        # ensure username was submitted
        if not request.form.get("username"):
            return apology("Missing username!")

        # ensure password1 and password2 were submitted
        elif not request.form.get("password1"):
            return apology("You must enter a password")

        elif not request.form.get("password2"):
            return apology("Please confirm your password")

        elif ((request.form.get("password1")) != (request.form.get("password2"))):
            return apology("passwords don't match!")

        #security>>add the password n the DB as a hashed pass..>> encrypting a password...
        hash= pwd_context.hash(request.form.get("password1"))

        #add new user to database
        result=db.execute("INSERT INTO users(username,hash) VALUES (:username, :hash)", username=request.form.get("username"),
        hash=hash)

        if not result:
            return apology("This username already exists")

        row=db.execute("SELECT * FROM users WHERE username= :username", username=request.form.get("username"))

        # remember which user has logged in
        session["user_id"]=row[0]["id"]

        return redirect (url_for("index"))

     # else if user reached route via GET (as by clicking a link or via redirect)
    else:
        return render_template("register.html")


@app.route("/sell", methods=["GET", "POST"])
@login_required
def sell():
    """Sell shares of stock."""
    if request.method=="POST" :
        if not request.form.get("NumOfShares"):
            return apology("Please specify a number of shares!")
        if not request.form.get("stockSym"):
            return apology("Please enter a stock name!")
        num=int(request.form.get("NumOfShares"))
        if num<=0:
            return apology("Please enter a positive number of shares!")

        #look for the stock's info(name)
        result=lookup(request.form.get("stockSym"))

        #if the user enters an invalid symbol, or an alphabetical word
        if not result:
            return apology("Please enter a valid symbol")
        name=result["name"]

        stock = db.execute("SELECT stockSymbol from portfolio WHERE stockSymbol=:stock AND id=:id", stock=name,id= session["user_id"])

        #if it returned an empty list
        if not stock:
            return apology("Make sure the symbol you entered belongs to a stock name that you own")

        else:
            #stock[0]==request.form.get("stockSym"):
            numShares = db.execute("SELECT stockSymbol , SUM(howMany) AS totalOwned FROM portfolio WHERE stockSymbol=:stock AND id=:id",stock=name,
            id= session["user_id"] )
            NumOfShares=int(request.form.get("NumOfShares"))
            if numShares[0]["totalOwned"] < NumOfShares:
                return apology("Are you sure you own this number of shares?")

            #if the user entered 2 valid info's
            else:

                price=NumOfShares*result["price"]
                listt=db.execute("SELECT cash from users WHERE id=:id", id=session["user_id"])
                #previous line returns a row from a dictionary
                newCash=listt[0]["cash"]+price

                #after each selling procedure, update users cash
                db.execute("UPDATE users SET cash= :cash WHERE id=:id", cash=newCash,id=session["user_id"])

                newShares= numShares[0]["totalOwned"]-NumOfShares

                #if the user sold all of his stock's shares,  delete the stock from the database
                if newShares==0:
                    db.execute("DELETE from portfolio WHERE id=:id AND stockSymbol=:stock", id= session["user_id"],stock=name)

                elif newShares>0:
                    Shares= -NumOfShares
                    db.execute("INSERT INTO portfolio (id, stockSymbol, howMany, price, totalPrice) VALUES (:id, :stock, :howMany, :price, :total)"
                    ,id= session["user_id"], stock=result["name"], howMany=Shares, price=result["price"] ,total= price)

                else:
                    return apology("Are you sure you own this number of shares?")

                 #make a sell table to store it all, and fill it
                db.execute("INSERT INTO sell (id, stockSymbol, howMany, price, totalPrice) VALUES(:id, :stock, :howMany, :stockprice, :total)"
                ,id= session["user_id"], stock=request.form.get("stockSym"), howMany=NumOfShares, stockprice=result["price"] ,total=price)



            return redirect(url_for("index"))

    else:
        return render_template("sell.html")

@app.route("/settings",methods=["GET","POST"])
@login_required
def changepass():
    if request.method =="POST":
        #if it's blank
        if not request.form.get("newPass"):
            return apology("please enter your old password")
        if request.form.get("newPass")!= request.form.get("passCon"):
            return apology("the two passwords don't match")
        else:
            #hash the new password
            hash=pwd_context.hash(request.form.get("newPass"))
            #update the users table with the new password
            db.execute("UPDATE users SET hash=:pas WHERE id=:id",pas=hash, id=session["user_id"])
            return render_template("changed.html")

    #if you reached the page via GET
    else:
        return render_template("changePass.html")

@app.route("/other")
@login_required
def other():
    return render_template("other.html")