/****************************************
 * Nolan Mullins
 * 0939720
 * 01/02/2017
 * 2750 - A2
 ****************************************/

    This program is a message management and viewing system. It will store messages
along with the user's data. The viewer will be able to move through mosts, sort them
and reload them. 

Known issues
 - posts have to be less than 25 lines long, else you will not be able to move to the next post

/**********************************/
	Instructions - compile
		type: make
		output:
		post
		addauthor
/**********************************/

/**********************************/
	Instructions - run
	
	type: ./post UserID
	type: ./addauthor UserID
	./view.py UserID
/**********************************/