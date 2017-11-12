/*function to determine max item in list*/
max(X,[X]).
max(H,[H|T]):- max(M,T), H > M.
max(Max,[H|T]):- max(Max,T), Max >= H.

/*finds position of X in list*/
f(X,[X],1).
f(X,[X|_],1).

/*recursive call*/
f(X,[_|T],C):- f(X,T,Temp), C is Temp + 1. 

/*deletes item at position X in list*/
d(_,[],_):- write('Incorrect Input!'), nl.
d(1,[_|T],T).

/*recursive call*/
d(X,[H|T],[H|R]):- Q is X-1, d(Q,T,R).

/*sorts list in descending order*/
descSort([],_). 
descSort([X],[X]).

/*recursive call*/
descSort(L,[Max|T]):- max(Max,L), f(Max,L,N), d(N,L,Temp), descSort(Temp,T).