;;; make-block name             - full name of the block, used for reference.
;;;            short-name       - short name, used in map drawing.
;;;            sprite           - name of the image to use as a sprite.
;;;            walkable?        - is the block player-walkable?
;;;            flame-blocking?  - does the block kill flames?
;;;            flame-function   - what happens when touched by a flame?
;;;            trigger-function - what happens when touched by a player?

(define blocks
  (list
   (make-block
    'scorched-grass 'ĝ (sprite "tile_earth") #t #f
    none
    none)

   (make-block
    'grass 'g (sprite "tile_grass") #t #f
    (lambda (x y)
      (turn-into 'scorched-grass x y))
    none)

   (make-block
    'plate 'p (sprite "tile_plate") #f #t
    none
    none)

   (make-block
    'crate 'c (sprite "tile_crate") #f #t
    (lambda (x y)
      (case (random 8)
        ((0) (turn-into 'powerup-flame x y))
        ((1) (turn-into 'powerup-bombs x y))
        ((2) (turn-into 'powerup-speed x y))
        ((3 4 5 6 7) (turn-into 'grass x y))))
    none)

   (make-block
    'belt-right '→ (animation "belt-right") #t #f
    none
    (lambda (player x y)
      (move-player player 0.1 0.0)))

   (make-block
    'belt-down '↓ (animation "belt-down") #t #f
    none
    (lambda (player x y)
      (move-player player 0.0 0.1)))

   (make-block
    'belt-left '← (animation "belt-left") #t #f
    none
    (lambda (player x y)
      (move-player player -0.1 0.0)))

   (make-block
    'belt-up '↑ (animation "belt-up") #t #f
    none
    (lambda (player x y)
      (move-player player 0.0 -0.1)))

   (make-block
    'powerup-flame 'f (sprite "powerup_fire") #t #t
    (lambda (x y)
      (turn-into 'grass x y))
    (lambda (player x y)
      (inc-player-firepower player 1)
      (turn-into 'grass x y)))

   (make-block
    'powerup-bombs 'b (sprite "powerup_bomb") #t #t
    (lambda (x y)
      (turn-into 'grass x y))
    (lambda (player x y)
      (inc-player-bombs player 1)
      (turn-into 'grass x y)))

   (make-block
    'powerup-speed 's (sprite "powerup_speed") #t #t
    (lambda (x y)
      (turn-into 'grass x y))
    (lambda (player x y)
      (inc-player-speed player 0.03)
      (turn-into 'grass x y)))

   (make-block
    'mud 'm (sprite "tile_mud") #t #f
    none
    (lambda (player x y)
      (modify-player-movement player 0.3)))))
