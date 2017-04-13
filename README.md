# ToyLisp
## Features
ToyLisp is a lisp interpreter(written in C),which include:

- Evaluations
- S-expressions
- Q-expressions
- Variables
- Functions
- Conditionals
- Strings.

## Compile code
For Linux:

```
gcc -std=c99 -Wall -o ToyLisp.out src/*.c -lm -ledit
```
For Windows:
```
gcc -std=c99 -Wall -o ToyLisp src/*.c -lm
```

Read [Manual](./Manual.md) for more information.

# Basic
## Evaluations
```
ToyLisp> + (/ 10 2) (- 5 1) (* 3 4)
21
ToyLisp> (/ 10 2)
5
ToyLisp> / 4 0
Error: Division By Zero.
```

## Q-Expressions
```
ToyLisp> {1 2 4}
{1 2 4}
ToyLisp> {+ 2 4 5}
{+ 2 4 5}
```
ToyLisp offer some basic built-in functions(`list`,`head`,`tail`,`eval`,`join`):

```
ToyLisp> list 1 2 3 5
{1 2 3 5}
ToyLisp> (head (list 1 2 3 5))
{1}
ToyLisp> (tail {1 2 3 5})
{2 3 5}
ToyLisp> (eval (head {(+ 1 2) (* 1 2)}))
3
ToyLisp> head {+ 1 2}
{+}
ToyLisp> ((eval (head {+ 1 2 3})) 1 2 3)
6
ToyLisp> join {1 2} {3 4}
{1 2 3 4}
```

## Variables
```
ToyLisp> def {a b} 10 20
()
ToyLisp> (* a b)
200
ToyLisp> * x y
Error: Unbound Symbol 'x'
ToyLisp> def {x y} a b
()
ToyLisp> (+ x y)
30
```

## Functions
Lambda expression:
```
ToyLisp> (\ {x y} {+ x y}) 10 20
30
```

`def` functions and variable arguments:
```
ToyLisp> def {add} (\ {x y} {+ x y})
()
ToyLisp> add 1 2
3
ToyLisp> add 1 2 3
Error: Function passed too many arguments. Got 3, Expected 2.
ToyLisp> def {add} (\ {x & y} {eval (join {+} (list x) y)})
()
ToyLisp> add 1 2 3
6
ToyLisp> add 1 2 3 4 5
15
```

## Conditionals
```
ToyLisp> def {x y} 1 2
()
ToyLisp> == x y
0
ToyLisp> <= x y
1
ToyLisp> >= 3 x
1
ToyLisp> != {} 1
1
ToyLisp> if (!= x y) {+ x y} {* x y}
3
```

## Strings
```
ToyLisp> "hello world!"
"hello world!"
ToyLisp> print "hello world!"
"hello world!"
()
ToyLisp> load "src/example.tl" ;comments:load file
"Hello World!"
()
```

## Standard Library
The Standard Library offer more powerful built-in functions based on the core language features list above.For example:
```
ToyLisp> map (\ {x} {* x 2}) {1 2 3}
{2 4 6}
ToyLisp> filter (\ {x} {> x 2}) {0 4 2 3 1}
{4 3}
ToyLisp> sum {1 2 3 4 5}
15
ToyLisp> product {1 2 3 4 5}
120
ToyLisp> fib 5;Fibonacci
5
```

Read the [source code](lib/standard_lib.tl) to get more information.

# Future
There still a lot of thing to do:
- [ ] Native Types(`double` especially!).
- [ ] User Defined Types.
- [ ] List Literal.
- [ ] Operating System Interaction.
- [ ] Macros.
- [ ] Variable Hashtable.
- [ ] Pool Allocation.
- [ ] Garbage Collection.
- [ ] Tail Call Optimisation.
- [ ] Lexical Scoping.
- [ ] Static Typing.

# Acknowledge
ToyLisp comes from [Build Your Own Lisp](http://www.buildyourownlisp.com/contents),a remarkable book written by [Daniel Holden](https://github.com/orangeduck).

