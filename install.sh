#!/bin/bash

gcc -o people src/main.c src/helpers.c

# Get the directory of the currently running script
SCRIPT_DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" &> /dev/null && pwd )"
echo $SCRIPT_DIR
# Check if .bashrc exists
if [ -e "$HOME/.bashrc" ]; then
    # Append lines to .bashrc
    echo 'export PATH="'$SCRIPT_DIR'/people:$PATH"' >> "$HOME/.bashrc"
    echo 'alias people="'$SCRIPT_DIR'/people"' >> "$HOME/.bashrc"
    source ~/.bashrc
fi

