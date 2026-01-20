# HW6 实验报告

通过修改已提供的Q-Learning框架得到Dyna-Q和Dyna-Q+，在规划阶段仍然使用算法学习到的环境模型。

## 对比Dyna-Q和Dyna-Q+在Maze环境中的表现

在相同的参数（`iter = 50, verbose_freq = 2`）下运行了Q-Learning，Dyna-Q（`planning_step = 5`）和Dyna-Q+（`planning_step = 5, k = 1e-5`），结果如下

### Q-Learning

```txt
episode_step: 16099
episode_step: 230
episode_step: 254
episode_step: 170
episode_step: 170
episode_step: 146
episode_step: 295
episode_step: 52
episode_step: 281
episode_step: 363
episode_step: 117
episode_step: 57
episode_step: 149
episode_step: 150
episode_step: 87
episode_step: 181
episode_step: 120
episode_step: 21
episode_step: 119
episode_step: 76
episode_step: 23
episode_step: 35
episode_step: 35
episode_step: 39
episode_step: 68
Q-Learning:
>>>vv^^#G
<>#>v<^#^
^^#>>>v#^
^>#>>>>>^
>^>>^#>^^
vvv<v^<^<
```

可以看到，Q-Learning在500个episode后仍未收敛。

### Dyna-Q

```txt
episode_step: 16773
episode_step: 25
episode_step: 63
episode_step: 100
episode_step: 21
episode_step: 30
episode_step: 25
episode_step: 40
episode_step: 39
episode_step: 23
episode_step: 21
episode_step: 16
episode_step: 15
episode_step: 16
episode_step: 15
episode_step: 17
episode_step: 14
episode_step: 18
episode_step: 14
episode_step: 20
episode_step: 16
episode_step: 15
episode_step: 14
episode_step: 14
episode_step: 18
DynaQ: (n_step = 5)
>>>>vvv#G
vv#vvvv#^
vv#v>vv#^
vv#>>>>>^
>>>^^#^^^
^^^^^>^^^
```

### Dyna-Q+
```txt
episode_step: 9478
episode_step: 63
episode_step: 62
episode_step: 33
episode_step: 56
episode_step: 67
episode_step: 43
episode_step: 21
episode_step: 26
episode_step: 21
episode_step: 16
episode_step: 20
episode_step: 17
episode_step: 22
episode_step: 16
episode_step: 18
episode_step: 14
episode_step: 16
episode_step: 14
episode_step: 14
episode_step: 16
episode_step: 14
episode_step: 20
episode_step: 14
episode_step: 18
DynaQ+ (n_step = 5, k = 1e-5):
v>>vv<v#G
vv#>v>v#^
>v#vvv<#^
>v#>>>>>^
>>>>^#^^^
>^^^^>^^^
```

可以看到，Dyna-Q和Dyna-Q+均能在50个episode内收敛，而Dyna-Q+在早期的收敛速度更快一些，因为Dyna-Q+通过对未被访问的状态-动作对增加奖励鼓励探索，从而可以更快地发现更优的路径。

## 根据Blocking Maze和Shortcut Maze对比Dyna-Q和Dyna-Q+

在Blocking Maze中，在环境未发生变化前，Dyna-Q+因为鼓励探索更快地找到了更优的路径，因此cumulative reward升高得更快；在环境发生变化后，因为原先的最优路径不复存在，因此两种算法均可以发现这种变化并调整策略。Dyna-Q+能够更快地发现环境的变化并调整策略，因此cumulative reward也更快地恢复增加；而Dyna-Q在环境变化后需要更多的时间来重新学习最优策略。

在Shortcut Maze中，在环境未发生变化前，两种算法差距不大，因为Dyna-Q+只是在早期收敛速度更快一些，收敛后两者表现相似甚至Dyna-Q的cumulative reward上升得更加稳定一点，因为Dyna-Q+在收敛后仍然鼓励探索，可能会导致一些不必要的探索行为。在环境发生变化后，Dyna-Q+因为探索能够发现环境的变化并调整策略，而Dyna-Q在这种情况下无法发现环境的变化，因为原先的最优路径仍然存在，因此环境变化后Dyna-Q+的cumulative reward增加得更快，而Dyna-Q的cumulative reward仍然以原来的速度增加。