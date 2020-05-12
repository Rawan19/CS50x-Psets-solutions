import os
import re
from flask import Flask, jsonify, render_template, request, url_for
from flask_jsglue import JSGlue

from cs50 import SQL
from helpers import lookup

# configure application
app = Flask(__name__)
JSGlue(app)

# ensure responses aren't cached
if app.config["DEBUG"]:
    @app.after_request
    def after_request(response):
        response.headers["Cache-Control"] = "no-cache, no-store, must-revalidate"
        response.headers["Expires"] = 0
        response.headers["Pragma"] = "no-cache"
        return response

# configure CS50 Library to use SQLite database
db = SQL("sqlite:///mashup.db")

@app.route("/")
def index():
    """Render map."""
    if not os.environ.get("API_KEY"):
        raise RuntimeError("API_KEY not set")

    #all this route does is pass the API key to index.html
    return render_template("index.html", key=os.environ.get("API_KEY"))

@app.route("/articles")
def articles():
    """Look up articles for geo"""

    #The URL parameters are available in request.args, which is a MultiDict that has a get method
    request.args.get("geo")

    #check for missing arguments
    if not(request.args.get("geo")):
        raise RuntimeError("Missing geo!")

    articles= lookup(request.args.get("geo"))

    #server response
    return jsonify(articles)

#This function is called by the typeahead plugin every time the user changes the mashup’s text box,
@app.route("/search")
def search():
    """Search for places that match query."""
    # q is the name of the http parameter
    request.args.get("q")

    #check for missing arguments
    if not(request.args.get("q")):
        raise RuntimeError("Missing geo!")

    #"%":match any number of characters
    q=request.args.get("q") + "%"

    #retrieve data from database
    rows=db.execute("SELECT * from places WHERE postal_code LIKE :pc OR place_name LIKE :city OR admin_name1 LIKE :state", pc=q,city=q,state=q)

    return jsonify(rows)

@app.route("/update")
def update():
    """Find up to 10 places within view"""
    #here’s the "back end" that outputs a JSON array of up to 10 places (i.e., cities)
    #that fall within the specified bounds (i.e., within the rectangle defined by those corners)

    # the request.args is bringing a "dictionary" object for you(from update() function in scripts.js)

    #in Python a programmer can raise an error exception at any point in a program
    #The statements used to deal with exceptions are raise and except

    #ensure parameters are present
    if not request.args.get("sw"):
        raise RuntimeError("missing sw")
    if not request.args.get("ne"):
        raise RuntimeError("missing ne")


    # A regular expression is a special sequence of characters that helps you match or find other strings or sets of strings
    #The module re provides full support for Perl-like regular expressions in Python.

    #ensure parameters are in lat,lng format
    if not re.search("^-?\d+(?:\.\d+)?,-?\d+(?:\.\d+)?$", request.args.get("sw")):
        raise RuntimeError("invalid sw")

    #This function searches for first occurrence of RE pattern within string with,it returns a match object on success, none on failure
    if not re.search("^-?\d+(?:\.\d+)?,-?\d+(?:\.\d+)?$", request.args.get("ne")):
        raise RuntimeError("invalid ne")

    # explode southwest corner into two variables
    (sw_lat, sw_lng) = [float(s) for s in request.args.get("sw").split(",")]

    # explode northeast corner into two variables
    (ne_lat, ne_lng) = [float(s) for s in request.args.get("ne").split(",")]

    # find 10 cities within view, pseudorandomly chosen if more within view
    if (sw_lng <= ne_lng):

        # doesn't cross the antimeridian
        rows = db.execute("""SELECT * FROM places
            WHERE :sw_lat <= latitude AND latitude <= :ne_lat AND (:sw_lng <= longitude AND longitude <= :ne_lng)
            GROUP BY country_code, place_name, admin_code1
            ORDER BY RANDOM()
            LIMIT 10""",
            sw_lat=sw_lat, ne_lat=ne_lat, sw_lng=sw_lng, ne_lng=ne_lng)

    else:

        # crosses the antimeridian
        rows = db.execute("""SELECT * FROM places
            WHERE :sw_lat <= latitude AND latitude <= :ne_lat AND (:sw_lng <= longitude OR longitude <= :ne_lng)
            GROUP BY country_code, place_name, admin_code1
            ORDER BY RANDOM()
            LIMIT 10""",
            sw_lat=sw_lat, ne_lat=ne_lat, sw_lng=sw_lng, ne_lng=ne_lng)

    # output places as JSON; jsonify takes the python dictionary(list) and converts it to a JSON object
    return jsonify(rows)
