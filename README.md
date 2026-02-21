# Solitaire
Matrix Solitaire in SFML
From the rules found here:
http://spoiledflushgames.com/matrix-solitaire/


Game Play:  For this game, you have to imagine a 3X3 grid of cards, sort of like an invisible tic-tac-toe board.  There are effectively nine slots you can play to, but you won’t be able to see them so much until you’ve already played a card there. If you are incapable of all this fancy imagining, download and print the play mat. So you draw cards and play them one at a time to the grid.  The row farthest from you is the top of the grid and the row closest to you is the bottom of the grid.  All these cards have “weight” to them, meaning you must play the cards to the bottom grid, or directly above another card. Cards cannot simply “float” in the air (except for Hearts, which we’ll get to later.)



The goal here is to match up cards of the same suit.  If you get a chain of three or more cards, you remove those cards from the matrix and score them.

A chain means that the cards are connected to each other either vertically or horizontally. The chain can go around corners, but cannot be diagonal.  When cards are removed from the board, any cards that were above them “fall” to the bottom of the grid.  (Unless it’s a Heart, but, yeah, later.) 



 

You must immediately remove a chain as soon as you make it, so you can’t store up chains for bigger scores.  In order to get a bigger chain, you must avoid making a chain of three.  Instead, try to set up plays where you create a chain of four, five, or six once the final card of the chain is played.

When you create a chain, make sure to note how many cards were in it before you take it off the board.  Keep each chain type in a separate score pile, because different chains are worth different amounts when you tally your score at the end.

The better your chain the more points you get, so if you’re smart, which you must be, since you bought this book, you’ll try to get the biggest chain possible for every play.  Knowing what card you’re about to draw next will help you immensely in this endeavor.

But be careful.  If there is a card in every slot on the grid (nine total cards), the game ends immediately.  (If playing the ninth card completes a chain, the chain is still removed and the game continues.)  Otherwise, the game ends when there are no cards left in the deck.  When the game ends, tally up your score.

For every three-card chain: 3 points (1 point per card)

For every four-card chain: 8 points (2 points per card)

For every five-card chain: 20 points (4 points per card)

For every six-card chain: 48 points (8 whopping points per card)

Then, subtract one point for every card still in the matrix.  So, if the game ended because you filled up the grid, that’s -9 points.

Chain Upgrades:  If you make a play that results in more than one chain, such as removing a chain from the board and causing higher cards to fall down into a new chain, you get a Chain Upgrade.  Whichever chain was smaller is scored as though it were one card larger.  So if you created a chain of four and a chain of three on the same turn, the chain of four would be counted as normal: 2 points per card, 8 points total, but the chain of three would be upgraded to 2 points per card as well, or 6 points total.  The total play would be worth 14 points instead of 11.  Obviously, this can become very lucrative in the higher chain values.  If both chains are the same length, only one of them gets the upgrade.







Winning and Losing:  Are solitaire games really about winning and losing?  Well, sort of.  Just try to score as many points as you can.   If you like, use the below chart as a frame of reference.

Points Scored:               Result:

-9-49:                           Loser.  Big, fat loser.

50-99:                          Well played.

100-149:                      Matrix Solitaire Expert.

150+:                           There is no spoon.

 

But there’s more!  Each suit also has a suit ability.  They are as follows:

Spades:  You may play Spades to a slot that already has a card, but only if the Spade is of higher value.  When you play a Spade to an occupied slot, it eliminates the old card.  So the King of Spades can eliminate any card that is Queen or lower.  The Ace of Spades is low and high simultaneously, so it can eliminate the King and the 2 can eliminate an Ace.  Cards eliminated by the Spades are removed from the board face down, and are worth 0 points.  However, if the Matrix is full, you cannot play any cards to it, not even a Spade.

Diamonds:  Diamonds have the ability to push cards up.  You can insert them anywhere into the grid and push whatever cards were on that spot up one space.  You can’t, however, stick them into a stack that is already three cards high.

Clubs:  Whenever you play a Club, you may choose to take the top card of the deck and put it on the bottom.

Hearts:  Hearts float.  This is key.  Hearts are the only suit that you can play to any point in the Matrix, even with no card underneath, top row, middle, bottom, whatever.  When cards under a Heart are removed from the board, the Hearts do not “fall” to the bottom like other cards, but instead remain suspended in mid air.  Keep in mind though that the floating Hearts are stationary, and cannot be moved once you place them (unless they are pushed up by a Diamond, of course).  Also, if a Spade happens to eliminate a floating Heart, it falls to the bottom, being a Spade and not a Heart. 
