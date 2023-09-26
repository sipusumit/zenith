; ModuleID = 'test'
source_filename = "test"

@0 = private unnamed_addr constant [16 x i8] c"Hello EveryOnen\00", align 1
@1 = private unnamed_addr constant [66 x i8] c"I know i am the greatest programmer that has ever lived !!!!!!!!n\00", align 1

declare i32 @printf(ptr, ...)

define i32 @main() {
entry:
  %0 = call i32 (ptr, ...) @printf(ptr @0)
  %1 = call i32 (ptr, ...) @printf(ptr @1)
  ret i32 69
}
