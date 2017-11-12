subD([],_,_,_):- print('Error').
subD(X,0,[],X).
subD([H|T],1,[H],T).
subD([H|T],X,[H|R],Q):- XX is X-1, subD(T,XX,R,Q).