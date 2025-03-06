library(dplyr)
library(Seurat)
library(patchwork)
library(viridis)

setwd('。。。。')

brain.data <- Read10X(data.dir = "./GSE107451_DGRP-551_w1118_WholeBrain_57k_0d_1d_3d_6d_9d_15d_30d_50d_10X_DGEM_MEX.mtx.tsv/")

metadata <- read.delim("GSE107451_DGRP-551_w1118_WholeBrain_57k_Metadata.tsv", row.names = 1)

brain <- CreateSeuratObject(counts = brain.data, meta.data = metadata)

# 去除平均表达不到0.001的基因
brain_a <- subset(brain, subset = Genotype == "DGRP-551")
brain_b <- subset(brain, subset = Genotype == "w1118")

# 分别计算品系 a 的平均表达量，筛选基因
avg_expression_a <- Matrix::rowMeans(GetAssayData(brain_a, slot = "counts"))
names(avg_expression_a) <- rownames(GetAssayData(brain_a, slot = "counts"))
selected_genes_a <- names(avg_expression_a[avg_expression_a > 0.001])

# 分别计算品系 b 的平均表达量，筛选基因
avg_expression_b <- Matrix::rowMeans(GetAssayData(brain_b, slot = "counts"))
names(avg_expression_b) <- rownames(GetAssayData(brain_b, slot = "counts"))
selected_genes_b <- names(avg_expression_b[avg_expression_b > 0.001])

common_genes <- intersect(selected_genes_a, selected_genes_b)
brain_filtered <- subset(brain, features = selected_genes)

# p = ggplot(brain@meta.data, aes(x = seurat_tsne1, y = seurat_tsne2, color = annotation)) +
#   geom_point(size = 0.1) +
#   theme_minimal() +
#   labs(title = "t-SNE plot", x = "t-SNE 1", y = "t-SNE 2") +
#   scale_color_viridis(discrete = TRUE) + # 自动为每个类别分配颜色
#   theme(legend.position = "none")
# 
# ggsave("suibian.png",plot=p)

# p = ggplot(brain@meta.data, aes(x = scenic_tsne1, y = scenic_tsne2, color = annotation)) +
#   geom_point(size = 0.1) +
#   theme_minimal() +
#   labs(title = "t-SNE plot", x = "t-SNE 1", y = "t-SNE 2") +
#   scale_color_viridis(discrete = TRUE) + # 自动为每个类别分配颜色
#   theme(legend.position = "none")
# 
# ggsave("suibian.png",plot=p)


# Visualize QC metrics as a violin plot
VlnPlot(brain, features = c("nGene", "nUMI", "percent.mito"), ncol = 3)


# FeatureScatter is typically used to visualize feature-feature relationships, but can be used
# for anything calculated by the object, i.e. columns in object metadata, PC scores etc.
# plot1 <- FeatureScatter(brain, feature1 = "nCount_RNA", feature2 = "percent.mt")
# plot2 <- FeatureScatter(brain, feature1 = "nCount_RNA", feature2 = "nFeature_RNA")
# plot1 + plot2

# Filter cells based on QC stats
# brain <- subset(brain, subset = nFeature_RNA > 200 & nFeature_RNA < 2500 & percent.mt < 5)


# normalize
brain <- NormalizeData(brain, normalization.method = "LogNormalize", scale.factor = 10000)


# Find variable features:
brain <- FindVariableFeatures(brain, selection.method = "vst", nfeatures = 973)

# 回归 UMI 数量和线粒体 reads 百分比的影响（内存不够了，用的下面这个）
brain <- ScaleData(brain, vars.to.regress = c("nUMI", "percent.mito"), model.use = "negbinom")
# brain <- ScaleData(brain, vars.to.regress = c("nUMI", "percent.mito"), model.use = "linear")

# all.genes <- rownames(pbmc)
# pbmc <- ScaleData(pbmc, features = all.genes)

# PCA
brain <- RunPCA(brain, features = VariableFeatures(object = brain), npcs = 100)

# Examine and visualize PCA results a few different ways
# print(brain[["pca"]], dims = 1:5, nfeatures = 5)
# DimPlot(brain, reduction = "pca")

pcs_to_use <- 1:82

# Cluster the cells
brain <- FindNeighbors(brain, dims = pcs_to_use)
brain <- FindClusters(brain, resolution = 2)

# 标记每个簇的特异性基因并进行差异表达分析
cluster_markers <- FindAllMarkers(brain, only.pos = TRUE, test.use = "bimod")

# 使用 t-SNE 进行降维，可视化聚类结果
brain <- RunTSNE(brain, dims = pcs_to_use, perplexity = 30)

# 绘制 t-SNE 图
DimPlot(brain, reduction = "tsne", group.by = "seurat_clusters")


