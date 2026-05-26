#!/usr/bin/env bash

set -euo pipefail

ROOT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")/.." && pwd)"
BIN="$ROOT_DIR/ft_ping"
PASS=0
FAIL=0

if [[ ! -x "$BIN" ]]; then
	make -C "$ROOT_DIR" >/dev/null 2>&1
fi

test_flag() {
	local name="$1"
	local args="$2"
	local expect_fail="${3:-0}"
	
	if sudo "$BIN" $args >/dev/null 2>&1; then
		if [[ $expect_fail -eq 0 ]]; then
			echo "✓ $name"
			((PASS++))
		else
			echo "✗ $name (expected to fail but passed)"
			((FAIL++))
		fi
	else
		if [[ $expect_fail -eq 1 ]]; then
			echo "✓ $name (correctly failed)"
			((PASS++))
		else
			echo "✗ $name (unexpected failure)"
			((FAIL++))
		fi
	fi
}

test_flag_output() {
	local name="$1"
	local args="$2"
	local expect_output="$3"
	
	output=$(sudo "$BIN" $args 2>&1 || true)
	if echo "$output" | grep -q "$expect_output"; then
		echo "✓ $name (output matched)"
		((PASS++))
	else
		echo "✗ $name (output mismatch)"
		echo "  Expected: $expect_output"
		echo "  Got: $output" | head -3
		((FAIL++))
	fi
}

echo "=== Testing Parsing & Flags ==="
echo ""

# Basic flags
echo "[Basic Flags]"
test_flag "verbose flag (-v)" "-v -c 1 127.0.0.1"
test_flag "numeric flag (-n)" "-n -c 1 127.0.0.1"
test_flag "count flag (-c 2)" "-c 2 127.0.0.1"
test_flag "interval flag (-i 0.5)" "-i 0.5 127.0.0.1"
test_flag "timeout flag (-W 2)" "-W 2 127.0.0.1"
test_flag "ttl flag (-t 128)" "-t 128 127.0.0.1"
test_flag "combined flags (-v -c 1 -i 0.5)" "-v -c 1 -i 0.5 127.0.0.1"

echo ""
echo "[Invalid Flags & Edge Cases]"
test_flag_output "missing host" "-c 1" "missing host operand"
test_flag_output "invalid option" "-x" "invalid option"
test_flag_output "count <= 0" "-c 0 127.0.0.1" "bad number of packets"
test_flag_output "interval < 0.2" "-i 0.1 127.0.0.1" "bad timing interval"
test_flag_output "ttl < 1" "-t 0 127.0.0.1" "out of range"
test_flag_output "ttl > 255" "-t 256 127.0.0.1" "out of range"
test_flag_output "timeout <= 0" "-W 0 127.0.0.1" "bad wait time"
test_flag_output "missing count value" "-c 127.0.0.1" "requires an argument"
test_flag_output "missing interval value" "-i 127.0.0.1" "requires an argument"
test_flag_output "missing ttl value" "-t 127.0.0.1" "requires an argument"

echo ""
echo "[Hostname Resolution]"
test_flag "numeric IP" "-c 1 127.0.0.1"
test_flag "localhost alias" "-c 1 localhost"

echo ""
echo "=== Results ==="
echo "Passed: $PASS"
echo "Failed: $FAIL"

if [[ $FAIL -gt 0 ]]; then
	exit 1
fi
