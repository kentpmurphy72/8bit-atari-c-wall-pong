#!/bin/sh
set -eu
cd "$(dirname "$0")/.."
revision=555282497c3ecf8b313d87d5973093af19c35bd5
mkdir -p .tools
if [ ! -d .tools/cc65 ]; then
    git clone --depth 1 --branch V2.19 https://github.com/cc65/cc65.git .tools/cc65
fi
actual=$(git -C .tools/cc65 rev-parse HEAD)
if [ "$actual" != "$revision" ]; then
    echo "Unexpected cc65 revision; expected $revision, got $actual" >&2
    exit 1
fi
make -C .tools/cc65 -j2 > .tools/cc65-build.log 2>&1 || {
    tail -60 .tools/cc65-build.log
    exit 1
}
echo 'cc65 ready. Run make and make test-6502.'
