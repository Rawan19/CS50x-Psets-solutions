import feedparser
import urllib.parse

def lookup(geo):
    """Looks up articles for geo."""

    # Function caching allows us to cache the return values of a function depending on the arguments
    #A cache is a way to store a limited amount of data such that future requests for said data can be retrieved faster

    #check cache for geo (it's always not there: the first time you search for a geo), so this condition won't be true
    if geo in lookup.cache:
        return lookup.cache[geo]


    #The URL quoting functions focus on taking program data and making it safe for use as URL components
    #feedparser module: get feed (articles)from Google, for that specific geo
    feed = feedparser.parse("http://news.google.com/news?geo={}&output=rss".format(urllib.parse.quote(geo, safe="")))

    #feedparser.parse() returns the RSS feed9in this case: title, link, and items(which are just tags in the html code of the RSS feed page, that you want to retrieve:)
    #https://news.google.com/news?cf=all&hl=en&pz=1&ned=us&geo=02138&output=rss

    # if no items in feed, get feed from Onion
    if not feed["items"]:
        feed = feedparser.parse("http://www.theonion.com/feeds/rss")

    # cache results, storing geos into this cache, as an array of dict obejects.
    lookup.cache[geo] = [{"link": item["link"], "title": item["title"]} for item in feed["items"]]

    # return results
    return lookup.cache[geo]

# initialize cache;just an empty dictionary
lookup.cache = {}
