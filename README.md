# latexwc
Program that calculates the word count of a latex file.
I am new to latex, so this is definitely not fleshed out and probably won't work well in every case.

## Installation Instructions
- Run `sudo make install` in the latexwc directory.

## Usage
Run with the --help flag to get a full list of available options.  
`latexwc --help`
### Get Simple Word Count
`latexwc <filename>`
### Ignore Heading and Titles
Use the -H flag.  
`latexwc -H <filename>`
### Ignore Stopwords
Remove stopwords (like 'a', 'an', 'where') from the count. Use the -s flag.  
`latexwc -s <filename>`