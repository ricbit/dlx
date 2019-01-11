import sys

n = int(sys.argv[1])
items = []
for i in xrange(1, 1 + n):
  items.extend(("q%d c%d r%d" % (i, i, i)). split())
for i in xrange(1, 1 + 2 * n):
  items.append("a%d" % i)
for i in xrange(-n + 1, n):
  items.append("b%d" % i)
print " ".join(items)
for q in xrange(1, 1 + n):
  for i in xrange(1, 1 + n):
    for j in xrange(1, 1 + n):
      print "q%d r%d c%d a%d b%d" % (q, i, j, i + j, i - j)
for i in xrange(1, 1 + 2 * n):
  print "a%d" % i
for i in xrange(-n + 1, n):
  print "b%d" % i
