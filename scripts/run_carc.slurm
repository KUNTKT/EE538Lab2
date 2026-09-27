#!/bin/bash
#SBATCH --job-name=ee538_lab2
#SBATCH --output=lab2-%j.out
#SBATCH --error=lab2-%j.err
#SBATCH --time=04:00:00
#SBATCH --cpus-per-task=4
#SBATCH --mem=48G
#SBATCH --gres=gpu:1

# Add account/partition lines only if your CARC allocation requires them.
# #SBATCH --partition=gpu
# #SBATCH --account=<your_account>

module purge
# module load gcc
# module load cuda
# module load python
source .venv/bin/activate
nvidia-smi
# Edit improved.txt and tests.txt before submitting. Each process releases its model.
python generate_hf.py --model qwen7b
python generate_hf.py --model qwen3_4b
python generate_hf.py --model qwen3_4b --prompts prompts/tests.txt --text-only
