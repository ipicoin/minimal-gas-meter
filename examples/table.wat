(module
  (type (;0;) (func (param i32) (result i32)))
  (import "env" "memory" (memory $0 1))
  (table $t 1 funcref)
  (elem $e (i32.const 0) func $f)
  (func $f)
  (func $main (type 0) (param i32) (result i32)
    (local $i i32)
    (table.grow $t (ref.null func) (i32.const 1))
    (table.fill $t (i32.const 0) (ref.null func) (i32.const 1))
    (table.init $t $e (i32.const 0) (i32.const 0) (i32.const 1))
    (elem.drop $e)
    (local.set $i (i32.const 1))
    (if (local.get 0)
      (then
        (loop $loop
          (table.copy $t $t (local.get $i) (i32.const 0) (i32.const 1))
          (br_if $loop
            (i32.lt_s
              (local.tee $i (i32.add (local.get $i) (i32.const 1)))
              (local.get 0)
            )
          )
        )
      )
      (else )
    )
    (return (table.size $t))
  )
  (export "run" (func $main))
)