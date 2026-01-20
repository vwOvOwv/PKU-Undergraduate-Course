"""
Code to create a simple graph in a similar format to the test cases, but much simplified.
"""
from search import SearchProblem, bfs, dfs, ucs, astar
import util


class GraphProblem(SearchProblem):
    """
    A search problem that focuses on simple graphs for testing
    """
    def __init__(self, start, goals, edges):
        self.start = start
        self.goals = goals
        # edges are a list of tuples of (src, action, dst, cost)
        self.edges = {}
        for (src, action, dst, cost) in edges:
            if src not in self.edges:
                self.edges[src] = []
            self.edges[src].append( (dst, action, cost) )


    def getStartState(self):
        """
        Returns the start state for the search problem.
        """
        return self.start

    def isGoalState(self, state):
        """
          state: Search state

        Returns True if and only if the state is a valid goal state.
        """
        return state in self.goals

    def getSuccessors(self, state):
        """
          state: Search state

        For a given state, this should return a list of triples, (successor,
        action, stepCost), where 'successor' is a successor to the current
        state, 'action' is the action required to get there, and 'stepCost' is
        the incremental cost of expanding to that successor.
        """
        return self.edges[state]

    def getCostOfActions(self, actions):
        """
         actions: A list of actions to take

        This method returns the total cost of a particular sequence of actions.
        The sequence must be composed of legal moves.
        """
        # Not going to implement for simple graph problems
        util.raiseNotDefined()


def read_graph(filename=None, graph=None):
    """
    Either filename or graph must not be none.

    filename is a file containing the graph.

    graph would be a string directly containing the graph (i.e. the content of 
    the file with the graph in it)
    """
    if graph is None and filename is None or graph is not None and filename is not None:
        raise Exception('Either filename or graph, but not both, must be given')
    if filename is not None:
        graph = open(filename).read()
    # read all lines that are not comments or blank from the given string
    rows = [line for line in graph.split('\n') if not line.strip().startswith('#') and not len(line.strip()) == 0]
    start = rows[0].split('=')[1].strip()
    goals = rows[1].split('=')[1].split(',')
    # remove any extra spaces
    goals = {g.strip() for g in goals if len(g.strip()) > 0}
    edges = []
    for row in rows[2:]:
        (src, action, dst, cost) = row.split()
        cost = float(cost)
        edges.append( (src, action, dst, cost) )
    return GraphProblem(start, goals, edges)
    

def test_create():
    graph = """
# Graph files look like this:
# (comments start with a # sign, blank lines are ignored)
# so this line will be ignored
# Note that goals is plural and can have a list of goal states
start=A
goals=G,
A A->B B 1 
A A->C C 1 
A A->D D 1 
C C->G G 1 
    """
    #problem = read_graph(filename='graph.txt')
    problem = read_graph(graph=graph)

    start = problem.getStartState()
    print(start)
    print(problem.goals)
    print(problem.isGoalState(start))
    print(problem.isGoalState('G'))
    print(problem.getSuccessors(start))


def testDFS():
    problem = read_graph(filename='test_cases/extra/g1.txt')
    dfs(problem)

if __name__ == '__main__':
    #test_create()
    testDFS()
