(load "./inc/stdio.lisp")

(let 'fak (fn '(0) (1)))
(let 'fak (fn '(1) (1)))
(let 'fak (fn '('e) (* e (fak (- e 1)))))

(let 'zero 'zero)
(let 'suc (fn '((suc zero)) zero))
(let 'suc (fn '((suc 'X)) X))
(println (suc (suc (suc zero))))