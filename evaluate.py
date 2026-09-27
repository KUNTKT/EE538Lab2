"""Compile a complete program and test its TXT file interface and answers."""
import argparse
import subprocess
from pathlib import Path
from reference import expected

parser = argparse.ArgumentParser()
parser.add_argument('source')
parser.add_argument('--cases', nargs='+', default=['tests/provided', 'tests/student'])
args = parser.parse_args()
source = Path(args.source)
binary = source.with_suffix('')
result_dir = source.parent / (source.stem + '_results')
result_dir.mkdir(exist_ok=True)
compiled = subprocess.run(['g++', '-std=c++17', '-O2', str(source), '-o', str(binary)],
                          capture_output=True, text=True)
log = compiled.stdout + compiled.stderr
if compiled.returncode:
    log += 'Compilation failed; tests were not run.\n'
else:
    cases = sorted(p for folder in args.cases for p in Path(folder).glob('*.input.txt'))
    passed = 0
    for case in cases:
        answer = expected(case)
        output = result_dir / f'{case.parent.name}_{case.name.replace(".input", ".output")}'
        output.unlink(missing_ok=True)
        try:
            run = subprocess.run([str(binary.resolve()), str(case.resolve()),
                                  str(output.resolve())], stdin=subprocess.DEVNULL,
                                 capture_output=True, text=True, timeout=5)
            rows = [list(map(int, line.split())) for line in output.read_text().splitlines()]
            ok = run.returncode == 0 and rows == answer
            detail = f'exit={run.returncode}'
        except (OSError, ValueError, subprocess.TimeoutExpired) as error:
            ok, detail = False, str(error)
        passed += ok
        log += f'{case.parent.name}/{case.name}: {"PASS" if ok else "FAIL"} ({detail})\n'
    log += f'{passed}/{len(cases)} tests passed\n'
source.with_suffix('.log').write_text(log, encoding='utf-8')
print(log)
