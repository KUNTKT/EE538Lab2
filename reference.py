"""Independent ordinary multiplication for checking test cases on the CPU."""
import sys
from pathlib import Path


def expected(filename):
    rows = [list(map(int, line.split()))
            for line in Path(filename).read_text().splitlines()]
    n = rows[0][0]
    assert len(rows[0]) == 1 and n in (1, 2, 4, 8), 'Invalid size'
    assert len(rows) == 2 * n + 1 and all(len(r) == n for r in rows[1:]), 'Invalid shape'
    assert all(-9 <= x <= 9 for r in rows[1:] for x in r), 'Invalid entry'
    a, b = rows[1:n + 1], rows[n + 1:]
    return [[sum(a[i][k] * b[k][j] for k in range(n))
             for j in range(n)] for i in range(n)]


if __name__ == '__main__':
    result = expected(sys.argv[1])
    Path(sys.argv[2]).write_text(
        '\n'.join(' '.join(map(str, row)) for row in result) + '\n')
