#!/usr/bin/env bash

set -euo pipefail

ROOT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")/.." && pwd)"
BIN="$ROOT_DIR/ft_ping"
TARGET="${1:-127.0.0.1}"
COUNT="${COUNT:-1}"

if [[ ! -x "$BIN" ]]; then
	make -C "$ROOT_DIR"
fi

echo "Running ft_ping under valgrind against $TARGET"
sudo valgrind \
	--leak-check=full \
	--show-leak-kinds=definite,indirect \
	--errors-for-leak-kinds=definite,indirect \
	--track-origins=yes \
	"$BIN" -c "$COUNT" "$TARGET"