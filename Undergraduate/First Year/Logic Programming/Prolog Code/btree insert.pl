/*test for emptyTree keyword*/
insert(I,emptyTree,bTree(nil,I,nil)).

/*test whether item to be inserted is greater or larger than root node*/
insert(I,bTree(nil,X,nil),bTree(bTree(nil,I,nil),X,nil)):- I=<X.
insert(I,bTree(nil,X,nil),bTree(nil,X,bTree(nil,I,nil))):- I>X.

/*handles trees with more nodes than a single root node*/
insert(I,bTree(W,X,nil),bTree(W,X,bTree(nil,I,nil))):- I>X.
insert(I,bTree(nil,X,Y),bTree(bTree(nil,I,nil),X,Y)):- I=<X.
insert(I,bTree(W,X,Z),bTree(R,X,Z)):- I=<X, insert(I,W,R).
insert(I,bTree(Z,X,Y),bTree(Z,X,R)):- I>X, insert(I,Y,R).



