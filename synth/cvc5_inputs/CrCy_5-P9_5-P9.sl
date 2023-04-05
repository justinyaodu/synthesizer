(set-logic BV)
(define-fun Spec ((k1 Bool) (k2 Bool) (k3 Bool)) Bool (xor k1 (and k3 (not k2))))
(synth-fun Imp ((k1 Bool) (k2 Bool) (k3 Bool)) Bool
((Start Bool) (depth1 Bool) (depth2 Bool) )
((Start Bool ((and depth1 depth1) (or depth1 depth1) (xor depth1 depth1) (not depth1) ))
(depth1 Bool ((and depth2 depth2) (or depth2 depth2) (xor depth2 depth2) (not depth2) ))
(depth2 Bool (k1 k2 k3 ))
))
(declare-var k1 Bool)
(declare-var k2 Bool)
(declare-var k3 Bool)
(constraint (= (Spec k1 k2 k3) (Imp k1 k2 k3)))
(check-synth)
