;; Kenneth Woodard
;; CS365 PA3
;; 4/12/23

;; calculate the number of elements in
;; a given list
(define (list-len l)
	(if (null? l)
		0
		(+ 1 (list-len (cdr l)))))

;; create a list of integers from 1 to n, 
;; given the integer n
(define (inc-list n)
    (if (= n 0)
		'()
		(append (inc-list (- n 1)) (list n))))

;; given a list, reverse the order and return it
(define (rev-list l)
    (if (null? l)
		'()
		(append (rev-list (cdr l)) (list (car l)))))

;; a function that takes in another function and its inputs
;; and then executes it and returns the results
(define (my-map f l)
    (if (null? l)
		'()
		(cons (apply f (list (car l)))
			(my-map f (cdr l)))))

;; an implementation of the merge-sort algorithm. 
;; it takes a list, and returns the same list sorted
(define (merge-sort l)
	(define (split l)
		(define (split-rec pair)
			(let ((front (car pair)) (back (cdr pair)))
				(if (>= (length front) (length back))
					pair
					(split-rec (cons (append front (list (car back))) (cdr back))))))
		(split-rec (cons (list (car l)) (cdr l))))

	(define (merge left right)
		(cond ((null? left) right)
			((null? right) left)
			((<= (car left) (car right))
				(cons (car left) (merge (cdr left) right)))
			(else
				(cons (car right) (merge left (cdr right))))))

	(cond ((null? l) '())
		((null? (cdr l)) l)
		(else
			(let* ((halves (split l))
					(left (car halves))
					(right (cdr halves)))
				(merge (merge-sort left) (merge-sort right))))))



