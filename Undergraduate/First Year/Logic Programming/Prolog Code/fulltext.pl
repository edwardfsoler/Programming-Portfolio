/*word replacement in list*/
textify([C1,C2,C3],_,[X],[X]).

/*base case*/
textify([C1,C2,C3],NC,[C1,C2,C3],[NC]).

/*recursive call with word at front of list*/
textify([C1,C2,C3],NC,[C1,C2,C3|T],[NC|R]):- textify([C1,C2,C3],NC,T,R).

/*recursive call with word not at front of list*/
textify(L,NC,[H|T],[H|L2]):- textify(L,NC,T,L2).  

/*base case for fullText*/
fullText([A,B,C],[NC],L,Y):- textify([A,B,C],NC,L,Y).

/*tests for first 3 letters in WordListToReplace and then recursively calls itself and textify function*/
fullText([A,B,C|T],[H1|T1],L,Y):- textify([A,B,C],H1,L,Temp), fullText(T,T1,Temp,Y). 