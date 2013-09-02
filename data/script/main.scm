(define (get-map) (map short-name->block-index M))
(define (get-block-sprites) (map block-sprite blocks))

(define (map-row M n) (list-head (list-tail M (* n cols)) cols))
(define (map-ref M x y) (list-ref (map-row M y) x))

(define gfx-dir "data/gfx/")
(define gfx-suffix ".png")
(define (sprite name) (string-append gfx-dir name gfx-suffix))
(define (animation? sprite) (list? sprite))
(define (animation-frames animation) (length animation))

(define (animation name)

  (define (dir->file-list dir)
    (let ((next (readdir dir)))
      (cond ((eof-object? next) '())
            ((or (string=? next ".")
                 (string=? next ".."))
             (dir->file-list dir))
            (else (cons next (dir->file-list dir))))))

  (define (file-name-trim s)
    (string-trim-both s (char-set-union char-set:letter
                                        char-set:punctuation)))

  (define (file-name-enumerate s1 s2)
    (< (string->number (file-name-trim s1))
       (string->number (file-name-trim s2))))

  (map (lambda (s) (string-append gfx-dir name "/" s))
       (sort (dir->file-list (opendir (string-append gfx-dir name)))
             file-name-enumerate)))

(define (make-block name short-name sprite walkable? flame-blocking?
                    flame-function trigger-function)
  (list name
        short-name
        sprite
        walkable?
        flame-blocking?
        flame-function
        trigger-function))

(define (block-sprite block) (caddr block))
(define (block-walkable? block) (cadddr block))
(define (block-flame-blocking? block) (list-ref block 4))
(define (block-flame-function block) (list-ref block 5))
(define (block-trigger-function block) (list-ref block 6))

(define (turn-into block-name x y)
  (c-turn-into (block-index block-name) x y))

(define none (lambda (. args) #f))

(define (block-index name) (list-index blocks (assoc name blocks)))
(define (short-name->block-index short-name)
  (let ((sn-blocks (map cdr blocks)))
    (list-index sn-blocks (assoc short-name sn-blocks))))

(define (point x y) (cons x y))
(define (point-x point) (car point))
(define (point-y point) (cdr point))
