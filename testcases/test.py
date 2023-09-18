import subprocess, os, itertools

COLORS = {"default": "\033[0m", "red": "\033[31m", "green": "\033[32m"}

def color(name, text):
  if os.isatty(1):
    return COLORS[name] + text + COLORS["default"]
  return text

total = 0
ac = 0

with open('testcases.txt') as test, open('testcases_ans.txt') as ans:
  for t, a, i in zip(test, ans, itertools.count(1)):
    print('%3d %30s' % (i, t.split()[1]), end=': ')
    total += 1
    try:
      tm = subprocess.run(('bash', '-c', t), stdout=subprocess.PIPE, stderr=subprocess.PIPE, text=True, encoding="utf-8", timeout=1)
      if tm.returncode != 0:
        print(color("red", "RE"), "stderr:", tm.stderr.strip())
      elif tm.stdout.strip() == a.strip():
        print(color("green", "AC"))
        ac += 1
      else:
        print(color("red", "WA"), "ans:", a.strip(), "stdout:", tm.stdout.strip())
    except TimeoutError:
      print(color("red", "TLE"))
    except:
      print(color("red", "RE"))

print(f'AC {ac}/{total}')
