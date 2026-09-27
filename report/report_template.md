# Lab 2 report

Name and USC ID: Kuntian Tang, 4855814938

Aim for 1 to 2 pages including tables. Attach code, test files, and logs separately.

## Prompts and model results

The improved prompt has three parts. (1) **File interface:** it names the invocation
`./matrix input.txt output.txt`, requires `argv[1]`/`argv[2]` with `std::ifstream`/`std::ofstream`,
forbids `std::cin` and hard-coded names, and describes the output file as exactly n lines of
n space-separated integers with "no n, no labels, no brackets, no blank lines". (2) **Algorithm:**
it fixes the data type (`std::vector<std::vector<long long>>`), the n == 1 scalar base case, all seven
products M1–M7 with their exact operands, the four combination formulas and the block placement, and
the valid sizes (n ∈ {1, 2, 4, 8}, entries in [−9, 9], no padding). (3) **Example:** the 2×2 sample
input and its exact output file.

> "Read the input filename from argv[1] and the output filename from argv[2]."

This sentence matters because the evaluator passes the two paths as arguments. Without it both basic
programs hard-coded their matrices inside `main()` and printed to the console, so even a correct
Strassen routine could never be tested. With it, both improved programs read `argv[1]` and wrote
`argv[2]` in the required format.

**Basic vs improved (qwen7b).** Basic used `int`, hard-coded 2×2 matrices and console output with a
"Result of multiplication:" label. Improved switched to `long long`, the file interface, and the exact
M1–M7 operands from the prompt. Neither compiled: both call their add/subtract helpers before these are
declared, and improved also has an unbalanced C22 line.
**Same improved prompt across models.** qwen7b kept the correct Strassen structure but failed on C++
mechanics. qwen3_4b produced a program that compiles and has correct file I/O, but it has lost the algorithm.

| Backend | Model alias | Prompt | Compiles | Supplied /3 | New /2 | Seven products and scalar base case |
|---|---|---|---|---|---|---|
| HF | qwen7b | basic | No: `add`/`subtract` used before declaration | N/A | N/A | Yes: 7 correct products, `n == 1` base (but `int`, no file I/O) |
| HF | qwen7b | improved | No: helpers undeclared; unbalanced `C22 = ...` line | N/A | N/A | Seven correct recursive products, `n == 1` base; C22 formula wrong (adds M7) |
| HF | qwen3_4b | basic | No: `operator+`/`operator-` undefined for `Matrix` | N/A | N/A | Seven calls, but wrong operands (e.g. P2 = (A12+A21)(B11+B12)); size-1 base |
| HF | qwen3_4b | improved | Yes | 1/3 (scalar) | 0/2 | No: seven calls on hard-coded 2×2 slices with no sums, e.g. M1 = strassen(A11, B11); `n == 1` base present; segfaults for n ≥ 2 |
| vLLM | qwen3_4b | improved | Yes | 1/3 (scalar) | 0/2 | Same source as HF (byte-identical), same defects |

## Test design and verification

| New case | Size | Bug or property targeted | Claimed answer agrees with reference |
|---|---|---|---|
| case1 | 4 | Swapped operands (B·A) and sign errors in differences such as B12 − B22; non-symmetric A, B with mixed signs | No: column 1 is wrong in every row (claimed −32, 30, 6, 12; reference −28, 22, −4, 23). The other 12 entries are correct |
| case2 | 8 | Block placement after two recursion levels: block-diagonal A, diagonal B = diag(1,2,3,4,1,2,3,4) | No: the claimed output ignores the scaling by B and puts non-zero values in the lower-left block, which must be zero |

**Hand check (case1, C[0][1]).** Row 0 of A is (9, −1, 2, 3) and column 1 of B is (−3, 4, 0, 1), so
C[0][1] = 9·(−3) + (−1)·4 + 2·0 + 3·1 = −27 − 4 + 0 + 3 = **−28**. This matches `case1.expected.txt`
from `reference.py`, not the model's −32. The model's worked answer used B[2][1] = 1 and B[3][1] = −1,
which are misread entries.

**Why claimed answers alone are inadequate.** Both claimed outputs are wrong, even though the model marked
case 1 "✅ This is the correct A * B". Used as expected files, they would make every correct program fail
and could let a program with the same arithmetic slip pass. In the case 1 response the model first
wrote an output, found it wrong, then "corrected" it to another wrong answer. Only the independent
reference (`reference.py`, plain triple loop) is trustworthy.

**Format corrections.** (1) Both generated inputs repeated n on a separate line between A and B. I
removed that line. (2) In case 2 the top-left and bottom-right blocks used values 10–16, which are outside
[−9, 9], so `reference.py` rejects them. I mapped each entry v ≥ 10 to 9 − v (10 → −1, …, 16 → −7) and kept
the block structure. (3) The case 2 response stopped at exactly 4096 tokens (`max_new_tokens`) in the middle of
the model's own recomputation, so `case2.claimed.txt` is the only complete "Claimed Output" block in the
response. For case 1 I used the model's final "Corrected" block. The raw response is unchanged in
`generated/hf/qwen3_4b/tests.raw.txt`. The model's prose also describes case 2 differently from its
input: it mentions a negative block, an identity block and diag(1..8).

## Your vLLM code

- **Engine:** `LLM(model=model_id, dtype=..., max_model_len=8192, gpu_memory_utilization=0.85)` loads
  the weights once. It corresponds to `AutoModelForCausalLM.from_pretrained(..., torch_dtype=...)`.
- **Chat formatting:** `llm.get_tokenizer().apply_chat_template([{'role': 'user', 'content': prompt}],
  tokenize=False, add_generation_prompt=True)` is the same call as in the HF script, so both backends
  see the same prompt text.
- **Sampling:** `SamplingParams(temperature=0.0, max_tokens=4096)`. `max_tokens` corresponds to HF
  `max_new_tokens`, and temperature 0 is greedy decoding, like `do_sample=False`.
- **Output:** `llm.generate([text], sampling)` corresponds to `model.generate`. `outputs[0].outputs[0].text`
  is the first completion of the first prompt. vLLM returns only the generated text, so the HF step that
  slices off the prompt tokens and decodes them is not needed.

**Across backends:** with qwen3_4b, the improved prompt and greedy decoding, the vLLM `improved.cpp`
is byte-identical to the HF one, so it gets the same 1/5 result.
**Environment note:** I ran the vLLM job directly on an interactive A100 node (logs:
`lab2-vllm-local.out/.err`), not through `run_vllm.slurm`. Two environment variables were needed, and
neither changes the code: `LD_PRELOAD` of a newer `libstdc++`, because the system library lacked
`CXXABI_1.3.15`, and `VLLM_USE_FLASHINFER_SAMPLER=0`, because the FlashInfer sampler JIT needs `nvcc`.
Failed attempts are in `logs_failed/`.

## Observation

qwen7b/improved shows that a precise prompt can produce the right algorithm but the wrong program. All
seven products use the exact operands from the prompt, and it has the file interface and `n == 1` base
case, but it does not compile. For exploration only, I made a separate copy,
`generated/manual_fix/qwen7b_improved_fixed.cpp`, with three marked fixes:
(1) forward declarations for `addMatrices`/`subtractMatrices`;
(2) `C22 = M1 − M2 + M3 + M6`, replacing the unbalanced line that also added M7;
(3) blocks allocated as `n/2 × n/2`, because the original created n/2 *empty* rows and caused a segfault.
The fixed copy passes 5/5. The original file is unchanged and is the one reported in the table.

qwen3_4b/improved fails the other way. It compiles and passes the scalar case because of the `n == 1`
branch, but every product takes a hard-coded `{{X[0][0], X[0][1]}, {X[1][0], X[1][1]}}` slice. It
indexes past a 1×1 block at n = 2 and never forms the required sums. The scalar pass also shows that a
passing test does not prove Strassen is implemented.
