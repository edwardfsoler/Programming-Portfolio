/*function which searches for the occurence of X in the list and replaces it with Y/*

/*base case*/
wordTextify(_,_,[],[]).

/*recursive call when X is at the front (and not) of the list*/
wordTextify(X,Y,[X|T],[Y|R]):- wordTextify(X,Y,T,R).
wordTextify(X,Y,[Z|T],[Z|R]):- wordTextify(X,Y,T,R).