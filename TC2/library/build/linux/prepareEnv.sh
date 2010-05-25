#!/bin/sh
# See licensing information in the file README.TXT.

# @source      prepareEnv.sh
# @description one-letter shortcuts for common tasks

# call using '.'
# . prepareEnv.sh

# export MYMAKE=colormake
export MYMAKE=make

# builds application
alias m=$MYMAKE

# builds application (debug)
alias d="$MYMAKE DEBUG=1"

# cleans files
alias c="$MYMAKE clean"

# html colorized source files creation
alias h="$MYMAKE html"

# running the program
alias r="$MYMAKE run"

# creates zip package
alias z="$MYMAKE zip"

# the end
