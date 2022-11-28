(let 'a 65)
(let 'b (+ a 1))

(let 'print (fn ('e)
    (each e (fn ('e) (printc e)))))

(print (concat a 10 b 10))

(let 'e (fn ('a 'b)
    (printc a)
    (printc b)))

# (debug e)
(eval print (67 10 68 10))

# (if (> a b)
#     (printc a)
#     (printc b))