findPath(A,Z,[A,Z],D):- edge(A,Z).
findPath(A,Z,[A,B|R],TD):- edge(A,B,D), findPath(B,Z,R,RD), TD is D + RD.