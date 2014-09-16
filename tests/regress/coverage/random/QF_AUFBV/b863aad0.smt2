(set-info :source |fuzzsmt|)
(set-info :smt-lib-version 2.0)
(set-info :category "random")
(set-info :status unknown)
(set-logic QF_AUFBV)
(declare-fun v0 () (_ BitVec 11))
(declare-fun a1 () (Array  (_ BitVec 16)  (_ BitVec 5)))
(declare-fun a2 () (Array  (_ BitVec 6)  (_ BitVec 15)))
(assert (let ((e3(_ bv171 8)))
(let ((e4(_ bv174 8)))
(let ((e5 (bvshl v0 v0)))
(let ((e6 (ite (= v0 ((_ zero_extend 3) e3)) (_ bv1 1) (_ bv0 1))))
(let ((e7 (ite (= (_ bv1 1) ((_ extract 3 3) e4)) e5 e5)))
(let ((e8 (store a1 ((_ sign_extend 8) e3) ((_ extract 5 1) e7))))
(let ((e9 (select a1 ((_ sign_extend 5) v0))))
(let ((e10 (store a1 ((_ zero_extend 8) e3) ((_ extract 6 2) e3))))
(let ((e11 (bvsub e5 ((_ sign_extend 6) e9))))
(let ((e12 (bvneg v0)))
(let ((e13 (bvsdiv e4 e3)))
(let ((e14 (bvnor v0 ((_ sign_extend 10) e6))))
(let ((e15 ((_ rotate_left 10) e7)))
(let ((e16 (bvult e13 e4)))
(let ((e17 (bvult e14 e11)))
(let ((e18 (bvsle e5 ((_ zero_extend 3) e3))))
(let ((e19 (distinct ((_ zero_extend 3) e13) e12)))
(let ((e20 (bvsle e11 e15)))
(let ((e21 (bvule e12 ((_ sign_extend 3) e13))))
(let ((e22 (distinct e5 ((_ sign_extend 10) e6))))
(let ((e23 (= e12 e5)))
(let ((e24 (distinct e5 ((_ sign_extend 3) e13))))
(let ((e25 (= e14 ((_ sign_extend 3) e3))))
(let ((e26 (bvsge e5 ((_ zero_extend 3) e4))))
(let ((e27 (bvsge e9 ((_ zero_extend 4) e6))))
(let ((e28 (= ((_ zero_extend 3) e13) e12)))
(let ((e29 (bvult ((_ sign_extend 6) e9) e7)))
(let ((e30 (bvsgt e5 e11)))
(let ((e31 (= e7 e5)))
(let ((e32 (bvult e7 e14)))
(let ((e33 (bvule e7 e11)))
(let ((e34 (= e12 ((_ sign_extend 3) e3))))
(let ((e35 (= e12 ((_ sign_extend 10) e6))))
(let ((e36 (distinct e12 v0)))
(let ((e37 (or e36 e19)))
(let ((e38 (or e26 e22)))
(let ((e39 (and e20 e29)))
(let ((e40 (and e21 e23)))
(let ((e41 (=> e17 e38)))
(let ((e42 (or e28 e16)))
(let ((e43 (and e35 e42)))
(let ((e44 (xor e41 e39)))
(let ((e45 (xor e37 e30)))
(let ((e46 (and e24 e32)))
(let ((e47 (= e43 e25)))
(let ((e48 (and e44 e34)))
(let ((e49 (and e31 e40)))
(let ((e50 (ite e27 e49 e27)))
(let ((e51 (= e46 e47)))
(let ((e52 (ite e51 e18 e48)))
(let ((e53 (and e52 e45)))
(let ((e54 (and e33 e53)))
(let ((e55 (xor e54 e54)))
(let ((e56 (or e50 e50)))
(let ((e57 (or e56 e55)))
(let ((e58 (and e57 (not (= e3 (_ bv0 8))))))
(let ((e59 (and e58 (not (= e3 (bvnot (_ bv0 8)))))))
e59
))))))))))))))))))))))))))))))))))))))))))))))))))))))))))

(check-sat)
(set-option :regular-output-channel "/dev/null")
(get-model)