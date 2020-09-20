# Crepuscule
An easy to use tokenizer without the regexp-like syntaxe or grammar wich look like someone slept on the keyboard.

# Why create a new library for parsing ?

# Goals of this library
- Ability to tokenize programming (c or python for example) or descriptive langages (html, json, xml for example)
- Easy to use
- Hard to misuse
- No need to learn a new langage or a DSL
- No dependy

# Non goals
- Be the most complete tokenizing library
- Be the fastest

# Build
You need a recent cmake and a recent compiler supporting c++20
I use gcc 10.1.0 and cmake 3.16.1

## TODO:
### Things I will do
- Handle the sorting of the elements in the config
- Refactor the main loop in the tokenisation
- Support msvc
- Tutorial
### Things I should do but I probably won't because of my lazyness
- Complete the tests
- Optimize it

