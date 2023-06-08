#!/bin/bash

TESTSIZE=16

# TODO: Don't hardcode this
CMD="../../src/backup"

rm -rf testdata
rm -rf testdir

mkdir -p testdir
mkdir -p testdata/1
mkdir -p testdata/1/empty
mkdir -p testdata/1/notempty
echo "testtestasdf" > testdata/1/notempty/testfile


dd if=/dev/urandom of=testdata/1/a bs=1M count=$TESTSIZE
dd if=/dev/urandom of=testdata/1/b bs=1M count=$TESTSIZE
dd if=/dev/urandom of=testdata/1/c bs=1M count=$TESTSIZE

ln -s a testdata/1/l
ln -s aa testdata/1/aa

cp -a -p testdata/1 testdata/2
echo "asdf1" > testdata/2/c
cat testdata/1/c >> testdata/2/c

sleep 1

cp -a -p testdata/2 testdata/3
echo "asdf2" > testdata/3/a
cat testdata/2/a >> testdata/3/a
rm testdata/3/l
ln -s b testdata/3/l

cp -a -p testdata/3 testdata/4
echo "asdf3" > testdata/4/b
cat testdata/3/b >> testdata/4/b

cp -a -p testdata/1 testdata/5
cp -a -p testdata/1 testdata/6
rm testdata/6/a
cp -a -p testdata/1 testdata/7

cp -a -p testdata/7 testdata/8
echo "asdf3" > testdata/8/b
cat testdata/7/b >> testdata/8/b

echo "Data created"

if ! $CMD init --repo testdir/to1 --compression zlib --compression-level 4 --encryption aes --password asdff --salt e --full-period 999; then
  echo "Error creating repo!"
  exit 1
fi
echo "Repo created"

OUT=$($CMD run --from testdata/1 --repo testdir/to1 --password asdff --progress simple --verbose 1)
echo "$OUT"
if ! ( ( echo "$OUT" | grep -q 'Copied: a' ) && ( echo "$OUT" | grep -q 'Copied: aa' ) && ( echo "$OUT" | grep -q 'Copied: b' ) &&  ( echo "$OUT" | grep -q 'Copied: c' ) ); then
  echo "Error backing up 1 dir!"
  exit 1
fi
echo "Backup 1 ok"
OUT=$($CMD run --from testdata/2 --repo testdir/to1 --password asdff --progress simple --verbose 1)
echo "$OUT"
if ! ( ( echo "$OUT" | grep -q 'Skipped: a' ) && ( echo "$OUT" | grep -q 'Skipped: b' ) && ( echo "$OUT" | grep -q 'Skipped: aa' ) &&\
( echo "$OUT" | grep -q 'Skipped: empty' ) && ( echo "$OUT" | grep -q 'Skipped: l' ) && ( echo "$OUT" | grep -q 'Skipped: notempty' ) &&  ( echo "$OUT" | grep -q 'Copied: c' ) ); then
  echo "Error backing up 2 dir!"
  exit 1
fi
echo "Backup 2 ok"

OUT=$($CMD run --from testdata/3 --repo testdir/to1 --password asdff --progress simple --verbose 1)
echo "$OUT"
if ! ( ( echo "$OUT" | grep -q 'Copied: a' ) && ( echo "$OUT" | grep -q 'Copied: l' )  && ( echo "$OUT" | grep -q 'Skipped: b' ) &&\
 ( echo "$OUT" | grep -q 'Skipped: empty' )  && ( echo "$OUT" | grep -q 'Skipped: notempty' ) &&  ( echo "$OUT" | grep -q 'Skipped: c' ) ); then
  echo "$OUT"
  echo "Error backing up 3 dir!"
  exit 1
fi
echo "Backup 3 ok"

OUT=$($CMD run --from testdata/4 --repo testdir/to1 --password asdff --progress simple --verbose 1)
echo "$OUT"
if ! ( ( echo "$OUT" | grep -q 'Skipped: a' ) && ( echo "$OUT" | grep -q 'Skipped: aa' ) && ( echo "$OUT" | grep -q 'Copied: b' ) &&  ( echo "$OUT" | grep -q 'Skipped: c' ) ); then
  echo "Error backing up 4 dir!"
  exit 1
fi
echo "Backup 4 ok"

OUT=$($CMD run --from testdata/5 --repo testdir/to1 --password asdff --progress simple --verbose 1)
echo "$OUT"
if ! ( ( echo "$OUT" | grep -q 'Copied: a' ) && ( echo "$OUT" | grep -q 'Copied: b' ) &&  ( echo "$OUT" | grep -q 'Copied: c' ) ); then
  echo "Error backing up 5 dir!"
  exit 1
fi
echo "Backup 5 ok"

OUT=$($CMD run --from testdata/6 --repo testdir/to1 --password asdff --progress simple --verbose 1)
echo "$OUT"
if ! ( ! ( echo "$OUT" | grep -q 'Copied: a' ) && ! ( echo "$OUT" | grep -q 'Skipped: a ' ) &&  ( echo "$OUT" | grep -q 'Skipped: aa' ) && ( echo "$OUT" | grep -q 'Skipped: b' ) &&  ( echo "$OUT" | grep -q 'Skipped: c' ) ); then
  echo "Error backing up 6 dir!"
  exit 1
fi
echo "Backup 6 ok"

OUT=$($CMD run --from testdata/7 --repo testdir/to1 --password asdff --progress simple --verbose 1)
echo "$OUT"
if ! ( ( echo "$OUT" | grep -q 'Copied: a' ) && ( echo "$OUT" | grep -q 'Skipped: aa' ) && ( echo "$OUT" | grep -q 'Skipped: b' ) &&  ( echo "$OUT" | grep -q 'Skipped: c' ) ); then
  echo "Error backing up 7 dir!"
  exit 1
fi
echo "Backup 7 ok"



i=$((0))
AIDS=()
OUT=$($CMD list --repo testdir/to1 --password asdff)
echo "$OUT"

mkdir testmount
$CMD mount --repo testdir/to1 --password asdff --to testmount &

while IFS= read -r l; do
  ((i++))
  aid=$(echo $l | grep -Eo '[0-9]+' | tail -1)
  AIDS+=("$aid")
  $CMD restore --repo testdir/to1 --password asdff --aid $aid --to testdir/res$((i))
  if ! diff --no-dereference -r testdata/$((i)) testdir/res$((i)); then
    echo "Archive not restored properly!"
    exit 1
  fi
    if ! diff --no-dereference -r testmount/$((aid)) testdir/res$((i)); then
      echo "Archive not mounted properly!"
      exit 1
    fi

  echo "Restore $((i)) OK"
done <<< "$OUT"

umount testmount

OUT=$($CMD diff --from testdata/4 --repo testdir/to1 --password asdff --progress none --verbose 1 --aid ${AIDS[0]} --aid2 ${AIDS[1]})
echo "$OUT"
if ! ( ( echo "$OUT" | grep -q 'c is different' ) && ! ( echo "$OUT" | grep -q 'a is different' ) && ! ( echo "$OUT" | grep -q 'l is different' )\
 && ! ( echo "$OUT" | grep -q 'aa is different' ) &&  ! ( echo "$OUT" | grep -q 'b is different' ) && ! ( echo "$OUT" | grep -q 'empty is different' ) && ! ( echo "$OUT" | grep -q 'notempty is different' ) ); then
  echo "Error comparing archive 1 and 2!"
  exit 1
fi
echo "OK comparing archive 1 and 2"


OUT=$($CMD diff --from testdata/4 --repo testdir/to1 --password asdff --progress none --verbose 1 --aid ${AIDS[1]} --aid2 ${AIDS[2]})
echo "$OUT"
if ! ( ( echo "$OUT" | grep -q 'a is different' ) && ( echo "$OUT" | grep -q 'l is different' ) && ! ( echo "$OUT" | grep -q 'c is different' ) \
&&  ! ( echo "$OUT" | grep -q 'b is different' ) && ! ( echo "$OUT" | grep -q 'empty is different' ) && ! ( echo "$OUT" | grep -q 'notempty is different' ) ); then
  echo "Error comparing archive 2 and 3!"
  exit 1
fi
echo "OK comparing archive 2 and 3"

OUT=$($CMD diff --diff-mode file --from testdata/4 --prefix a --repo testdir/to1 --password asdff --progress none --verbose 1 --aid ${AIDS[1]} --aid2 ${AIDS[2]})
echo "$OUT"
if ! ( ( echo "$OUT" | grep -q 'a is different' ) && ! ( echo "$OUT" | grep -q 'l is different' ) ); then
  echo "Error comparing archive 2 and 3! (file 1)"
  exit 1
fi
echo "OK comparing archive 2 and 3 (file 1)"

OUT=$($CMD diff --diff-mode file --from testdata/4 --prefix l --repo testdir/to1 --password asdff --progress none --verbose 1 --aid ${AIDS[1]} --aid2 ${AIDS[2]})
echo "$OUT"
if ! ( ! ( echo "$OUT" | grep -q 'a is different' ) && ( echo "$OUT" | grep -q 'l is different' ) ); then
  echo "Error comparing archive 2 and 3! (file 2)"
  exit 1
fi
echo "OK comparing archive 2 and 3 (file 2)"

OUT=$($CMD diff --diff-mode file --from testdata/4 --prefix b --repo testdir/to1 --password asdff --progress none --verbose 1 --aid ${AIDS[1]} --aid2 ${AIDS[2]})
echo "$OUT"
if ! ( ! ( echo "$OUT" | grep -q 'a is different' ) &&  ! ( echo "$OUT" | grep -q 'l is different' ) &&  ( echo "$OUT" | grep -q 'b are same' )  ); then
  echo "Error comparing archive 2 and 3! (file 3)"
  exit 1
fi
echo "OK comparing archive 2 and 3 (file 3)"

OUT=$($CMD diff --from testdata/4 --repo testdir/to1 --password asdff --progress none --verbose 1 --aid ${AIDS[2]})
echo "$OUT"
if ! ( ( echo "$OUT" | grep -q 'b is different' ) && ! ( echo "$OUT" | grep -q 'a is different' ) && ! ( echo "$OUT" | grep -q 'l is different' ) \
&& ! ( echo "$OUT" | grep -q 'aa is different' ) &&  ! ( echo "$OUT" | grep -q 'c is different' ) && ! ( echo "$OUT" | grep -q 'empty is different' ) && ! ( echo "$OUT" | grep -q 'notempty is different' ) ); then
  echo "Error comparing archive 3 and current!"
  exit 1
fi
echo "OK comparing archive 3 and current"

OUT=$($CMD diff --from testdata/3 --repo testdir/to1 --password asdff --progress none --verbose 1 )
echo "$OUT"
if ! ( ! ( echo "$OUT" | grep -q 'b is different' ) && ( echo "$OUT" | grep -q 'a is different' ) && ( echo "$OUT" | grep -q 'l is different' )\
 && ! ( echo "$OUT" | grep -q 'aa is different' ) &&  ( echo "$OUT" | grep -q 'c is different' ) && ! ( echo "$OUT" | grep -q 'empty is different' ) && ! ( echo "$OUT" | grep -q 'notempty is different' ) ); then
  echo "Error comparing archive last and current!"
  exit 1
fi
echo "OK comparing archive last and current"


if ! $CMD init --repo testdir/to2 --compression zlib --compression-level 4 --encryption aes --password asdff --salt e; then
  echo "Error creating repo!"
  exit 1
fi
echo "Repo created"

OUT=$($CMD run --from testdata/1 --repo testdir/to2 --password asdff --progress simple --verbose 1)
echo "$OUT"
if ! ( (echo "$OUT" | grep -q 'Backup is full because there are no backups')&&( echo "$OUT" | grep -q 'Copied: a' ) && ( echo "$OUT" | grep -q 'Copied: aa' ) && ( echo "$OUT" | grep -q 'Copied: b' ) &&  ( echo "$OUT" | grep -q 'Copied: c' ) ); then
  echo "Error backing up 1 dir!"
  exit 1
fi
echo "Backup 1 ok"

OUT=$($CMD run --from testdata/1 --repo testdir/to2 --password asdff --progress simple --verbose 1)
echo "$OUT"
if ! ( ! ( echo "$OUT" | grep -q 'Backup is full' ) && ( echo "$OUT" | grep -q 'Skipped: a' ) && ( echo "$OUT" | grep -q 'Skipped: aa' ) && ( echo "$OUT" | grep -q 'Skipped: b' ) &&  ( echo "$OUT" | grep -q 'Skipped: c' )); then
  echo "Error backing up 2 dir!"
  exit 1
fi
echo "Backup 2 ok"

OUT=$($CMD run --from testdata/1 --repo testdir/to2 --password asdff --progress simple --verbose 1)
echo "$OUT"
if ! ( ! ( echo "$OUT" | grep -q 'Backup is full' )&& ( echo "$OUT" | grep -q 'Skipped: a' ) && ( echo "$OUT" | grep -q 'Skipped: aa' ) && ( echo "$OUT" | grep -q 'Skipped: b' ) &&  ( echo "$OUT" | grep -q 'Skipped: c' ) ); then
  echo "Error backing up 3 dir!"
  exit 1
fi
echo "Backup 3 ok"

OUT=$($CMD run --from testdata/1 --repo testdir/to2 --password asdff --progress simple --verbose 1)
echo "$OUT"
if ! ( ( echo "$OUT" | grep -q 'Backup is full because of the interval') &&( echo "$OUT" | grep -q 'Copied: a' ) && ( echo "$OUT" | grep -q 'Copied: aa' ) && ( echo "$OUT" | grep -q 'Copied: b' ) &&  ( echo "$OUT" | grep -q 'Copied: c' )); then
  echo "Error backing up 4 dir!"
  exit 1
fi
echo "Backup 4 ok"

OUT=$($CMD run --from testdata/1 --repo testdir/to2 --password asdff --progress simple --verbose 1)
echo "$OUT"
if ! ( ! ( echo "$OUT" | grep -q 'Backup is full' ) && ( echo "$OUT" | grep -q 'Skipped: a' ) && ( echo "$OUT" | grep -q 'Skipped: aa' ) && ( echo "$OUT" | grep -q 'Skipped: b' ) &&  ( echo "$OUT" | grep -q 'Skipped: c' ) ); then
  echo "Error backing up 5 dir!"
  exit 1
fi
echo "Backup 5 ok"

OUT=$($CMD run --from testdata/1 --repo testdir/to2 --password asdff --progress simple --verbose 1)
echo "$OUT"
if ! ( ! ( echo "$OUT" | grep -q 'Backup is full' ) && ( echo "$OUT" | grep -q 'Skipped: a' ) && ( echo "$OUT" | grep -q 'Skipped: aa' ) && ( echo "$OUT" | grep -q 'Skipped: b' ) &&  ( echo "$OUT" | grep -q 'Skipped: c' ) ); then
  echo "Error backing up 6 dir!"
  exit 1
fi
echo "Backup 6 ok"

OUT=$($CMD run --from testdata/1 --repo testdir/to2 --password asdff --progress simple --verbose 1)
echo "$OUT"
if ! (  ( echo "$OUT" | grep -q 'Backup is full because of the interval' ) &&( echo "$OUT" | grep -q 'Copied: a' ) && ( echo "$OUT" | grep -q 'Copied: aa' ) && ( echo "$OUT" | grep -q 'Copied: b' ) &&  ( echo "$OUT" | grep -q 'Copied: c' ) ); then
  echo "Error backing up 7 dir!"
  exit 1
fi
echo "Backup 7 ok"

OUT=$($CMD run --from testdata/1 --repo testdir/to2 --password asdff --progress simple --verbose 1 --type full)
echo "$OUT"
if ! ( ( echo "$OUT" | grep -q 'Backup is full because of the config') &&( echo "$OUT" | grep -q 'Copied: a' ) && ( echo "$OUT" | grep -q 'Copied: aa' ) && ( echo "$OUT" | grep -q 'Copied: b' ) &&  ( echo "$OUT" | grep -q 'Copied: c' )); then
  echo "Error backing up 8 dir!"
  exit 1
fi
echo "Backup 8 ok"

OUT=$($CMD run --from testdata/1 --repo testdir/to2 --password asdff --progress simple --verbose 1)
echo "$OUT"
if ! ( ! ( echo "$OUT" | grep -q 'Backup is full' ) && ( echo "$OUT" | grep -q 'Skipped: a' ) && ( echo "$OUT" | grep -q 'Skipped: aa' ) && ( echo "$OUT" | grep -q 'Skipped: b' ) &&  ( echo "$OUT" | grep -q 'Skipped: c' ) ); then
  echo "Error backing up 9 dir!"
  exit 1
fi
echo "Backup 9 ok"

OUT=$($CMD run --from testdata/1 --repo testdir/to2 --password asdff --progress simple --verbose 1)
echo "$OUT"
if ! ( ! ( echo "$OUT" | grep -q 'Backup is full' ) && ( echo "$OUT" | grep -q 'Skipped: a' ) && ( echo "$OUT" | grep -q 'Skipped: aa' ) && ( echo "$OUT" | grep -q 'Skipped: b' ) &&  ( echo "$OUT" | grep -q 'Skipped: c' ) ); then
  echo "Error backing up 10 dir!"
  exit 1
fi
echo "Backup 10 ok"

OUT=$($CMD run --from testdata/1 --repo testdir/to2 --password asdff --progress simple --verbose 1)
echo "$OUT"
if ! (  ( echo "$OUT" | grep -q 'Backup is full because of the interval' ) &&( echo "$OUT" | grep -q 'Copied: a' ) && ( echo "$OUT" | grep -q 'Copied: aa' ) && ( echo "$OUT" | grep -q 'Copied: b' ) &&  ( echo "$OUT" | grep -q 'Copied: c' ) ); then
  echo "Error backing up 11 dir!"
  exit 1
fi
echo "Backup 11 ok"

rm -rf testdata
rm -rf testdir
rm -rf testmount

exit 0