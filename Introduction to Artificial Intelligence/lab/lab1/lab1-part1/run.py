import pacman
import autograder

"""
run.py runs things that look like command-line arguments
for Berkeley Python. Leave the 'python pacman.py' part
at the beginning, just like running from the command line.

You should comment out all lines in the file except the one
you wan to run!
"""

#pacman.main('python pacman.py --layout tinyMaze --pacman GoWestAgent')
#pacman.main('python pacman.py -l tinyMaze -p SearchAgent -a fn=tinyMazeSearch')
pacman.main('python pacman.py -l mediumMaze -p SearchAgent -a fn=dfs')


# autograder
#autograder.run('python autograder.py')
#autograder.run('python autograder -q q1')
#autograder.run('python autograder.py -t test_cases/q2/graph_bfs_vs_dfs')