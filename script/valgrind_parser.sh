#!/usr/bin/env bash

set -euo pipefail

ROOT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")/.." && pwd)"
BIN="$ROOT_DIR/ft_ping"
TARGET="${1:-127.0.0.1}"
COUNT="${COUNT:-1}"
FLAGS="${FLAGS:--v}"

if [[ ! -x "$BIN" ]]; then
	make -C "$ROOT_DIR" >/dev/null 2>&1
fi

echo "🔍 Running ft_ping under Valgrind against $TARGET"
echo "   Flags: $FLAGS -c $COUNT"
echo ""

sudo valgrind \
	--leak-check=full \
	--show-leak-kinds=definite,indirect \
	--errors-for-leak-kinds=definite,indirect \
	--track-origins=yes \
	--suppressions=/usr/share/valgrind/default.supp \
	"$BIN" $FLAGS -c "$COUNT" "$TARGET"

echo ""
echo "✅ Valgrind check complete"