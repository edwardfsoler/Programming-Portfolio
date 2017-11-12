d(_,[],[]).
d(H,[H|T],T).
d(H,[F|B],[F|R]):-d(H,B,R).

exclude([],X,X).
exclude(_,[],[]).
exclude([H|T],L,R):- d(H,L,B), exclude(T,B,R).