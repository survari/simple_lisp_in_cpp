# Simple Lisp in C++ (SLICPP)

This is a very simple Lisp-like language for learning purpose.

Code example:
```lisp
(load "inc/stdio.lisp")

(let 'caesar_shift (fn ('message 'shift)
    (each message (fn ('e)
        (if (and (>= e 65) (<= e 90))
            (+ 65 (% (+ (- e 65) shift) 26))

            (if (and (>= e 97) (<= e 122))
                (+ 97 (% (+ (- e 97) shift) 26))
                e))))))

(let 'caesar_encode (fn ('message 'shift)
    (caesar_shift message shift)))

(let 'caesar_decode (fn ('message 'shift)
    (caesar_shift message (- 26 shift))))

(let 'decr "Hello world, this is a small test!")
(let 'encr (caesar_encode decr 1))

(println encr)
(println (caesar_decode encr 1))
```

## Todo

- [ ] FFI integration to load C/C++ code from SLICPP
- [ ] Add builtin function for user input