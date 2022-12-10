(load "./inc/stdio.lisp")

(println "======== Test Fak")
(let 'fak (fn '(0) (1)))
(let 'fak (fn '(1) (1)))
(let 'fak (fn '('e)
    (* e (fak (- e 1)))))

(debug (fak 100))