############################ RNA-seq analysis using R
# Differential expression analysis
# http://combine-australia.github.io/RNAseq-R/06-rnaseq-day1.html

# code tested by Cheng Li, 2017.10


###############   load all the packages we will need to analyse the data.

### install R packages from Bioconductor, only install once
# http://combine-australia.github.io/RNAseq-R/00-r-rstudio-intro.html
source("http://bioconductor.org/biocLite.R")
biocLite("edgeR") # also install "limma"
biocLite("Glimma")
biocLite("gplots")
biocLite("org.Mm.eg.db")
biocLite("RColorBrewer")


# load the packages
library(edgeR)
library(limma)
library(Glimma)
library(gplots)
library(org.Mm.eg.db)
library(RColorBrewer)


############## Reading in the count data

# Read the data into R
seqdata <- read.delim("data/GSE60450_Lactation-GenewiseCounts.txt", 
                      stringsAsFactors = FALSE)

# The seqdata object contains information about genes (one gene per row), 
# the first column has the Entrez gene id, the second has the gene length 
# and the remaining columns contain information about the number of reads 
# aligning to the gene in each experimental sample. 
dim(seqdata)
head(seqdata)

# Read the sample information into R
# There are two replicates for each cell type and timepoint
sampleinfo <- read.delim("data/SampleInfo.txt")
sampleinfo


################### Format the data

# We need to make a new matrix containing only the counts, but we can store 
# the gene identifiers (the EntrezGeneID column) as rownames

# Remove first two columns from seqdata
countdata <- seqdata[,-(1:2)]

head(countdata)

# Store EntrezGeneID as rownames
rownames(countdata) <- seqdata[,1]
head(countdata)

colnames(countdata)
#  shorten these to contain only the relevant information about each sample
# using substr, you extract the characters starting at position 1 and 
# stopping at position 7 of the colnames
colnames(countdata) <- substr(colnames(countdata),start=1,stop=7)

head(countdata)

# the column names are now the same as SampleName in the sampleinfo file. 
# check if sample information in sampleinfo is in the same order as 
# the columns in countdata.
colnames(countdata)==sampleinfo$SampleName
table(colnames(countdata)==sampleinfo$SampleName)


################# Filtering to remove lowly expressed genes

# use the cpm function from the edgeR library (M D Robinson, McCarthy, and 
# Smyth 2010) to generate the CPM values and then filter. Note that by 
# converting to CPMs we are normalising for the different sequencing depths 
# for each sample.

library(edgeR)

# Obtain CPMs, counts per million counts
myCPM <- cpm(countdata)

head(myCPM)

# retain genes if they are expressed at a counts-per-million (CPM) above 0.5 
# in at least two samples.
# Which values in myCPM are greater than 0.5?
thresh <- myCPM > 0.5
# This produces a logical matrix with TRUEs and FALSEs
head(thresh)

# Summary of how many TRUEs there are in each row
# There are 11433 genes that have TRUEs in all 12 samples.
table(rowSums(thresh))  # bimodal distribution!

# we would like to keep genes that have at least 2 TRUES in each row of thresh
keep <- rowSums(thresh) >= 2
# Subset the rows of countdata to keep the more highly expressed genes
counts.keep <- countdata[keep,]
summary(keep)
table(keep)

dim(counts.keep)

# A CPM of 0.5 is used as it corresponds to a count of 10-15 for the library 
# sizes in this data set.  If the count is any smaller, it is considered to 
# be very low, indicating that the associated gene is not expressed in that 
# sample.
colSums(countdata) # about 20 million counts / sample
12 / (colSums(countdata) / 1e6) # CPM of 12 in each sample


##################### Convert counts to DGEList object

# create a DGEList object. This is an object used by edgeR to store count data.

y <- DGEList(counts.keep)
?DGEList
y

# See what slots are stored in y
names(y)

# Library size information is stored in the samples slot
y$samples

################# Quality control

# different plots to check that the data is good quality, and that the samples 
# are as we would expect

# Library sizes and distribution plots
# check how many reads we have for each sample in the y.
y$samples$lib.size

# The names argument tells the barplot to use the sample names on the x-axis
# The las argument rotates the axis names
barplot(y$samples$lib.size,names=colnames(y),las=2)
# Add a title to the plot
title("Barplot of library sizes")


# Get log2 counts per million
logcounts <- cpm(y,log=TRUE)
head(logcounts)

# Check distributions of samples using boxplots
boxplot(logcounts, xlab="", ylab="Log2 counts per million",las=2)
# Let's add a blue horizontal line that corresponds to the median logCPM
abline(h=median(logcounts),col="blue")
title("Boxplots of logCPMs (unnormalised)")

# overall the density distributions of raw log-intensities are not 
# identical but still not very different. 

# Question: Do any samples appear to be different compared to the others?


################### Multidimensional scaling (MDS) plots
# Cheng's comment: MDS plot looks similar to PCA, but is not the same

# If your experiment is well controlled and has worked well, what we 
# hope to see is that the greatest sources of variation in the data are 
# the treatments/groups we are interested in. It is also an incredibly 
# useful tool for quality control and checking for outliers
library(limma)
plotMDS(y)
?plotMDS

# colour the samples according to the grouping information. We can also 
# change the labels, or instead of labels we can have points.

# We specify the option to let us plot two plots side-by-sde
par(mfrow=c(1,2))
# Let's set up colour schemes for CellType
# How many cell types and in what order are they stored?
levels(sampleinfo$CellType)
as.numeric(sampleinfo$CellType) # stored values

## Let's choose purple for basal and orange for luminal
col.cell <- c("purple","orange")[sampleinfo$CellType]
col.cell
data.frame(sampleinfo$CellType,col.cell)

# Redo the MDS with cell type colouring
plotMDS(y,col=col.cell)
# Let's add a legend to the plot so we know which colours correspond to which cell type
legend(-4,3.8,fill=c("purple","orange"), y.intersp=0.8,
       legend=levels(sampleinfo$CellType), cex=1, bty="n")
# ?legend
title("Cell type")

# Similarly color for status
levels(sampleinfo$Status)
col.status <- c("blue","red","black")[sampleinfo$Status]
col.status

plotMDS(y,col=col.status)
legend(-4,3.8,fill=c("blue","red","black"),y.intersp=0.8,
       legend=levels(sampleinfo$Status),cex=0.9, bty="n")
title("Status")

# Question: Look at the MDS plot coloured by cell type. Is there 
# something strange going on with the samples? Identify the two 
# samples that don¡¯t appear to be in the right place.


# I'm going to write over the sampleinfo object with the corrected sample info
sampleinfo <- read.delim("data/SampleInfo_Corrected.txt")
sampleinfo

# Redo the above MDSplot with corrected information

# Question: What is the greatest source of variation in the data 
# (i.e. what does dimension 1 represent)? What is the second greatest 
# source of variation in the data?

par(mfrow=c(1,1))

#generate an interactive MDS plot using the Glimma package.
library(Glimma)
labels <- paste(sampleinfo$SampleName, sampleinfo$CellType, sampleinfo$Status)
group <- paste(sampleinfo$CellType,sampleinfo$Status,sep=".")
group <- factor(group)

glMDSPlot(y, labels=labels, groups=group, folder="mds") # no output?


##################### Hierarchical clustering with heatmaps

# using the heatmap.2 function from the gplots package
library(gplots)

# calculate a matrix of euclidean distances from the 
# logCPM (logcounts object) for the 500 most variable genes


# select data for the 500 most variable genes and plot the heatmap

# We estimate the variance for each row in the logcounts matrix
var_genes <- apply(logcounts, 1, var)
head(var_genes)

# Get the gene IDs for the top 500 most variable genes
select_var <- names(sort(var_genes, decreasing=TRUE))[1:500]
head(select_var)

# Subset logcounts matrix
highly_variable_lcpm <- logcounts[select_var,]
dim(highly_variable_lcpm)

head(highly_variable_lcpm)


# The RColorBrewer package has nicer colour schemes, accessed using 
# the brewer.pal function. ¡°RdYlBu¡± is a common choice, and ¡°Spectral¡± 
# is also nice.
library(RColorBrewer)

## Get some nicer colours
mypalette <- brewer.pal(11,"RdYlBu") # red, yellow, blue
#mypalette <- brewer.pal(11,"Spectral")
morecols <- colorRampPalette(mypalette) # generate a function
morecols 

# Set up colour vector for celltype variable
col.cell <- c("purple","orange")[sampleinfo$CellType]

# Plot the heatmap
?heatmap.2

#par(mar=c(0,0,0,0))
heatmap.2(highly_variable_lcpm,col=rev(morecols(50)),trace="none", 
          main="Top 500 most variable genes across samples",
          ColSideColors=col.cell, scale="row" )
# Error in plot.new() : figure margins too large. No color key. 
# How to correct?


################ Normalisation for composition bias
?calcNormFactors

# Apply normalisation to DGEList object
# update the normalisation  factors in the DGEList object (their 
# default values are 1).
# 
y <- calcNormFactors(y)

y$samples

#  If we plot mean difference plots using the plotMD function for 
# these samples, we should be able to see the composition bias problem.
?plotMD

par(mfrow=c(1,2))
par(mar=c(4,3,3,1))
plotMD(logcounts,column = 7)
abline(h=0,col="grey")
plotMD(logcounts,column = 11)
abline(h=0,col="grey")

# our DGEList object contains the normalisation factors, if we redo 
# these plots using y, we should see the composition bias problem 
# has been solved.

par(mfrow=c(1,2))
plotMD(y,column = 7)
abline(h=0,col="grey")
plotMD(y,column = 11)
abline(h=0,col="grey")

# save a few data objects to use later
# save(group,y,logcounts,sampleinfo,file="day1objects.Rdata")


################ Differential expression with limma-voom

### Create the design matrix
group

# want to know which genes are differentially expressed between 
# pregnant and lactating in basal cells only

# Specify a design matrix without an intercept term
design <- model.matrix(~ 0 + group)
design

## Make the column names of the design matrix a bit nicer
colnames(design) <- levels(group)
design
# Each column of the design matrix tells us which samples 
# correspond to each group


### Voom transform the data

# We can add plot=TRUE to generate a plot of the mean-variance trend. 
# This plot can also tell us if there are any genes that look really 
# variable in our data, and if we¡¯ve filtered the low counts adequately.
par(mfrow=c(1,1))
v <- voom(y,design,plot = TRUE)

# The voom normalised log2 counts can be found in v$E. 
names(v)
v

# We can repeat the box plots for the normalised data to compare to 
# before normalisation. The expression values in v$E are already log2 
# values so we don¡¯t need to log-transform.

par(mfrow=c(1,2))
boxplot(logcounts, xlab="", ylab="Log2 counts per million",las=2,main="Unnormalised logCPM")
## Let's add a blue horizontal line that corresponds to the median logCPM
abline(h=median(logcounts),col="blue")

boxplot(v$E, xlab="", ylab="Log2 counts per million",las=2,main="Voom transformed logCPM")
## Let's add a blue horizontal line that corresponds to the median logCPM
abline(h=median(v$E),col="blue")


### Testing for differential expression

#First we fit a linear model for each gene using the lmFit function in 
# limma. lmFit needs the voom object and the design matrix that we have 
# already specified, which is stored within the voom object.

# Fit the linear model
fit <- lmFit(v)
names(fit)

# lmFit estimates group means according to the design matrix, as well 
# as gene-wise variances.


# we need to specify which comparisons we want to test. The comparison 
# of interest can be specified using the makeContrasts function.

# we are interested in knowing which genes are differentially expressed 
# between the pregnant and lactating group in the basal cells. This is 
# done by defining the null hypothesis as basal.pregnant - 
# basal.lactate = 0 for each gene. 

cont.matrix <- makeContrasts(B.PregVsLac=basal.pregnant - basal.lactate,
                             levels=design)

cont.matrix
# The contrast matrix tells limma which columns of the design matrix 
# we are interested in testing our comparison.


# apply the contrasts matrix to the fit object to get the statistics 
# and estimated parameters of our comparison that we are interested in.
fit.cont <- contrasts.fit(fit, cont.matrix)
fit.cont 

# call the eBayes function, which performs empirical Bayes shrinkage 
# on the variances, and estimates moderated t-statistics and the 
# associated p-values.

fit.cont <- eBayes(fit.cont)
fit.cont 

dim(fit.cont)

#  use the limma decideTests function to generate a quick summary 
# of differentially expressed (DE) genes for the contrasts.
summa.fit <- decideTests(fit.cont)
summary(summa.fit)


# The limma topTable function summarises the output in a table format.
topTable(fit.cont,coef="B.PregVsLac",sort.by="p")

# By default the table will be sorted by the B statistic, which is the 
# log-odds of differential expression. We will explicitly rank by p-value

# Significant DE genes for a particular comparison can be identified 
# by selecting genes with a p-value smaller than a chosen cut-off value 
# and/or a fold change greater than a chosen value in this table.


### Adding annotation and saving the results

# We would like to add some annotation information using the org.Mm.eg.db package.
library(org.Mm.eg.db)
?org.Mm.eg.db

columns(org.Mm.eg.db)

# build up our annotation information in a separate data frame 
# using the select function.
ann <- select(org.Mm.eg.db,keys=rownames(fit.cont),
              columns=c("ENTREZID","SYMBOL","GENENAME"))

head(ann)

# double check that the ENTREZID column matches exactly to 
# our fit.cont rownames.
table(ann$ENTREZID==rownames(fit.cont))

# store the annotation information into the genes slot of fit.cont
fit.cont$genes <- ann

# Now when we run the topTable command, the annotation information 
# should be included in the output.
topTable(fit.cont,coef="B.PregVsLac",sort.by="p")

# To get the full table (i.e. the information for all genes, not 
# just the top 10) we can specify n="Inf".
limma.res <- topTable(fit.cont,coef="B.PregVsLac",sort.by="p",n="Inf")

# We can save the results table using the write.csv function, 
# which writes the results out to a csv file, which you can open in excel.
write.csv(limma.res,file="B.PregVsLacResults.csv",row.names=FALSE) # happy advisor!

# if 100 genes are significant at a 5% false discovery rate (FDR), we are 
# willing to accept that 5 will be false positives. Note that the 
# decideTests function displays significant genes at 5% FDR.


### Plots after testing for DE

# Let¡¯s do a few plots to make sure everything looks good and that 
# we haven¡¯t made a mistake in the analysis. Genome-wide plots that 
# are useful for checking are MAplots (or MDplots) and volcano plots. 

# There are functions in limma for plotting these with fit.cont as input.
# We want to highlight the significant genes. We can get this from decideTests.
par(mfrow=c(1,2))
plotMD(fit.cont,coef=1,status=summa.fit[,"B.PregVsLac"])

# For the volcano plot we have to specify how many of the top genes to hightlight.
# We can also specify that we want to plot the gene symbol for the highlighted genes.
# let's highlight the top 100 most DE genes
volcanoplot(fit.cont,coef=1,highlight=100,names=fit.cont$genes$SYMBOL)


#Before following up on the DE genes with further lab work, it is 
# recommended to have a look at the expression levels of the individual 
# samples for the genes of interest. 

par(mfrow=c(1,3))
par(mar=c(6,4,3,1))
# Let's look at the first gene in the topTable, Wif1, which has a 
# rowname 24117
stripchart(v$E["24117",]~group)
# This plot is ugly, let's make it better
stripchart(v$E["24117",]~group,vertical=TRUE,las=2,cex.axis=1,
           pch=16,col=1:6,method="jitter")
# Let's use nicer colours
nice.col <- brewer.pal(6,name="Dark2")
stripchart(v$E["24117",]~group,vertical=TRUE,las=2,cex.axis=1,
           pch=16,cex=1.3,col=nice.col,method="jitter",
           ylab="Normalised log2 expression",main="Wif1")
# Notice anything interesting about the expression of this gene?


######################## Gene Set Testing

# gene set testing, which aims to understand which pathways/gene 
# networks the differentially expressed genes are implicated in.

# Competitive gene set tests, like goana and camera ask the question 
# whether the differentially expressed genes tend to be over-represented 
# in the gene set, compared to all the other genes in the experiment.

# we will perform a gene ontology (GO) enrichment analysis using the 
# goana function in limma. There are approximately 20,000 GO terms, 
# and they are split into three categories: BP (biological process), 
# MF (molecular function) and CC (cellular component).

# goana has an advantage over other methods, such as DAVID, in that 
# there is the option to take into account the gene length bias 
# inherent in RNA-Seq data. (long genes tend to be significant)

# goana takes the fit.cont object, the coefficient of interest and 
#the species. The top set of most enriched GO terms can be viewed 
#with the topGO function.

# biocLite("GO.db") # install once
library(GO.db)

go <- goana(fit.cont, coef="B.PregVsLac",species = "Mm")

# the N column represents the total number of genes that are 
# annotated with each GO term. The Up and Down columns represent 
# the number of differentially expressed genes that overlap with 
# the genes in the GO term
topGO(go, n=10)
?topGO

# An additional refinement is to supply goana with the gene lengths 
# using the covariate argument. 

# In the original data matrix that we loaded into R, there is a column 
# called ¡°Length¡±.
colnames(seqdata)

# In order to get the gene lengths for every gene in fit.cont, 
# we can use the match command. Note that the gene length supplied 
# needs to be in the correct order.
m <- match(rownames(fit.cont), seqdata$EntrezGeneID)
gene_length <- seqdata$Length[m]
head(gene_length)

# Rerun goana with gene length information
# biocLite("BiasedUrn") # install once
library(BiasedUrn)
go_length <- goana(fit.cont,coef="B.PregVsLac",species="Mm",
                   covariate=gene_length)
topGO(go_length, n=10)

# KEGG pathway enrichment
k <- kegga(fit.cont, coef="B.PregVsLac",species = "Mm")
topKEGG(k, n=10)
