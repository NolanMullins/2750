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
 - If a post is longer than 24 lines and wider than 80 chars, scrolling may be a little wierd
 - posts longer than 48 chars will be scrollable but when going up, the user will be brought to
   top of the post

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