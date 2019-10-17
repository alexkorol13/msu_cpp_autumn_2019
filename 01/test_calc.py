import subprocess


def check(expr, res):
    out = subprocess.run(['./main.exe', expr], stdout=subprocess.PIPE, stderr=subprocess.STDOUT)
    if len(out.stdout) == 0:
        out.stdout = out.stderr
    line = out.stdout.decode('utf-8')
    while line[-1] == '\n' or line[-1] == '\r':
        line = line[:-1]
    if str(line) != str(res):
        print("error: '{0}' != {1}, got {2}".format(expr, res, line))
    else:
        print("'{0}' = {1}, OK".format(expr, res))


check("4", "4")
check("-2147483648/-2147483648", "1")
check('  4  ', '4')
check('2 + 2', '4')
check('-2 + 2', '0')
check('2 *- 2', '-4')
check('  -  2 +4-   3/ - 1-   5', '0')
check('+2/9', 'Wrong data format')
check('9/0', 'ZeroDivisionError')
check('  -  --  -2 * 2 / 2 - 2+8    ----1', '9')
