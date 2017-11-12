#lang racket

(define member?
  (lambda (x list)
    (cond ((null? list) #f)
          ((equal? x (car list)) #t)
          (else (member? x (cdr list))))))

(define Subset?
  (lambda (l1 l2)
    (cond ((null? l1) #t)
          ((member? (car l1) l2) (Subset? (cdr l1) l2))
          (else #f))))

(define remDup
  (lambda (l)
    (cond ((null? l) '())
          ((member? (car l) (cdr l)) (remDup (cdr l)))
          (else (cons (car l) (remDup (cdr l)))))))

(define union
  (lambda (l1 l2)
    (remDup (append l1 l2))))

(define back
    (lambda (x list)
      (cond ((null? list) '())
            ((equal? (car (cdr(car list))) x) (cons (car (car list)) (back x (cdr list))))
            (else (back x (cdr list))))))

(define front
    (lambda (x list)
      (cond ((null? list) '())
            ((equal? (car (car list)) x) (cons (car (cdr (car list))) (front x (cdr list))))
            (else (front x (cdr list))))))

(define element?
  (lambda (x list)
    (cond ((null? list) #f)
          ((equal? x (car list)) #t)
          (else (element? x (cdr list))))))

(define product
  (lambda (x l)
         (cond ((or (null? x) (null? l)) '())
               (else (append (list(list x (car l))) (product x (cdr l)))))))

(define product2
  (lambda (l1 l2)
         (cond ((or (null? l1) (null? l2)) '())
               (else (append (product (car l1) l2) (product2 (cdr l1) l2))))))

(define tempTransitive?
  (lambda (list1 list2)
    (cond ((null? list1) #t)
          ((not (Subset? (product2 (back (car (cdr (car list1))) list2) (front (car (cdr (car list1))) list2)) list2)) #f)
          (else (tempTransitive? (cdr list1) list2)))))

(define Transitive?
  (lambda (list)
    (tempTransitive? list list)))


(define tempAnti-Transitive?
  (lambda (list1 list2)
    (cond ((or (null? list1) (null? list2)) #t)
          ((and (Subset? (product2 (back (car (cdr (car list1))) list2) (front (car (cdr (car list1))) list2)) list2) (not (null? (product2 (back (car (cdr (car list1))) list2) (front (car (cdr (car list1))) list2))))) #f)
          (else (tempAnti-Transitive? (cdr list1) list2)))))

(define Anti-Transitive?
  (lambda (list)
    (tempAnti-Transitive? list list)))


(define Irreflexive?
  (lambda (list)
    (cond ((null? list) #t)
          ((equal? (car (car list)) (car (cdr (car list)))) #f)
          (else (Irreflexive? (cdr list))))))

(define tempCompose
  (lambda (t1 l1 l2)
    (cond ((null? t1) '())
          (else (append  (product2 (back (car (cdr (car t1))) l1) (front (car (cdr (car t1))) l2)) (tempCompose (cdr t1) l1 l2))))))


(define Compose
  (lambda (l1 l2)
    (union (tempCompose l1 l1 l2) (tempCompose l2 l1 l2))))
