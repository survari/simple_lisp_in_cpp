##
new_namespace:
(def a 3)
:new_namespace
##

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

(let 'fib (fn ('n)
      (if (= 0 n)
        0
        (fib1 0 1 1 n))))

(let 'fib1 (fn ('f1 'f2 'i 'to)
    (if (= i to)
        f2
        (fib1 f2 (+ f1 f2) (+ i 1) to))))

(println (concat "Fib(250) = " (i2s (fib 250))))

(let 'main (fn ('args)
    # (println new_namespace.a)
    (println "Moin!")
    (printf "Hallo Fib(250) = {}\n" (fib 250))
    (println "Moin2!")

    # (println {
    #     "person": "maria",
    #     "age": 43,
    #     "rev": (fn ('a) (rev a))    # reverse list a
    # }.name)

    (printf "argc: {}\n" (len args))
    (each args (fn ('e) (println e)))))

# (println (concat "Fib(250) = " (i2s 250)))

##
\w = [a-zA-Z]+[a-zA-Z0-9-_]*

Types:
    0-9+    - numbers
    \w      - variable value
    '\w     - variable name (word, for definitions)
    ".*"    - string - is an array of numbers
    (...)   - array/list
    {...}   - json

Builtin functions:
    (definitions)
    ~~defc  - load function from standard c library~~
    let     - define variable

    (structure)
    while   q a ...   - run all args a... while q == 0
    each    var f     - for each element e in var runs f(e)
    if      q a b     - runs a if q==0, else runs b

    (list)
    concat                      - concat list
    len     a / len a b c ...   - length of list / length of all lists as list      (len (1 2) (4) (1 2 3)) => (2 1 3)  |   (+ (len (1 2) (4) (1 2 3))) => 6
    rev                         - reverse
    at      arr pos             - get element at position

    (math)
    +       a b c ...     -
    -       a b           -
    *       a b c ...     -
    /       a b           -
    %       a b           -
    pow     a p           -
    sqrt    p n           -
    log     b n           -

    (logic)
    &       - and
    |       - or
    ^       - xor
    ~       - not
    =       - equal
    <       - less
    >       - more
    <=      - less or equal
    >=      - bigger or equal
    !=      - not equal

    (io)
    print           -
    println         -
    printf          -
    input           -

    (fs)
    read-file           -
    write-file          -
    delete-file         -
    read-directory      -
    create-directory    -
    delete-directory    -
    fstat               -

    (expression)
    fn  args expr1 ...  - lambda
    eval list1 ...      - evaluate unevaluable list -> '(+ 1 2) can be evaluated by (eval '(+ 1 2))
##