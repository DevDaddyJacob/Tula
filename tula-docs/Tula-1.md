# Tula 1 - Overview

Essentially aiming to be Lua with the some basic typing from TypeScript, however to start, we will drop the typing and pick it back up later.

We also will implement **automatic memory management**.


## Data Types
The following data types are going to be supported:
  - Booleans (bool)
  - Integers (int)
  - Decimals (dec)
  - String (str)
  - Nil (nil)


## Operators
The following operators are going to be used:
  - Arithmetic
    - __add__ [`+` infix operator] (x + y)
    - __subtract__ [`-` infix operator] (x - y)
    - __multiply__ [`*` infix operator] (x * y)
    - __divide__ [`/` infix operator] (x / y)
    - __negate__ [`-` prefix operator] (-x)
  - Comparison / Equality
    - __less than__ [`<` infix operator] (x < y)
    - __greater than__ [`>` infix operator] (x > y)
    - __less than or equal__ [`<=` infix operator] (x <= y)
    - __greater than or equal__ [`>=` infix operator] (x >= y)
    - __value equality__ [`==` infix operator] (x == y)
    - __value inequality__ [`!=` infix operator] (x != y)
    - __type equality__ [`===` infix operator] (x === y)
    - __type inequality__ [`!==` infix operator] (x !== y)
  - Logical
    - __not__ [`!` prefix operator] (!x)
    - __and__ [`and` infix operator] (x and y)
    - __or__ [`or` infix operator] (x or y)
  - Other
    - __string concatenation__ [`+` infix operator] ("hello " + "world")


## Statements
Expressions are followed by a semicolon (`;`), statements can be packed into a series of statements in place of a single one by wrapping them in a block (`{}`)

We probably want to also bake in a `print` statement or something.


## Variables
Variables are defined using a `var` statement, and can optionally be initilized at declaration, but when not initialized at declaration, the default will be `nil`


## Control Flow

### If Statements
```
if (condition) {
  print("yes");
} else {
  print("no");
}
```

### While Loops
```
var a = 1;
while (a < 10) {
  print(a);
  a = a + 1;
}
```

### For Loops
```
for (var a = 1; a < 10; a = a + 1) {
  print(a);
}
```


## Functions
Function calls look the same as in C
```c
sumNums(100, 230, 50);
```

Parentheses are mandatory.

Function declaration will use the `function` or `func` key words

Function body always has to be inside of a block, and you can return a value using a return statement.
```
func sumNums(a, b) {
  return a + b;
}
```

All functions must have a return statement, a return statement doesn't require a value and in instaces where a value is ommited, `nil` is returned.

### Argument vs Parameter
An argument is an actual value you pass to a function when you call it. So a function call has an argument list. Sometimes you hear actual parameter used for these.

A parameter is a variable that holds the value of the argument inside the body of the function. Thus, a function declaration has a parameter list. Others call these formal parameters or simply formals.


## Closures
Functions are first class, meaning they are real values you can get a reference to, store in variables, pass around etc.

Meaning this is valid:
```
func addPair(a, b) {
  return a + b;
}

func identity(a) {
  return a;
}

print(identity(addPair)(1, 2)); // Prints "3".
```

Since function declarations are statements, you can declare local functions inside another function.
```
func outerFunction() {
  func localFunction() {
    print("I'm local!");
  }

  localFunction();
}
```

And allows the following to work, giving `inner()` access to a local variable `outside`:
```
func returnFunction() {
  var outside = "outside";

  func inner() {
    print(outside);
  }

  return inner;
}

var fn = returnFunction();
fn();
```


## Classes
Classes are defined with the `class` keyword. Class bodies contain the functions which are declared like functions just without the `functio` or `func` keyword.

When the class declaration is executed, a class object is created and stored in variable named after the class, and similar to functions, classes are first class.
```
class Dog {
  sit() {
    print("sitting");
  }

  barkAt(what) {
    print("woof at " + what);
  }
}
```

This also means that the following is valid:
```
// Store it in variables.
var someVariable = Dog;

// Pass it to functions.
someFunction(Dog);
```

Creating instances is done by calling the class name like a function:
```
var dog = Dog();
print(dog); // "Dog instance".
```

### Instantiation & Initialization
Properties can be freely added onto objects:
```
dog.breed = "pug";
dog.size = "small";
```

Accessing a field or method on the current object from within a method you can use either `this` or `instance` keywords.

Instantiation can be customized by defining a method in the class with the name being identical to the class name.


### Inheritance
Classes can be inherited using the less-than operator (`<`).
```
class Dog < Animal {
  drink() {
    print("slurp");
  }
}
```

Accessing methods is similar to Java where you can use the `super` keyword