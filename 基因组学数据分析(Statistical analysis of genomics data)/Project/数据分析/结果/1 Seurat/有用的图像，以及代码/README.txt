这些图是这么用的：
tsne linear和negbinom分别是两种scale作出的原始图像annotation是在negbinom基础上按原文res.2进行染色的结果。
tsne用metadata的图也有
1B 1C 1D是对应的图像。
根据1B情况发现52类应该存在子类。
对52类子类进行再聚类，并发现了新的聚类
用它将原52类拆分为52a 52b 52c三类
1C 1D能发现原文中与目前所作类的对应关系。纯度表格在Rmarkdown的最后一块有。
可以指出目前肉眼可见的一些差别，并且指出可以通过特定子类再聚类的手段进行进一步拆分。
csv是clustermarkers文档，negbinom的。linear的之前用的intersect，有问题。有需要也可以用。
