# people

**what is?**  ー This is a CLI tool for tracking the number of days since you have made contact with people.  
**why a CLI?**  ー 99% of people don't know what a terminal is -- lightweight & discreet  
    
[usage](#usage)  
[installation script on linux](#installation-script-for-linux)  
[manual installation](#manual-installation)  
[creating an alias](#creating-an-alias)  
[usage walkthrough](#walkthrough)  


## usage
```
./people  add     [person's name]
./people  check   [person's name OR substring thereof]
./people  check   all
./people  forget  [person's name]
./people  forget  all
./people  days    [number]
./people  info
```

## installation script for linux
If you are using a linux distribution, you can simply run this command in the program's directory:
```bash
sudo chmod +rwx install.sh;./install.sh
```
You might need to open new terminal window right after installation to be able to call `people` with its alias like e.g. `people check all` sans the `./`.  
<br>

## manual installation
Download this repo using the green `<>Code` button above, or via the terminal:  
```
git clone https://github.com/CallumBeaney/people  
```  
The provided executable was compiled on an AArch64 processor. While the source code is compatible with Linux, macOS, Ubuntu, Solaris etc.   running it on other operating systems may result in a `bad CPU` error. To fix this, simply make the program with e.g. GCC make utility like so, and the source will be repackaged for your setup:  
```
gcc -o people src/main.c src/helpers.c
```
  
  
## creating an alias
N.B. If you have used `install.sh` this will be done for you.  
  
For the sake of being able to quickly check without moving to the containing folder, you can create an alias and add it to your PATH.  
If using a macOS, the process will look like this:  

1. Open a terminal and type:
```
nano ~/.zshrc
```  
2. In the file, add the following lines with the path to the executable 'people' replacing the paths below. Do not remove `:$PATH` from within the quotes.  
```
export PATH="/Users/username/Applications/program_folder/people:$PATH"
alias people='/Users/username/Applications/program_folder/people'
```  
3. Then exit and save by `^X` and submitting `Y` to confirm save on exit, and then reload your profile file by running:
```
source ~/.zshrc
```  
4. You can now run People like this:  

```
% people add John Titor        
	Added John Titor to your People List
% people check John titor
	John titor - last checked 0 days ago
    Reset number of days passed to 0?
    Y/N: n
```


## walkthrough
1. First, add some people:  
```
./people add Fred Durst 
    Added Fred Durst to your People List
./people add David Hume
    Added David Hume to your People List
```
2. Check one of them by the exact name or by a substring:
```
./people check fred durst
    fred durst - last checked 0 days ago
./people check fred
    fred durst - last checked 0 days ago
```
Case isn't important; spelling is.  
You will be prompted with an offer to reset the date associated with this name.   
  
3. Imagine that two months earlier 'Joanna Newsom' was added to this list:

```       
./people check all

David Hume      - last checked  0       days ago
Fred Durst      - last checked  0       days ago
Joanna Newsom   - last checked  59      days ago  ! IMPORTANT
```
Again, you will be prompted with an offer to reset the dates associated with these names to 0 days.  
That `! IMPORTANT` is triggered when your checking interval stored in the file `timespan` is smaller than the elapsed days since you last checked a person.  
You won't have a `timespan` file initially; one will be made automatically.  
  
4. To change that interval, run the following command with your preferred number of days before you're given a warning:  
```
./people days 100                 
    Interval to compare dates set to: 100 days 

./people check all        
    David Hume      - last checked  0       days ago
    Fred Durst      - last checked  0       days ago
    Joanna Newsom   - last checked  59      days ago
```
The program will auto-sort your name list alphabetically when you do a checkall.  
  
5. To stop checking in on a person using their exact name:  
```
./people forget fred durst
```
6. Your `namefile` file would then look like below. You can add names manually; the syntax must follow the below example & there must be an empty final line in the file.  If it gets corrupted, you can delete it, and when you next run an ADD command, a fresh file will be generated.
```
1   David Hume,29/3/2023
2   Joanna Newsom,29/1/2023
3      
```
7. To forget all people in your list, just use:
```
./people forget all
```
  
