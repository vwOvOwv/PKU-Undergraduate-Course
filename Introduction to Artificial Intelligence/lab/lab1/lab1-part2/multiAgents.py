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



class ReflexAgent(Agent):
    """
      A reflex agent chooses an action at each choice point by examining
      its alternatives via a state evaluation function.

      The code below is provided as a guide.  You are welcome to change
      it in any way you see fit, so long as you don't touch our method
      headers.
    """

    def getAction(self, gameState):
        """
        You do not need to change this method, but you're welcome to.

        getAction chooses among the best options according to the evaluation function.

        Just like in the previous project, getAction takes a GameState and returns
        some Directions.X for some X in the set {North, South, West, East, Stop}
        """
        # Collect legal moves and successor states
        legalMoves = gameState.getLegalActions()

        # Choose one of the best actions
        scores = [self.evaluationFunction(
            gameState, action) for action in legalMoves]
        bestScore = max(scores)
        bestIndices = [index for index in range(
            len(scores)) if scores[index] == bestScore]
        # Pick randomly among the best
        chosenIndex = random.choice(bestIndices)

        "Add more of your code here if you want to"

        return legalMoves[chosenIndex]

    def evaluationFunction(self, currentGameState, action):
        """
        Design a better evaluation function here.

        The evaluation function takes in the current and proposed successor
        GameStates (pacman.py) and returns a number, where higher numbers are better.

        The code below extracts some useful information from the state, like the
        remaining food (newFood) and Pacman position after moving (newPos).
        newScaredTimes holds the number of moves that each ghost will remain
        scared because of Pacman having eaten a power pellet.

        Print out these variables to see what you're getting, then combine them
        to create a masterful evaluation function.
        """
        # Useful information you can extract from a GameState (pacman.py)
        successorGameState = currentGameState.generatePacmanSuccessor(action)
        newPos = successorGameState.getPacmanPosition()
        newFood = successorGameState.getFood().asList()
        newGhostStates = successorGameState.getGhostStates()
        newScaredTimes = [
            ghostState.scaredTimer for ghostState in newGhostStates]

        # NOTE: this is an incomplete function, just showing how to get current state of the Env and Agent.

        return successorGameState.getScore()

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

    def __init__(self, evalFn = 'scoreEvaluationFunction', depth = '2'):
        self.index = 0 # Pacman is always agent index 0
        self.evaluationFunction = util.lookup(evalFn, globals())
        self.depth = int(depth)

class MinimaxAgent(MultiAgentSearchAgent):
    """
      Your minimax agent (question 1)
    """

    def getAction(self, gameState):
        """
          Returns the minimax action from the current gameState using self.depth
          and self.evaluationFunction.
          Here are some method calls that might be useful when implementing minimax.
          gameState.getLegalActions(agentIndex):
            Returns a list of legal actions for an agent
            agentIndex=0 means Pacman, ghosts are >= 1
          gameState.generateSuccessor(agentIndex, action):
            Returns the successor game state after an agent takes an action
          gameState.getNumAgents():
            Returns the total number of agents in the game
        """
        "*** YOUR CODE HERE ***"
        #有搜索层数限制
        #鬼可能有多个
        def get_action(gameState,index,depth):
            # if gameState.getLegalActions(index)==[]:
            if self.depth+1==depth or gameState.getLegalActions(index)==[]:
                return (self.evaluationFunction(gameState),Directions.STOP) 
            if index==0:
                return max_action(gameState,depth)
            else:
                return min_action(gameState,depth,index)
        def max_action(gameState,depth):
            v=float('-inf')
            actions=gameState.getLegalActions(0)
            max_act=Directions.STOP
            for action in actions:
              successor=gameState.generateSuccessor(0,action)
              tmp=get_action(successor,1,depth)#depth不加1
              if v<tmp[0]:
                  v=tmp[0]
                  max_act=action
            return (v,max_act)
        def min_action(gameState,depth,index):
            v=float('inf')
            actions=gameState.getLegalActions(index)
            min_act=Directions.STOP
            for action in actions:
              successor=gameState.generateSuccessor(index,action)
              agentNum=gameState.getNumAgents()
              if index+1<agentNum:
                  tmp=get_action(successor,index+1,depth)
              else:
                  tmp=get_action(successor,0,depth+1)
              if v>tmp[0]:
                  v=tmp[0]
                  min_act=action
            return (v,min_act)
        
        return get_action(gameState,0,1)[1]
        util.raiseNotDefined()

class AlphaBetaAgent(MultiAgentSearchAgent):
    """
      Your minimax agent with alpha-beta pruning (question 2)
    """

    def getAction(self, gameState):
        """
          Returns the minimax action using self.depth and self.evaluationFunction
        """
        "*** YOUR CODE HERE ***"
        def get_action(gameState,index,depth,alpha,beta):
            if self.depth+1==depth or gameState.getLegalActions(index)==[]:
                return (self.evaluationFunction(gameState),Directions.STOP) 
            if index==0:
                return max_action(gameState,depth,alpha,beta)
            else:
                return min_action(gameState,depth,index,alpha,beta)
        def max_action(gameState,depth,alpha,beta):
            v=float('-inf')
            actions=gameState.getLegalActions(0)
            max_act=Directions.STOP
            for action in actions:
                successor=gameState.generateSuccessor(0,action)
                tmp=get_action(successor,1,depth,alpha,beta)#depth不加1
                if v<tmp[0]:
                    v=tmp[0]
                    max_act=action
                if v>beta:
                    return (v,max_act)
                alpha=max(alpha,v)
            return (v,max_act)
        def min_action(gameState,depth,index,alpha,beta):
            v=float('inf')
            actions=gameState.getLegalActions(index)
            min_act=Directions.STOP
            for action in actions:
                successor=gameState.generateSuccessor(index,action)
                agentNum=gameState.getNumAgents()
                if index+1<agentNum:
                    tmp=get_action(successor,index+1,depth,alpha,beta)
                else:
                    tmp=get_action(successor,0,depth+1,alpha,beta)
                if v>tmp[0]:
                    v=tmp[0]
                    min_act=action
                if v<alpha:
                    return (v,min_act)
                beta=min(beta,v)
            return (v,min_act)
        
        alpha=float('-inf')
        beta=float('inf')
        return get_action(gameState,0,1,alpha,beta)[1]
        util.raiseNotDefined()

class MCTSAgent(MultiAgentSearchAgent):
    """
      Your MCTS agent with Monte Carlo Tree Search (question 3)
    """

    def getAction(self, gameState):#找出当前状态下最合适的动作

        class Node:
            '''
            We have provided node structure that you might need in MCTS tree.
            '''
            def __init__(self):
                self.index = None
                self.direction = None
                self.parent = None
                self.children = []
                self.state = None
                self.totalReward=0
                self.count=1

        root = Node()
        root.index=0
        root.state=gameState
        agentNum=0
        foodNum=0
        
        def calUCB(parent,child):
            ans=child.totalReward/child.count+sqrt(2*log(parent.count)/child.count)
            return ans
        
        def Selection(curNode):
            "*** YOUR CODE HERE ***"
            while curNode.children!=[]:
                maxi=float('-inf')
                for child in curNode.children:
                    UCB=calUCB(curNode,child)
                    if UCB>maxi:
                        maxi=UCB
                        curNode=child
            return curNode
            util.raiseNotDefined()
        
        def Expansion(curNode):
            "*** YOUR CODE HERE ***"
            index=curNode.index
            actions=curNode.state.getLegalActions(index)
            if actions!=[]:
                for action in actions:
                    successor=curNode.state.generateSuccessor(index,action)
                    child=Node()
                    child.state=successor
                    child.parent=curNode
                    child.direction=action
                    if index+1<agentNum:
                        child.index=index+1
                    else:
                        child.index=0
                    curNode.children.append(child)
                return curNode
            else:#可能出现没有子结点的情况（游戏结束）
                return None
            util.raiseNotDefined()

        def Simulation(curNode):
            "*** YOUR CODE HERE ***"
            depth=1#记录模拟深度
            if curNode.index==0:#利用pacman的启发函数找一个扩展出的结点
                h_min=float('inf')
                for child in curNode.children:
                    h=HeuristicFunction(child.state)
                    if h<h_min:
                        h_min=h
                        tmpNode=child
            else:#鬼随机选
                tmpNode=random.choice(curNode.children)
            while tmpNode.state.getLegalActions(tmpNode.index)!=[] and depth<=20:#搜索深度
                depth+=1
                actions=tmpNode.state.getLegalActions(tmpNode.index)
                if tmpNode.index==0:
                    h_min=float('inf')
                    for action in actions:
                        tmpSuccessor=tmpNode.state.generateSuccessor(tmpNode.index,action)
                        h=HeuristicFunction(tmpSuccessor)
                        if h<h_min:
                            h_min=h
                            successor=tmpSuccessor
                else:#鬼随机走一步
                    action=random.choice(actions)
                    successor=tmpNode.state.generateSuccessor(tmpNode.index,action)
                n_node=Node()
                n_node.state=successor
                if tmpNode.index+1<agentNum:
                    n_node.index=tmpNode.index+1
                else:
                    n_node.index=0
                tmpNode=n_node
            if tmpNode.state.isLose():
                return -300
            return -tmpNode.state.getNumFood()-len(tmpNode.state.getCapsules())#用剩余的豆子数和capsule数作为reward
            util.raiseNotDefined()

        def Backpropagation(curNode,reward):
            "*** YOUR CODE HERE ***"
            while curNode!=None:
                if curNode.index==1:
                    curNode.totalReward+=reward
                    curNode.count+=1
                else:
                    curNode.totalReward-=reward
                    curNode.count+=1
                curNode=curNode.parent
            return
            util.raiseNotDefined()

        def HeuristicFunction(currentGameState):
            "*** YOUR CODE HERE ***"
            foodGrid=currentGameState.getFood()
            foods=foodGrid.asList()
            pacmanPosition=currentGameState.getPacmanPosition()
            ghostPositions=currentGameState.getGhostPositions()
            #找最近的食物的位置
            minFoodDis=float('inf')
            for food in foods:
                dis=abs(food[0]-pacmanPosition[0])+abs(food[1]-pacmanPosition[1])
                minFoodDis=min(dis,minFoodDis)
            #找最近的鬼的位置
            minGhostDis=float('inf')
            i=1
            for ghostPosition in ghostPositions:
                if currentGameState.getGhostState(i).scaredTimer>0:#不考虑scared的鬼
                    continue
                i+=1
                dis=abs(ghostPosition[0]-pacmanPosition[0])+abs(ghostPosition[1]-pacmanPosition[1])
                minGhostDis=min(dis,minGhostDis)
            #找最近的capsule的位置
            minCapsuleDis=0
            capsulePositions=currentGameState.getCapsules()
            if capsulePositions!=[]:
                minCapsuleDis=float('inf')
                for capsulePosition in capsulePositions:
                    dis=abs(capsulePosition[0]-pacmanPosition[0])+abs(capsulePosition[1]-pacmanPosition[1])
                    if dis<minCapsuleDis:
                        minCapsuleDis=dis
            return 50*minCapsuleDis+100*minFoodDis-50*minGhostDis#尽量多吃豆子和capsule，同时尽量和鬼保持距离
            util.raiseNotDefined()

        "*** YOUR CODE HERE ***"
        """
        多次重复以下步骤直至达到时间限制：
            Selection,Expansion,Simulation,Backpropagation
        这个时候所有的子结点里有一个的UCB值最大，选这个子结点就可以
        """ 
        
        def bfs():#在较安全时用bfs加快吃豆子速度
                visited=[]
                class queueNode:
                    def __init__(self):
                        self.position = None
                        self.path = [Directions.STOP]
                queue=util.Queue()
                pacmanPosition=gameState.getPacmanPosition()
                (x,y)=(pacmanPosition[0],pacmanPosition[1])
                data=queueNode()
                data.position=(x,y)
                visited.append((x,y))
                queue.push(data)
                dx=[0,0,-1,1]
                dy=[1,-1,0,0]
                walls=gameState.getWalls()
                foods=gameState.getFood()
                while queue.isEmpty()==False:
                    q=queue.pop()
                    (x,y)=q.position
                    if foods[x][y]==True:
                        return q.path[1]
                    for i in range(4):
                        xx=x+dx[i]
                        yy=y+dy[i]
                        if walls[xx][yy]==False and (xx,yy) not in visited:
                            visited.append((xx,yy))
                            nextNode=queueNode()
                            nextNode.position=(xx,yy)
                            if i==0:
                                nextNode.path=q.path+[Directions.NORTH]
                            elif i==1:
                                nextNode.path=q.path+[Directions.SOUTH]
                            elif i==2:
                                nextNode.path=q.path+[Directions.WEST]
                            else:
                                nextNode.path=q.path+[Directions.EAST]
                            queue.push(nextNode)
        #所有鬼都scared时用bfs吃豆子
        flag=1
        i=1
        agentNum=gameState.getNumAgents()
        while i<agentNum:
            # print(i)
            if gameState.data.agentStates[i].scaredTimer<=0:
                flag=0
                break
            i+=1
        if flag==1:
            return bfs()
        ghostPositions=gameState.getGhostPositions()
        pacmanPosition=gameState.getPacmanPosition()
        minGhostDis=float('inf')
        for ghostPosition in ghostPositions:
            dis=abs(ghostPosition[0]-pacmanPosition[0])+abs(ghostPosition[1]-pacmanPosition[1])
            minGhostDis=min(dis,minGhostDis)
        if minGhostDis>=5:
            return bfs()
        
        #有危险时用mcts找出较合适的动作
        for i in range(300):
            agentNum=gameState.getNumAgents()
            foodNum=gameState.getNumFood()
            curNode=Selection(root)
            curNode=Expansion(curNode)
            if curNode!=None:
                reward=Simulation(curNode)
                Backpropagation(curNode,reward)
        maxi=float('-inf')
        nextNode=None
        for child in root.children:
            nextState=gameState.generateSuccessor(0,child.direction)
            pacmanPosition=nextState.getPacmanPosition()
            ghostPositions=nextState.getGhostPositions()
            minDis=float('inf')
            for ghostPosition in ghostPositions:
                dis=abs(ghostPosition[0]-pacmanPosition[0])+abs(ghostPosition[1]-pacmanPosition[1])
                minDis=min(minDis,dis)
            if minDis<=1:#排除可能撞上鬼的动作
                continue
            if child.totalReward/child.count>maxi:#根据效益选出最优动作
                maxi=child.totalReward/child.count
                nextNode=child
        
        if nextNode==None:#没有找到最优动作，这意味着面临着必死局面
            return Directions.STOP
        if nextNode.direction==Directions.STOP and minDis>=5:#pacman可能由于搜索深度不足找不到豆子，并且这时附近没有鬼，可以尝试向靠近食物的方向走
            actions=gameState.getLegalActions(0)
            foodPositions=gameState.getFood()
            foodPositions=foodPositions.asList()
            tmpMinDis=float('inf')
            for action in actions:
                if action==Directions.STOP:
                    continue
                successor=gameState.generateSuccessor(0,action)
                pacmanPosition=successor.getPacmanPosition()
                for foodPosition in foodPositions:
                    dis=abs(foodPosition[0]-pacmanPosition[0])+abs(foodPosition[1]-pacmanPosition[1])
                    if dis<tmpMinDis:
                        tmpMinDis=dis
                        nextAction=action
            nextNode.direction=nextAction
        return nextNode.direction
        util.raiseNotDefined()