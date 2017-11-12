/*counts the number of occurances of X in a list, returns the count and removes all X from the list*/
countDel(_,[],[],0).
countDel(X,[X|T],R1,R2):- countDel(X,T,R1,Temp), R2 is Temp + 1.

/*recursive calll*/
countDel(X,[H|T],[H|R1],R2):- countDel(X,T,R1,R2).

/*Outputs tuples containing items in the list together with their occurence count*/
collCount([],[]).
collCount([H],[(H,1)]). 

/*recursive call/call to countDel*/
collCount([H|T],[(H,N)|T2]):- countDel(H,[H|T],Temp,N), collCount(Temp,T2).    