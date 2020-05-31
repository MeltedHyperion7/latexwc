# latexwc
Program that calculates the word count of a latex file.
I am new to latex, so this is definitely not fleshed out and probably won't work well in every case.

## Installation Instructions
- Run `sudo make install` in the latexwc directory.

## Usage
### Get Simple Word Count
`latexwc <filename>`
### Ignore Heading and Titles
Use the -h flag.
`latexwc -h <filename>`
### Ignore Stopwords
Remove stopwords (like 'a', 'an', 'where') from the count. Use the -s flag.
`latexwc -s <filename>`