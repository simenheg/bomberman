(add-to-load-path (dirname (current-filename)))
(load-from-path "common-blocks.scm")

(define rows 9)
(define cols 13)
(define p1-start (point 0 0))
(define p2-start (point (- cols 1) (- rows 1)))

(define M
  '(g ↓ c c c c c c c c c c c
    g ↓ c p c p c p c p c p c
    c ↓ c c c c c c c c c c c
    c ↓ c p c p c p c p c p c
    c c c c c c c c c c c c c
    c p c p c p c p c p c p c
    c c c c c c c c c c c c c
    c p c p c p c p c p c p g
    c c c c c c c c c c c g g))
