#!/bin/bash
#SBATCH --job-name=ee538_lab2_vllm
#SBATCH --output=lab2-vllm-%j.out
#SBATCH --error=lab2-vllm-%j.err
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
source .venv-vllm/bin/activate
nvidia-smi
python generate_vllm.py --model qwen3_4b
