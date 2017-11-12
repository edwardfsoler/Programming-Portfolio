s(X,Z):- np(X,Y),vp(Y,Z). /*initial difference list sentence structure*/

np(X,Z):- det(X,Y), noun(Y,Z). /*nounphrase split up into det and noun*/
np(X,Z):- np(X,Y), pp(Y,Z).  /*nounphrase split up into nounphrase and preposition phase*/
vp(X,Z):- verb(X,Y), np(Y,Z).  /*verbphrase split up into verb and np*/
vp(W,Z):- verb(W,X), np(X,Y), pp(Y,Z).  /*verbphrase split up into verb, np and preposition phrase*/ 
pp(X,Z):- prep(X,Y), np(Y,Z). /*preposition phase split up into preposition and noun phrase*/

/*list of predetermined words*/

det([the|X],X).
det([a|X],X).
noun([waiter|Y],Y).
noun([meal|Y],Y).
noun([table|Y],Y). 
noun([day|Y],Y).
verb([brought|W],W).
prep([to|Z],Z).
prep([of|Z],Z).


 