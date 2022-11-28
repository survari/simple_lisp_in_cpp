(include "inc/stdio.lisp")

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

# (let 'decr "11, 1 Alle Bewohner der Erde aber hatten eine Sprache und ein und dieselben Worte. 2 Als sie nun von Osten aufbrachen, fanden sie eine Ebene im Land Schinar und liessen sich dort nieder. 3 Und sie sagten zueinander: Auf, wir wollen Ziegel formen und sie hart brennen. So diente ihnen der Ziegel als Baustein, und der Asphalt diente ihnen als Mörtel. 4 Und sie sagten: Auf, wir wollen eine Stadt bauen und einen Turm, dessen Spitze bis an den Himmel reicht, und uns so einen Namen machen, damit wir uns nicht über die ganze Erde zerstreuen. 5 Da stieg der EWIGE herab, um die Stadt zu besehen und den Turm, die die Menschen bauten. 6 Und der EWIGE sprach: Sieh, alle sind ein Volk und haben eine Sprache. Und dies ist erst der Anfang ihres Tuns. Nun wird ihnen nichts mehr unmöglich sein, was immer sie sich zu tun vornehmen. 7 Auf, lasst uns hinabsteigen und dort ihre Sprache verwirren, dass keiner mehr die Sprache des andern versteht. 8 Und der EWIGE zerstreute sie von dort über die ganze Erde, und sie liessen davon ab, die Stadt zu bauen. 9 Darum nannte man sie Babel, denn dort hat der Herr die Sprache aller Bewohner der Erde verwirrt, und von dort hat der EWIGE sie über die ganze Erde zerstreut.")
(let 'decr "Hallo Welt, kannst du mich hören? Dies ist nur ein kleiner Test!")
(let 'encr (caesar_encode decr 1))

(println encr)
(println (caesar_decode encr 1))