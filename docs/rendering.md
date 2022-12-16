# Rendering: Strategy and Design

## Software Design

Our display is handled by a class called `GameWindow`. This class has various `TextureDisplayer` attributes or `TextureDisplayer` arrays that display elements (a class that has sprites, a texture and several methods). Moreover it has a cursor (`sf::Cursor`), a game window (`sf::RenderWindow`) and a map that comes from shared with the server data). In `GameWindow`, we initialize all the objects we want to display and place them in different places in the window. This class also detects different interactions like clicks or keyboard presses.

### TextureDisplayer

The `TextureDisplayer` class takes as parameter a path and has as attributes a texture, a sprite vector and a type. This class allows to move specific sprites, to get information about the different elements of sfml and to manage objects.

### PriorityCardStruct & ActionCardStruct

The `PriorityCardStruct` and `ActionCardStruct` classes are structures that display the different action cards that are present in the player's HUD. They both have texture attributes (`TextureDisplayer`), a title (`sf::Text`), a type (string), a body (`sf::Text`) and a bonus (int). Its different elements allow to display all the right cards at the right places. `PriorityCardStruct` has in addition an array of strings named text which load at initialization all the possible texts for a card.
