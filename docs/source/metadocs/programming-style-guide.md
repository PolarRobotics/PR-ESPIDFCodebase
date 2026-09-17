# Programming Style Guide
## Overview
- This document contains rules that are to be followed when creating or updating the primary robot code repositories (e.g., [ESP32PRCodebase](https://github.com/PolarRobotics/ESP32PRCodebase)).
- There are three main areas: [programming principles](./programming-style-guide.md#programming-principles), [naming conventions](./programming-style-guide.md#naming-conventions), and [formatting](./programming-style-guide.md#formatting). Most formatting can be done automatically using the [astyle](./programming-style-guide.md#astyle) formatter.
- All code that is in the `production`, `dev/main`, or `dev/experimental` branches *must* follow this guide unless explicit approval is granted by the programming team lead.
	- You should strive to follow this guide as you are writing code. However, this is not always feasible, so you should make sure to clean up your code before submitting a pull request to one of the three aforementioned branches.
- This guide may seem strict, and it is, but for good reason:
	- It is much easier to understand the purpose of code that you did not write.
	- It helps reduce *maintenance load* (and other problems commonly referred to as "[technical debt](https://stackoverflow.blog/2023/12/27/stop-saying-technical-debt/)") – that is, the amount of effort required to keep the code working.
	- Certain parts of this guide are designed to entirely avoid difficult and obscure problems we have encountered and solved already, which would waste development time.
	- Many companies institute similar policies.
	- It will help you develop good coding style.

```{admonition} The biggest takeaway
:class: tip

If you don't remember anything else from this document, at least remember this: our code must be *readable* and *maintainable*. Someone that *isn't you* should be able to pick up where you left off.
```

## Programming Principles
### High-Level
- Document, document, document.
	- This is documentation. There must also be documentation in the code (i.e., comments). 
- Prioritize readability and maintainability over clever tricks.
	- If you do something that isn't easily understandable, you *must* document it well.
- Make it work, make it right, make it fast.
- Always test your code on live hardware (i.e., on a robot).
	- This is a required part of the process to merge code into `production`.
- Always start by testing the simplest case. Walk before you run.
- Follow principles of polymorphism and encapsulation where possible.
	- If you haven't taken ECCS 1621 Programming 2 yet, don't worry too much about these concepts. You'll learn about them eventually.

### Code-Level
- No `using namespace std`.
	- Sometimes this is taught as a way to ease learning C++. Here, we don't use it in order to enhance readability. 
	- See also: https://stackoverflow.com/questions/1452721/whats-the-problem-with-using-namespace-std
- Use `#include` guards, not `#pragma once`
	- Include guards should be named in the following format: `FILENAME_H`
		- i.e., `MACRO_CASE` with the `.` replaced with `_`.
	- See also: https://en.wikipedia.org/wiki/Include_guard
	- A file with include guards looks like this:
```cpp
// foo.h
#ifndef FOO_H
#define FOO_H

// your code here

#endif /* FOO_H */
```

- Do not define functions in header files unless they are incredibly simple.
	- If you do this, it can cause problems because functions defined inside classes are automatically `inline`d.
```cpp
// foo.h
class Foo {
	// acceptable
	Foo() {}

	// acceptable
	Foo(int bar) {
		this->bar = bar;
	}

	Bar getBar() {
		return this->bar;
	}

	// not acceptable
	Foo(int bar, int car, Bar other, ...) {
		...
		...
		...
	}
}
```

## Naming Conventions
- Do not obfuscate variable names. 
	- Make your names clear and logical.
	- Avoid abbreviating except where the meaning is blatantly obvious (ex. Quarterback → QB in `Quarterback.cpp`).
	- When in doubt, error on the side of a more descriptive name.
	- It is also strongly recommended to clarify what the variable does using a comment at its declaration
- Do not prefix variables with letters indicating type (ex. `bool bFoo`).
	- In some cases it may be acceptable to do this for pointers (ex. `int* pBar`), but usually if you feel like you are forced to do this, you should rethink your naming conventions on a larger level.
- Do not prefix private members with `m_` (ex. `m_foo`).
- Avoid prefixing private members with `_` (ex. `_bar`).
	- This may sometimes be acceptable (with permission) if the variable would logically start with a number or other invalid starting character.
- Variables and functions should use `camelCase` (or rarely, `snake_case`)
	- If the name includes an abbreviation, acronym, etc. of some kind (ex. ESP) then you should use capitals appropriately (ex. `ESP` instead of `Esp`).
		- In such cases, it may be more readable to separate consecutive acronyms with `_`.
- Classes and Enums should use `UpperCamelCase`.
	- Enum members should use `MACRO_CASE` or (rarely `snake_case` with approval from a senior dev)
		- Note: right now we only use lowercase enum with robot config related stuff. And debouncer.
- Constants should use `MACRO_CASE`
- Include guards must use `MACRO_CASE` with the `.` (separating the filename from the extension) replaced with `_`.
	- For example, a file named `foo.h` would use `FOO_H` as the include guard `#define`.
- Filenames should generally be `UpperCamelCase` if they primarily house a class, or normal `camelCase` if they do not.

## Formatting
- Whatever you do, be consistent.
- Don't violate one of these rules unless you have a good reason.
	- You should also ask the programming team lead first.
	- Most formatting can be done automatically by [astyle](./programming-style-guide.md#astyle) (see the section below).

### Things astyle *cannot* do automatically
- Functions with a long list of parameters shall be formatted with the parameters on line(s) after the opening parenthesis `(` with a single tab indent, with the closing parenthesis `)` and opening brace `{` on the next line. For example:
```cpp
void foo(
	int bar,
	int car,
	int dar
) {
...
}
```
- note: astyle will not do this automatically, but it will not mess with it given my config

- Classes and constructors with a long list of base classes or member initializers shall be formatted the same way as functions with many parameters. For example:
```cpp
class Foo : Bar, Car {
	Foo(
		int a, 
		int b, 
		int c,
		int d
	) : Bar(a, b, c, d), 
			Car(a, b, c, d) {
		this->a = a;
	}
}
```
- Constructor parameters that directly initialize a class member shall be named the same as the class member, and the `this` keyword shall be used to reference the class member. See the codeblock above for an example.
- As of 2024-08-25, `astyle` may move the `*` or `&` of pointer dereferences or address references. [This is a known issue and is being worked on.](https://sourceforge.net/p/astyle/bugs/578/) Fix appropriately or surround with `astyle` guards (see [here](#disabling-formatting-sectionally)).

### Things astyle can do automatically
- `public` and `private` keywords inside `class` headers shall be indented one tab. Class members under them shall be indented two tabs.
- Spaces shall be placed between:
	- any non-function keyword and open parenthesis
		- ex. `if (` instead of `if(`
	- comparison or assignment operators and anything else (except increment operators, i.e., `++`/`--`)
		- ex. `x == 0` instead of `x==0`, `y = 0` instead of `y=0`
	- closing parentheses and open braces
		- ex. `) {` instead of `){`
	- a comma and the next token
		- ex. `(foo, bar)` instead of `(foo,bar)`
- Pointers and references shall be formatted with the `*` or `&` at the end of the type with no space, and one space between the `*` or `&` and the variable name. For example:
```cpp
// correct
char* str = "Hello World!";

// incorrect
char *str2 = "Foo Bar";
```

- Pointer dereferences and address references shall be formatted with the `*` or `&` directly attached to the identifier name.
	- Be careful to distinguish between these and pointer/reference declarations.
	- As of 2024-08-25, `astyle` may move the `*` or `&` of pointer dereferences or address references. [This is a known issue and is being worked on.](https://sourceforge.net/p/astyle/bugs/578/) Fix appropriately or surround with `astyle` guards (see [here](#disabling-formatting-sectionally)).
```cpp
// correct
void foo(int bar, int* ptr);

foo(*bar, &var);

// incorrect
foo(* bar, & var);
```

- Open braces (`{`) should be placed on the same line as the end parenthesis of a function or conditional. For example:
```cpp
void foo() {
...
}
// instead of
void foo()
{
...
}
```

- `else` or `else if` should be placed on the line after the closing brace of the preceding `if` block. 
```cpp
if (foo == bar) {
	...
}
else if (foo == car) {
	...
} 
else {
	...
}
```

- Leave at least one blank line between function definitions or conditionals. For example:
```cpp
void foo() {
...
}

// comment
void bar() {
...
}
```

- Avoid `if` or `else` statements without braces unless they are separated from other code by at least one line above and below.
	- Do not place the action of an `if` statement on the same line as the conditional. Instead, use a ternary conditional operator. For example:
```cpp
// acceptable
if (foo)
	bar();
else
	car();

// not acceptable
if (foo) bar();
else car();

// acceptable
foo ? bar() : car();
```

### astyle
- `astyle`, or [Artistic Style](https://astyle.sourceforge.net/), is an automatic C++ formatter

#### Installation
##### Windows
- https://astyle.sourceforge.net/install.html#_Windows_Version
- Distributes `astyle.exe` with download (ex. `astyle-3.5.2-x64.zip`)
- Can be ran as-is, but would be best to add to `PATH`

##### Linux
- https://astyle.sourceforge.net/install.html#_Linux_Version
- Requires `make`, `cmake`, and `gcc`.
```sh
cd ~/src
cp /mnt/hgfs/share/astyle-3.5.2.tar.bz2 .
tar -xjvf astyle-3.5.2.tar.bz2
cd astyle-3.5.2
```

- Assumed to be run from root of `astyle` repo:
```sh
mkdir as-gcc-exe
cd as-gcc-exe
cmake  ../
make
sudo make install
```

#### Usage
- https://astyle.sourceforge.net/astyle.html#_Usage
- Generally the easiest way to use a customized version is to add `.astylerc` to the project root directory (it must be specifically at the project root).
	- Then, running `astyle --project` will consider the `.astylerc` option file.

##### Disabling formatting sectionally
- You can disable parsing of sections of code with a syntax similar to `#defines`
	- https://astyle.sourceforge.net/astyle.html#_Disable_Formatting
	- Basically, put line comments with `*INDENT-OFF*` then `*INDENT-ON*`
	- Also: "A line-end comment tag `*NOPAD*` will disable the `pad-oper`, `align-pointer`, and `align-reference` options."

##### Running `astyle`
- Run from project root, for use on files inside `./src`
	- Make sure to escape wildcards – they must not be processed before astyle receives them
	- If `--recursive` is enabled inside the project file, it does not need to be included in the command call.
- Do not run on all files in the repository otherwise it can cause issues.

###### Linux (or Windows when `astyle` is in your `PATH`)
```sh
astyle --project src/\*
```

###### Windows (embedded in repository)
```sh
./astyle/astyle.exe --project src/\*
```

##### Current `.astylerc`
```sh
## General
# !!! If enabled, astyle will NOT retain a backup of the original, unformatted file.
# "Do not retain a backup of the original file. The original file is purged after it is formatted."
# ONLY enable this option when you are CERTAIN that the formatting is working properly and to your liking (or you are using version control CAREFULLY).
--suffix=none

# Recursively process a folder.
--recursive

# ! Will remove excess whitespace. Probably don't want to use this often if at all.
#--squeeze-ws

# Maximum length of a single line of code (in characters)
# Lines longer than this will be broken after a logical operator
--max-code-length=125
--break-after-logical

## Braces
# Open braces are placed on the same line as the closing paren of the header/function/conditional.
--style=attach

# Breaks closing headers (e.g. 'else', 'catch', ...) from their immediately preceding closing braces.
--break-closing-braces

# Attach open braces to namespaces, classes, etc.
--attach-namespaces
--attach-classes
--attach-extern-c

# Attach the closing while to the same line of the closing brace in a do-while statement.
--attach-closing-while

# Attach the return type to the function name.
--attach-return-type
--attach-return-type-decl

## Indents
# Use tab indents with 2 spaces
--indent=tab=2 

# Converts tabs into spaces in the *non-indentation* part of the line.
--convert-tabs

# Indent the contents of classes and structs
--indent-classes
--indent-namespaces

# Indent, instead of align, continuation lines following lines that contain an opening paren `(` or an assignment `=`. This includes function definitions and declarations and return statements.
--indent-after-parens

# Indent 'case' one indent from 'switch' 
--indent-switches

# Indent the contents of 'case' blocks
--indent-cases

# Indent preprocessor blocks
--indent-preproc-block 
#--indent-preproc-cond

## Other Whitespace
# Pad empty lines around header blocks (e.g. 'if', 'for', 'while'...).
--break-blocks

# Format pointers and references with the `*` or `&` appended to the type with no space, 
# and one space between the `*` or `&` and the variable name.
--align-pointer=type
# not needed if --align-pointer-type is used
#--align-reference=type 

# Insert space padding around operators. This will also pad commas. 
# Any end of line comments will remain in the original column, if possible. 
# Note that there is no option to unpad. Once padded, they stay padded.
--pad-oper

# Insert space padding between a header (e.g. 'if', 'for', 'while'...) and the following paren.
# This can be used with unpad-paren to remove unwanted spaces.
--pad-header

# Remove extra space padding around parens on the inside and outside.
# Only padding that has not been requested by other options will be removed.
--unpad-paren
#--unpad-brackets

# Break one line headers (e.g. 'if', 'while', 'else', ...) from a statement residing on the same line.
--break-one-line-headers
```

