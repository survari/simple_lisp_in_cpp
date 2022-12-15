(debug (filter (fn ('e) (= 0 (% e 2))) (1 2 3 4 5 6 7 8 9 10)))
(debug (collect (fn ('last 'next) (+ last next)) (1 2 3 4 5 6 7 8 9 10)))

(dfor 0 10000000 (fn ('i)
    (debug i)))