# How rules are computed

# The concept

To play the rules, the player first selects on his window all the necessary elements to play a card. This can be the number of boxes, the path of a caravan or the position of a control pawn.

Once he is done, he can click on the end of turn button. Then, the data concerning the card played is then transferred to the server thanks to a class which serves as a data structure (this class is RuleArgsStruct). The server will then launch the corresponding rules and check if the actions requested by the player are possible. if possible, then the structure will be communicated to all players who execute the map.

# Display of chages

Running the rule will update the map and after a few moments the client will detect this update and display the changes on the screen.