# Your two additional cases

Use your edited `prompts/tests.txt` to request two cases, one n = 4 and one n = 8.
Choose two distinct bug targets. Save the model's input and proposed output as:

- `case1.input.txt` and `case1.claimed.txt`
- `case2.input.txt` and `case2.claimed.txt`

Do not include Markdown fences or labels in these files. Keep the original
claimed answers even if they are wrong. On a CARC compute node, run:

```bash
python reference.py tests/student/case1.input.txt tests/student/case1.expected.txt
python reference.py tests/student/case2.input.txt tests/student/case2.expected.txt
diff -w tests/student/case1.claimed.txt tests/student/case1.expected.txt
diff -w tests/student/case2.claimed.txt tests/student/case2.expected.txt
```

No diff output means agreement (exit 0); a difference is an observation to report.
Read `reference.py` and check one output entry by hand in your report.
If a generated input violates the stated format, fix its transcription/format
and describe the correction; preserve the raw model response.
`evaluate.py` automatically includes all `.input.txt` files in this directory.
