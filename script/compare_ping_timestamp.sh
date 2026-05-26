#!/usr/bin/env bash

set -euo pipefail

ROOT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")/.." && pwd)"
BIN="$ROOT_DIR/ft_ping"
TARGET="${1:-127.0.0.1}"
COUNT="${COUNT:-4}"
INTERVAL="${INTERVAL:-1}"
MAX_DIFF_MS="${MAX_DIFF_MS:-30}"

if [[ ! -x "$BIN" ]]; then
	make -C "$ROOT_DIR"
fi

extract_times() {
	grep -oE 'time=[0-9]+(\.[0-9]+)?' | cut -d= -f2
}

ft_output="$(sudo "$BIN" -c "$COUNT" -i "$INTERVAL" "$TARGET")"
sys_output="$(ping -c "$COUNT" -i "$INTERVAL" "$TARGET")"

mapfile -t ft_times < <(printf '%s\n' "$ft_output" | extract_times)
mapfile -t sys_times < <(printf '%s\n' "$sys_output" | extract_times)

if [[ ${#ft_times[@]} -eq 0 || ${#sys_times[@]} -eq 0 ]]; then
	echo "Unable to extract RTT values from one of the outputs" >&2
	exit 1
fi

min_count=${#ft_times[@]}
if [[ ${#sys_times[@]} -lt $min_count ]]; then
	min_count=${#sys_times[@]}
fi

echo "Seq | ft_ping(ms) | ping(ms) | diff(ms)"
echo "---- | ----------: | --------: | -------:"

for ((i = 0; i < min_count; i++)); do
	ft="${ft_times[$i]}"
	sys="${sys_times[$i]}"
	diff="$(awk -v a="$ft" -v b="$sys" 'BEGIN { d = a - b; if (d < 0) d = -d; printf "%.3f", d }')"
	printf '%3d | %11s | %8s | %7s\n' "$((i + 1))" "$ft" "$sys" "$diff"
	if awk -v d="$diff" -v max="$MAX_DIFF_MS" 'BEGIN { exit !(d > max) }'; then
		echo "Difference above ${MAX_DIFF_MS} ms on sequence $((i + 1))" >&2
		exit 1
	fi
done

echo "All compared RTT values are within ${MAX_DIFF_MS} ms"