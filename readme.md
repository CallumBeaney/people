# people
## what is?
This is a CLI tool for tracking the number of days since you have made contact with people.  
Why a CLI? It is discreet and the vast majority of people don't even know what a terminal is.  
Below is a walkthrough and instructions on making this program accessible system-wide with an alias for smoother use.  

## usage
```
./people  add     forename surname
./people  check   forename surname
./people  check   all
./people  forget  forename surname
./people  forget  all
./people  days    number [see Walkthrough below]
```

## installation
Download this repo using the green `<> Code` button above, or via the terminal:  
```
git clone https://github.com/CallumBeaney/people
```
Unless you wish to make this program accessible system-wide using an alias, you only need the executable named `people`.

## walkthrough
1. First, add a user. If you don't have a `yellowpages` file (you won't initially), one will be created.   
```
./people add Fred Durst 
    Added Fred Durst to your People List
./people add David Hume
    Added David Hume to your People List
```
2. Now let's check one of them:
```
./people check fred durst
    fred durst - last checked 0 days ago
```
3. You will be prompted with an offer to reset the date associated with this name.  Let's imagine that, 2 months earlier, 'Joanna Newsom' was added to this list, and you want to see the full list. You won't have a `timespan` file initially; one will be made automatically. 

```       
./people check all
Joanna Newsom   - last checked  59      days ago  ! IMPORTANT
Fred Durst      - last checked  0       days ago
David Hume      - last checked  0       days ago
```
Again, you will be prompted with an offer to reset the date associated with these names.  

4.  That `! IMPORTANT` is triggered when your checking interval stored in the file `timespan` is smaller than the elapsed days since you last checked a person. To change that interval, run the following command with your preferred number of days before you're given a warning:

```
./people days 100                 
    Interval to compare dates set to: 100 days 
./people check all        
    Joanna Newsom   - last checked  59      days ago
    Fred Durst      - last checked  0       days ago
    David Hume      - last checked  0       days ago
```

5. To stop checking in on a person:
```
./people forget fred durst
```
6. Your `yellowpages` file would then look like below. If you want to manually add a large number of names, you can, however it is imperative that there is an empty line as the final line in the file. The syntax must always follow the below example, with no whitespace after the comma, and D/M/Y dating. If it gets corrupted, you can delete it, and when you next run an ADD command, a fresh file will be generated.
```
1   Joanna Newsom,29/1/2023
2   David Hume,29/3/2023
3      
```
7. To forget all people in your list, just use:
```
./people forget all
```

### making this program accessible system-wide with an alias
For the sake of being able to quickly check without moving to the containing folder and executing from there like a standard C executable, here is how you can create an alias and add it to your PATH. You can find instructions on how to do this online for your specific operating system. If using a macOS, the process will look like this:  

1. Open a terminal and type:
```
nano ~/.zshrc
```  
2. In the file, add the following lines with the path to the executable 'people' replacing the paths below. Do not remove `:$PATH` from within the quotes.  
```
export PATH="/path/to/program/people:$PATH"
alias people='/path/to/program/people'
```  
3. Then exit and save by `^X` and submitting `Y` to confirm save on exit, and then reload your profile file by running:
```
source ~/.zshrc
```  
By now, you can call your program from anywhere with just e.g. 'people check all', but it will need to be configured to only read and write your People List data from a specific folder.  
  
4. Move to the `/src/` directory containing the source files, open `constants.h`, and at lines 4 and 5, change the inside of the quotes to the path of that folder. It should look initially look something like this:
``` 
#define TIMEFILE "timespan"
#define NAMEFILE "yellowpages"
```  
5. ...and with the given path, it might look something like this:
``` 
#define TIMEFILE "/Users/userName/Applications/people/src/timespan"
#define NAMEFILE "/Users/userName/Applications/people/src/yellowpages"
```  
If you want to make the files invisible in the main folder system, you can prepend with a dot on macOS e.g. `~/people/.timespan`.
6. Make the program with e.g. GCC make utility from the `/src/` directory: 
```
gcc -o people main.c helpers.c
```  
7. You can now run People like this:

```
% people add John Titor        
	Added John Titor to your People List
% people check John titor
	John titor - last checked 0 days ago
    Reset number of days passed to 0?
    Y/N: n
```
