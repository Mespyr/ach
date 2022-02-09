# ilu!
A stack-based programming language which compiles to assembly.

## examples
All examples are located in [examples/](examples/)

## quick start
Clone this repo and run:
```
./build.sh
```
There should be an executeable called `ilu` which you can run.

## usage
```
USAGE: ./ilu <FILENAME> <OPTIONS>
OPTIONS:
	-h, --help	show help
	-f, --fasm	use fasm to compile program
	-n, --nasm	use nasm to compile program
	-u, --unsafe	compile without type checking program
```

### syntax
Each program has an entry point through the `main` function
```ruby
def main in
  # code here
end
```
Comments are like in python and ruby
```ruby
# this is a comment
def main in
  # another comment
end
```

#### stack manipulation
Declaring numbers pushes one value on the stack, an integer.
```ruby
# stack [34, 35]
def main in
  34 35
end
```
Pushing a string onto the stack pushes 2 values:
- the length of the string (integer)
- a pointer to the string
```ruby
# stack [5, "Hello"]
def main in
  "Hello"
end
```
To push a null-terminated string, or a C-style string:
```ruby
# stack ["Hello\0"]
def main in
  "Hello"c
end
```
#### intrinsics
| name | function                                | stack                  |
|------|-----------------------------------------|------------------------|
| dup  | duplicate top value on the stack        | [a] -> [a, a]          |
| swp  | swap top two values on the stack        | [a, b] -> [b, a]       |
| rot  | rotate the top three to the left        | [a, b, c] -> [b, c, a] |
| over | copy second value on stack over         | [a, b] -> [a, b a]     |
| pop  | pop top value from the stack            | [a, b] -> [a]          |
| dump | dump top value on the stack into stdout | [a, b] -> [a]          |
##### intrinsic examples
```ruby
def main in
  # dup
  12  # stack [12]
  dup # stack [12, 12]
  pop pop

  # swp
  12 13 # stack [12, 13]
  swp   # stack [13, 12]
  pop pop

  # rot
  12 13 14 # stack [12, 13, 14]
  rot      # stack [13, 14, 12]
  pop pop pop

  # over
  12 13 # stack [12, 13]
  over  # stack [12, 13, 12]
  pop pop pop

  # pop
  12  # stack [12]
  pop # stack []

  # print 13 for debugging
  13 dump
end
```

#### expressions
ilu uses [reverse polish notation](https://en.wikipedia.org/wiki/Reverse_Polish_notation) for all expressions and function calls.
the following operations are supported:

If the stack is currently [a, b]
- `+` - add a and b (a + b)
- `-` - subtract b from a (a - b)
- `*` - multiply a and b (a \* b)
- `/` - divide a and b (a / b). pushes the quotient and remainder on the stack
- `=` - compare a and b. pushes 1 if equal, 0 if not
- `>` - compare a and b. pushes 1 if a is greater, 0 if not
- `<` - compare a and b. pushes 1 if a is less, 0 if not
- `>=` - compare a and b. pushes 1 if a is greater or equal, 0 if not
- `<=` - compare a and b. pushes 1 if a is less or equal, 0 if not
- `!=` - compare a and b. pushes 1 if not equal, 0 if not
- `or` - compare a and b. pushes 1 if either a or b is not equal to 0, 0 if not
- `and` - compare a and b. pushes 1 if both a or b is not equal to 0, 0 if not
- `not` - if b is equal to 0, push 1, else push 0
- `>>` - shift a by b bytes to the right
- `<<` - shift a by b bytes to the left
- `orb` - bitwise or
- `andb` - bitwise and

See [bitwise calculator](https://miniwebtool.com/bitwise-calculator/)

##### expression examples
```ruby
def main in
  # 12 + 13 -> 25
  12 13 + dump

  # 14 - 12 -> 2
  14 12 - dump

  # 7 * 3 -> 21
  7 3 * dump

  # 9 / 2 -> 4, 1
  9 2 / dump dump

  # 1 == 2 -> 0
  1 2 = dump
  # 2 == 2 -> 1
  2 2 = dump

  # 3 > 2 -> 1
  3 2 > dump
  # 3 > 4 -> 0
  3 4 > dump

  # 2 < 3 -> 1
  2 3 < dump
  # 3 < 2 -> 0
  3 2 < dump

  # 12 >= 12 -> 1
  12 12 >= dump
  # 11 >= 12 -> 0
  11 12 >= dump
  # 13 >= 12 -> 1
  13 12 >= dump

  # 12 <= 12 -> 1
  12 12 <= dump
  # 11 <= 12 -> 1
  11 12 <= dump
  # 13 <= 12 -> 0
  13 12 <= dump

  # 1 != 0 -> 1
  1 0 != dump
  # 1 != 1 -> 0
  1 1 != dump

  # 4 or 0 -> 1
  4 0 or dump
  # 3 or 4 -> 1
  3 4 or dump
  # 0 or 0 -> 0
  0 0 or dump

  # 4 and 0 -> 0
  4 0 and dump
  # 3 and 4 -> 1
  3 4 and dump
  # 0 and 0 -> 0
  0 0 and dump

  # not 1 -> 0
  1 not dump
  # not 0 -> 1
  0 not dump

  # 12 >> 1 -> 6
  12 1 >> dump
  # 12 << 1 -> 24
  12 1 << dump

  # 12 orb 1 -> 13
  12 1 orb dump

  # 2 andb 2 -> 2
  2 2 andb dump
  # 2 andb 1 -> 0
  2 1 andb dump
end
```

## inspiration and learning resources and stuff
Based off [https://gitlab.com/tsoding/porth/-/tree/master](Porth).

## what i want the language to look like in the future (sort of)
```ruby
include "std/core.ilu"

struct Struct
  Num int
  String str
end

def main in
  Struct let my_struct in

  my_struct "Hello"c !Struct.String
  my_struct 69       !Struct.Num

  my_struct @Struct.String cstr-to-str puts
  my_struct @Struct.Num putu "\n" puts
end
```
