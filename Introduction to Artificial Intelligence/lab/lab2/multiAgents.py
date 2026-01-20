# multiAgents.py
# --------------
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


from util import manhattanDistance
from game import Directions
import random, util
from math import sqrt, log

from game import Agent
import mnist
from copy import deepcopy
from MnistModel import modeldict


def predfigure(agent, gameState):
    if gameState.data.layout.predy is None: 
        predy = gameState.data.layout.y.copy()
        for (x, y) in gameState.data.food.asList():
            predy[x][y] = agent.model(mnist.getdata(gameState.data.layout.figureidx[x][y]).reshape(1, -1))
        gameState.data.layout.predy = predy
    else:
        predy = gameState.data.layout.predy
    return predy, gameState


def getmodel(name):
    ret = modeldict.get(name, None)
    assert ret is not None, f"model {name} is not implemented in MnistModel/modeldict, available models are {list(modeldict.keys())}"
    return ret


def scoreEvaluationFunction(currentGameState):
    """
      This default evaluation function just returns the score of the state.
      The score is the same one displayed in the Pacman GUI.

      This evaluation function is meant for use with adversarial search agents
      (not reflex agents).
    """
    return currentGameState.getScore()

class MultiAgentSearchAgent(Agent):
    """
      This class provides some common elements to all of your
      multi-agent searchers.  Any methods defined here will be available
      to the MinimaxPacmanAgent, AlphaBetaPacmanAgent & ExpectimaxPacmanAgent.

      You *do not* need to make any changes here, but you can if you want to
      add functionality to all your adversarial search agents.  Please do not
      remove anything, however.

      Note: this is an abstract class: one that should not be instantiated.  It's
      only partially specified, and designed to be extended.  Agent (game.py)
      is another abstract class.
    """

    def __init__(self, evalFn = 'scoreEvaluationFunction', depth = '2', model: str="Null"):
        self.index = 0 # Pacman is always agent index 0
        self.evaluationFunction = util.lookup(evalFn, globals())
        self.depth = int(depth)
        self.model = getmodel(model)()

    def getVisAction(self, gameState):
        pass

    def getAction(self, gameState):
        predy, gameState = predfigure(self, gameState)
        gameState = deepcopy(gameState)
        gameState.data.layout.y = gameState.data.layout.predy
        action = self.getVisAction(gameState)
        return action, predy

from util import Queue

class ReflexAgent(MultiAgentSearchAgent):

    def bfsstate(self, gameState):
        gameState = deepcopy(gameState)
        for (x, y) in gameState.data.food.asList():
            if gameState.data.layout.yscore[gameState.data.layout.y[x][y]] < 0:
                gameState.data.layout.walls[x][y] = True
        visited = set()
        queue = Queue()
        queue.push((gameState,1))
        visited.add(gameState.getPacmanPosition())
        foods = gameState.getPosFood().asList()
        while not queue.isEmpty():
            cur, curdist = queue.pop()
            ndist = curdist + 1
            for act in cur.getLegalActions():
                next = cur.generatePacmanSuccessor(act)
                npos = next.getPacmanPosition()
                if npos not in visited:
                    if npos in foods:
                        return ndist
                    queue.push((next, ndist))
                    visited.add(npos)
        return 1000


    def ReflexevaluationFunction(self, currentGameState, action):
        successorGameState = currentGameState.generatePacmanSuccessor(action)
        newPos = successorGameState.getPacmanPosition()
        newFood = successorGameState.getPosFood().asList()
        mindist = 0
        if len(newFood) > 0:
            mindist = self.bfsstate(successorGameState)
            #mindist = min([manhattanDistance(newPos, nf) for nf in newFood])
        return successorGameState.getScore() - 1e-3*mindist

    def getVisAction(self, gameState):
        # Collect legal moves and successor states
        legalMoves = gameState.getLegalActions()
        if len(legalMoves) > 1:
            legalMoves = [action for action in legalMoves if action != "Stop"]

        # Choose one of the best actions
        scores = [self.ReflexevaluationFunction(
            gameState, action) for action in legalMoves]
        bestScore = max(scores)
        bestIndices = [index for index in range(
            len(scores)) if scores[index] == bestScore]
        # Pick randomly among the best
        chosenIndex = random.choice(bestIndices)
        return legalMoves[chosenIndex]


class MinimaxAgent(MultiAgentSearchAgent):
    """
      Your minimax agent (question 1)
    """

    def getVisAction(self, gameState):

        
        def minimax(state, depth, agent):
            '''
                Returns the best value-action pair for the agent
            '''
            # print(gameState.getNumAgents())
            assert gameState.getNumAgents() == 2
            nextDepth = depth-1 if agent == 0 else depth
            if nextDepth == 0 or state.isWin() or state.isLose():
                return self.evaluationFunction(state), None

            bestOf, bestVal = (max, float("-inf")) if agent == 0 else (min, float("inf"))
            nextAgent = (agent + 1) % state.getNumAgents()
            bestAction = None
            for action in state.getLegalActions(agent):
                successorState = state.generateSuccessor(agent, action)
                valOfAction, _ = minimax(successorState, nextDepth, nextAgent)
                if bestOf(bestVal, valOfAction) == valOfAction:
                    bestVal = valOfAction
                    bestAction = action
            return bestVal, bestAction

        val, action = minimax(gameState, self.depth+1, self.index)
        return action

class AlphaBetaAgent(MultiAgentSearchAgent):
    """
      Your minimax agent with alpha-beta pruning (question 2)
    """

    def getVisAction(self, gameState):
        """
          Returns the minimax action using self.depth and self.evaluationFunction
        """
        "*** YOUR CODE HERE ***"
        inf = float("inf")
        def alphaBeta(state, depth, alpha, beta, agent):
            isMax = agent == 0
            nextDepth = depth-1 if isMax else depth
            if nextDepth == 0 or state.isWin() or state.isLose():
                return self.evaluationFunction(state), None

            nextAgent = (agent + 1) % state.getNumAgents()
            bestVal = -inf if isMax else inf
            bestAction = None
            bestOf = max if isMax else min

            for action in state.getLegalActions(agent):
                successorState = state.generateSuccessor(agent, action)
                valOfAction, _ = alphaBeta(
                    successorState, nextDepth, alpha, beta, nextAgent)
                if bestOf(bestVal, valOfAction) == valOfAction:
                    bestVal, bestAction = valOfAction, action

                if isMax:
                    if bestVal > beta:
                        return bestVal, bestAction
                    alpha = max(alpha, bestVal)
                else:
                    if bestVal < alpha:
                        return bestVal, bestAction
                    beta = min(beta, bestVal)

            return bestVal, bestAction

        _, action = alphaBeta(gameState, self.depth+1, -inf, inf, self.index)
        return action


class MCTSAgent(MultiAgentSearchAgent):

    def getVisAction(self, gameState):

        class Node:

            def __init__(self, data):
                self.north = None
                self.east = None
                self.west = None
                self.south = None
                self.stop = None
                self.parent = None
                self.statevalue = data[0]
                self.numerator = data[1]
                self.denominator = data[2]

        data = [gameState, 0, 1]
        cgstree = Node(data)

        def Selection(cgs, cgstree):
            action = None
            while cgstree.north is not None or cgstree.east is not None or cgstree.south is not None or cgstree.west is not None:

                children = []
                destin = (cgstree.north, "North")
                children.append(destin)
                destin = (cgstree.east, "East")
                children.append(destin)
                destin = (cgstree.south, "South")
                children.append(destin)
                destin = (cgstree.west, "West")
                children.append(destin)
                destin = (cgstree.stop, "Stop")
                children.append(destin)

                best_child, action = best_UCT(children)

                cgs = best_child
                if action == "North":
                    cgstree.north.statevalue = best_child
                    cgstree = cgstree.north
                if action == "East":
                    cgstree.east.statevalue = best_child
                    cgstree = cgstree.east
                if action == "South":
                    cgstree.south.statevalue = best_child
                    cgstree = cgstree.south
                if action == "West":
                    cgstree.west.statevalue = best_child
                    cgstree = cgstree.west
                if action == "Stop":
                    cgstree.stop.statevalue = best_child
                    cgstree = cgstree.stop
            return (cgs, cgstree, action)

        def Expansion(cgs, cgstree):
            legal_actions = cgstree.statevalue.getLegalActions(0)
            for action in legal_actions:
                dummy_data = [
                    cgstree.statevalue.generateSuccessor(0, action), 1, 1]
                dummy_node = Node(dummy_data)
                if action == "North":
                    cgstree.north = dummy_node
                    cgstree.north.parent = cgstree

                elif action == "East":
                    cgstree.east = dummy_node
                    cgstree.east.parent = cgstree

                elif action == "South":
                    cgstree.south = dummy_node
                    cgstree.south.parent = cgstree

                elif action == "West":
                    cgstree.west = dummy_node
                    cgstree.west.parent = cgstree

                elif action == "Stop":
                    cgstree.stop = dummy_node
                    cgstree.stop.parent = cgstree

        def Simulation(cgs, cgstree):
            WinorLose = 0
            while cgstree.statevalue.isWin() is False and cgstree.statevalue.isLose() is False:
                cgs, cgstree, action = Selection(cgs, cgstree)
                Expansion(cgs, cgstree)
            if cgstree.statevalue.isLose():
                WinorLose = 0
            elif cgstree.statevalue.isWin():
                WinorLose = 1
            return WinorLose, cgstree

        def Backpropagation(cgstree, WinorLose):
            while cgstree.parent is not None:
                cgstree.numerator = cgstree.numerator + WinorLose
                cgstree.denominator = cgstree.denominator + 1
                cgstree = cgstree.parent
            return cgstree

        def best_UCT(children):
            i = 0
            while i < len(children):
                if children[i][0] is None or children[i][1] == 'Stop':
                    children.pop(i)
                else:
                    i = i+1
            children_UCT = []
            for i in range(len(children)):
                value = ((children[i][0].numerator / children[i][0].denominator) + sqrt(2) * sqrt(
                    ((log(children[i][0].parent.denominator))/log(2.71828)) / children[i][0].denominator)), children[i][1]

                children_UCT.append(value)
            max_index = 0
            equal_counter = 1
            for i in range(len(children_UCT)-1):
                if children_UCT[i][0] == children_UCT[i+1][0]:
                    equal_counter = equal_counter + 1
            if equal_counter == len(children_UCT):
                decision_maker = random.randint(1, 101)
                if decision_maker < 70:
                    eval_list = []
                    max_index_list = []
                    for i in range(len(children)):
                        eval_list.append(HeuristicFunction(
                            children[i][0].statevalue))
                    max_index_list.append(eval_list.index(max(eval_list)))
                    maxval = eval_list.pop(max_index_list[-1])
                    eval_list.insert(max_index_list[-1], -9999)
                    while maxval in eval_list:
                        max_index_list.append(eval_list.index(max(eval_list)))
                        eval_list.pop(max_index_list[-1])
                        eval_list.insert(max_index_list[-1], -9999)
                    max_index = random.choice(max_index_list)
                else:
                    max_index = random.randint(0, len(children)-1)

            else:
                maximumvalueofUCT = -9999
                for i in range(len(children_UCT)):
                    if children_UCT[i][0] > maximumvalueofUCT:
                        max_index = i
                        maximumvalueofUCT = children_UCT[i][0]
            return (children[max_index][0].statevalue, children[max_index][1])

        def HeuristicFunction(currentGameState):
            new_position = currentGameState.getPacmanPosition()
            new_food = currentGameState.getPosFood().asList()

            food_distance_min = float('inf')
            for food in new_food:
                food_distance_min = min(
                    food_distance_min, manhattanDistance(new_position, food))

            for i in currentGameState.getNegFood().asList():
                if manhattanDistance(i, new_position) <= 0.5:
                    return -float("inf")

            ghost_distance = 0
            ghost_positions = currentGameState.getGhostPositions()

            for i in ghost_positions:
                ghost_distance = manhattanDistance(new_position, i)
                if (ghost_distance < 1):
                    return -float('inf')

            food = currentGameState.getNumFood()
            pellet = len(currentGameState.getCapsules())

            food_coefficient = 999999
            pellet_coefficient = 19999
            food_distance_coefficient = 999

            game_rewards = 0
            if currentGameState.isLose():
                game_rewards = game_rewards - 99999
            elif currentGameState.isWin():
                game_rewards = game_rewards + 99999

            answer = (1.0 / (food + 1) * food_coefficient) + ghost_distance + (
                1.0 / (food_distance_min + 1) * food_distance_coefficient) + (
                1.0 / (pellet + 1) * pellet_coefficient) + game_rewards

            return answer

        def best_UCT_end(children):
            i = 0
            while i < len(children):
                if children[i][0] is None or children[i][1] == 'Stop':
                    children.pop(i)
                else:
                    i = i+1
            children_UCT = []
            for i in range(len(children)):
                value = ((children[i][0].numerator / children[i][0].denominator) + sqrt(2) * sqrt(
                    ((log(children[i][0].parent.denominator))/log(2.71828)) / children[i][0].denominator)), children[i][1]
                children_UCT.append(value)
            max_index = 0
            equal_counter = 1
            for i in range(len(children_UCT)-1):
                if children_UCT[i][0] == children_UCT[i+1][0]:
                    equal_counter = equal_counter + 1
            if equal_counter == len(children_UCT):
                decision_maker = random.randint(1, 101)
                if decision_maker < 102:
                    eval_list = []
                    max_index_list = []
                    for i in range(len(children)):
                        eval_list.append(HeuristicFunction(
                            children[i][0].statevalue))
                    max_index_list.append(eval_list.index(max(eval_list)))
                    maxval = eval_list.pop(max_index_list[-1])
                    eval_list.insert(max_index_list[-1], -9999)
                    while maxval in eval_list:
                        max_index_list.append(eval_list.index(max(eval_list)))
                        eval_list.pop(max_index_list[-1])
                        eval_list.insert(max_index_list[-1], -9999)
                    max_index = random.choice(max_index_list)
                else:
                    max_index = random.randint(0, len(children)-1)

            else:
                maximumvalueofUCT = -9999
                for i in range(len(children_UCT)):
                    if children_UCT[i][0] > maximumvalueofUCT:
                        max_index = i
                        maximumvalueofUCT = children_UCT[i][0]
            return (children[max_index][0].statevalue, children[max_index][1])

        def endSelection(cgs, cgstree):
            if cgstree.north is not None or cgstree.east is not None or cgstree.south is not None or cgstree.west is not None:
                children = []
                # print "hey i am in while loop"
                destin = (cgstree.north, "North")
                children.append(destin)
                destin = (cgstree.east, "East")
                children.append(destin)
                destin = (cgstree.south, "South")
                children.append(destin)
                destin = (cgstree.west, "West")
                children.append(destin)
                destin = (cgstree.stop, "Stop")
                children.append(destin)
                best_child, action = best_UCT_end(children)
                return action

        for i in range(20):
            gameState, cgstree, action = Selection(gameState, cgstree)
            Expansion(gameState, cgstree)
            WinorLose, cgstree = Simulation(gameState, cgstree)
            cgstree = Backpropagation(cgstree, WinorLose)
            gameState = cgstree.statevalue
        return endSelection(gameState, cgstree)
