# ACTIVE SKILLS LIST
The active skill data, described in the section MODELS is mandatory for the rest of the protocol to work. I.e. if some data or an event arrives with namespace "mycroft.weather", the skill id "mycroft.weather" must have been advertised as recently used in the recent skills model beforehand, otherwise all requests on that namespace will be ignored on both client and serverside and considered a protocol error.
Recent skills are ordered from the last used to the oldest, so the first item of the model will always be the the one showing any QML GUI, if available.

# EVENTS
```javascript
{
    "type": "mycroft.events.triggered"
    "namespace": "weather.mycroft"
    "event_name": "system.pick",
    "parameters": {"item": 3}
}
```

* event names that start with "system." are available to all skills, like previous/next/pick. the skill author can have as many custom events as he wants
* same message format goes both ways python->gui and gui->python

# SKILL DATA

At the center of data sharing there is a key/value dictionary that is kept synchronized between the server and the GUI client.
Values can either be simple strings, numbers and booleans or be more complicated data models as described in the MODELS section.

## Sets a new key/value in the sessionData dictionary
Either sets a new key/value pair or replace an existing old value.
```javascript
{
    "type": "mycroft.session.set",
    "namespace": "weather.mycroft"
    "data": {
        "temperature": "28",
        "icon": "cloudy",
        "forecast": [{...},...] //if it's a list a model gets created, or resetted if it was already existing, see the MODELS section
    }
}
```

## Deletes a key/value pair from the sessionData dictionary
```javascript
{
    "type": "mycroft.session.delete",
    "namespace": "weather.mycroft"
    "property": "temperature"
}
```

All properties already in the dictionary need to be sent as soon as a new client connects to the web socket

The exact message format would be in both direction both server->gui and gui->server


# MODELS
Models are for both skill data and active skills, distinction is just between "namespace": "mycroft.system.active_skills" and "namespace": "mycroft.weather"
All operations have a type which starts by "type": "mycroft.session.list."

The format of the data passed via the socket is of an ordered map
```javascript
[
{
  "key1": "value1",
  "key2": "value2"
},
{
  "key1": "value3",
  "key2": "value4"
}
]
```

It must always be an array, even if contains a single item, and each item must contain the exact same set of keys, even if some of them could be empty.
If subsequent inserts of items will contain a different set of keys, or toehr keys more, those new keys will be ignored by the GUI.

## Inserts new items at position
```javascript
{
    "type": "mycroft.session.list.insert",
    "namespace": "mycroft.system.active_skills" // skill: mycroft.weather
    "property": "forecast" //the key of the main data map this list in contained into
    "position": 2
    "values": [{"date": "tomorrow", "temperature" : 13, ...}, ...] //values must always be in array form
}
```

Values is an ordered dict, for a shopping cart it would need multiple roles like product name, price, image

## Updates item values starting at the given position, as many items as there are in the array
```javascript
{
    "type": "mycroft.session.list.update",
    "namespace": "mycroft.system.active_skills" // skill: mycroft.weather
    "property": "forecast" //in the future this can become a path if we want lists of lists
    "position": 2
    "values": [{"date": "tomorrow", "temperature" : 13, ...}, ...] //values must always be in array form
}
```

## Move items within the list
```javascript
{
    "type": "mycroft.session.list.move",
    "namespace": "mycroft.system.active_skills" // skill: mycroft.weather
    "property": "forecast"
    "from": 2
    "to": 5
    "items_number": 2 //optional in case we want to move a big chunk of list at once
}
```

## Remove items from the list
```javascript
{
    "type": "mycroft.session.list.remove",
    "namespace": "mycroft.system.active_skills" // skill: mycroft.weather
    "property": "forecast" //in the future this can become a path if we want lists of lists
    "position": 2
    "items_number": 5 //optional in case we want to get rid a big chunk of list at once
}
```

# SHOW GUI
```javascript
{
    "type": "mycroft.gui.show",
    "namespace": "weather.mycroft"
    "gui_urls": ["file:///opt/mycroft/weather.mycroft/ui/currentweather.qml", "..."]
}
```

It should always be an array, even if containing a single url. If multiple urls are provided, the GUI is guaranteed to show the first one, with the possibility of seeing the other pages as well, for instance with an horizontal swipe.

The skill author to have that would just call a functin like self.show_gui("currentweather.qml") the construction of the proper message is done behind the scenes.

TODO: mycroft.gui.replace, OR mycroft.gui.clear
TODO: storing delegates order on the server, resting-faces, mini-faces
