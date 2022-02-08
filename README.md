# ilu!
A stack-based programming language which compiles to assembly.

### quick start
Clone this repo and run:
```
./build.sh
```
There should be an executeable called `ilu` which you can run.

### examples
Hello World!
```python
@include "std/core.ilu"

def main in
    "Hello World!\n" puts
end
```

Print numbers from 0-100 and print them:
```python
@include "std/core.ilu"

def main in
	0 while dup 100 <= do
		dup putu
		"\n" puts
		1 +
	end pop
end
```

### inspiration and learning resources and stuff
Based off [https://gitlab.com/tsoding/porth/-/tree/master](Porth).
