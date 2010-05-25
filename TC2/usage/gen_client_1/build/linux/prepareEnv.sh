#!/bin/sh

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

export libdir=../../../../library
export LD_LIBRARY_PATH=$libdir/build/linux:$LD_LIBRARY_PATH

# the end
