```
┌─┐┌─┐┌─┐┌─┐┬  ┌─┐
├─┘├┤ │ │├─┘│  ├┤ 
┴  └─┘└─┘┴  ┴─┘└─┘
```

## what this program does
This is a CLI tool for tracking the number of days since you have made contact with people. If you want to ensure that you remember to 'check in' with friends, family or colleages at regular intervals, this is a discreet & unobtrusive way of doing so.

Below is a walkthrough and instructions on making this program accessible system-wide with an alias for smoother use.

## usage
```
./people  add     forename surname
./people  check   forename surname
./people  check   all
./people  forget  forename surname
./people  days    number [see Walkthrough below]
```

## walkthrough
First, add a user. If you don't have a 'yellow pages' file (you won't initially), one will be created.   
```
./people add Fred Durst 
    Added Fred Durst to your People List
./people add David Hume
    Added David Hume to your People List
```
Now let's check one of them:
```
./people check fred durst
    fred durst - last checked 0 days ago
```
You will be prompted with an offer to reset the date associated with this name.  Let's imagine that, 2 months earlier, 'Joanna Newsom' was added to this list.  

```       
./people check all
Joanna Newsom   - last checked  59      days ago  ! IMPORTANT
Fred Durst      - last checked  0       days ago
David Hume      - last checked  0       days ago
```
Again, you will be prompted with an offer to reset the date associated with this name.  

That ! IMPORTANT is triggered when your checking interval stored in the file 'timespan' is smaller than the elapsed days since you last checked a person. To change that interval, run the following command with your preferred number of days before you're given a warning:

```
./people days 100                 
    Interval to compare dates set to: 100 days 
./people check all        
    Joanna Newsom   - last checked  59      days ago
    Fred Durst      - last checked  0       days ago
    David Hume      - last checked  0       days ago
```

To stop checking in on a person:
```
./people forget fred durst
```
Your 'yellowpages' file would then look like below. It is imperative that there is an empty line as the final line in the file. You should not be messing with this file. If it gets corrupted, you can delete it, and when you next run an ADD command, a fresh file will be generated.
```
1   Joanna Newsom,29/1/2023
2   David Hume,29/3/2023
3      
```

### making this program accessible system-wide with an alias
For the sake of being able to quickly check without moving to the containing folder, you can create an alias and add it to your PATH.
You can find instructions on how to do this online.  
In my case, using a Mac M1, this process would look like this:  
Open a terminal and type:
```
nano ~/.zshrc
```
In the file, add the following lines with the path to the executable 'people' replacing the paths below. Do not remove :$PATH from within the quotes.
```
export PATH="/path/to/program/people:$PATH"
alias people='/path/to/program/people'
```
Then exit and save by ^X and submitting 'Y' to confirm save on exit, and then reload your profile file by running:
```
source ~/.zshrc
```
Now you can run People like this:

```
% people add John Titor        
	Added John Titor to your People List
% people check John titor
	John titor - last checked 0 days ago
    Reset number of days passed to 0?
    Y/N: n
```
etc etc