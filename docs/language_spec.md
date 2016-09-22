# cpp-cas language spec

The cpp-cas language is meant to be a very simple wrapper around standard mathematical notation. It is designed in order to provide people with a mathematical background an easy way to start programming in it, while still allowing programmers to get stuff done.

## Declarations

Variable declarations are all implicit, similar to python. 

```
a = 5 + 2
```

Declares the value of a, and then sets it to be 7. 

Functions are defined in the same way that mathematical functions are.

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

Functions that take no arguments will be composed. Take the following

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
(x^2 + 3) + 8
```

Therefore, the following is valid:

```
h(x) = f(g(x))
```

## Commands

cpp-cas has a set of commands that can be used to carry out more advanced operations. Arguments are optional The command format is 

```
command [arguments]: input
```

Command      | Arguments       | Use
------------ | --------------- | -----------
`simplify`   | none            | Simplifies equation without moving variables from their side
`solve`      | `for x [and y]` | Tries to isolate the variables provided. Returns a functor
`substitute` | none            | Allows functions to remain unevaluated. ex. `f(7)` will be `(7) + 2` instead of `9`.
`interpolate`| <code>constant &#124; linear &#124; cosine &#124; spline</code> | Allows tables to be evaluated with interpolation.
`root`       | none            | Takes the root of a quadratic or a cubic functor
`limit`      | none            | Finds the domain limits of a single variable functor
`differentiate`     | `[partial] 1st | 2nd...`  | Calculates the n-th [partial] derivative of the provided expression
`integrate`    | <code>definite &#124; indefinite</code>   | Calculates the indefinite integral
`integrate_definite`| <code>x,{a,b}</code> | Returns the definite integral of the given function for x in the range a to b

The command is optional and assignments are allowed as part of the input.

## Type system

cpp-cas is a statically typed language with inferred type. The following are the list of types in the language.

Name         | Use
------------ | ------
`num`        | Basic number type that supports integers and fractional numbers.
`imag`       | Imaginary number type
`constant`   | A mathematical type with special meaning (such as pi or e)
`array`      | Array of a single non-array type
`matrix`     | Multidimensional array of non-array type.
`table`      | Multidimensional array of a numbered type that can have labels, and describes a function relation.
`functor`    | Callable function object. Can be called with `var()`
`expression` | An expression that may be assigned or inputed into a function

### Implicit Conversions

These conversions will happen even if not explicitly casted (or casted through assignment)

From      | To        | Validity
--------- | --------- | --------
`num`     | `imag`    | Always
`imag`    | `num`     | Runtime error if imaginary component is not 0
`constant`| `num`     | Compile time error if constant is imaginary
`constant`| `imag`    | Always
`table`   | `functor` | Always
