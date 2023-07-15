#!/bin/bash

TESTSIZE=16

# TODO: Don't hardcode this
CMD="../../src/backup"

TESTDATADIR=backup

rm -rf "$TESTDATADIR"

mkdir -p "$TESTDATADIR"/testdir
mkdir -p "$TESTDATADIR"/testdata/1
mkdir -p "$TESTDATADIR"/testdata/1/empty
mkdir -p "$TESTDATADIR"/testdata/1/notempty
echo "testtestasdf" > "$TESTDATADIR"/testdata/1/notempty/testfile


dd if=/dev/urandom of="$TESTDATADIR"/testdata/1/a bs=1M count=$TESTSIZE
dd if=/dev/urandom of="$TESTDATADIR"/testdata/1/b bs=1M count=$TESTSIZE
dd if=/dev/urandom of="$TESTDATADIR"/testdata/1/c bs=1M count=$TESTSIZE

ln -s a "$TESTDATADIR"/testdata/1/l
ln -s aa "$TESTDATADIR"/testdata/1/aa

cp -a -p "$TESTDATADIR"/testdata/1 "$TESTDATADIR"/testdata/2
echo "asdf1" > "$TESTDATADIR"/testdata/2/c
cat "$TESTDATADIR"/testdata/1/c >> "$TESTDATADIR"/testdata/2/c

sleep 1

cp -a -p "$TESTDATADIR"/testdata/2 "$TESTDATADIR"/testdata/3
echo "asdf2" > "$TESTDATADIR"/testdata/3/a
cat "$TESTDATADIR"/testdata/2/a >> "$TESTDATADIR"/testdata/3/a
rm "$TESTDATADIR"/testdata/3/l
ln -s b "$TESTDATADIR"/testdata/3/l

cp -a -p "$TESTDATADIR"/testdata/3 "$TESTDATADIR"/testdata/4
echo "asdf3" > "$TESTDATADIR"/testdata/4/b
cat "$TESTDATADIR"/testdata/3/b >> "$TESTDATADIR"/testdata/4/b

cp -a -p "$TESTDATADIR"/testdata/1 "$TESTDATADIR"/testdata/5
cp -a -p "$TESTDATADIR"/testdata/1 "$TESTDATADIR"/testdata/6
rm "$TESTDATADIR"/testdata/6/a
cp -a -p "$TESTDATADIR"/testdata/1 "$TESTDATADIR"/testdata/7

cp -a -p "$TESTDATADIR"/testdata/7 "$TESTDATADIR"/testdata/8
echo "asdf3" > "$TESTDATADIR"/testdata/8/b
cat "$TESTDATADIR"/testdata/7/b >> "$TESTDATADIR"/testdata/8/b

echo "Data created"

if ! $CMD init --repo "$TESTDATADIR"/testdir/to1 --compression zlib --compression-level 4 --encryption aes --password asdff --salt e; then
  echo "Error creating repo!"
  exit 1
fi
echo "Repo created"

OUT=$($CMD run --from "$TESTDATADIR"/testdata/1 --repo "$TESTDATADIR"/testdir/to1 --password asdff --progress simple --verbose 1)
echo "$OUT"
if ! ( ( echo "$OUT" | grep -q 'Copied: a' ) && ( echo "$OUT" | grep -q 'Copied: aa' ) && ( echo "$OUT" | grep -q 'Copied: b' ) &&  ( echo "$OUT" | grep -q 'Copied: c' ) ); then
  echo "Error backing up 1 dir!"
  exit 1
fi
echo "Backup 1 ok"
OUT=$($CMD run --from "$TESTDATADIR"/testdata/2 --repo "$TESTDATADIR"/testdir/to1 --password asdff --progress simple --verbose 1)
echo "$OUT"
if ! ( ( echo "$OUT" | grep -q 'Skipped: a' ) && ( echo "$OUT" | grep -q 'Skipped: b' ) && ( echo "$OUT" | grep -q 'Skipped: aa' ) &&\
( echo "$OUT" | grep -q 'Skipped: empty' ) && ( echo "$OUT" | grep -q 'Skipped: l' ) && ( echo "$OUT" | grep -q 'Skipped: notempty' ) &&  ( echo "$OUT" | grep -q 'Copied: c' ) ); then
  echo "Error backing up 2 dir!"
  exit 1
fi
echo "Backup 2 ok"

OUT=$($CMD run --from "$TESTDATADIR"/testdata/3 --repo "$TESTDATADIR"/testdir/to1 --password asdff --progress simple --verbose 1)
echo "$OUT"
if ! ( ( echo "$OUT" | grep -q 'Copied: a' ) && ( echo "$OUT" | grep -q 'Copied: l' )  && ( echo "$OUT" | grep -q 'Skipped: b' ) &&\
 ( echo "$OUT" | grep -q 'Skipped: empty' )  && ( echo "$OUT" | grep -q 'Skipped: notempty' ) &&  ( echo "$OUT" | grep -q 'Skipped: c' ) ); then
  echo "$OUT"
  echo "Error backing up 3 dir!"
  exit 1
fi
echo "Backup 3 ok"

OUT=$($CMD run --from "$TESTDATADIR"/testdata/4 --repo "$TESTDATADIR"/testdir/to1 --password asdff --progress simple --verbose 1)
echo "$OUT"
if ! ( ( echo "$OUT" | grep -q 'Skipped: a' ) && ( echo "$OUT" | grep -q 'Skipped: aa' ) && ( echo "$OUT" | grep -q 'Copied: b' ) &&  ( echo "$OUT" | grep -q 'Skipped: c' ) ); then
  echo "Error backing up 4 dir!"
  exit 1
fi
echo "Backup 4 ok"

OUT=$($CMD run --from "$TESTDATADIR"/testdata/5 --repo "$TESTDATADIR"/testdir/to1 --password asdff --progress simple --verbose 1)
echo "$OUT"
if ! ( ( echo "$OUT" | grep -q 'Copied: a' ) && ( echo "$OUT" | grep -q 'Copied: b' ) &&  ( echo "$OUT" | grep -q 'Copied: c' ) ); then
  echo "Error backing up 5 dir!"
  exit 1
fi
echo "Backup 5 ok"

OUT=$($CMD run --from "$TESTDATADIR"/testdata/6 --repo "$TESTDATADIR"/testdir/to1 --password asdff --progress simple --verbose 1)
echo "$OUT"
if ! ( ! ( echo "$OUT" | grep -q 'Copied: a' ) && ! ( echo "$OUT" | grep -q 'Skipped: a ' ) &&  ( echo "$OUT" | grep -q 'Skipped: aa' ) && ( echo "$OUT" | grep -q 'Skipped: b' ) &&  ( echo "$OUT" | grep -q 'Skipped: c' ) ); then
  echo "Error backing up 6 dir!"
  exit 1
fi
echo "Backup 6 ok"

OUT=$($CMD run --from "$TESTDATADIR"/testdata/7 --repo "$TESTDATADIR"/testdir/to1 --password asdff --progress simple --verbose 1)
echo "$OUT"
if ! ( ( echo "$OUT" | grep -q 'Skipped: a' ) && ( echo "$OUT" | grep -q 'Skipped: aa' ) && ( echo "$OUT" | grep -q 'Skipped: b' ) &&  ( echo "$OUT" | grep -q 'Skipped: c' ) ); then
  echo "Error backing up 7 dir!"
  exit 1
fi
echo "Backup 7 ok"



i=$((0))
AIDS=()
OUT=$($CMD list --repo "$TESTDATADIR"/testdir/to1 --password asdff)
echo "$OUT"

mkdir "$TESTDATADIR"/testmount
$CMD mount --repo "$TESTDATADIR"/testdir/to1 --password asdff --to "$TESTDATADIR"/testmount &

while IFS= read -r l; do
  ((i++))
  aid=$(echo $l | grep -Eo '[0-9]+' | tail -1)
  AIDS+=("$aid")
  $CMD restore --repo "$TESTDATADIR"/testdir/to1 --password asdff --aid $aid --to "$TESTDATADIR"/testdir/res$((i))
  if ! diff --no-dereference -r "$TESTDATADIR"/testdata/$((i)) "$TESTDATADIR"/testdir/res$((i)); then
    echo "Archive not restored properly!"
    exit 1
  fi
    if ! diff --no-dereference -r "$TESTDATADIR"/testmount/$((aid)) "$TESTDATADIR"/testdir/res$((i)); then
      echo "Archive not mounted properly!"
      exit 1
    fi

  echo "Restore $((i)) OK"
done <<< "$OUT"

umount "$TESTDATADIR"/testmount

OUT=$($CMD diff --from "$TESTDATADIR"/testdata/4 --repo "$TESTDATADIR"/testdir/to1 --password asdff --progress none --verbose 1 --aid ${AIDS[0]} --aid2 ${AIDS[1]})
echo "$OUT"
if ! ( ( echo "$OUT" | grep -q 'c is different' ) && ! ( echo "$OUT" | grep -q 'a is different' ) && ! ( echo "$OUT" | grep -q 'l is different' )\
 && ! ( echo "$OUT" | grep -q 'aa is different' ) &&  ! ( echo "$OUT" | grep -q 'b is different' ) && ! ( echo "$OUT" | grep -q 'empty is different' ) && ! ( echo "$OUT" | grep -q 'notempty is different' ) ); then
  echo "Error comparing archive 1 and 2!"
  exit 1
fi
echo "OK comparing archive 1 and 2"


OUT=$($CMD diff --from "$TESTDATADIR"/testdata/4 --repo "$TESTDATADIR"/testdir/to1 --password asdff --progress none --verbose 1 --aid ${AIDS[1]} --aid2 ${AIDS[2]})
echo "$OUT"
if ! ( ( echo "$OUT" | grep -q 'a is different' ) && ( echo "$OUT" | grep -q 'l is different' ) && ! ( echo "$OUT" | grep -q 'c is different' ) \
&&  ! ( echo "$OUT" | grep -q 'b is different' ) && ! ( echo "$OUT" | grep -q 'empty is different' ) && ! ( echo "$OUT" | grep -q 'notempty is different' ) ); then
  echo "Error comparing archive 2 and 3!"
  exit 1
fi
echo "OK comparing archive 2 and 3"

OUT=$($CMD diff --diff-mode file --from "$TESTDATADIR"/testdata/4 --prefix a --repo "$TESTDATADIR"/testdir/to1 --password asdff --progress none --verbose 1 --aid ${AIDS[1]} --aid2 ${AIDS[2]})
echo "$OUT"
if ! ( ( echo "$OUT" | grep -q 'a is different' ) && ! ( echo "$OUT" | grep -q 'l is different' ) ); then
  echo "Error comparing archive 2 and 3! (file 1)"
  exit 1
fi
echo "OK comparing archive 2 and 3 (file 1)"

OUT=$($CMD diff --diff-mode file --from "$TESTDATADIR"/testdata/4 --prefix l --repo "$TESTDATADIR"/testdir/to1 --password asdff --progress none --verbose 1 --aid ${AIDS[1]} --aid2 ${AIDS[2]})
echo "$OUT"
if ! ( ! ( echo "$OUT" | grep -q 'a is different' ) && ( echo "$OUT" | grep -q 'l is different' ) ); then
  echo "Error comparing archive 2 and 3! (file 2)"
  exit 1
fi
echo "OK comparing archive 2 and 3 (file 2)"

OUT=$($CMD diff --diff-mode file --from "$TESTDATADIR"/testdata/4 --prefix b --repo "$TESTDATADIR"/testdir/to1 --password asdff --progress none --verbose 1 --aid ${AIDS[1]} --aid2 ${AIDS[2]})
echo "$OUT"
if ! ( ! ( echo "$OUT" | grep -q 'a is different' ) &&  ! ( echo "$OUT" | grep -q 'l is different' ) &&  ( echo "$OUT" | grep -q 'b are same' )  ); then
  echo "Error comparing archive 2 and 3! (file 3)"
  exit 1
fi
echo "OK comparing archive 2 and 3 (file 3)"

OUT=$($CMD diff --from "$TESTDATADIR"/testdata/4 --repo "$TESTDATADIR"/testdir/to1 --password asdff --progress none --verbose 1 --aid ${AIDS[2]})
echo "$OUT"
if ! ( ( echo "$OUT" | grep -q 'b is different' ) && ! ( echo "$OUT" | grep -q 'a is different' ) && ! ( echo "$OUT" | grep -q 'l is different' ) \
&& ! ( echo "$OUT" | grep -q 'aa is different' ) &&  ! ( echo "$OUT" | grep -q 'c is different' ) && ! ( echo "$OUT" | grep -q 'empty is different' ) && ! ( echo "$OUT" | grep -q 'notempty is different' ) ); then
  echo "Error comparing archive 3 and current!"
  exit 1
fi
echo "OK comparing archive 3 and current"

OUT=$($CMD diff --from "$TESTDATADIR"/testdata/3 --repo "$TESTDATADIR"/testdir/to1 --password asdff --progress none --verbose 1 )
echo "$OUT"
if ! ( ! ( echo "$OUT" | grep -q 'b is different' ) && ( echo "$OUT" | grep -q 'a is different' ) && ( echo "$OUT" | grep -q 'l is different' )\
 && ! ( echo "$OUT" | grep -q 'aa is different' ) &&  ( echo "$OUT" | grep -q 'c is different' ) && ! ( echo "$OUT" | grep -q 'empty is different' ) && ! ( echo "$OUT" | grep -q 'notempty is different' ) ); then
  echo "Error comparing archive last and current!"
  exit 1
fi
echo "OK comparing archive last and current"


if ! $CMD init --repo "$TESTDATADIR"/testdir/to2 --compression zlib --compression-level 4 --encryption aes --password asdff --salt e; then
  echo "Error creating repo!"
  exit 1
fi
echo "Repo created"

OUT=$($CMD run --from "$TESTDATADIR"/testdata/1 --repo "$TESTDATADIR"/testdir/to2 --password asdff --progress simple --verbose 1)
echo "$OUT"
if ! ( ( echo "$OUT" | grep -q 'Copied: a' ) && ( echo "$OUT" | grep -q 'Copied: aa' ) && ( echo "$OUT" | grep -q 'Copied: b' ) &&  ( echo "$OUT" | grep -q 'Copied: c' ) ); then
  echo "Error backing up 1 dir!"
  exit 1
fi
echo "Backup 1 ok"

OUT=$($CMD run --from "$TESTDATADIR"/testdata/1 --repo "$TESTDATADIR"/testdir/to2 --password asdff --progress simple --verbose 1)
echo "$OUT"
if ! (  ( echo "$OUT" | grep -q 'Skipped: a' ) && ( echo "$OUT" | grep -q 'Skipped: aa' ) && ( echo "$OUT" | grep -q 'Skipped: b' ) &&  ( echo "$OUT" | grep -q 'Skipped: c' )); then
  echo "Error backing up 2 dir!"
  exit 1
fi
echo "Backup 2 ok"

OUT=$($CMD run --from "$TESTDATADIR"/testdata/1 --repo "$TESTDATADIR"/testdir/to2 --password asdff --progress simple --verbose 1)
echo "$OUT"
if ! (  ( echo "$OUT" | grep -q 'Skipped: a' ) && ( echo "$OUT" | grep -q 'Skipped: aa' ) && ( echo "$OUT" | grep -q 'Skipped: b' ) &&  ( echo "$OUT" | grep -q 'Skipped: c' ) ); then
  echo "Error backing up 3 dir!"
  exit 1
fi
echo "Backup 3 ok"

OUT=$($CMD run --from "$TESTDATADIR"/testdata/1 --repo "$TESTDATADIR"/testdir/to2 --password asdff --progress simple --verbose 1)
echo "$OUT"
if ! ( ( echo "$OUT" | grep -q 'Skipped: a' ) && ( echo "$OUT" | grep -q 'Skipped: aa' ) && ( echo "$OUT" | grep -q 'Skipped: b' ) &&  ( echo "$OUT" | grep -q 'Skipped: c' )); then
  echo "Error backing up 4 dir!"
  exit 1
fi
echo "Backup 4 ok"

OUT=$($CMD run --from "$TESTDATADIR"/testdata/1 --repo "$TESTDATADIR"/testdir/to2 --password asdff --progress simple --verbose 1)
echo "$OUT"
if ! (  ( echo "$OUT" | grep -q 'Skipped: a' ) && ( echo "$OUT" | grep -q 'Skipped: aa' ) && ( echo "$OUT" | grep -q 'Skipped: b' ) &&  ( echo "$OUT" | grep -q 'Skipped: c' ) ); then
  echo "Error backing up 5 dir!"
  exit 1
fi
echo "Backup 5 ok"

OUT=$($CMD run --from "$TESTDATADIR"/testdata/1 --repo "$TESTDATADIR"/testdir/to2 --password asdff --progress simple --verbose 1)
echo "$OUT"
if ! (  ( echo "$OUT" | grep -q 'Skipped: a' ) && ( echo "$OUT" | grep -q 'Skipped: aa' ) && ( echo "$OUT" | grep -q 'Skipped: b' ) &&  ( echo "$OUT" | grep -q 'Skipped: c' ) ); then
  echo "Error backing up 6 dir!"
  exit 1
fi
echo "Backup 6 ok"

OUT=$($CMD run --from "$TESTDATADIR"/testdata/1 --repo "$TESTDATADIR"/testdir/to2 --password asdff --progress simple --verbose 1)
echo "$OUT"
if ! (  ( echo "$OUT" | grep -q 'Skipped: a' ) && ( echo "$OUT" | grep -q 'Skipped: aa' ) && ( echo "$OUT" | grep -q 'Skipped: b' ) &&  ( echo "$OUT" | grep -q 'Skipped: c' ) ); then
  echo "Error backing up 7 dir!"
  exit 1
fi
echo "Backup 7 ok"

OUT=$($CMD run --from "$TESTDATADIR"/testdata/1 --repo "$TESTDATADIR"/testdir/to2 --password asdff --progress simple --verbose 1)
echo "$OUT"
if ! ( ( echo "$OUT" | grep -q 'Skipped: a' ) && ( echo "$OUT" | grep -q 'Skipped: aa' ) && ( echo "$OUT" | grep -q 'Skipped: b' ) &&  ( echo "$OUT" | grep -q 'Skipped: c' )); then
  echo "Error backing up 8 dir!"
  exit 1
fi
echo "Backup 8 ok"

OUT=$($CMD run --from "$TESTDATADIR"/testdata/1 --repo "$TESTDATADIR"/testdir/to2 --password asdff --progress simple --verbose 1)
echo "$OUT"
if ! (  ( echo "$OUT" | grep -q 'Skipped: a' ) && ( echo "$OUT" | grep -q 'Skipped: aa' ) && ( echo "$OUT" | grep -q 'Skipped: b' ) &&  ( echo "$OUT" | grep -q 'Skipped: c' ) ); then
  echo "Error backing up 9 dir!"
  exit 1
fi
echo "Backup 9 ok"

OUT=$($CMD run --from "$TESTDATADIR"/testdata/1 --repo "$TESTDATADIR"/testdir/to2 --password asdff --progress simple --verbose 1)
echo "$OUT"
if ! (  ( echo "$OUT" | grep -q 'Skipped: a' ) && ( echo "$OUT" | grep -q 'Skipped: aa' ) && ( echo "$OUT" | grep -q 'Skipped: b' ) &&  ( echo "$OUT" | grep -q 'Skipped: c' ) ); then
  echo "Error backing up 10 dir!"
  exit 1
fi
echo "Backup 10 ok"

OUT=$($CMD run --from "$TESTDATADIR"/testdata/1 --repo "$TESTDATADIR"/testdir/to2 --password asdff --progress simple --verbose 1)
echo "$OUT"
if ! ( ( echo "$OUT" | grep -q 'Skipped: a' ) && ( echo "$OUT" | grep -q 'Skipped: aa' ) && ( echo "$OUT" | grep -q 'Skipped: b' ) &&  ( echo "$OUT" | grep -q 'Skipped: c' ) ); then
  echo "Error backing up 11 dir!"
  exit 1
fi
echo "Backup 11 ok"

rm -rf "$TESTDATADIR"

exit 0