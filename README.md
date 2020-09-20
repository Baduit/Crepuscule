# Crepuscule
An easy to use tokenizer without the regexp-like syntaxe or grammar wich look like someone slept on the keyboard.

# API
## The config
TODO
## The Tokenizer
TODO

# Example
TODO

# How to use it in your project
First solution: use cmake

Second solution: build it, include the file \<crepuscule/crepuscule.hpp> and link your project with the builded library

# Why create a new library for that ?
Everytime I want to tokenize or parse something either I end up writing some ugly code that work for my very specific case and I'm not happy because it is ugly and not evolutive.
Sometimes I read the documentation about some fancy tokenizer/lexer/parser and it always use regexp or an embeded langage which is similar (sometimes worse) and I'm like "f*ck it, I'm outta here". I personnaly don't like regexp at all, yes there are powerfull, yes you can find a lot of already written regexp or help on it on the internet and yes it can be used with almost all programming langages, BUT when I wrote code the goal is to have explicit code, I try to have a comprehensive and clean code (it is not always a success and I must admit especially on pet project I can be too lazy to refactor), sometimes I write a comment because I admit explicitly that I have failed to be expressive and then there are the regexp, an ugly langage full of special characters and really really compact and that make it hard and slow to read. Moreover, when you write your regexp you need often to escape characters so it add some \ and it is worse or use raw strings.

So I have written this, it only tokenizes (no parsing); the configuration use an actual C++ data stucture and you create it with actual c++ code.

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
- A header only version in 1 file only
- Complete the tests
- Optimize it

