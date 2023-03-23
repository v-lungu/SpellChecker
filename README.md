# SpellChecker

A spell checker that does basic spelling corrections of 1 edit distance currently. I will update this project in the future to include 2 edit distance as well as some nifty tricks to find more likely words with certain kinds of spelling errors. 

To run: build the project and run ./SpellChecker with 1 argument being the word you'd like to have spelled correctly.

The way this project works is by taking a large dataset of words and parsing it for frequency of use. This process takes a long time so if it has already been done and the _map.txt objectfor that dataset already exists, it can just create the map from the pre-existing file. 

The spell checker will then create all possible candidate words for the input by completing an edit of 1 letter with all possible permuations of deletion, insertion, transposition, and replacements. If out of those edits there are multiple valid words found in the dataset, it will choose the likeliest one by calculating the probability of all candidates being the correct spelling based on the dataset. 
