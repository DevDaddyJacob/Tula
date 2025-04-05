# Tula Coding Conventions (Language Development)

## Java
Java classes will have their declarations be sorted in a defined order based on a set of defined rules, starting with rule 1 and each group created by rule 1 will then be evaluated and sorted / grouped based on rule 2, and so on.

### Rule 1 - Declaration Order
1. Class (static) variables
2. Instace variables
3. Static initialization blocks
4. Constructors
5. Class (static) methods
6. Instance methods

### Rule 2 - Access Modifier Order
1. Public
2. Protected
3. Package (no access modifier)
4. Private

### Rule 4 - Final Members
Variables marked as `final` are grouped together within their access modifier sections and are placed at the top of the section.

### Rule 5 - Initialized Variables
Variables initialized at declaration are sorted above those which are not.