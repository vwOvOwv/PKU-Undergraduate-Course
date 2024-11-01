### 2021.9.27

cheat <- read.table("cheating.txt", header=T)
cheat

### factor数据类型

cheat$type
cheat$type <- factor(cheat$type, levels=c("本科","硕士","博士"))
cheat$type
as.numeric(cheat$type)

### 日期和金额的散点图
type.col <- c("red", "blue", "black") # 本、硕、博的颜色
type.col[cheat$type]

par(mfrow=c(1,2)) # 划分为一行两列的画图区域
plot(cheat$date, cheat$amount, xlab="日期", ylab="金额", pch=16,
	col=type.col[cheat$type])

plot(cheat$date, cheat$amount, xlab="日期", ylab="金额(log)", 
	log="y", pch=16, col=type.col[cheat$type])

### 加图例：https://www.cnblogs.com/all1008/p/10037314.html
# 网络搜索 “R语言 加图例”
legend("topleft",                     #图例位置为右上角
  legend=c("本科","硕士","博士"),        #图例内容
  col=type.col,                 	 #图例颜色
  pch=16)      				 #图例样式

# write as formula y~x
plot(cheat$amount~cheat$date, xlab="日期", ylab="金额", pch=16)

### 金额和学生类别的关系
plot(cheat$amount~cheat$type, xlab="学生类别", ylab="金额(log)", log="y",
	col=type.col)

# 检验差异是否显著
cheat$amount[cheat$type=="本科"]

t.test(cheat$amount[cheat$type=="本科"], cheat$amount[cheat$type=="硕士"])
