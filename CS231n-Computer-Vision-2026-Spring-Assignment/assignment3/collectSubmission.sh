#!/bin/bash
set -euo pipefail

CODE=(
	# Transformer implementation files
	"cs231n/transformer_layers.py"
	"cs231n/classifiers/transformer.py"
	"cs231n/captioning_solver_transformer.py"

	# Self-Supervised Learning implementation files
	"cs231n/simclr/contrastive_loss.py"
	"cs231n/simclr/data_utils.py"
	"cs231n/simclr/utils.py"
	"cs231n/simclr/model.py"

	# DDPM implementation files
	"cs231n/unet.py"
	"cs231n/gaussian_diffusion.py"
	"cs231n/ddpm_trainer.py"
	"cs231n/emoji_dataset.py"
	"cs231n/clip_dino.py"
)

NOTEBOOKS=(
	"Transformer_Captioning.ipynb"
	"Self_Supervised_Learning.ipynb"
	"DDPM.ipynb"
	"CLIP_DINO.ipynb"
)

PDFS=(
  	"Transformer_Captioning.ipynb"
	"Self_Supervised_Learning.ipynb"
	"DDPM.ipynb"
	"CLIP_DINO.ipynb"
)

FILES=( "${CODE[@]}" "${NOTEBOOKS[@]}" )
ZIP_FILENAME="a3_code_submission.zip"
PDF_FILENAME="a3_inline_submission.pdf"

for FILE in "${FILES[@]}"
do
	if [ ! -f ${FILE} ]; then
		echo -e "Required file ${FILE} not found, Exiting."
		exit 0
	fi
done

echo -e "### Zipping file ###"
rm -f ${ZIP_FILENAME}
zip -q "${ZIP_FILENAME}" -r ${NOTEBOOKS[@]} $(find . -name "*.py") -x "makepdf.py"

echo -e "### Creating PDFs ###"
python makepdf.py --notebooks "${PDFS[@]}" --pdf_filename "${PDF_FILENAME}"

echo -e "### Done! Please submit ${ZIP_FILENAME} and ${PDF_FILENAME} to Gradescope. ###"