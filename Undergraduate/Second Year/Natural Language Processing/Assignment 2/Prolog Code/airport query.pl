%definition of reduce function which performs semantic operations on verbs
reduce(A^F,A,F).

%arrive(Airline, Left From, Flight, Craft, Scheduled Arrival, ETA, Remarks)
arrive('Air Malta', 'Zurich', 'KM 491', '320', '20:00', '20:33', landed).
arrive('Swiss International Airlines', 'Zurich', 'LX 4370', '320', '20:00', '20:33', 'landed').
arrive('Air Malta', 'Hamburg', 'KM 369', '319', '20:55', '20:43', landed).
arrive('Air Malta', 'Berlin - Tegel', 'KM 377', '32R', '22:05', '21:52', airborne).
arrive('Ryanair', 'Bristol', 'FR 3774', '738', '22:20', '22:23', airborne).

%depart(Airline, Destination, Flight, Craft, Scheduled Departure, ETA, Remarks)
depart('Ryanair', 'Poznan', 'FR 5226', '738', '20:10', '20:15', airborne).
depart('Ryanair', 'Luton', 'FR 3883', '738', '20:35', '21:00', closed).
depart('Air Malta', 'Paris', 'KM 329', '319', '21:45', '23:00', delayed).
depart('Alitalia', 'Rome', 'AZ 887', '320', '22:05', '22:00', opened).
depart('Ryanair', 'Bristol', 'FR 3771', '738', '22:20', '22:20', opened).

%function called to handle natural language
%function first empties database, enters data into database, handles grammar and semantics
%and interprets the semantic meaning
process(S,R):- emptyAll(), assertions(), s(SEM,S,[]),!, interpret(SEM,R).
process(_,[]):- write('Bad English'),nl.

%semantic interpreter extracts the semantic phrase
interpret(q1(Q),A):- q1(Q,A).
interpret(q2(Q),A):- q2(Q,A).
interpret(q3(Q),A):- q3(Q,A).

%different types of questions are handled in different ways:

%yes/no questions
q1(SEM,yes):- call(SEM),!.
q1(_,no).

%questions with one or multiple answers
q2(SEM,B):- breakdown(SEM,B).

%quantity related questions
q3(SEM,X):- counter(SEM,X).

%function to get the head of a list
getHead([B|_],B).

%extracts all possible answers to a question from the database, constructing a list with all answers
%and handles cases where no answer, 1 answer or multiple answers exist.
breakdown(SEM,'Invalid Question'):- reduce(SEM,X,Y), findall(X,Y,B), length(B,0).
breakdown(SEM,W):- reduce(SEM,X,Y), findall(X,Y,B), length(B,1), getHead(B,W).
breakdown(SEM,W):- reduce(SEM,X,Y), findall(X,Y,W).

%assertion function which asserts all facts into the database to be queried by the user
assertions():- arrive(A,B,C,D,E,F,G), asserta(G(C)), asserta(arriving(A)), asserta(G(B)), asserta(arriving(B)), asserta(arriving(A,B)), asserta(arriving(C,E)), asserta(arrivingE(C,F)), asserta(aircraft(C,D)), asserta(statusA(C,G)), asserta(statusA(B,G)), fail.
assertions():- depart(A,B,C,D,E,F,G), asserta(G(C)), asserta(leaving(A)), asserta(G(B)), asserta(leaving(B)), asserta(leaving(A,B)), asserta(leaving(C,E)), asserta(leavingE(C,F)), asserta(aircraft(C,D)), asserta(statusA(C,G)), asserta(statusB(B,G)), fail.
assertions().

%function to empty the database of all current facts
emptyAll():- retractall(arriving(A)), retractall(landed(B)), retractall(airborne(B)), retractall(opened(B)), retractall(closed(B)), retractall(delayed(B)), retractall(arriving(A,B)), retractall(arrivingE(C,F)), retractall(aircraft(C,D)), retractall(statusA(C,G)), retractall(statusB(B,G)), retractall(leaving(B)), retractall(leaving(A,B)), retractall(statusB(C,G)), retractall(leavingE(C,F)).

%function to count the number of answers to a query.
%used to answer quantity related questions by counting all the returned answers
counter(A,B):- A =.. List, length(List,2), count(List,B).

%functions handling different required counts
%each line handles a different type of quantity measure
count([arriving|[A]],B):- findall(*, arrive(_,A,_,_,_,_,_), Count), length(Count,B).
count([leaving|[A]],B):- findall(*, depart(_,A,_,_,_,_,_), Count), length(Count,B).
count([landed|[_]],B):- findall(*, arrive(_,_,_,_,_,_,'landed'), Count), length(Count,B).
count([airborne|[_]],B):- findall(*, arrive(_,_,_,_,_,_,'airborne'), Count), length(Count,C), findall(*, depart(_,_,_,_,_,_,'airborne'), Count2), length(Count2,D), B is C+D.
count([opened|[_]],B):- findall(*, depart(_,_,_,_,_,_,'opened'), Count), length(Count,B).
count([closed|[_]],B):- findall(*, depart(_,_,_,_,_,_,'closed'), Count), length(Count,B).
count([delayed|[_]],B):- findall(*, arrive(_,_,_,_,_,_,'delayed'), Count), length(Count,C), findall(*, depart(_,_,_,_,_,_,'delayed'), Count2), length(Count2,D), B is C+D.
count([flights|[A]],B):- findall(*, arrive(A,_,_,_,_,_,_), Count), length(Count,C), findall(*, depart(A,_,_,_,_,_,_), Count2), length(Count2,D), B is C+D.

%root point of sentence grammar, with semantic handling added:

%questions are split up into 3, with q3 relating to quantity questions,
%q2 to questions with text answers and q1 to yes/no questions
s(q3(S)) --> num(_), np(NP), vp2(VP), aux(_), p(_), {reduce(VP,NP,S)}.
s(q3(S)) --> num(_), np(_), vp2(S).
s(q2(S)) --> wh(_), d(_), np(_), adj(_), np(_), vp1(S).
s(q2(S)) --> wh(_), d(_), np(_), vp(S).
s(q2(S)) --> wh(_), vp(S).
s(q2(S)) --> wh(which), np(_), vp3(S).
s(q2(S)) --> wh(_), np(NP), vp(VP), {reduce(VP,NP,S)}.
s(q1(S)) --> np(NP), vp(VP), {reduce(VP,NP,S)}.
s(q1(S)) --> np(_), vp(S).

%list of possible noun phrase branches which could be followed
np(NP) --> aux(_), np(NP).
np(NP) --> n(_), np(NP).
np(_) --> n(_).
np(_) --> det(_).
np(NP) --> det(_), np(NP).
np(NP) --> p(_), np(NP).
np(NP) --> conj(_), np(NP).
np(_) --> conj(_).
np(NP) --> wh(_), np(NP).
np(NP) --> d(_), np(NP).
np(NP) --> pn(NP), np(_).
np(NP) --> pn(NP).

%list of all possible verb phrase branches which could be followed
vp(VP) --> p('on'), v1(VP).
vp(VP) --> v(VP).
vp(VP) --> v4(VP), p('to').
vp(VP) --> v2(VP), p('from').
vp(VP) --> v1(V), p('to'), np(N), {reduce(V,N,VP)}.
vp(VP) --> v1(V), np(_), p('to'), np(N), {reduce(V,N,VP)}.
vp(VP) --> v(V), np(_), p('from'), np(N), {reduce(V,N,VP)}.
vp(VP) --> v(V), np(N), {reduce(V,N,VP)}.
vp1(VP) --> v1(V), np(N), {reduce(V,N,VP)}.

vp2(VP) --> p('on'), v1(VP).
vp2(VP) --> v2(VP).
vp2(VP) --> v4(VP), p('to').
vp2(VP) --> v4(V), p('to'), np(N), {reduce(V,N,VP)}.
vp2(VP) --> v2(V), np(_), p('to'), np(N), {reduce(V,N,VP)}.
vp2(VP) --> v2(V), np(_), p('from'), np(N), {reduce(V,N,VP)}.
vp2(VP) --> v2(V), np(N), {reduce(V,N,VP)}.
vp2(VP) --> v2(V), np(N), {reduce(V,N,VP)}.

vp3(VP) --> p('on'), v1(VP).
vp3(VP) --> v3(VP).
vp3(VP) --> v3(V), p('to'), np(N), {reduce(V,N,VP)}.
vp3(VP) --> v3(V), np(_), p('to'), np(N), {reduce(V,N,VP)}.
vp3(VP) --> v3(V), np(_), p('from'), np(N), {reduce(V,N,VP)}.
vp3(VP) --> v3(V), np(N), {reduce(V,N,VP)}.
vp3(VP) --> v3(V), np(N), {reduce(V,N,VP)}.

%proper nouns such as 'Air Malta' or 'FR 5778'
pn(N) --> [N].

%list of accepted nouns
n(flight) --> [flight]. 
n(flights) --> [flights].
n(gate) --> [gate].
n(time) --> [time].
n(type) --> [type].
n(airline) --> [airline].

%list of auxiliary words
aux(has) --> [has].
aux(did) --> [did].
aux(are) --> [are].
aux(do) --> [do].
aux(does) --> [does].

%list of wh type questions which relate to sentences of type q2
wh(when) --> [when].
wh(what) --> [what].
wh(how) --> [how].
wh(which) --> [which].
wh(where) --> [where].

%quantity related questions all start with a structure of type num
num(_) --> wh(_), quantity(_).

%quantity indicator 'many'
quantity(many) --> [many].

%list of conjunction words which are included in theh language
conj(any) --> [any].
conj(of) --> [of].

%list of adjectives which are included in the language
adj(estimated) --> [estimated].
adj(estimate) --> [estimate].

%list of determinents which are included in the language
d(is) --> [is].
d(was) --> [was].

%list of propositions which are included in the language
p(off) --> [off].
p(on) --> [on].
p(from) --> [from].
p(to) --> [to].
p(for) --> [for].
p(there) --> [there].

%list of determiners which are included in the language
det(the) --> [the].
det(been) --> [been].

%list of verbs included in the language, with semantics being handled by means of the
%reduce function. Each verb may take 1 or 2 parameters. Verbs are split up into different
%groups to ensure grammatic and semantic correctness
v1(X^Y^statusB(X,Y)) --> [status].
v(X^Y^statusA(X,Y)) --> [status].
v(X^Y^statusB(X,Y)) --> [status].

v2(X^aircraft(X)) --> [aircrafts].
v(X^Y^aircraft(X,Y)) --> [aircraft].
v(X^Y^aircraft(X,Y)) --> [plane].
v2(X^aircraft(X)) --> [planes].

v(X^opened(X)) --> [opened].
v2(opened(_)) --> [opened].

v2(closed(_)) --> [closed].
v(X^closed(X)) --> [closed].

v(X^delayed(X)) --> [delayed].
v2(delayed(_)) --> [delayed].

v(X^landed(X)) --> [landed].
v(X^landed(X)) --> [land].
v2(landed(_)) --> [landed].


v(X^airborne(X)) --> [airborne].
v2(airborne(_)) --> [airborne].

v(X^arriving(X)) --> [arrive].
v(X^Y^arriving(X,Y)) --> [arrive].
v(X^Y^arriving(X,Y)) --> [arrival].
v1(X^Y^arrivingE(X,Y)) --> [arrival].
v(X^arriving(X)) --> [arriving]. 
v(X^arriving(X)) --> [arriving].
v2(X^arriving(X)) --> [arriving]. 
v2(X^arriving(X)) --> [arriving].
v2(X^arriving(X)) --> [arrive].
v3(X^Y^arriving(Y,X)) --> [arrive].

v(X^arriving(X)) --> [flying].
v(X^leaving(X)) --> [flying].
v1(X^Y^leaving(Y,X)) --> [flying].
v1(X^Y^leaving(Y,X)) --> [fly].
v(X^Y^arriving(Y,X)) --> [flying].
v(X^Y^arriving(Y,X)) --> [fly].
v2(X^Y^arriving(X,Y)) --> [fly].
v4(X^Y^leaving(X,Y)) --> [fly].
v2(X^flights(X)) --> [flights].

v(X^Y^leaving(X,Y)) --> [leave].
v(X^leaving(X)) --> [leave].
v(X^leaving(X)) --> [leaving].

v(X^Y^leaving(X,Y)) --> [depart].
v(X^Y^leaving(X,Y)) --> [departure].
v1(X^Y^leavingE(X,Y)) --> [departure].
v(X^leaving(X)) --> [departing].
v(X^leaving(X)) --> [depart].
v2(X^leaving(X)) --> [departing].
v2(X^leaving(X)) --> [depart].
v3(X^Y^leaving(Y,X)) --> [depart].