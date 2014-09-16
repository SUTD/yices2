(set-info :source |fuzzsmt|)
(set-info :smt-lib-version 2.0)
(set-info :category "random")
(set-info :status unknown)
(set-logic QF_UFBV)
(declare-fun f0 ( (_ BitVec 15)) (_ BitVec 9))
(declare-fun f1 ( (_ BitVec 6) (_ BitVec 4)) (_ BitVec 12))
(declare-fun p0 ( (_ BitVec 16) (_ BitVec 16)) Bool)
(declare-fun p1 ( (_ BitVec 8)) Bool)
(declare-fun v0 () (_ BitVec 11))
(assert (let ((e1(_ bv52 7)))
(let ((e2 (f1 ((_ extract 5 0) e1) ((_ extract 4 1) e1))))
(let ((e3 (bvadd e2 e2)))
(let ((e4 (bvor ((_ sign_extend 4) e1) v0)))
(let ((e5 (f0 ((_ zero_extend 3) e2))))
(let ((e6 (ite (p1 ((_ extract 7 0) e4)) (_ bv1 1) (_ bv0 1))))
(let ((e7 (ite (p0 ((_ zero_extend 9) e1) ((_ zero_extend 4) e3)) (_ bv1 1) (_ bv0 1))))
(let ((e8 (p1 ((_ zero_extend 7) e6))))
(let ((e9 (p1 ((_ zero_extend 1) e1))))
(let ((e10 (p1 ((_ extract 10 3) v0))))
(let ((e11 (bvsgt ((_ sign_extend 4) e1) v0)))
(let ((e12 (p1 ((_ sign_extend 1) e1))))
(let ((e13 (bvsgt ((_ zero_extend 5) e1) e2)))
(let ((e14 (p0 ((_ zero_extend 15) e6) ((_ sign_extend 7) e5))))
(let ((e15 (bvsle e6 e6)))
(let ((e16 (bvslt ((_ zero_extend 2) e1) e5)))
(let ((e17 (bvuge e2 e2)))
(let ((e18 (distinct e3 ((_ sign_extend 5) e1))))
(let ((e19 (bvsle e2 ((_ sign_extend 11) e6))))
(let ((e20 (bvult e4 ((_ sign_extend 10) e7))))
(let ((e21 (not e17)))
(let ((e22 (ite e14 e14 e21)))
(let ((e23 (or e10 e22)))
(let ((e24 (= e12 e18)))
(let ((e25 (not e8)))
(let ((e26 (xor e13 e23)))
(let ((e27 (and e26 e9)))
(let ((e28 (ite e15 e16 e25)))
(let ((e29 (not e20)))
(let ((e30 (not e19)))
(let ((e31 (not e11)))
(let ((e32 (not e29)))
(let ((e33 (not e24)))
(let ((e34 (or e30 e32)))
(let ((e35 (and e34 e31)))
(let ((e36 (xor e28 e28)))
(let ((e37 (and e27 e33)))
(let ((e38 (ite e35 e36 e37)))
e38
)))))))))))))))))))))))))))))))))))))))

(check-sat)
(set-option :regular-output-channel "/dev/null")
(get-model)