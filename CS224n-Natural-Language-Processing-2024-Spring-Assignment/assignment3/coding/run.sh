#!/bin/bash

if [ "$1" = "train" ]; then
	CUDA_VISIBLE_DEVICES=0 python run.py train --train-src=./zh_en_data/train.zh --train-tgt=./zh_en_data/train.en --dev-src=./zh_en_data/dev.zh --dev-tgt=./zh_en_data/dev.en --vocab=vocab.json --cuda --lr=5e-4 --patience=1 --valid-niter=200 --batch-size=32 --dropout=.3
elif [ "$1" = "test" ]; then
	CUDA_VISIBLE_DEVICES=0 python run.py decode model.bin ./zh_en_data/test.zh ./zh_en_data/test.en outputs/test_outputs.txt --cuda
elif [ "$1" = "dev" ]; then
	CUDA_VISIBLE_DEVICES=0 python run.py decode model.bin ./zh_en_data/dev.zh ./zh_en_data/dev.en outputs/dev_outputs.txt --cuda
elif [ "$1" = "train_local" ]; then
	python run.py train --train-src=./zh_en_data/train.zh --train-tgt=./zh_en_data/train.en --dev-src=./zh_en_data/dev.zh --dev-tgt=./zh_en_data/dev.en --vocab=vocab.json --lr=5e-4
elif [ "$1" = "test_local" ]; then
	python run.py decode model.bin ./zh_en_data/test.zh ./zh_en_data/test.en outputs/test_outputs.txt
elif [ "$1" = "vocab" ]; then
	python vocab.py --train-src=./zh_en_data/train.zh --train-tgt=./zh_en_data/train.en vocab.json
elif [ "$1" = "tensorboard" ]; then
	tensorboard --logdir runs --bind_all
else
	echo "Invalid Option Selected"
fi
