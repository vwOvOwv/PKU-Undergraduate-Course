@echo off
if exist assignment4_submission.zip (
    del /q assignment4_submission.zip
)
if exist src\__pycache__ (
    rmdir /s /q src\__pycache__
)

"%ProgramFiles%\7-Zip\7z.exe" a assignment4_submission.zip src\ vanilla.model.params vanilla.finetune.params rope.finetune.params vanilla.nopretrain.dev.predictions vanilla.nopretrain.test.predictions vanilla.pretrain.dev.predictions vanilla.pretrain.test.predictions rope.pretrain.dev.predictions rope.pretrain.test.predictions london_baseline_accuracy.txt
