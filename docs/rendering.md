# Rendering: Strategy and Design

## Software Design

Our display is managed by a class called "GameWindow" which allows you to choose which part of the game to display (the game, the menu, ...). This class has as an attribute an element of the "MapDisplay" class which itself contains as an attribute an instance of the "HexDisplay" class and an array of elements of the "TextureData" class.

The HexDisplay class is composed of an array of textures which represents the game map. It therefore allows to display the terrain which is represented by hexagons. This class contains an array of vertices and an array with the textures of the hexagons of the map. It also has a table containing the difficulty of each hexagon. It also has a function which allows to define the textures of the vertices from the name of the texture and the table containing the difficulties.

The TextureData class is used to display a mobile element on the map. It therefore represents elements such as barbarians, cities or caravans. This class contains the position of the texture, its offset allowing to position it as we wish, the name of a texture and a texture which is loaded by the "loadtexture" method using its name.