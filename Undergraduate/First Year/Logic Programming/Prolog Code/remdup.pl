/*function to find the max item in a list*/
max(X,[X]).
max(H,[H|T]):- max(M,T), H > M.
max(Max,[H|T]):- max(Max,T), Max >= H.

/*function to delete all occurences of an item in a list*/
d(_,[],[]).
d(X,[X|T],F):- d(X,T,F).
d(X,[H|T],[H|R]):- d(X,T,R).

/*function to remove duplicate items and output list in descending order*/
remDup([],[]).
remDup([_],[]). 
remDup(L,[Max|T]):- max(Max,L), d(Max,L,Temp), remDup(Temp,T). 
