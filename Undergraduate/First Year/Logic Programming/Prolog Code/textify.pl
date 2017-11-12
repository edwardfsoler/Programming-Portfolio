/*function to replace occurence of C1,C2 and C3 with NC in a list*/
textify([C1,C2,C3],_,[X],[X]).

/*base case*/
textify([C1,C2,C3],NC,[C1,C2,C3],[NC]).

/*recursive calls when text to be replaced is at the front (and not) of the list*/
textify([C1,C2,C3],NC,[C1,C2,C3|T],[NC|R]):- textify([C1,C2,C3],NC,T,R).
textify(L,NC,[H|T],[H|L2]):- textify(L,NC,T,L2).  