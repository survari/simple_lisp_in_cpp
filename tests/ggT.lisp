# functions to write strings as printc can only write a single char
(let 'print (fn ('e) (each e (fn ('f) (printc f)))))
(let 'println (fn ('e) (print (concat e 10))))

# recursively turn a number into a string
(let 'i2s_2 (fn ('num 'str)
    (if (> num 1)
        (i2s_2 (floor (/ num 10)) (concat (+ (% num 10) 48) str))
        (if (= num 1) (concat "1" str) str))))

(let 'i2s (fn ('num)
    (if (< num 10) (+ num 48) (i2s_2 num :str '()))))

# euclidean algorithm
(let 'ggT (fn ('a 'b)
    (if (<= a 0)
        b
        (if (<= b 0)
            a
            (if (> a b)
                (ggT (- a b) b)
                (ggT a (- b a)))))))

(let 'test_cases '((12 3) (62 64) (24 64) (56 13) (21 35)))
(each test_cases (fn ('e)       # e is always (x y)
    (let 'a (head e))           # head returns the first element
    (let 'b (head (tail e)))    # tail returns the list without the last element

    (println (concat "ggT(" (i2s a) ", " (i2s b) ") = " (i2s (ggT a b))))))