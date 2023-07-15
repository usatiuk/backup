#!/bin/bash

# TODO: Don't hardcode this
CMD="../../src/backup"

TESTDATADIR=ignore

rm -rf "$TESTDATADIR"

mkdir -p "$TESTDATADIR"/testdir
mkdir -p "$TESTDATADIR"/testdata/1
mkdir -p "$TESTDATADIR"/testdata/1/empty
mkdir -p "$TESTDATADIR"/testdata/1/notempty

mkdir -p "$TESTDATADIR"/testdata/1/emptyi
mkdir -p "$TESTDATADIR"/testdata/1/notemptyi

mkdir -p "$TESTDATADIR"/testdata/1/notempty2/empty
mkdir -p "$TESTDATADIR"/testdata/1/notempty2/notemptyi2

echo "testtestasdf1" > "$TESTDATADIR"/testdata/1/notempty/testfile
echo "testtestasdf2" > "$TESTDATADIR"/testdata/1/notemptyi/testfile
echo "testtestasdf3" > "$TESTDATADIR"/testdata/1/notempty2/notempty
echo "testtestasdf4" > "$TESTDATADIR"/testdata/1/notempty2/notemptyi2/test4
echo "testtestasdf5" > "$TESTDATADIR"/testdata/1/notempty2/notemptyi2/test5
echo "testtestasdf6" > "$TESTDATADIR"/testdata/1/notempty2/notemptyi2/test6
echo "testtestasdf7" > "$TESTDATADIR"/testdata/1/notempty2/notemptyi2/test7

echo ".*4" > "$TESTDATADIR"/testdata/1/notempty2/.ignore
echo ".*5" > "$TESTDATADIR"/testdata/1/notempty2/notemptyi2/.ignore
touch "$TESTDATADIR"/testdata/1/notemptyi/.nobackup
touch "$TESTDATADIR"/testdata/1/emptyi/.nobackup

echo "Data created"

if ! $CMD init --repo "$TESTDATADIR"/testdir/to1 --compression zlib --compression-level 4 --encryption aes --password asdff --salt e; then
  echo "Error creating repo!"
  exit 1
fi
echo "Repo created"

OUT=$($CMD run --from "$TESTDATADIR"/testdata/1 --repo "$TESTDATADIR"/testdir/to1 --password asdff --progress simple --verbose 1)
echo "$OUT"
if ! ( ( echo "$OUT" | grep -q 'Copied: notempty/testfile' )\
    && ( echo "$OUT" | grep -q 'Copied: notempty' )\
    && ( echo "$OUT" | grep -q 'Copied: notempty2' )\
    && ( echo "$OUT" | grep -q 'Copied: notempty2/notempty' )  \
    && ! ( echo "$OUT" | grep -q 'notempty2/notemptyi2/test4 ' )\
    && ! ( echo "$OUT" | grep -q 'notempty2/notemptyi2/test5 ' )\
    && ! ( echo "$OUT" | grep -q 'notemptyi ' )\
    && ! ( echo "$OUT" | grep -q 'emptyi ' )\
    && ! ( echo "$OUT" | grep -q 'notemptyi/testfile' )\
    && ( echo "$OUT" | grep -q 'Copied: notempty2/notemptyi2/test6' )\
    && ( echo "$OUT" | grep -q 'Copied: notempty2/notemptyi2/test7' ) ); then
  echo "Error backing up 1 dir!"
  exit 1
fi
echo "Backup 1 ok"

i=$((0))
AIDS=()
OUT=$($CMD list --repo "$TESTDATADIR"/testdir/to1 --password asdff)
echo "$OUT"
while IFS= read -r l; do
  ((i++))
  aid=$(echo $l | grep -Eo '[0-9]+' | tail -1)
  AIDS+=("$aid")
done <<< "$OUT"

OUT=$($CMD list-files --aid ${AIDS[0]}  --from "$TESTDATADIR"/testdata/1 --repo "$TESTDATADIR"/testdir/to1 --password asdff --progress simple --verbose 1)
echo "$OUT"
if ! ( ( echo "$OUT" | grep -q ' notempty/testfile' )\
    && ( echo "$OUT" | grep -q ' notempty' )\
    && ( echo "$OUT" | grep -q ' notempty2' )\
    && ( echo "$OUT" | grep -q ' notempty2/notempty' )  \
    && ! ( echo "$OUT" | grep -q 'notempty2/notemptyi2/test4 ' )\
    && ! ( echo "$OUT" | grep -q 'notempty2/notemptyi2/test5 ' )\
    && ! ( echo "$OUT" | grep -q 'notemptyi ' )\
    && ! ( echo "$OUT" | grep -q 'emptyi ' )\
    && ! ( echo "$OUT" | grep -q 'notemptyi/testfile' )\
    && ( echo "$OUT" | grep -q ' notempty2/notemptyi2/test6' )\
    && ( echo "$OUT" | grep -q ' notempty2/notemptyi2/test7' ) ); then
  echo "Error listing 1 dir!"
  exit 1
fi
echo "List 1 ok"

rm -rf "$"$TESTDATADIR"/testdataDIR"

exit 0