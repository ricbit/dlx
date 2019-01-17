import random
import sys
import string

def gen_option():
  return "".join(random.sample(string.lowercase, 3))

nitems = int(sys.argv[1])
noptions = int(sys.argv[2])
items = list(set(gen_option() for _ in xrange(nitems)))
print " ".join(items)
for _ in xrange(noptions):
  n = random.randint(1, len(items))
  option = random.sample(items, n)
  print " ".join(option)


