/*counts number of occurences of X in list and deletes all occurences*/

/*base case*/
countDel(_,[],[],0).

/*deletes occurence and increments count*/
countDel(X,[X|T],R1,R2):- countDel(X,T,R1,Temp), R2 is Temp + 1.

/*recursive call*/
countDel(X,[H|T],[H|R1],R2):- countDel(X,T,R1,R2).
