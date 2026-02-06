#!/usr/bin/env bash
set -euo pipefail

ROOT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")/.." && pwd)"
PROS_FILE="$ROOT_DIR/project.pros"

if [[ ! -f "$PROS_FILE" ]]; then
  echo "project.pros not found at $PROS_FILE"
  exit 1
fi

python3 - <<'PY'
import json
from pathlib import Path

path = Path("project.pros")
data = json.loads(path.read_text())
name = data["py/state"]["project_name"]

# Expect format: X.Y.Z NAME...
parts = name.split(" ", 1)
ver = parts[0]
rest = parts[1] if len(parts) > 1 else ""

nums = ver.split(".")
if len(nums) != 3 or not all(n.isdigit() for n in nums):
    raise SystemExit(f"Unsupported version format: {ver}")

maj, minr, pat = map(int, nums)
pat += 1
if pat >= 100:
    pat = 0
    minr += 1
if minr >= 100:
    minr = 0
    maj += 1

new_ver = f"{maj}.{minr}.{pat:02d}"
new_name = f"{new_ver} {rest}".strip()
data["py/state"]["project_name"] = new_name
path.write_text(json.dumps(data, indent=2) + "\n")
print(f"Updated project_name: {name} -> {new_name}")
PY

cd "$ROOT_DIR"
if command -v pros >/dev/null 2>&1; then
  pros build
  pros upload
else
  echo "pros CLI not found in PATH. Version bumped only."
  echo "Use the VS Code PROS extension to Build/Upload."
fi
