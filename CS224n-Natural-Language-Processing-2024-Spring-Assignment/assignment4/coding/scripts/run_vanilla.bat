@echo off

@REM Pretrain the model
python src\run.py pretrain vanilla wiki.txt ^
        --writing_params_path vanilla.pretrain.params
        
@REM Finetune the model
python src\run.py finetune vanilla wiki.txt ^
        --reading_params_path vanilla.pretrain.params ^
        --writing_params_path vanilla.finetune.params ^
        --finetune_corpus_path birth_places_train.tsv
        
@REM Evaluate on the dev set; write to disk
python src\run.py evaluate vanilla wiki.txt  ^
        --reading_params_path vanilla.finetune.params ^
        --eval_corpus_path birth_dev.tsv ^
        --outputs_path vanilla.pretrain.dev.predictions
        
@REM Evaluate on the test set; write to disk
python src\run.py evaluate vanilla wiki.txt  ^
        --reading_params_path vanilla.finetune.params ^
        --eval_corpus_path birth_test_inputs.tsv ^
        --outputs_path vanilla.pretrain.test.predictions
