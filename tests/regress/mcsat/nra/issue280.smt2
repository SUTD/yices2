(set-logic QF_NRA)
(declare-fun x () Real)
(declare-fun y () Real)
(declare-fun z () Real)
(assert
 (let (($x77 (not (<= (* y (/ 40.0 441.0)) x))))
 (let (($x108 (not (not (and (not (and (<= z 75.0) (<= 0.0 z))) $x77)))))
 (not (not (not $x108))))))
(assert
 (let (($x77 (not (<= (* y (/ 40.0 441.0)) x))))
 (let (($x108 (not (not (and (not (and (<= z 75.0) (<= 0.0 z))) $x77)))))
 (let (($x24 (not (not (not $x108)))))
 (let (($x65 (<= 0.0 z)))
 (let (($x198 (not $x65)))
 (let (($x91 (<= z 75.0)))
 (let (($x68 (and $x91 $x65)))
 (let (($x214 (<= 0.0 y)))
 (let (($x123 (not (<= z (/ 1.0 21.0)))))
 (let (($x109 (or $x123 $x214)))
 (let (($x168 (or $x198 $x109)))
 (let (($x50 (and (not (not $x168)) (and $x168 $x68))))
 (let ((?x21 (+ (+ (/ 4.0 11.0) (* y (/ 160.0 33.0))) (* x (/ (- 588.0) 11.0)))))
 (let ((?x33 (+ (+ (/ (- 19343.0) 165.0) (* y (/ (- 841.0) 6.0))) (* x (/ 4.0 2.0)))))
 (let ((?x126 (+ (+ (/ (- 667.0) 550.0) (* y (/ 116.0 825.0))) (* x (/ (- 4.0) 2.0)))))
 (let (($x159 (and (not (not (not (<= (* z (+ ?x126 (* z ?x33))) ?x21)))) (not (and (and $x50 (not $x77)) $x198)))))
 (let (($x19 (not $x123)))
 (let ((?x90 (* x x)))
 (let (($x43 (= ?x90 (+ 1.0 (* y (* y (- 1.0)))))))
 (let (($x66 (and $x43 $x19)))
 (let (($x178 (and (not (not (and $x66 (and $x66 (not (not (and $x214 $x19))))))) $x19)))
 (let ((?x25 (+ (+ (/ 8.0 11.0) (* y (/ 1.0 11.0))) (* x (/ (- 1.0) 11.0)))))
 (let ((?x78 (* z (+ (/ (- 4.0) 1.0) (* z (/ (- 1.0) 1.0))))))
 (let ((?x29 (* z (+ (/ (- 5.0) 2.0) (* z (+ (/ (- 1.0) 3.0) ?x78))))))
 (let ((?x146 (* z (+ (/ (- 6.0) 1.0) (* z (+ (/ (- 1.0) 5.0) ?x29))))))
 (let (($x166 (and (and (and $x214 $x123) (not (not (<= ?x146 ?x25)))) (or $x123 (<= 0.0 x)))))
 (let (($x129 (and (and (and (and (not $x198) $x198) (not $x198)) (not (not (not $x166)))) $x50)))
 (let (($x150 (and (not (and (and (and (not $x91) (and $x109 $x91)) $x129) $x168)) $x43)))
 (let (($x158 (and (and (not (and (and (not $x198) $x166) (not (not $x109)))) $x150) (and $x178 (not (not $x159))))))
 (not (not (and (and $x24 (not (not $x158))) $x24)))))))))))))))))))))))))))))))))
(check-sat)
