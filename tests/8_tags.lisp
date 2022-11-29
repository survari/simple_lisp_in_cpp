# strings are number-arrays with the first element having the string tag
("ABC") # becomes (:str 65 66 67)

# we can define objects like this!
(let person (:name "abert" :alter "53"))
(println person.name)       # albert    - is formed into (of person 'name) || a.b.c => (of (of a "b") "c")
(println (of person 'name)) # albert    - of is the same as .; of works with both words and strings
(eval "person.name")        # evaluates into and is the same as (eval person.name) => albert

# tags: returns all tags of each elements (even duplicates!) as words
(debug (tags person))   # (name alter)

# wordstring: turns words (like n) into strings (like "n")
(debug (each (tags person) (wordstring e))) # ("name" "albert")