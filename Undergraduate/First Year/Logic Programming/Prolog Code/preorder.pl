/*function to append an item to a list*/
app([],X,[X]).
app([H|T],X,[H|R]):-app(T,X,R).

/*function to merge two lists together*/
listMerge(L,[],L).
listMerge(L,[H|T],R):- app(L,H,R1), listMerge(R1,T,R).

/*handles case of empty list*/
preOrder(emptyTree,[]).

/*handling of various cases of tree structures*/
preOrder(bTree(nil,Y,nil),[Y]).
preOrder(bTree(nil,Y,Z),[Y|R]):- preOrder(Z,R).
preOrder(bTree(X,Y,nil),[Y|R]):- preOrder(X,R).

/*left and right subtrees handles seperately and then merged together*/
preOrder(bTree(X,Y,Z),([Y|R])):-preOrder(X,R1), preOrder(Z,R2), listMerge(R1,R2,R).
  