# (let 'add (fn ('a 'b) (+ a b)))
# (add 1 2)
# (printc (add
#     (add
#         (add 63 1)
#         1 )
#     1))

# (let 'a (fn '(e) (printc e)))
# (each "Hallo Welt!\n" (fn '(e) (printc e))

(let 'range (fn ('a 'b)
    (if (>= a b)
        a
        (concat a (range a+1 b)))))

(let 'print (fn ('str)
    (each str
        (fn '(e) (printc e)))))

(each (range 0 10) (fn ('e) (print "Hallo Welt 2!\n")))