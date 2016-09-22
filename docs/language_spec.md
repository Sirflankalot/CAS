# cpp-cas language spec

The cpp-cas language is meant to be a very simple wrapper around standard mathematical notation. It is designed in order to provide people with a mathematical background an easy way to start programming in it, while still allowing programmers to get stuff done.

## Declarations

Variable declarations are all implicit, similar to python. 

```
a = 5 + 2
```

Declares the value of a, and then sets it to be 7. 

Functions are defined in the same way that mathamatical functions are.

```
f(x) = 5x + 2
```

f will be declared as a function object (functor) that takes a single value. This function object contains the expression 5x + 2.

## Evaluation

Function evaluation is done using the same syntax as defining it.

```
y = f(7)
```

The variable y is declared and is set to the value 37. 

Functions that take no arguments will be simplified via composition. Take the following

```
f(x) = x + 8
g(x) = x^2 + 3
```

If you then give 

```
f(g())
```

you will get 

```
x^2 + 11
```

Therefore, the following is valid:

```
h(x) = f(g(x))
```

## Type system

cpp-cas is a dynamic and strongly typed language. The following are the list of types in the language.

Name | Usage
--- | ---
test|test