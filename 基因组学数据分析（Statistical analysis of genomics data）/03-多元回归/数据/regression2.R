### Chapter 3 on regression, ISLR

#library(ISLR)

#data(package = "ISLR") # check datasets

# Use File menu to set the working directory that contains "Advertising.csv"
# or use setwd(), example:
# setwd("C:\\LCH\\Statistics\\Lectures\\Regression") # set working directory


Ad <- read.csv("Advertising.csv") # read data from working directory
head(Ad)
dim(Ad)

pairs(Ad[,2:5], pch=16, col=rgb(0,0,1,0.4)) # pairwise scatterplot


### make scatterplots
par(pch=16) # set parameter for plotting solid dots
par(mar=c(4,4,1,1)) # set margins

attach(Ad)  # so we can use column names directly

plot(TV, Sales)

### add regression lines
plot(TV, Sales)
abline(lm(Sales~TV), col="blue", lwd=3)


sales.lm <- lm(Sales~TV)
sales.lm
summary(sales.lm)

?lm # check returned values
y.hat <- fitted(sales.lm)
segments(TV, Sales, TV, y.hat, col="gray40", lwd=1.2)  # Figure 3.1

### check model accuracy, (3.15)
RSE <- sqrt(sum(residuals(sales.lm)^2) / 198)
RSE
RSE / mean(Sales)  # percentage error

### R square, the proportion of variance explained

RSS <- sum(residuals(sales.lm)^2)
TSS <- sum( (Sales-mean(Sales))^2)
1 - RSS/TSS  # R square, (3.17)

cor(TV, Sales)^2

# residual plot, check wether variance is constant (Figure 3.11)
plot(TV, residuals(sales.lm))
abline(h=0, lwd=2, col="blue")


##### Multiple linear regression

# multiple regression
sales.lm2 <- lm(Sales ~ TV + Radio + Newspaper)
sales.lm2
summary(sales.lm2)  # Table 3.4

# R square
cor(fitted(sales.lm2), Sales)^2

RSS2 <- sum(residuals(sales.lm2)^2)
RSE2 <- sqrt(RSS2/196)
RSE2
RSE2 / mean(Sales)

# residual plot
par(mfrow=c(1,3)) # set 1 by 3 panels

plot(TV, residuals(sales.lm2))
abline(h=0, lwd=2, col="blue")

plot(Radio, residuals(sales.lm2))
abline(h=0, lwd=2, col="blue")

plot(Newspaper, residuals(sales.lm2))
abline(h=0, lwd=2, col="blue")
par(mfrow=c(1,1))


### Interactive spinning 3D Scatterplot
# https://cran.r-project.org/web/packages/rgl/index.html

#install.packages("rgl") 
# ‘rgl’是用R版本3.4.4 来建造的, 有可能需要升级R软件
# install a new package from online
# install only once, will install many other packages
# or use menu "Packages/Install package from local zip files" (rgl_0.93.986.zip).

library(rgl)  # use an installed package

attach(mtcars)
plot3d(wt, disp, mpg, col="red", size=6) 
# displacement: 排气量

example(plot3d)  # color as additional dimension (X axis)
?plot3d  # example() runs the example code in this help page
# good for visualizing single-cell data?

example(surface3d)
?surface3d

example(persp3d)
?persp3d


### Fit Sales ~ TV + Radio
sales.lm3 <- lm(Sales ~ TV + Radio)
summary(sales.lm3)
sale.beta <- coef(sales.lm3)  # fitted beta values

plot(TV, Sales)
plot(Radio, Sales)

plot3d(TV, Radio, Sales, size=10)

plot3d(TV, Radio, fitted(sales.lm3), add=T, col="red", size=8)
# add predicted values

## add the predicted surface
sale.x <- 1:max(TV)  # coordinates of X axis
sale.y <- 1:max(Radio) # coordinates of Y axis

sales.pred <- function(tv, radio) {  
    # a function predict sales from tv and radio values
    #return(as.numeric(c(1, tv, radio) %*% sale.beta ) )
    as.numeric(sale.beta[1] + sale.beta[2]*tv + sale.beta[3]*radio)
}
sales.pred(100, 20)

outer(1:9, 1:9, FUN="*")

sale.z <- outer(sale.x, sale.y, FUN=sales.pred)

plot3d(TV, Radio, Sales, size=10)
surface3d(sale.x, sale.y, sale.z, col="red", add=T) # Figure 3.5


### Fit Sales ~ TV + Radio + TV * Radio
sales.lm4 <- lm(Sales ~ TV + Radio + TV:Radio)
summary(sales.lm4)
sale.beta4 <- coef(sales.lm4)  # fitted beta values

plot3d(TV, Radio, Sales, size=10)
plot3d(TV, Radio, fitted(sales.lm4), add=T, col="red", size=8)

sales.pred4 <- function(tv, radio) {  
    # a function predict sales from tv and radio values
    as.numeric(sale.beta4[1] + sale.beta4[2]*tv + sale.beta4[3]*radio
	+ sale.beta4[4]*tv*radio)
}
sales.pred4(100, 20)

sale.z4 <- outer(sale.x, sale.y, FUN=sales.pred4)

plot3d(TV, Radio, Sales, size=10)
surface3d(sale.x, sale.y, sale.z4, col="red", add=T) 



########### using colors

### use semi-transparent colors
x <- rnorm(10000) # generate random normal values
y <- rnorm(10000)

plot(x, y, pch=16) # solid points

plot(x, y, pch=16, col=rgb(0,0,0, 0.05)) # transparent points


### the "iris" dataset
?iris
iris[1:10,]  # show the first 10 rows
head(iris)
str(iris) # data structure

attach(iris) # access variables of the "iris" data frame

# pairwise scatterplot of all variables
pairs(iris, pch=16, col=rgb(0,0,0,.2))

# plot 3 species in different color: red, blue and green
my.col = c(rgb(1,0,0,.5), rgb(0,0,1,.5), rgb(0,1,0,.5))

levels(Species)  # Species is a factor variable type
as.numeric(Species) # convert it to numeric

my.col[as.numeric(Species)] # generate colors for each data point
iris.col <- my.col[as.numeric(Species)]

pairs(iris[,1:4], pch=16, col=iris.col)




