import pacman
import autograder

"""
run.py runs things that look like command-line arguments
for Berkeley Python. Leave the 'python pacman.py' part
at the beginning, just like running from the command line.

You should comment out all lines in the file except the one
you wan to run!
"""

# pacman
pacman.main('python pacman.py')
pacman.main('python pacman.py -p MinimaxAgent -l minimaxClassic -a depth=4')


# autograder
autograder.run('python autograder.py')
autograder.run('python autograder -q q1')
autograder.run('python autograder.py -q q1 --no-graphics')
autograder.run('python autograder.py -t test_cases/q2/0-small-tree')