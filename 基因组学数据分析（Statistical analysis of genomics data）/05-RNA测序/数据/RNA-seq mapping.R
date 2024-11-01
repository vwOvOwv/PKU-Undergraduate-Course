### RNA-seq analysis in R
# code tested by Qiang Shi and Cheng Li, 2017.10

# Alignment and feature counting
# http://combine-australia.github.io/RNAseq-R/07-rnaseq-day2.html

setwd("/lustre/user/liclab/liocean/lic/rnaseq_demo")

### install package 

# Install Rsubread package (Subread sequence alignment for R)
# http://bioconductor.org/packages/release/bioc/html/Rsubread.html
# source("https://bioconductor.org/biocLite.R")
# biocLite("Rsubread")


########################### data import ##########################
library(Rsubread)

fastq.files <- list.files(path = "./data", pattern = ".fastq.gz$", full.names = TRUE)
fastq.files


############################ Alignment ###########################
#### Build the index
# See above paragraph: "we have provided the index files for you". You do not need to run command below.
# buildindex(basename="chr1_mm10",reference="chr1.fa")

##### Aligning reads to chromosome 1 of reference genome
align(index="data/chr1_mm10", readfile1=fastq.files)

args(align)

?align

### a summary of the proportion of reads that mapped to the reference genome

bam.files <- list.files(path = "./data", pattern = ".BAM$", full.names = TRUE)
bam.files

props <- propmapped(files=bam.files)
props


### Try aligning the fastq files allowing multi-mapping reads 
# (set unique = FALSE), and allowing for up to 6 ¡°best¡± locations 
# to be reported (nBestLocations = 6).

align(index="data/chr1_mm10", readfile1=fastq.files, unique=F,
      nBestLocations = 6, output_format="SAM",
      output_file=paste(as.character(fastq.files),"multi.sam", sep="."))

sam.files <- list.files(path = "./data", pattern = ".multi.sam$", full.names = TRUE)
sam.files

### click to open a SAM file

# file.remove(sam.files)  # remove non-needed files

props <- propmapped(files=sam.files)
props

####################### Quality control ######################
# Extract quality scores
qs <- qualityScores(filename="data/SRR1552450.fastq.gz",nreads=1000)

?qualityScores

# Check dimension of qs
dim(qs)
class(qs) # check data type

# Check first few elements of qs with head
head(qs)
# A quality score of 30 corresponds to a 1 in 1000 chance of 
# an incorrect base call. (A quality score of 10 is a 1 in 10 
# chance of an incorrect base call.) 

# distributin of quality score at each base position
boxplot(qs, pch='.') 

image(qs)
image(qs, col=rainbow(16))
example(rainbow)

########################## Counting reads for genes#########################
fc <- featureCounts(bam.files, annot.inbuilt="mm10")

# See what slots are stored in fc
names(fc)

# The statistics of the read mapping can be seen with fc$stats.
# We know the real reason why the majority of the reads aren¡¯t mapping 
# - they¡¯re not from chr 1
fc$stat


# The counts for the samples are stored in fc$counts
# Take a look at the dimensions to see the number of genes
dim(fc$counts)

# Take a look at the first 6 lines
head(fc$counts)

# The annotation slot shows the annotation information that 
# featureCounts used to summarise reads over genes. 
head(fc$annotation)

# Package versions used
sessionInfo()

