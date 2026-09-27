# Supplied TXT cases

- `scalar`: n = 1, checks the scalar base case and a negative answer.
- `sample`: n = 2, checks the documented file example; A * B differs from B * A.
- `identity4`: n = 4, checks a simple recursive case whose answer is A.

Each `.input.txt` has a matching `.expected.txt` for inspection.
`evaluate.py` computes answers independently using `reference.py`;
it does not trust a model-generated expected output.
