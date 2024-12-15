(module
  (type (;0;) (func (param i32) (result i32)))
  (data $static "\aa\bb\cc\dd")
  (import "env" "memory" (memory $0 1))
  (func $main (type 0) (param i32) (result i32)
    (local $i i32)
    (memory.grow (local.get 0))
    (memory.fill (i32.const 0) (i32.const 0) (i32.const 65536))
    (memory.init $static (i32.const 0) (i32.const 0) (i32.const 4))
    (data.drop $static)
    (local.set $i (i32.const 1))
    (if (local.get 0)
      (then
        (loop $loop
          (memory.copy (i32.mul (local.get $i) (i32.const 0x10000)) (i32.const 0) (i32.const 0x10000))
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
    (return (memory.size))
  )
  (export "run" (func $main))
)