idRem(0,_,_,_):- print('error').
idRem(_,[],[],0):- print('error').
idRem(1,[H|T],T,H).
idRem(X,[H|T],[H|R],Y):- Q is X-1, idRem(Q,T,R,Y).