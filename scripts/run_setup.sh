#!/usr/bin/env bash
set -euo pipefail

# Run from the Lab2_Prompt_Strassen_Starter root directory.
# Make sure USC VPN is on before logging into CARC from off campus.

module purge || true
# Uncomment/modify these only if your CARC environment requires them.
# module load gcc
# module load cuda
# module load python

python -m venv .venv
source .venv/bin/activate
pip install --upgrade pip
pip install -r requirements.txt

python - <<'PY'
import torch, transformers
print('torch:', torch.__version__)
print('transformers:', transformers.__version__)
print('cuda available:', torch.cuda.is_available())
PY
