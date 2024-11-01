### https://www.stat.berkeley.edu/~s133/factors.html

mons = c("March","April","January","November","January",
 "September","October","September","November","August",
 "January","November","November","February","May","August",
 "July","December","August","August","September","November",
 "February","April")

mons

as.numeric(mons) # 错误

mons = factor(mons)
mons

as.numeric(mons) # 数字是Levels中的序号

levels(mons)

table(mons)

plot(mons)


#### pie chart
pie(c(Sky = 78, "Sunny side of pyramid" = 17, "Shady side of pyramid" = 5), 
	init.angle = 315, col = c("deepskyblue", "yellow", "yellow3"), 
	border = FALSE)
 

#### testing for association
x <- matrix(c(8,1264,88,11279), nrow=2, byrow=T)

chisq.test(x)