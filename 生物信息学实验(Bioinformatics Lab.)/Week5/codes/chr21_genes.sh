#!/bin/bash

anno_path="/gpfs1/share/class2/annotation.gtf"
awk -F '\t' '$3=="gene"{print $0}' $anno_path > /home/teach80_pkuhpc/peiyuliu/genes.txt
awk -F '\t' '$3=="gene"{print $9}' $anno_path | awk -F ' ' '{print $4}' > /home/teach80_pkuhpc/peiyuliu/gene_types.txt
sort /home/teach80_pkuhpc/peiyuliu/gene_types.txt | uniq > uniq_gene_types.txt
