# 数据导入和清洗
data <- read.csv('E:/PKU/课件 大二下/生信实验/TCGA-BRCA.methylation27.tsv',sep='\t')
clean <- na.omit(data)

install.packages('pheatmap')
library(dplyr)
library(ggplot2)
library(pheatmap)
library(tidyr)

# 计算均值
with_average <- clean %>% 
  mutate(row_mean = rowMeans(across(where(is.numeric))))

all <- with_average[, c("CompositeElementREF","row_mean")]
colnames(all) <- c('X.id','methylation_level')
all_annotated <- inner_join(all, map)

# 筛选甲基化程度较高的标签
high_meth <- with_average %>% filter(row_mean > 0.95) %>% 
  arrange(desc(row_mean))

result <- high_meth[, c("CompositeElementREF","row_mean")]
colnames(result) <- c('X.id','methylation_level')

write.csv(result, 'E:/PKU/课件 大二下/生信实验/high_meth.csv')

# 筛选甲基化程度较低的标签
low_meth <- with_average %>% filter(row_mean < 0.05) %>% 
  arrange(row_mean)

result2 <- low_meth[, c("CompositeElementREF","row_mean")]
colnames(result2) <- c('X.id','methylation_level')

# 进行基因/染色体位置注释
map <- read.csv('E:/PKU/课件 大二下/生信实验/HM27.hg38.manifest.gencode.v36.probeMap', sep='\t')

high_annotated <- inner_join(result, map)

low_annotated <- inner_join(result2, map)

high_annotated$chrom <- factor(high_annotated$chrom, 
                               levels = c(paste0("chr", 1:22), "chrX"))

# 对高表达量作频率分布图
ggplot(high_annotated, aes(x = chrom)) + 
  geom_bar(position = "dodge") +
  labs(title = "High Methylation Distribution by Chromosome")

low_annotated$chrom <- factor(low_annotated$chrom, 
                               levels = c(paste0("chr", 1:22), "chrX"))

#对低表达量作频率分布图
ggplot(low_annotated, aes(x = chrom)) + 
  geom_bar(position = "dodge") +
  labs(title = "Low Methylation Distribution by Chromosome")

#作甲基化程度分布图
ggplot(all, aes(x = methylation_level)) +
  geom_histogram(binwidth = 0.05, fill = "blue", color = "black", alpha = 0.7) +  # 设置条形图的宽度
  labs(title = "Methylation Level Distribution", 
       x = "Methylation Level", 
       y = "Frequency") +
  theme_minimal()

all_annotated$chrom <- factor(all_annotated$chrom, 
                              levels = c(paste0("chr", 1:22), "chrX"))

#甲基化标签的染色体分布图
ggplot(all_annotated, aes(x = chrom)) + 
  geom_bar(position = "dodge") +
  labs(title = "Methylation Tags Distribution by Chromosome")

# 绘制点阵热图
ggplot(all_annotated, aes(x = chromStart, y = chrom, color = methylation_level)) +
  geom_point(alpha = 0.7, size = 2) +  # 使用点而非矩形，设置透明度和点的大小
  labs(title = "Methylation Level Dot Heatmap by Chromosome",
       x = "Position",
       y = "Chromosome") +
  scale_color_gradient(low = "blue", high = "red") +  # 颜色渐变
  theme_minimal() +
  theme(axis.text.y = element_text(angle = 0, hjust = 1))
