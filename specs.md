# Ion Kernel Specs

// sets of macros = "flavors"

## Core

### Data Types

* bool
* i8 -> i128
* u8 -> u128
* f8 -> f128
* char
* string
* expr: expression
* fn: function
* array: ordered product type
* record: unordered, key-value product type
* set: unordered product type
* type: sum type
* enum: similar to "enum classes" from cpp
* class
* obj
* interface
* generic
* unit

* nil
* undefined

### Symbols

* \+
* \-
* *
* /
* %
* =
* =>
* ?
* :
* ;
* ,
* ()
* {}
* []
* ""
* ''
* ``
* \#
* .
* <
* \>
* <=
* \>=
* !
* ==
* !=
* &&
* !|
* ||
* |
* |*
* ^
* $
* &
* @
* !&
* ~
* ^^
* ! ("Bang," different that Logical Not): Not nil-or-undefine-able(needs a value)

* \\* : special chars

* %* : macro

#### Bitshifts

* \>>
* <<
* !~
* !&
* !|
* !^

### Annotations

* makeNew
* operator

## Prelude

### Base Classes

* Identifier
* Literal(one for each type, plus a root "Literal," "OOP," "Character," and "Numeric")

## Ideology

## Structure

## Backends
