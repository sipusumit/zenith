; ModuleID = 'test'
source_filename = ".\\examples\\main.zn"

@0 = private unnamed_addr constant [12 x i8] c"Hello World\00", align 1

declare i32 @printf(ptr, ...)

define i32 @main() {
entry:
  %0 = alloca ptr, align 8
  store ptr @0, ptr %0, align 8
  %a = load ptr, ptr %0, align 8
  %1 = call i32 (ptr, ...) @printf(ptr %a)
  %2 = alloca i32, align 4
  store i32 0, ptr %2, align 4
  %b = load i32, ptr %2, align 4
  ret i32 %b
}
