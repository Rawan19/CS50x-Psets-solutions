#CS50 FINAL PROJECT
#This project aims to teach egyptian deaf children writing in a fun and easy way! under the supervision of a parent
#Egyptian deaf schools have a very poorr quality in education, some children even gradute without being able to write!
#This is just the 1st version, in the future, they'll be able to learn other words such as verbs, fruits, math, science, and computer science!
from cs50 import SQL
from flask import Flask, flash, redirect, render_template, request, session, url_for
from flask_session import Session
#This CryptContext object is provided for new python applications to quickly and easily add password hashing support
from passlib.apps import custom_app_context as pwd_context
from tempfile import mkdtemp

from helpers import *

# configure application
app = Flask(__name__)



animalss={'cow':'بقرة','dog': 'كلب','monkeyy':'قرد','sheep':'قرد','panda':'باندا','rabbit':'أرنب', 'sheep':'خروف' }
adj={"angry":"عصبي", "happy":"فرحان","love":"محب","hung":"جعان","thirst":"عطشان","scared":"خايف"}
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
db = SQL("sqlite:///deaflove.db")
@app.route("/")
@login_required
def home():


    return  render_template("home.html")


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

        return redirect (url_for("home"))

     # else if user reached route via GET (as by clicking a link or via redirect)
    else:
        return render_template("register.html")

@app.route("/index", methods=["GET", "POST"])
@login_required
def index():

        #initial score
    score=0
    # if user reached route via POST (as by submitting a form via POST)
    if request.method == "POST":

        for key,value in animalss.items():
            if (request.form.get(key)==value):
                score+=1
        print (score)

        for key,value in adj.items():
            if (request.form.get(key)==value):
                score+=1
        print (score)
        db.execute("INSERT INTO scores (id, Adjectives) VALUES (:id, :a)"
            ,id= session["user_id"], a= score )


        db.execute("INSERT INTO scores (id, animals) VALUES (:id, :animals)"
            ,id= session["user_id"], animals= score )

    rows=db.execute("SELECT * FROM scores WHERE id= :id ", id=session["user_id"])



    return render_template("index.html",score=rows)






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

        # ensure username exists and password is correct, it's one user, so there's only 1 row, and 4 columns
        if len(rows) != 1 or not pwd_context.verify(request.form.get("password"), rows[0]["hash"]):
            return apology("invalid username and/or password")

        # remember which user has logged in
        session["user_id"] = rows[0]["id"]

        # redirect user to home page
        return redirect(url_for("home"))

    # else if user reached route via GET (as by clicking a link or via redirect)
    else:
        a=" static/letters.jpg"
        return render_template("login.html")

@app.route("/logout")
def logout():
    """Log user out."""

    # forget any user_id
    session.clear()

    # redirect user to login form
    return redirect(url_for("login"))


@app.route("/animals", methods=["GET", "POST"])
@login_required
def animals():
    """learn animals"""

    return render_template("animals.html")

@app.route("/adjectives")
@login_required
def adjectives():
    return render_template("adjectives.html")



@app.route("/test", methods=["GET", "POST"])
@login_required
def test():
    """test your writing skills!"""





    return render_template("test.html")



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