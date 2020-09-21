(set-option :produce-unsat-model-interpolants true)
(set-logic QF_UFNRA)
(declare-const v3 Bool)
(declare-const v4 Bool)
(declare-const r1 Real)
(declare-const r5 Real)
(declare-const v13 Bool)
(declare-const v14 Bool)
(declare-const v15 Bool)
(declare-const r6 Real)
(declare-const r7 Real)
(declare-const r8 Real)
(declare-const r9 Real)
(declare-const r10 Real)
(declare-const r11 Real)
(declare-const r12 Real)
(declare-const r13 Real)
(assert (or (and (> 3.13140760 (+ r1 0.6873817 (/ 0.6873817 r1) 3515183.0) (/ 0.6873817 r1) (/ 0.6873817 r1)) (> 3.13140760 (+ r1 0.6873817 (/ 0.6873817 r1) 3515183.0) (/ 0.6873817 r1) (/ 0.6873817 r1)) (> (+ r1 0.6873817 (/ 0.6873817 r1) 3515183.0) (/ 273.0 (+ r1 0.6873817 (/ 0.6873817 r1) 3515183.0)) (/ 273.0 (+ r1 0.6873817 (/ 0.6873817 r1) 3515183.0))) v4 (> (+ r1 0.6873817 (/ 0.6873817 r1) 3515183.0) (/ 273.0 (+ r1 0.6873817 (/ 0.6873817 r1) 3515183.0)) (/ 273.0 (+ r1 0.6873817 (/ 0.6873817 r1) 3515183.0))) (or (= r5 (/ 273.0 (+ r1 0.6873817 (/ 0.6873817 r1) 3515183.0)) r1) (= r5 (/ 273.0 (+ r1 0.6873817 (/ 0.6873817 r1) 3515183.0)) r1) v3 (< 273.0 (/ 273.0 (+ r1 0.6873817 (/ 0.6873817 r1) 3515183.0)) r1) v15 (distinct 0.6873817 (+ r1 0.6873817 (/ 0.6873817 r1) 3515183.0) r1)) v13 (or (= r5 (/ 273.0 (+ r1 0.6873817 (/ 0.6873817 r1) 3515183.0)) r1) (= r5 (/ 273.0 (+ r1 0.6873817 (/ 0.6873817 r1) 3515183.0)) r1) v3 (< 273.0 (/ 273.0 (+ r1 0.6873817 (/ 0.6873817 r1) 3515183.0)) r1) v15 (distinct 0.6873817 (+ r1 0.6873817 (/ 0.6873817 r1) 3515183.0) r1)) (<= 3.13140760 (+ r1 0.6873817 (/ 0.6873817 r1) 3515183.0) r7 3515183.0)) (distinct v14 (xor (> (+ r1 0.6873817 (/ 0.6873817 r1) 3515183.0) (/ 273.0 (+ r1 0.6873817 (/ 0.6873817 r1) 3515183.0)) (/ 273.0 (+ r1 0.6873817 (/ 0.6873817 r1) 3515183.0))) (= (/ 273.0 (+ r1 0.6873817 (/ 0.6873817 r1) 3515183.0)) r5)) (>= (+ r1 0.6873817 (/ 0.6873817 r1) 3515183.0) r1 r1 (/ 0.6873817 r1)) (distinct 0.6873817 273.0 0.6873817)) (= (/ 273.0 (+ r1 0.6873817 (/ 0.6873817 r1) 3515183.0)) 3.13140760)))
(assert (or (> 3.13140760 3515183.0 43133730.0) (or (= r5 (/ 273.0 (+ r1 0.6873817 (/ 0.6873817 r1) 3515183.0)) r1) (= r5 (/ 273.0 (+ r1 0.6873817 (/ 0.6873817 r1) 3515183.0)) r1) v3 (< 273.0 (/ 273.0 (+ r1 0.6873817 (/ 0.6873817 r1) 3515183.0)) r1) v15 (distinct 0.6873817 (+ r1 0.6873817 (/ 0.6873817 r1) 3515183.0) r1)) (or v14 v14)))
(check-sat-assuming-model (r1 r5 r6 r7 r8 r9 r10 r11 r12 r13 ) (8.0 0.5267 76425293.0 91625528.0 0.5767521 651.0 8600278.0 4674.75361 0.6 0.0732225489 ))
(get-unsat-model-interpolant)