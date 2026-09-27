# Lab 2 Prompt Engineering for Matrix Programs

Use the English Tutorial for the full instructions. Work from this directory on CARC.
This one-week lab combines precise prompts, two models, TXT test data, and a short
student-written vLLM function. The Hugging Face implementation is provided.

## Files you edit or create

- `model_config.json`: use qwen7b (Qwen2.5-Coder-7B-Instruct) and qwen3_4b (Qwen3-4B-Instruct-2507).
- `prompts/improved.txt`: write the program, algorithm, and file-interface requirements.
- `prompts/tests.txt`: choose two distinct test goals.
- `generate_vllm.py`: implement `generate()` using the four TODO comments.
- `tests/student/`: save two generated inputs, claimed answers, and verified answers.
- `report/report_template.md`: complete a 1 to 2 page report.

Keep `prompts/basic.txt` unchanged. Prompt files are sent exactly as written:
the script does not automatically insert `benchmark/spec.txt`.

## Hugging Face

```bash
bash scripts/run_setup.sh
source .venv/bin/activate
# Finish both prompt templates, then submit from this directory.
sbatch scripts/run_carc.slurm
squeue -u "$USER"
```

The job runs basic/improved for qwen7b and qwen3_4b, then requests test cases once.
Each model is loaded in a separate process. Use exactly these two models.
Qwen3-4B-Instruct-2507 is a newer 4B non-thinking instruction model. The HF
requirements include transformers>=4.51.0 for Qwen3 support. Run inference only
inside a GPU allocation. Interactive equivalents:

```bash
python generate_hf.py --model qwen3_4b
python generate_hf.py --model qwen7b
python generate_hf.py --model qwen3_4b --prompts prompts/tests.txt --text-only
```

Outputs go to `generated/hf/<alias>/`. The test response is `tests.raw.txt`.
Each request saves `.prompt.txt` and `.raw.txt`; program requests also save `.cpp`.
The script extracts the first fenced block if present. Settings and package
versions are recorded in `settings.json`. Reruns replace matching files.

## vLLM coding task

Complete `generate()` yourself using the HF example and vLLM documentation.
Use offline inference; no web server or API key is needed. Install separately:

```bash
python -m venv .venv-vllm
source .venv-vllm/bin/activate
python -m pip install --upgrade pip
python -m pip install -r requirements-vllm.txt
sbatch scripts/run_vllm.slurm
```

vLLM selects its compatible PyTorch dependencies. The setup remains an ordinary
Python virtual environment and one CARC GPU; account/partition/module names are
only filled in if your allocation needs them. No particular GPU model is assumed.
The course staff should check the installed vLLM release against CARC's Python
and GPU driver before class. See the official installation link below.

## TXT tests

Three visible cases with expected outputs are in `tests/provided/`. Create your
two additional cases following `tests/student/README.md`. `reference.py` computes
answers with ordinary multiplication independently of the generated Strassen code.
In a CARC compute session, run:

```bash
source .venv/bin/activate
python evaluate.py generated/hf/qwen3_4b/improved.cpp
```

Repeat for basic/improved from both models and the vLLM source. Or in Bash:

```bash
for src in generated/hf/*/*.cpp generated/vllm/*/*.cpp; do
  python evaluate.py "$src"
done
```

The evaluator compiles each complete program with g++ and passes input/output
paths as two arguments. The executable has no suffix, for example `improved.cpp`
produces `improved`, following the Linux convention. It checks TXT row structure and values, allowing spacing
differences. Each `.log` records pass/fail; each `<prompt>_results/` keeps TXT outputs.
You can also compile/run one program manually in the compute session:

```bash
g++ -std=c++17 -O2 generated/hf/qwen3_4b/improved.cpp -o matrix
./matrix tests/provided/sample.input.txt sample.output.txt
cat sample.output.txt
```

The sample output should be `19 22` on the first line and `43 50` on the second.
The generated C++ owns both `main()` and file I/O; no C++ driver is supplied.
If you are on a login node, request a CPU compute session first, for example
`salloc --time=00:10:00`, with allocation settings if your account requires them.

## Submission

Submit to CodeGrade with your config, prompts, completed
`generate_vllm.py`, `generated/` sources/responses/settings/logs/TXT outputs,
`tests/student/`, completed report, and HF/vLLM CARC job logs.
Exclude `.venv`, `.venv-vllm`, downloaded weights, caches, and executables.
Do not keep revising prompts until all models pass. Preserve original model
outputs; if you fix code for exploration, save a separate copy and describe it.

## References

- [Qwen3 4B model and HF example](https://huggingface.co/Qwen/Qwen3-4B-Instruct-2507)
- [Qwen 7B model](https://huggingface.co/Qwen/Qwen2.5-Coder-7B-Instruct)
- [vLLM offline inference](https://docs.vllm.ai/en/latest/getting_started/quickstart/)
- [vLLM GPU installation](https://docs.vllm.ai/en/latest/getting_started/installation/gpu/)
