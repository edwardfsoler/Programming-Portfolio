/*base case*/
occur(_,[],0).

/*increments count and recursively calls itself when item is at front of list*/
occur(X,[X|T],A):- occur(X,T,Temp), A is Temp+1.

/*recursively calls itself when item is not at fron of list*/
occur(X,[_|T],A):- occur(X,T,A).