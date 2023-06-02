#!/bin/bash

CMD=./sembackup

rm -rf testdata
rm -rf testdir

mkdir -p testdir
mkdir -p testdata/1
mkdir -p testdata/1/empty
mkdir -p testdata/1/notempty

mkdir -p testdata/1/emptyi
mkdir -p testdata/1/notemptyi

mkdir -p testdata/1/notempty2/empty
mkdir -p testdata/1/notempty2/notemptyi2

echo "testtestasdf1" > testdata/1/notempty/testfile
echo "testtestasdf2" > testdata/1/notemptyi/testfile
echo "testtestasdf3" > testdata/1/notempty2/notempty
echo "testtestasdf4" > testdata/1/notempty2/notemptyi2/test4
echo "testtestasdf5" > testdata/1/notempty2/notemptyi2/test5
echo "testtestasdf6" > testdata/1/notempty2/notemptyi2/test6
echo "testtestasdf7" > testdata/1/notempty2/notemptyi2/test7

echo ".*4" > testdata/1/notempty2/.ignore
echo ".*5" > testdata/1/notempty2/notemptyi2/.ignore
touch testdata/1/notemptyi/.nobackup
touch testdata/1/emptyi/.nobackup

echo "Data created"

if ! $CMD init --repo testdir/to1 --compression zlib --compression-level 4 --encryption aes --password asdff --salt e --full-period 999; then
  echo "Error creating repo!"
  exit 1
fi
echo "Repo created"

OUT=$($CMD run --from testdata/1 --repo testdir/to1 --password asdff --progress simple --verbose 1)
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
OUT=$($CMD list --repo testdir/to1 --password asdff)
echo "$OUT"
while IFS= read -r l; do
  ((i++))
  aid=$(echo $l | grep -Eo '[0-9]+' | tail -1)
  AIDS+=("$aid")
done <<< "$OUT"

OUT=$($CMD list-files --aid ${AIDS[0]}  --from testdata/1 --repo testdir/to1 --password asdff --progress simple --verbose 1)
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

rm -rf testdata
rm -rf testdir

exit 0