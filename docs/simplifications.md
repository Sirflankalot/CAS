# Simplifications

This document is a list of mathematical features that will eventually be integrated into the C++ CAS project.

1. Solving basic arithmetic while following proper order of operations.
2. Provide highly efficient algorithms for performing arithmetic on arbitrarily large numbers. (GMP, the GNU Multiple Precision
library may prove useful for this. Though primitive types can be used if and when they provide a performance benefit.)
2. Simplifying expressions that contain one or more variable(s). (The use of a symbolic C++ algebra library like SymbolicC++ may prove
useful to this end. Alternatively this functionality may be implemented from scratch using pure C++.)
3. Solving equations of one variable. (Need to interpret the equation in terms of the variable being solved for)
4. Finding the roots of quadratic equations using the quadratic formula.
5. Finding the limit of a given function at a given point using the direct substitution method if the function is continuous at the
 given domain value or other methods if not continuous at the given point.
6. Finding the n-th derivative of a given function.
7. Finding the n-th partial derivative of a given function with respect to a given input variable.
7. Finding the indefinite integral of a given function.
8. Finding the definite integral of a given function for a range from a given x value 'a' to another such value 'b' assuming the
function is continuous from x = a to x = b.
