# AVL Dictionary

A simple C program that loads words from `dictionary.txt` into an AVL tree and checks sentence spelling efficiently.

## Features
- Builds a balanced AVL tree from the dictionary file
- Performs case-insensitive word lookup
- Marks words as correct or incorrect
- Suggests nearest matching words using tree traversal

## Usage
1. Place `dictionary.txt` in the same folder as `main.c`
2. Compile `main.c` with a C compiler
3. Run the executable and enter a sentence to check

## Test cases
```
AVL tree height: 10
AVL tree size: 242

Enter a sentence:
I studdy mathmatcs

i is CORRECT

studdy is INCORRECT
Suggestions:
1. student
2. structure
3. study

mathmatcs is INCORRECT
Suggestions:
1. mathematics
2. market
3. melon
```