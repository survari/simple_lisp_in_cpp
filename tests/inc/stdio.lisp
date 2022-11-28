(let 'print (fn ('e)
    (each e (fn ('f)
        (printc f)))))

(let 'println (fn ('e)
    (print (concat e 10))))

(let 'i2s_2 (fn ('num 'str)
    (if (> num 1)
        (i2s_2 (/ num 10) (concat (+ (% num 10) 48) str))
        (if (= num 1)
            (concat str "1") str))))

(let 'i2s (fn ('num)
    (if (< num 10)
        (+ num 48)
        (i2s_2 num ""))))