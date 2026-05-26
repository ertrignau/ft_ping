#!/usr/bin/env bash

set -euo pipefail

ROOT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")/.." && pwd)"
BIN="$ROOT_DIR/ft_ping"
TARGET="${1:-127.0.0.1}"
COUNT="${COUNT:-100}"
INTERVAL="${INTERVAL:-0.2}"

if [[ ! -x "$BIN" ]]; then
	make -C "$ROOT_DIR"
fi

echo "Benchmarking ft_ping against $TARGET"
start_ns="$(date +%s%N)"
tmp_output="$(mktemp /tmp/ft_ping_perf.XXXXXX)"

sudo "$BIN" -c "$COUNT" -i "$INTERVAL" "$TARGET" >"$tmp_output"
end_ns="$(date +%s%N)"

elapsed_ms="$(awk -v start="$start_ns" -v end="$end_ns" 'BEGIN { printf "%.3f", (end - start) / 1000000 }')"
per_packet_ms="$(awk -v total="$elapsed_ms" -v count="$COUNT" 'BEGIN { printf "%.3f", total / count }')"

echo "Total time: ${elapsed_ms} ms"
echo "Average per packet: ${per_packet_ms} ms"
echo "Raw output stored in ${tmp_output}"