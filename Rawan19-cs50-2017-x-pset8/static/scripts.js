// Google Map
var map;

// markers for map
var markers = [];

// info window
var info = new google.maps.InfoWindow();

// execute when the DOM is fully loaded
$(function() {

    // styles for map
    // https://developers.google.com/maps/documentation/javascript/styling
    var styles = [

        // hide Google's labels
        {
            featureType: "all",
            elementType: "labels",
            stylers: [
                {visibility: "off"}
            ]
        },

        // hide roads
        {
            featureType: "road",
            elementType: "geometry",
            stylers: [
                {visibility: "off"}
            ]
        }

    ];

    // options for map
    // https://developers.google.com/maps/documentation/javascript/reference#MapOptions
    var options = {

        //The initial Map center. Required.
        center: {lat: 37.4236, lng: -122.1619}, // Stanford, California

        //Enables/disables all default UI. May be overridden individually.
        disableDefaultUI: true,

        //The initial Map mapTypeId. Defaults to ROADMAP.
        mapTypeId: google.maps.MapTypeId.ROADMAP,
        maxZoom: 14,

        //The display options for the Pan control.
        panControl: true,

        //Styles to apply to each of the default map types. Note that for satellite/hybrid and terrain modes, these styles will only apply to labels and geometry
        styles: styles,

        //initial Map zoom level.
        zoom: 13,
        zoomControl: true
    };

    // get DOM node in which map will be instantiated
    var canvas = $("#map-canvas").get(0);

    // instantiate map, takes 2 arguments: the variable that holds the element id, where the map is, and the list of objects for initilizing the map
    map = new google.maps.Map(canvas, options);

    // configure UI once Google Map is idle (i.e., loaded)
    google.maps.event.addListenerOnce(map, "idle", configure);

});

/**
 * Adds marker for place to map.
 */
function addMarker(place)
{
   //var latlng={lat:parseFloat(place.latitude), lng: parseFloat(place.longitude)};
    var latlng={lat:place['latitude'], lng: place['longitude']};
    var marker= new google.maps.Marker({

        //position: {lat: parseFloat(place.latitude), lng: parseFloat(place.longitude)},
        position: latlng,
        map: map

    });


  // google.maps.event.
   marker.addListener('click',function(){

            // getJSON loads JSON data from the server using a GET HTTP request.
       g=place['postal_code'].toString();
       var url="/articles?geo="+g;
       $.getJSON(url, function(articles){
           //since articles returns an array of objects, let's check if the city has any articles
           if(articles.length){
                //JSON.stringify(articles);
          var ul='<ul id=article>';
          for(i=0;i<articles.length;i++){
               //ul+=Handlebars.compile('<li>{{articles.title}}</li>');
              ul+='<li>'+'<a href='+articles[i].link+'>'+ articles[i].title+'</a></li>';
          }


          ul+='</ul>';
          showInfo(marker, ul);
           }

           //if the city doesn't have any articles
           else{
               var div='<div id=empty>';
               div+='<h1>'+'No articles are currently available here'+'</h1';
               div+='/div';
               showInfo(marker,div);
           }


       });







   });
   //adds a new element to markers array
   markers.push(marker);
}

/**
 * Configures application.
 */
function configure()
{
    // update UI after map has been dragged; "dragend" event: This event is fired when the user stops dragging the map.
    google.maps.event.addListener(map, "dragend", function() {

        // if info window isn't open
        // http://stackoverflow.com/a/12410385
        if (!info.getMap || !info.getMap())
        {
            update();
        }
    });

    // update UI after zoom level changes
    google.maps.event.addListener(map, "zoom_changed", function() {
        update();
    });

    // configure typeahead
    //typeahead(options, [*datasets])
    $("#q").typeahead({
        highlight: false,
        minLength: 1
    },
    {
        display: function(suggestion) { return null; },
        limit: 10,

        //search) is the function that the plugin will call as soon as the user starts typing
        //so that the function can respond with an array of search results based on the user’s input.
        source: search,
        //whatever is the data that you got back from search, execute the folllowing on it
        templates: {

            // Handlebars, a templating language for JavaScript similar in spirit to Jinja for Python.
            //what data (suggestions) do you want the user to see, in the dropdown? the rows you just got back from db in search func
            suggestion: Handlebars.compile(
               "<div> {{place_name}}, {{admin_name1}}, {{postal_code}}</div>"
            )
        }
    });

    // re-center map after place is selected from drop-down
    //.on()Attach an event handler function(in this case, anonymous function) for one or more events(typeahead:selected") to the selected elements.
    $("#q").on("typeahead:selected", function(eventObject, suggestion, name) {

        // set map's center
        map.setCenter({lat: parseFloat(suggestion.latitude), lng: parseFloat(suggestion.longitude)});

        // update UI
        update();
    });

    // hide info window when text box has focus
    //"focus": JavaScript event, when you click inside the textBox, what do you want to happen(what function do you  want to execute)?
    // function( Event eventObject ): A function to execute each time the event is triggered.
    $("#q").focus(function(eventData) {
        info.close();
    });

    // re-enable ctrl- and right-clicking (and thus Inspect Element) on Google Map
    // https://chrome.google.com/webstore/detail/allow-right-click/hompjdfbfmmmgflfjdlnkohcplmboaeo?hl=en
    document.addEventListener("contextmenu", function(event) {
        event.returnValue = true;
        event.stopPropagation && event.stopPropagation();
        event.cancelBubble && event.cancelBubble();
    }, true);

    // update UI
    update();

    // give focus to text box, when you reload the page, you'll find it's automatically focused, this is why it takes no arguments his time.
    //I don't want anything to happen, I just want to focus on the textbox, after exeuting configures
    $("#q").focus();
}

/**
 * Removes markers from map.
 */
function removeMarkers()
{
    // this function will need to remove any and all markers from the map!
    // TODO
    for(var i=0;i<markers.length;i++){
        marker[i].setMap(null);
    }
    markers=[];
}

/**
 * Searches database for typeahead's suggestions.
 */
 //The value of the text box (i.e., whatever the user has typed in total) is passed to search as query
function search(query, syncResults, asyncResults)
{
    // get places matching query (asynchronously)
    var parameters = {
        q: query
    };

    //Load JSON-encoded data from the server using a GET HTTP request.
    //search uses jQuery’s getJSON method to contact /search asynchronously, passing in one parameter, geo, the value of which is query.
    $.getJSON(Flask.url_for("search"), parameters)

    //Promise ($.ajax())callbacks — .done(), .fail()
    //Once /search responds, the anonymous function passed to done will be called and passed data
    .done(function(data, textStatus, jqXHR) {


        //asyncResults is a "callback" function that search should call as soon as it’s done searching for matches
        // call typeahead's callback with search results (i.e., places)
        // so that the plugin can iterate over the places therein (assuming /search found matches) in order to update the plugin’s drop-down. Phew.
        asyncResults(data);
    })

    // if something goes wrong, fail is instead called.
    .fail(function(jqXHR, textStatus, errorThrown) {

        // a concole is llike a command line, but the browser can communicate with,F12
        // log error to browser's console(console.log: prints a string to the console,no return value)
        console.log(errorThrown.toString());

        // call typeahead's callback with no results
        asyncResults([]);
    });
}

/**
 * Shows info window at marker with content.
 */
function showInfo(marker, content)
{
    // start div
    var div = "<div id='info'>";
    if (typeof(content) == "undefined")
    {
        // http://www.ajaxload.info/
        div += "<img alt='loading' src='/static/ajax-loader.gif'/>";
    }
    else
    {
        // this function is creating a string of HTML dynamically,
        div += content;
    }

    // end div
    div += "</div>";

    // set info window's content
    info.setContent(div);

    // open info window (if not already open)
    info.open(map, marker);
}

/**
 * Updates UI's markers.
 */
function update()
{
    // get map's bounds
    var bounds = map.getBounds();
    var ne = bounds.getNorthEast();
    var sw = bounds.getSouthWest();

    // get places within bounds (asynchronously)
    var parameters = {
        ne: ne.lat() + "," + ne.lng(),
        q: $("#q").val(),
        sw: sw.lat() + "," + sw.lng()
    };
    $.getJSON(Flask.url_for("update"), parameters)

    //the anonymous function passed to done is called and passed data, the value of which /update returned to JSON
    .done(function(data, textStatus, jqXHR) {

       // remove old markers from map
       removeMarkers();

       // add new markers to map
       for (var i = 0; i < data.length; i++)
       {
           addMarker(data[i]);

       }
    })
    .fail(function(jqXHR, textStatus, errorThrown) {

        // log error to browser's console
        console.log(errorThrown.toString());
    });
}
