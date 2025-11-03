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
episode_step: 12008
episode_step: 188
episode_step: 54
episode_step: 39
episode_step: 51
episode_step: 24
episode_step: 19
episode_step: 18
episode_step: 17
episode_step: 19
episode_step: 14
episode_step: 16
episode_step: 17
episode_step: 24
episode_step: 24
episode_step: 21
episode_step: 16
episode_step: 14
episode_step: 14
episode_step: 17
episode_step: 14
episode_step: 14
episode_step: 14
episode_step: 16
episode_step: 15
DynaQ+ (n_step = 5, k = 1e-5):
>>>>vvv#G
vv#vvvv#^
>v#vvvv#^
>v#>>>>>^
>>>>^#^>^
^^^^^>^^<
```

## 根据Blocking Maze和Shortcut Maze对比Dyna-Q和Dyna-Q+

Dyna-Q+