 
 任务描述：
  RLBook P8 TicTacToe游戏 利用P10页公式的状态值估计，生成针对固定对手TicTacToePolicyDefault（下在当前棋盘的第一个空位）的最优对策。
  我方为X（先手），对手为O（后手）。 
  训练结束后，设置verbose为true，输出与默认策略对打一局的流程。
 g++ version 8.1.0及以上可编译。
 关于样例代码：
  TicTacToe类为游戏环境类，其中提供表示动作的Action类，表示状态的State类。
  TicTacToe::step(action)为落子操作，游戏状态发生转移。TicTacToe::step_back()撤销操作，恢复上一步状态。 
  TicTacToe::State::action_space()获取当前状态的动作空间。
  其余接口见tictactoe.hpp代码及注释。
  main中实现了双方随机落子的算法，并打印游戏每步状态和动作信息。TicTacToePolicyDefault实现了默认策略，建议修改其中state.turn == TicTacToe::PLAYER_X判断分支来实现己方策略，另一个判断分支保持默认策略不变。（可在该类中维护状态估值表，利用此表确定己方策略。）
