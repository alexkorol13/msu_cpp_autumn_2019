import subprocess


def check(expr, res):
    out = subprocess.run(['./main.exe', expr], stdout=subprocess.PIPE)
    if out.stdout != res:
        print("error: {0} != {1}, got {2}".format(expr, res, out.stdout))
    else:
        print("OK")

check('4', '4')
check('  4  ', '4')
check('2 + 2', '4')
check('-2 + 2', '0')
check('2 *- 2', '-4')
check('  -  2 +4-   3/ - 1-   5', '0')
check('+2/9', 'error: unary plus is not required')
check('9/0', 'ZeroDivisionError')
check('  -  --  -2 * 2 / 2 - 2+8    ----1', '9')
