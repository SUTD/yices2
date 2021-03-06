(set-logic QF_BV)
(set-info :status sat)

(declare-fun c () (_ BitVec 8))
(declare-fun s () (_ BitVec 8))
(declare-fun x () (_ BitVec 8))

(assert (= c #b00000111))
(assert (= s #b01110000))

(assert (= (bvshl c x) s))

(check-sat)

(exit)
