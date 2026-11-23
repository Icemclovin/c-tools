#!/bin/sh
# Small smoke tests: run each tool on the example files and check the output.
set -e
cd "$(dirname "$0")"
fail() { echo "FAIL: $1"; exit 1; }

./minigrep/minigrep ERROR examples/server.log | grep -q "Total found: 2" || fail "minigrep should find 2 ERROR lines"

./hexdump/hexdump examples/server.log | head -1 | grep -q "^00000000 49 4E 46 4F" || fail "hexdump first line"
# every line has the same width up to the ASCII column, also the last (short) one
[ "$(./hexdump/hexdump examples/server.log | cut -d'|' -f1 | awk '{print length}' | sort -u | wc -l)" -eq 1 ] || fail "hexdump alignment"

./csv2json/csv2json examples/stal.csv examples/stal.json >/dev/null
python3 -c "import json,sys; d=json.load(open('examples/stal.json')); assert len(d)==3 and d[0]['temperatuur']=='22.5' and d[2]['ventilatie']==''" || fail "csv2json output"

echo "All tests passed"
