# Lab 2 report

Name and USC ID:

Aim for 1 to 2 pages including tables. Attach code, test files, and logs separately.

## Prompts and model results

Explain how your improved prompt specifies the TXT interface and Strassen.
Quote one sentence you wrote and explain why it matters.
Compare basic vs improved within one model, then the same improved prompt across models.

| Backend | Model alias | Prompt | Compiles | Supplied /3 | New /2 | Seven products and scalar base case |
|---|---|---|---|---|---|---|
| HF | qwen7b | basic | | | | |
| HF | qwen7b | improved | | | | |
| HF | qwen3_4b | basic | | | | |
| HF | qwen3_4b | improved | | | | |
| vLLM | qwen3_4b | improved | | | | |

Use the two configured models. Mark test counts N/A if
compilation fails. Five passing numerical tests do not prove Strassen is used.

## Test design and verification

| New case | Size | Bug or property targeted | Claimed answer agrees with reference |
|---|---|---|---|
| case1 | 4 | | |
| case2 | 8 | | |

Show one entry C[i][j] = sum(A[i][k] * B[k][j]) calculated by hand.
Explain why trusting the model's claimed answers alone would be inadequate.
If an input needed a format correction, describe it.

## Your vLLM code

Explain the lines you wrote for engine creation, chat formatting, sampling,
and output extraction. What corresponds to `model.generate` and `max_new_tokens`
in the HF example? Compare the same model and improved prompt across backends.
Identical source text or timing measurements are not required.

## Observation

Describe one observed success, failure, or difference using the saved code/logs.
Avoid ranking models in general from this small exercise. Note any manual code
fixes separately; keep original generated files for the table.
