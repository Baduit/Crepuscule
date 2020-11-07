# Crepuscule
An easy to use tokenizer without the regexp-like syntax or grammar which look like someone slept on the keyboard.

# API
## The config
TODO
## The Tokenizer
TODO

# Example
TODO

# How to use it in your project
First solution: use cmake

Second solution: build it, include the file \<crepuscule/crepuscule.hpp> and link your project with the built library

# Why create a new library for that ?
Every time I want to tokenize or parse something either I end up writing some ugly code that work for my very specific case and I'm not happy because it is ugly and not evolutive.
Sometimes I read the documentation about some fancy tokenizer/lexer/parser and it always use regexp or an embedded language which is similar (sometimes worse) and I'm like "f*ck it, I'm outta here". I personally don't like regexp at all, yes there are powerful, yes you can find a lot of already written regexp or help on it on the internet and yes it can be used with almost all programming languages, BUT when I wrote code the goal is to have explicit code, I try to have a comprehensive and clean code (it is not always a success and I must admit especially on pet project I can be too lazy to refactor), sometimes I write a comment because I admit explicitly that I have failed to be expressive and then there are the regexp, an ugly language full of special characters and really really compact and that make it hard and slow to read. Moreover, when you write your regexp you need often to escape characters, so it adds some \ and it is worse or use raw strings.

So, I have written this, it only tokenizes (no parsing); the configuration use an actual C++ data structure and you create it with actual C++ code.


# Goals of this library
- Ability to tokenize programming (c or python for example) or descriptive langages (html, json, xml for example)
- Easy to use
- Hard to misuse
- No need to learn a new langage or a DSL
- No dependency

# Non goals
- Be the most complete tokenizing library
- Be the fastest

# Build
You need a recent cmake and a recent compiler supporting c++20
On my Ubuntu wsl I use gcc 10.1.0 or clang 10.0.0 and cmake 3.16.1.
On my Windows 10 I use the compiler shipped with Visual Studio 16.7.3 and cmake 3.18.1

## TODO:
### Things I will do
- Tutorial
### Things I should do but I probably won't because of my lazyness
- Support any std::string/std::string_view like type and make the used type configurable
- A header only version in 1 file only
- Complete the tests
- Optimize it

