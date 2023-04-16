#!/bin/bash

gcc -o people src/main.c src/helpers.c

# Get the directory of the currently running script
SCRIPT_DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" &> /dev/null && pwd )"

# Check if .bashrc exists
if [ -e "$HOME/.bashrc" ]; then
        # Append lines to .bashrc if people command does not exist

        if ! command -v people >/dev/null 2>&1; then
                echo 'export PATH="'$SCRIPT_DIR'/people:$PATH"' >> "$HOME/.bashrc"
                echo 'alias people="'$SCRIPT_DIR'/people"' >> "$HOME/.bashrc"
        fi

        source ~/.bashrc
        echo People is successfully installed! Now, open a new terminal window to be able to run the program.
fi

