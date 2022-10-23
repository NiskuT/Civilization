# Diagram Explanation

## State diagram

\diafile state-diagram.dia

At the beginning of the round, a choice is given to the player according to the cards he got before. If there are cards that can be resolved at the beginning of the round, these cards will be suggested to him. He can choose to resolve them or not.
After that the player has to choose between one of the five priority cards he get in front of him. If this card has multiple actions possible, he choose between one of these actions. Then, if he get a bonus on this card he choose to play the bonus (boxes) or not.
After that, he can resolve the card. 
Finally, if this player was the last player the barbare wheel (event dial) is rotated.

If you want to know more information about it, we would recommand you to read carefully the rules ([here](https://niskut.github.io/Civilization/md_docs_rules_rules.html))

## Class Diagram

\diafile class-diagram.dia

Each player has various resources, wonders, action cards and counters. He can also create cities, exchange cards and play some of them. He is linked to the caravan class, control pawn class and the city class, which also have their own attributes.

Each hexagonal map cell has a level attribute, which symbolizes its nature (grassland, hill, desert, forest...) and another one, named special which can have several arguments like natural wonder, city state, barbarian village or resource.

The rule class allows to define all the possible actions for a card or a player. When a player starts his turn, he chooses an action to play first from a list of different possible actions. This list is actually a list of rule objects that define each action. These objects are instantiated from a json file that defines the set of possible rules like a manual.

Finally a Game class governs the game with two main functions. One to count the turns and one to check the victory conditions.

To better understand the different elements of the diagram, Ie invite you to read the rules by following this link: [here](https://niskut.github.io/Civilization/md_docs_rules_rules.html)


