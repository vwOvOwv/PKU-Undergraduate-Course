# search.py
# ---------
# Licensing Information:  You are free to use or extend these projects for
# educational purposes provided that (1) you do not distribute or publish
# solutions, (2) you retain this notice, and (3) you provide clear
# attribution to UC Berkeley, including a link to http://ai.berkeley.edu.
# 
# Attribution Information: The Pacman AI projects were developed at UC Berkeley.
# The core projects and autograders were primarily created by John DeNero
# (denero@cs.berkeley.edu) and Dan Klein (klein@cs.berkeley.edu).
# Student side autograding was added by Brad Miller, Nick Hay, and
# Pieter Abbeel (pabbeel@cs.berkeley.edu).


"""
In search.py, you will implement generic search algorithms which are called by
Pacman agents (in searchAgents.py).
"""

import util
import heapq

class SearchProblem:
    """
    This class outlines the structure of a search problem, but doesn't implement
    any of the methods (in object-oriented terminology: an abstract class).

    You do not need to change anything in this class, ever.
    """

    def getStartState(self):
        """
        Returns the start state for the search problem.
        """
        util.raiseNotDefined()

    def isGoalState(self, state):
        """
          state: Search state

        Returns True if and only if the state is a valid goal state.
        """
        util.raiseNotDefined()

    def getSuccessors(self, state):
        """
          state: Search state

        For a given state, this should return a list of triples, (successor,
        action, stepCost), where 'successor' is a successor to the current
        state, 'action' is the action required to get there, and 'stepCost' is
        the incremental cost of expanding to that successor.
        """
        util.raiseNotDefined()

    def getCostOfActions(self, actions):
        """
         actions: A list of actions to take

        This method returns the total cost of a particular sequence of actions.
        The sequence must be composed of legal moves.
        """
        util.raiseNotDefined()


def tinyMazeSearch(problem):
    """
    Returns a sequence of moves that solves tinyMaze.  For any other maze, the
    sequence of moves will be incorrect, so only use this for tinyMaze.
    """
    from game import Directions
    s = Directions.SOUTH
    w = Directions.WEST
    return  [s, s, w, s, w, w, s, w]

def depthFirstSearch(problem):
    """
    Search the deepest nodes in the search tree first.

    Your search algorithm needs to return a list of actions that reaches the
    goal. Make sure to implement a graph search algorithm.

    To get started, you might want to try some of these simple commands to
    understand the search problem that is being passed in:

    print("Start:", problem.getStartState())
    print("Is the start a goal?", problem.isGoalState(problem.getStartState()))
    print("Start's successors:", problem.getSuccessors(problem.getStartState()))
    """
    "*** YOUR CODE HERE ***"
    Frontier=util.Stack()
    Visited=[]
    Frontier.push((problem.getStartState(),[]))
    while Frontier.isEmpty()==False:
        state,actions=Frontier.pop()
        Visited.append(state)
        if problem.isGoalState(state):
            return actions
        for next in problem.getSuccessors(state):
            n_state=next[0]
            n_direction=next[1]
            if n_state not in Visited:
                Frontier.push((n_state,actions+[n_direction]))
    util.raiseNotDefined()


def breadthFirstSearch(problem):
    """Search the shallowest nodes in the search tree first."""
    "*** YOUR CODE HERE ***"
    Frontier=util.Queue()
    Visited=[]
    Frontier.push((problem.getStartState(),[]))
    Visited.append(problem.getStartState())
    while Frontier.isEmpty()==False:
        state,actions=Frontier.pop()
        if problem.isGoalState(state):
            return actions
        for next in problem.getSuccessors(state):
            n_state=next[0]
            n_direction=next[1]
            if n_state not in Visited:
                Frontier.push((n_state,actions+[n_direction]))
                Visited.append(n_state)
    util.raiseNotDefined()
    
#! 例题答案如下
# def breadthFirstSearch(problem):
#     """Search the shallowest nodes in the search tree first."""
#     #python pacman.py -l mediumMaze -p SearchAgent -a fn=bfs --frameTime 0
#     #python pacman.py -l bigMaze -p SearchAgent -a fn=bfs -z .5 --frameTime 0
#     "*** YOUR CODE HERE ***"

#     Frontier = util.Queue()
#     Visited = []
#     Frontier.push( (problem.getStartState(), []) )
#     #print 'Start',problem.getStartState()
#     Visited.append( problem.getStartState() )

#     while Frontier.isEmpty() == 0:
#         state, actions = Frontier.pop()
#         if problem.isGoalState(state):
#             #print 'Find Goal'
#             return actions 
#         for next in problem.getSuccessors(state):
#             n_state = next[0]
#             n_direction = next[1]
#             if n_state not in Visited:
                
#                 Frontier.push( (n_state, actions + [n_direction]) )
#                 Visited.append( n_state )

def uniformCostSearch(problem):
    """Search the node of least total cost first."""
    "*** YOUR CODE HERE ***"
    Frontier=util.PriorityQueue()
    Visited=[]
    Frontier.push((problem.getStartState(),[]),0)
    while Frontier.isEmpty()==False:
        state,actions=Frontier.pop()
        if problem.isGoalState(state):
            return actions
        if state not in Visited:
            Visited.append(state)
            for next in problem.getSuccessors(state):
                n_state=next[0]
                n_direction=next[1]
                if n_state not in Visited:
                    Frontier.push((n_state,actions+[n_direction]),problem.getCostOfActions(actions+[n_direction]))
    util.raiseNotDefined()

def nullHeuristic(state, problem=None):
    """
    A heuristic function estimates the cost from the current state to the nearest
    goal in the provided SearchProblem.  This heuristic is trivial.
    """
    return 0

def aStarSearch(problem, heuristic=nullHeuristic):
    """Search the node that has the lowest combined cost and heuristic first."""
    "*** YOUR CODE HERE ***"
    Frontier=util.PriorityQueue()
    Visited=[]
    Frontier.push((problem.getStartState(),[]),heuristic(problem.getStartState(),problem))
    while Frontier.isEmpty()==False:
        state,actions=Frontier.pop()
        if problem.isGoalState(state):
            return actions
        if state not in Visited:
            Visited.append(state)
            for next in problem.getSuccessors(state):
                n_state=next[0]
                n_direction=next[1]
                if n_state not in Visited:
                    Frontier.push((n_state,actions+[n_direction]),problem.getCostOfActions(actions+[n_direction])+heuristic(n_state,problem))
    util.raiseNotDefined()

# Abbreviations
bfs = breadthFirstSearch
dfs = depthFirstSearch
astar = aStarSearch
ucs = uniformCostSearch
