# people
### what this program does
This is a basic CLI tool for tracking the number of days since you have made contact with people. If you want to ensure that you 'check in' with friends, family or colleages over a longer period of time, this is a discreet and hidden way of doing so.

It is designed to be simple and unobtrusive. 

## usage
```
./people  add     forename surname
./people  check   all
./people  check   forename surname
./people  forget  forename surname
./people  days    number [interval of days between checks]
```
## walkthrough
First, add a user. If you don't have a 'yellow pages' file (you won't initially), one will be created. arguments additional to 
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
