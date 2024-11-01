### Chapter 3 on regression, ISLR

library(ISLR) # install the R package beforehand

#data(package = "ISLR") # check datasets

# Use File menu to set the working directory that contains "Advertising.csv"
# or use setwd(), example:
# setwd("C:\\LCH\\Statistics\\Lectures\\Regression") # set working directory


Ad <- read.csv("Advertising.csv") # read data
head(Ad)
dim(Ad)

attach(Ad)  # so we can use column names directly

par(mfrow=c(1,1)) # set 1 by 1 panel

par(pch=16) # set parameter for plotting solid dots
par(mar=c(4,4,1,1)) # set margins

plot(TV, Sales)

par(mfrow=c(1,3)) # set 1 by 3 panels

plot(TV, Sales)
plot(Radio, Sales)
plot(Newspaper, Sales)

### add regression lines, Figure 2.1
par(mfrow=c(1,3)) # set 1 by 3 panels

plot(TV, Sales)
abline(lm(Sales~TV), col="blue", lwd=3)

plot(Radio, Sales)
abline(lm(Sales~Radio), col="blue", lwd=3)

plot(Newspaper, Sales)
abline(lm(Sales~Newspaper), col="blue", lwd=3)


### linear regression, Figure 3.1
par(mfrow=c(1,1)) 
plot(TV, Sales)
abline(lm(Sales~TV), col="blue", lwd=3)


sales.lm <- lm(Sales~TV)
sales.lm
summary(sales.lm)

?lm # check returned values
y.hat <- fitted(sales.lm)
segments(TV, Sales, TV, y.hat, col="gray40")

### model accuracy, (3.15)
RSE <- sqrt(sum(residuals(sales.lm)^2) / 198)
RSE
RSE / mean(Sales)  # percentage error

### R square, the proportion of variance explained

RSS <- sum(residuals(sales.lm)^2)
TSS <- sum( (Sales-mean(Sales))^2)
1 - RSS/TSS  # R square, (3.17)

cor(TV, Sales)^2

# residual plot
plot(TV, residuals(sales.lm))
abline(h=0)


### Multiple linear regression
# correlation between variables
par(mfrow=c(1,3)) # set 1 by 3 panels

plot(TV, Radio)
plot(TV, Newspaper)
plot(Radio, Newspaper)

# multiple regression
sales.lm2 <- lm(Sales ~ TV + Radio + Newspaper)
sales.lm2
summary(sales.lm2)

# R square
cor(fitted(sales.lm2), Sales)^2

RSS2 <- sum(residuals(sales.lm2)^2)
RSE2 <- sqrt(RSS2/196)
RSE2
RSE2 / mean(Sales)

# residual plot
par(mfrow=c(1,3)) # set 1 by 3 panels

plot(TV, residuals(sales.lm2))
abline(h=0)

plot(Radio, residuals(sales.lm2))
abline(h=0)

plot(Newspaper, residuals(sales.lm2))
abline(h=0)


######### R graphs

x <- seq(0, 1, .05)
plot(x, x, ylab="y", type="l")

for (i in 2:8)
    lines(x, x^i, col=i, lwd=i)

example(plot)
example(boxplot)
example(hist)
example(persp)


