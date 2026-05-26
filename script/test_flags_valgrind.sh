#!/usr/bin/env bash

set -euo pipefail

ROOT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")/.." && pwd)"
BIN="$ROOT_DIR/ft_ping"
TARGET="${1:-127.0.0.1}"

if [[ ! -x "$BIN" ]]; then
	make -C "$ROOT_DIR" >/dev/null 2>&1
fi

run_valgrind_test() {
	local name="$1"
	local args="$2"
	
	echo ""
	echo "=== Testing: $name ==="
	sudo valgrind \
		--leak-check=full \
		--show-leak-kinds=definite,indirect \
		--errors-for-leak-kinds=definite,indirect \
		--track-origins=yes \
		--log-file="/tmp/valgrind_$$.log" \
		"$BIN" $args
	
	if grep -q "ERROR SUMMARY: 0 errors" "/tmp/valgrind_$$.log"; then
		echo "✓ No errors detected"
	else
		echo "⚠ Check /tmp/valgrind_$$.log for details"
	fi
	rm -f "/tmp/valgrind_$$.log"
}

echo "🔍 Running parsing tests under Valgrind..."
echo ""

run_valgrind_test "Basic IP" "-c 1 $TARGET"
run_valgrind_test "Verbose mode" "-v -c 1 $TARGET"
run_valgrind_test "Numeric flag" "-n -c 1 $TARGET"
run_valgrind_test "Custom interval" "-i 0.5 -c 2 $TARGET"
run_valgrind_test "Custom timeout" "-W 3 -c 1 $TARGET"
run_valgrind_test "Custom TTL" "-t 32 -c 1 $TARGET"
run_valgrind_test "Combined flags" "-v -n -c 1 -i 0.5 -t 64 $TARGET"

echo ""
echo "✅ All Valgrind tests completed"
