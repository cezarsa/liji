from liji.ext._liji import inspect
import ujson
from datetime import datetime

COUNT = 500000
t0 = datetime.now()

for i in xrange(COUNT):
    jsondata = open('../fixtures/bigone.json').read()
    inspect(jsondata, ["events", "picture"])

t = datetime.now() - t0
print t

for i in xrange(COUNT):
    parsed = ujson.loads(jsondata)
    [item.get('user', {}).get('picture', None) for item in parsed['events']]

t = datetime.now() - t0
print t
