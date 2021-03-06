(set-info :source |fuzzsmt|)
(set-info :smt-lib-version 2.0)
(set-info :category "random")
(set-info :status unknown)
(set-logic QF_LIA)
(declare-fun v0 () Int)
(assert (let ((e1 1))
(let ((e2 2))
(let ((e3 (- v0)))
(let ((e4 (+ v0 e3)))
(let ((e5 (- e3 v0)))
(let ((e6 (- e5 e5)))
(let ((e7 (- e3 e4)))
(let ((e8 (* e3 e2)))
(let ((e9 (* (- e2) e3)))
(let ((e10 (* e4 e1)))
(let ((e11 (> e7 e3)))
(let ((e12 (>= v0 e9)))
(let ((e13 (<= e4 e9)))
(let ((e14 (<= e6 e9)))
(let ((e15 (> e6 e3)))
(let ((e16 (<= e9 e10)))
(let ((e17 (<= e7 e6)))
(let ((e18 (distinct e6 e3)))
(let ((e19 (= e3 e8)))
(let ((e20 (distinct v0 e8)))
(let ((e21 (< e5 e10)))
(let ((e22 (ite e17 v0 e5)))
(let ((e23 (ite e15 e9 e22)))
(let ((e24 (ite e20 e8 e3)))
(let ((e25 (ite e12 e4 e8)))
(let ((e26 (ite e14 e3 e23)))
(let ((e27 (ite e14 e3 v0)))
(let ((e28 (ite e18 e25 e6)))
(let ((e29 (ite e20 e10 v0)))
(let ((e30 (ite e14 e7 e28)))
(let ((e31 (ite e11 e23 e9)))
(let ((e32 (ite e19 e27 e3)))
(let ((e33 (ite e15 e32 e9)))
(let ((e34 (ite e15 e7 e32)))
(let ((e35 (ite e16 e24 e27)))
(let ((e36 (ite e13 e5 e9)))
(let ((e37 (ite e13 e33 e29)))
(let ((e38 (ite e21 e24 e8)))
(let ((e39 (>= e30 e5)))
(let ((e40 (> e27 e36)))
(let ((e41 (> e29 e23)))
(let ((e42 (distinct e32 e30)))
(let ((e43 (distinct e38 e37)))
(let ((e44 (<= e25 e37)))
(let ((e45 (> v0 e32)))
(let ((e46 (<= e29 e34)))
(let ((e47 (= e27 e38)))
(let ((e48 (< v0 e36)))
(let ((e49 (>= e28 e3)))
(let ((e50 (>= e29 e7)))
(let ((e51 (> e23 e26)))
(let ((e52 (distinct e5 e9)))
(let ((e53 (<= e3 e33)))
(let ((e54 (>= e34 e22)))
(let ((e55 (> e25 e32)))
(let ((e56 (= e8 e22)))
(let ((e57 (>= e7 e9)))
(let ((e58 (> e5 e27)))
(let ((e59 (distinct e37 e38)))
(let ((e60 (distinct e35 e26)))
(let ((e61 (>= e25 e22)))
(let ((e62 (> e10 e29)))
(let ((e63 (>= e30 e9)))
(let ((e64 (distinct e7 e8)))
(let ((e65 (> e34 e35)))
(let ((e66 (> e25 e26)))
(let ((e67 (> e8 e35)))
(let ((e68 (distinct e22 e4)))
(let ((e69 (> e8 e22)))
(let ((e70 (= e38 e32)))
(let ((e71 (> e30 e23)))
(let ((e72 (distinct e30 e9)))
(let ((e73 (> e22 e26)))
(let ((e74 (>= e5 e38)))
(let ((e75 (> e8 e23)))
(let ((e76 (<= e9 e24)))
(let ((e77 (< e27 e23)))
(let ((e78 (< e9 e27)))
(let ((e79 (>= e38 e25)))
(let ((e80 (= e7 e3)))
(let ((e81 (distinct e10 e6)))
(let ((e82 (= e6 e6)))
(let ((e83 (= e36 e28)))
(let ((e84 (< e29 e29)))
(let ((e85 (= e28 e8)))
(let ((e86 (distinct e25 e6)))
(let ((e87 (> e38 e22)))
(let ((e88 (distinct e30 e36)))
(let ((e89 (= e6 e30)))
(let ((e90 (<= e6 e8)))
(let ((e91 (< e34 e25)))
(let ((e92 (< e5 e33)))
(let ((e93 (< e37 e31)))
(let ((e94 (xor e77 e59)))
(let ((e95 (and e89 e11)))
(let ((e96 (not e91)))
(let ((e97 (not e46)))
(let ((e98 (not e45)))
(let ((e99 (= e53 e12)))
(let ((e100 (ite e69 e96 e55)))
(let ((e101 (and e47 e63)))
(let ((e102 (xor e18 e20)))
(let ((e103 (ite e68 e60 e76)))
(let ((e104 (ite e50 e66 e44)))
(let ((e105 (xor e61 e49)))
(let ((e106 (ite e87 e73 e78)))
(let ((e107 (=> e43 e71)))
(let ((e108 (or e52 e51)))
(let ((e109 (ite e74 e80 e54)))
(let ((e110 (xor e98 e13)))
(let ((e111 (=> e95 e106)))
(let ((e112 (xor e108 e102)))
(let ((e113 (not e109)))
(let ((e114 (and e85 e65)))
(let ((e115 (or e17 e14)))
(let ((e116 (ite e111 e103 e101)))
(let ((e117 (or e100 e92)))
(let ((e118 (=> e82 e81)))
(let ((e119 (= e79 e86)))
(let ((e120 (or e117 e99)))
(let ((e121 (and e64 e41)))
(let ((e122 (= e88 e67)))
(let ((e123 (xor e56 e93)))
(let ((e124 (and e83 e21)))
(let ((e125 (=> e115 e118)))
(let ((e126 (ite e70 e97 e97)))
(let ((e127 (or e114 e90)))
(let ((e128 (and e110 e16)))
(let ((e129 (= e127 e57)))
(let ((e130 (not e128)))
(let ((e131 (ite e84 e72 e123)))
(let ((e132 (= e107 e125)))
(let ((e133 (and e75 e39)))
(let ((e134 (xor e132 e42)))
(let ((e135 (= e105 e15)))
(let ((e136 (ite e40 e40 e122)))
(let ((e137 (or e134 e131)))
(let ((e138 (= e135 e94)))
(let ((e139 (xor e58 e137)))
(let ((e140 (xor e113 e116)))
(let ((e141 (not e62)))
(let ((e142 (and e130 e120)))
(let ((e143 (and e121 e139)))
(let ((e144 (ite e142 e19 e129)))
(let ((e145 (= e104 e112)))
(let ((e146 (xor e140 e145)))
(let ((e147 (and e119 e144)))
(let ((e148 (or e133 e141)))
(let ((e149 (xor e126 e138)))
(let ((e150 (not e124)))
(let ((e151 (not e146)))
(let ((e152 (ite e136 e148 e48)))
(let ((e153 (xor e147 e149)))
(let ((e154 (= e143 e152)))
(let ((e155 (not e150)))
(let ((e156 (ite e154 e153 e153)))
(let ((e157 (=> e151 e156)))
(let ((e158 (and e155 e157)))
e158
)))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))

(check-sat)
