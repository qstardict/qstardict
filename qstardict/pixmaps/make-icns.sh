#!/bin/bash
set -x -e

CURRENT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
RESOLUTIONS=(16 32 64 128 256 512)

OUTPUT_DIR="$CURRENT_DIR/qstardict.iconset"
OUTPUT_FILE="$CURRENT_DIR/qstardict.icns"

mkdir -p "$OUTPUT_DIR"

resize() {
  inkscape -z "$CURRENT_DIR/../qstardict.svg" -w $1 -h $1 -e "$OUTPUT_DIR/icon_$2.png"
}

for i in "${RESOLUTIONS[@]}"; do
  resize $i "${i}x${i}"
  resize $((i * 2)) "${i}x${i}@2x"
done

iconutil -c icns --output "$OUTPUT_FILE" "$OUTPUT_DIR"
