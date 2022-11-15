# Rendering: Strategy and Design

## Software Design

Our display is managed by a class called `GameWindow` which allows you to choose which part of the game to display (the game, the menu, ...). This class has as an attribute an element of the `HubDisplay` and one of `MapDisplay`.

### HudDisplay

The purpose of the `HubDisplay` class is to display game elements that are not part of the map. For example player cards. This class therefore contains a texture and, its position, the name of the corresponding image, its size and the name of the element displayed. The methods associated with this class are used to load an image, move the element or retrieve the displayed image.the purpose of the hud class is to display game elements that are not part of the map. For example player cards. This class therefore contains a texture and, its position, the name of the corresponding image, its size and the name of the element displayed. The methods associated with this class are used to load the texture, move the element or retrieve the displayed image.

### MapDisplay

`MapDisplay` class which contains as an attribute a vector of sprites for mobile elements, an instance of the `HexDisplay` class and an array of elements of the `TextureData` class.

The HexDisplay class is composed of an array of textures which represents the game map. It therefore allows to display the terrain which is represented by hexagons. This class contains an array of vertices and an array with the textures of the hexagons of the map. It also has a table containing the difficulty of each hexagon. A method named `loadHexTexture` allows to define the textures of the vertices from the name of the texture and the table containing the difficulties. An other method allow to change the position of the map with an offset.

The TextureData class is used to load textures and update the vector of sprites of `MapDisplay` class. It therefore represents elements such as barbarians, cities or caravans. This class contains the position of the texture, an offset allowing to position it as we wish, the name of a texture and a texture which is loaded by the `loadtexture` method using its name.