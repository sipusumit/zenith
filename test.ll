; ModuleID = 'test'
source_filename = ".\\examples\\test.zn"

@0 = private unnamed_addr constant [12 x i8] c"Hello Ayush\00", align 1

declare i32 @printf(ptr, ...)

define i32 @main() {
entry:
  %0 = call i32 (ptr, ...) @printf(ptr @0)
  ret i32 0
}
