# Part 2. 

### 目录

*   [介绍](#介绍)
*   [欢迎](#欢迎)
*   [Q1: Minimax](#Q1)
*   [Q2: Alpha-Beta Pruning](#Q2)
*   [Q3: MCTS](#Q3)

### 介绍

在这个项目中，你将为经典版本的吃豆人设计Agent。在此过程中，你将实现minimax搜索，alpha-beta以及蒙特卡洛树搜索。

代码库与前一个项目相比变化不大，但请从全新的安装开始，而不是将项目1的文件混在一起。

与项目1一样，本项目包括一个自动评分器，供你在机器上对答案进行评分。

用以下指令可以对所有题目进行评测。

`python autograder.py`

你也可以通过```-q```参数指定对题目q2进行评测：

`python autograder.py -q q2`

你也可以通过```-t```参数指定使用的测试用例：

`python autograder.py -t test_cases/q2/0-small-tree`

你可以通过使用`--graphics`标志强制显示图形界面，或者通过使用`--no-graphics`标志强制不显示图形界面。

这个项目的代码包含以下文件:

**你需要修改的文件:**

[`multiAgents.py`](multiAgents.py)  你将在这里实现三种算法。

**你需要阅读的文件:**

[`pacman.py`](pacman.py)   运行Pacman游戏的主要文件。这个文件还描述了一个Pacman `GameState`类型。

[`game.py`](game.py)   吃豆人世界运作的逻辑。这个文件描述了几个支持类型，如AgentState、Agent、Direction和Grid。

[`util.py`](util.py)   实现算法时的可能会用到的数据结构。

**你可以忽略的文件:**

[`graphicsDisplay.py`](graphicsDisplay.py)   Pacman图像显示界面

[`graphicsUtils.py`](graphicsUtils.py)   支持图像

[`textDisplay.py`](textDisplay.py)   Pacman的ASCII编码

[`ghostAgents.py`](ghostAgents.py)   Ghosts程序

[`keyboardAgents.py`](keyboardAgents.py)   键盘接口

[`layout.py`](layout.py)   导入地图的程序

[`autograder.py`](autograder.py)   自动评分器

[`testParser.py`](testParser.py)   解析测试用例

[`testClasses.py`](testClasses.py)   通用

[`test_cases/`](test_cases)   测试用例文件夹

[`multiagentTestClasses.py`](multiagentTestClasses.html)   Project 2 specific autograding test classes

**需要修改并提交的文件:** 你需要在作业中填写[`multiAgents.py`](multiAgents.py)的部分内容，并且最终**只需要**提交这个文件，并附上你的代码和注释。请不要改变这个文件夹中的其他文件，或提交这个文件以外的任何文件。

**评估:** 我们使用```autograder.py```对你的提交进行评分，但包含的测试用例与本地给出的样例文件有所不同。你可以在本地运行评分器对你的代码进行评测，但仅限于帮助调试代码，该分数不等于最终分数。

**学术诚信:** 我们会将你的代码与课堂上其他提交的代码进行逻辑查重。如果你拷贝了别人的代码，并做一些微小的修改，我们会很容易发现，请不要尝试。我们相信你们会独立完成作业。


* * *

### <a name="欢迎"></a> Multi-Agent Pacman

首先，玩一个经典的吃豆子游戏:

`python pacman.py`

现在，查看`multiAgents.py`中提供的`ReflexAgent`: 他为我们提供了一些必要的方法以查看环境当前的状态。

`python pacman.py -p ReflexAgent`

`python pacman.py -p ReflexAgent -l testClassic`

检查它的代码（在`multiAgents.py`中），确保你明白它在做什么。

* * *

### <a name="Q1"></a> Question 1 (4 points): Minimax

现在你将在`multiAgents.py`中提供的`MinimaxAgent`类中使用minimax搜索算法，编写一个与幽灵👻进行对抗的智能体（幽灵的数量被设置为1），其中搜索树应该可以扩展到任意深度。你可以使用我们提供的`self.evaluationFunction`对搜索树的叶子进行评分，默认为`scoreEvaluationFunction`。`MinimaxAgent`扩展了`MultiAgentSearchAgent`，它可以访问`self.depth`和`self.evaluationFunction`。确保你编写的minimax代码在适当的地方引用这两个变量，这些变量是根据命令行选项来指定的。

_Important:_ 一个搜索层被认为是一个吃豆人的动作和幽灵的反应共同确定的，所以深度2搜索将涉及吃豆人和幽灵分别移动两次。

_Grading_: 我们将检查你的代码以确定它是否探索了正确的游戏状态数量。这是检测学生正确实现minimax算法的唯一可靠方法。因此，autograder会对你的代码调用`GameState.generateSuccessor`的次数非常敏感。如果你调用的次数多或少，autograder都会报错。为了测试和调试你的代码，请运行

`python autograder.py -q q1`

这将显示你的算法在一些小规模测试用例上的表现。想要在没有图形界面的情况下运行它，请使用:

`python autograder.py -q q1 --no-graphics`

_**提示**_

* 正确实现minimax有可能导致吃豆人在某些测试中输掉比赛。不必担心，因为这是期望中的结果，它不会影响最终评分。
* 这一部分中吃豆人测试的评估函数已经写好了（`self.evaluationFunction`）。请不要改变这个函数，但是要认识到，现在我们是在评估*states*，而不是行动。
* 在 "minimaxClassic "布局中，初始状态的最小值分别是9、8、7、-492，深度为1、2、3和4。请注意，尽管深度4的minimax预测很难，但你的minimax智能体往往会赢（对我们来说是665/1000局）。
    
    `python pacman.py -p MinimaxAgent -l minimaxClassic -a depth=4`
    
* 吃豆人始终是agent 0。
* minimax中的所有状态都应该是`GameStates`类型的，要么被传递给`getAction`，要么由`gamestate.generateSuccessor`方法生成。
* 在`openClassic`和`mediumClassic`（默认）等较大的棋盘上，你会发现吃豆人擅长生存，但不擅长取胜。他经常会白费力气，毫无进展。它甚至会在一个点旁边乱蹦乱跳，却不吃那个点，因为它不知道吃了那个点之后会去哪里。如果你看到这种行为，不必担心。
* 当吃豆人认为他的死亡是不可避免的，他会试图尽快结束游戏，避免持续的生存惩罚。有时候，这对基于随机策略的幽灵来说是错误的，但minimax算法总是假设最坏的情况:
    
    `python pacman.py -p MinimaxAgent -l trappedClassic -a depth=3`
    
    单纯为了确保你明白为什么吃豆人在这种情况下会冲到最近的Ghost。

* * *

### <a name="Q2"></a> Question 2 (4 points): Alpha-Beta Pruning

在`AlphaBetaAgent`中创建一个新的智能体，使用alpha-beta修剪来更有效地探索Minimax树。你应该会发现这个算法比minimax搜索更快（也许深度3 alpha-beta将与深度2 minimax运行得一样快）。理想情况下，`smallClassic`上的深度3应该在每次移动中耗时几秒钟或更少。

`python pacman.py -p AlphaBetaAgent -a depth=3 -l smallClassic`

`AlphaBetaAgent`的minimax值应该与`MinimaxAgent`的minimax值相同。同样，在minimaxClassic布局中，初始状态的minimmax值分别为9,8,7和-492，深度分别为1,2,3和4。

_Grading_: 因为我们需要检查你的代码以确定它是否探索了正确数量的状态，所以执行alpha-beta修剪时，务必不要改变子子节点的访问顺序。换句话说，后续状态应该总是按照`GameState.getLegalActions`返回的顺序进行处理。同样，不要调用`GameState.generatessuccer`超过必要次数。


为了测试和调试你的代码，运行

`python autograder.py -q q2`

这将显示你的算法在一些小规模测试用例上的表现，以及一个吃豆人游戏。要在没有图形的情况下运行它，请使用

`python autograder.py -q q2 --no-graphics`

正确实现的alpha-beta剪枝算法有可能导致吃豆人在一些测试下失败，不必担心。

* * *

### <a name="Q3"></a> Question 3 (4 points): MCTS (Monte Carlo Tree Search)

Minimax和alpha-beta都很好，但它们都假设你是在与一个做出最优决定的对手比赛。任何赢过井字棋的人都可以告诉你，情况并不总是如此。在这个问题中，你将实现`MCTSAgent`。

在`MCTSAgent`中创建一个新的基于蒙特卡洛树搜索智能体，并完成`selection`, `expansion`等函数。在完成相应的函数后，请使用自动评分器进行验证。我们在其中预留了`Node`类，它是你在实现蒙特卡洛树中需要使用的节点数据结构。

提示：在MCTS实现中，rollout过程请尽量使用在lab1-part1中实现的启发式函数，否则在最终的测试环境，可能会出现意外失败的情况，并影响你最终的分数。

`python autograder.py -q q3`

要查看MCTSAgent在Pacman中的表现，请运行:

`python pacman.py -p MCTSAgent -l testClassic`

MCTS的正确实现同样可能导致Pacman在一些测试场景下失败。这不是一个问题：因为它是正确的行为，它将通过我们最终的测试。


* * *

### 最后

请注意，我们在一些测试样例中设置了maxTime参数，以避免程序陷入无意义的循环。在最终的测试样例中，我们将其设置为10min，请检查你的程序用时。
我们的程序最终运行于服务器集群，通常比笔记本环境拥有更好的运行效率，因此不必担心。

### Submission

你只需要提交`multiAgents.py`以及包含学号信息的yaml文件，以方便我们进行自动评分。
