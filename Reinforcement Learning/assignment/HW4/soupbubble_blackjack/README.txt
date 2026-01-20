SoapBubble
Sutton RL book P95 Example 5.2
要求：
  对于样例代码中default_bubble_generator生成的肥皂泡，使用蒙特卡洛估计的方法，在SoapBubble::inner_heights_mc中计算出泡泡中间各点的高度。
用SoapBubble::print_bubble输出最终结果。
提示：
  1. SoapBubble::inner_heights_dp使用动态规划方法计算得到了中间各点的高度，可作为参考答案。
  2. 参考答案和蒙特卡洛估计得到的结果有一定误差，±0.05之内视为正常。


Blackjack
  Sutton RL book P93 Example 5.1 & P99 Example 5.3
要求：
  使用带蒙特卡洛估计的策略迭代算法，生成对Dealer默认策略（17及以上Stick，否则Hit）的最优对策。
  除main函数部分需做相应修改（使用BlackjackPolicyLearnableDefault::print_policy输出最终策略）外，其余部分已经写好。
  只需完成BlackjackPolicyLearnableDefault中的update_policy和update_value两个函数即可。
提示：
  1. 答案参见书上P100 Figure 5.2 π*
  2. 见blackjack_main.cpp中注释