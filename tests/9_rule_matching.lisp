(let 'fak (fn (1) (0)))
(let 'fak (fn ('e) (* e (fak (- e 1)))))

(debug (fak 3))