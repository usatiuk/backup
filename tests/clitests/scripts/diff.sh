#!/bin/bash

# TODO: Don't hardcode this
CMD="../../src/backup"

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
mkdir -p testdata/1/notempty2/notemptyi2/ignoredir

echo "testtestasdf1" > testdata/1/notempty/testfile
echo "testtestasdf2" > testdata/1/notemptyi/testfile
echo "testtestasdf3" > testdata/1/notempty2/notempty
echo "testtestasdf4" > testdata/1/notempty2/notemptyi2/test4
echo "testtestasdf5" > testdata/1/notempty2/notemptyi2/test5
echo "testtestasdf6" > testdata/1/notempty2/notemptyi2/test6
echo "testtestasdf7" > testdata/1/notempty2/notemptyi2/test7
echo "testtestasdf8" > testdata/1/notempty2/notemptyi2/ignoredir/testa
echo "testtestasdf9" > testdata/1/filexd

echo ".*4" > testdata/1/notempty2/.ignore
echo ".*5" > testdata/1/notempty2/notemptyi2/.ignore
touch testdata/1/notemptyi/.nobackup
touch testdata/1/emptyi/.nobackup
touch testdata/1/notempty2/notemptyi2/ignoredir/.nobackup

cp -a -p testdata/1 testdata/2
echo "testtestasdf4u" > testdata/2/notempty2/notemptyi2/test4
echo "testtestasdf5u" > testdata/2/notempty2/notemptyi2/test5
echo "testtestasdf6u" > testdata/2/notempty2/notemptyi2/test6
echo "testtestasdf7u" > testdata/2/notempty2/notemptyi2/test7
echo "testtestasdf8u" > testdata/2/notempty2/notemptyi2/ignoredir/testa
echo "testtestasdf9u" > testdata/2/filexd

cp -a -p testdata/2 testdata/3
echo "testtestasdf4uu" > testdata/3/notempty2/notemptyi2/test4
echo "testtestasdf5uu" > testdata/3/notempty2/notemptyi2/test5
echo "testtestasdf6uu" > testdata/3/notempty2/notemptyi2/test6
echo "testtestasdf7uu" > testdata/3/notempty2/notemptyi2/test7
echo "testtestasdf8uu" > testdata/3/notempty2/notemptyi2/ignoredir/testa
echo "testtestasdf9uu" > testdata/3/filexd

cp -a -p testdata/2 testdata/4
echo "testtestasdf4uuu" > testdata/4/notempty2/notemptyi2/test4
echo "testtestasdf5uuu" > testdata/4/notempty2/notemptyi2/test5
echo "testtestasdf6uuu" > testdata/4/notempty2/notemptyi2/test6
echo "testtestasdf7uuu" > testdata/4/notempty2/notemptyi2/test7
echo "testtestasdf8uuu" > testdata/4/notempty2/notemptyi2/ignoredir/testa
echo "testtestasdf9uuu" > testdata/4/filexd

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
    && ( echo "$OUT" | grep -q 'Copied: filexd' )  \
    && ! ( echo "$OUT" | grep -q 'notempty2/notemptyi2/test4 ' )\
    && ! ( echo "$OUT" | grep -q 'notempty2/notemptyi2/test5 ' )\
    && ! ( echo "$OUT" | grep -q 'notemptyi ' )\
    && ! ( echo "$OUT" | grep -q 'emptyi ' )\
    && ! ( echo "$OUT" | grep -q 'notemptyi/testfile' )\
    && ! ( echo "$OUT" | grep -q 'notemptyi2/ignoredir/testa' )\
    && ( echo "$OUT" | grep -q 'Copied: notempty2/notemptyi2/test6' )\
    && ( echo "$OUT" | grep -q 'Copied: notempty2/notemptyi2/test7' ) ); then
  echo "Error backing up 1 dir!"
  exit 1
fi
echo "Backup 1 ok"

OUT=$($CMD run --from testdata/2 --repo testdir/to1 --password asdff --progress simple --verbose 1)
echo "$OUT"
if ! ( ! ( echo "$OUT" | grep -q 'notempty2/notemptyi2/test4 ' )\
    && ! ( echo "$OUT" | grep -q 'notempty2/notemptyi2/test5 ' )\
    && ! ( echo "$OUT" | grep -q 'notemptyi2/ignoredir/testa ' )\
    && ( echo "$OUT" | grep -q 'Copied: filexd' )  \
    && ( echo "$OUT" | grep -q 'Copied: notempty2/notemptyi2/test6' )\
    && ( echo "$OUT" | grep -q 'Copied: notempty2/notemptyi2/test7' ) ); then
  echo "Error backing up 2 dir!"
  exit 1
fi
echo "Backup 2 ok"

OUT=$($CMD run --from testdata/3 --repo testdir/to1 --password asdff --progress simple --verbose 1)
echo "$OUT"
if ! ( ! ( echo "$OUT" | grep -q 'notempty2/notemptyi2/test4 ' )\
    && ! ( echo "$OUT" | grep -q 'notempty2/notemptyi2/test5 ' )\
    && ! ( echo "$OUT" | grep -q 'notemptyi2/ignoredir/testa ' )\
        && ( echo "$OUT" | grep -q 'Copied: filexd' )  \
    && ( echo "$OUT" | grep -q 'Copied: notempty2/notemptyi2/test6' )\
    && ( echo "$OUT" | grep -q 'Copied: notempty2/notemptyi2/test7' ) ); then
  echo "Error backing up 3 dir!"
  exit 1
fi
echo "Backup 3 ok"


i=$((0))
AIDS=()
OUT=$($CMD list --repo testdir/to1 --password asdff)
echo "$OUT"
while IFS= read -r l; do
  ((i++))
  aid=$(echo $l | grep -Eo '[0-9]+' | tail -1)
  AIDS+=("$aid")
done <<< "$OUT"

OUT=$($CMD diff --from testdata/4 --repo testdir/to1 --password asdff --progress none --verbose 1 --aid ${AIDS[0]} --aid2 ${AIDS[1]})
echo "$OUT"
if ! ( ( echo "$OUT" | grep -q 'notempty2/notemptyi2/test6 is different' ) && ( echo "$OUT" | grep -q 'filexd is different' ) && ( echo "$OUT" | grep -q 'notempty2/notemptyi2/test7 is different' ) \
 && ! ( echo "$OUT" | grep -q 'notempty2/notemptyi2/test4' ) &&  ! ( echo "$OUT" | grep -q 'notempty2/notemptyi2/test5' )  &&  ! ( echo "$OUT" | grep -q 'notemptyi2/ignoredir/testa' ) ); then
  echo "Error comparing archive 1 and 2!"
  exit 1
fi
echo "OK comparing archive 1 and 2"
OUT=$($CMD diff --from testdata/4 --repo testdir/to1 --password asdff --progress none --verbose 1 --aid ${AIDS[1]} --aid2 ${AIDS[2]})
echo "$OUT"
if ! ( ( echo "$OUT" | grep -q 'notempty2/notemptyi2/test6 is different' ) && ( echo "$OUT" | grep -q 'filexd is different' ) && ( echo "$OUT" | grep -q 'notempty2/notemptyi2/test7 is different' ) \
 && ! ( echo "$OUT" | grep -q 'notempty2/notemptyi2/test4' ) &&  ! ( echo "$OUT" | grep -q 'notempty2/notemptyi2/test5' ) &&  ! ( echo "$OUT" | grep -q 'notemptyi2/ignoredir/testa' ) ); then
  echo "Error comparing archive 2 and 3!"
  exit 1
fi
echo "OK comparing archive 2 and 3"

OUT=$($CMD diff --from testdata/4 --repo testdir/to1 --password asdff --progress none --verbose 1 --aid ${AIDS[2]})
echo "$OUT"
if ! ( ( echo "$OUT" | grep -q 'notempty2/notemptyi2/test6 is different' ) && ( echo "$OUT" | grep -q 'filexd is different' ) && ( echo "$OUT" | grep -q 'notempty2/notemptyi2/test7 is different' ) \
 && ! ( echo "$OUT" | grep -q 'notempty2/notemptyi2/test4' ) &&  ! ( echo "$OUT" | grep -q 'notempty2/notemptyi2/test5' ) &&  ! ( echo "$OUT" | grep -q 'notemptyi2/ignoredir/testa' ) ); then
  echo "Error comparing archive 3 and current!"
  exit 1
fi
echo "OK comparing archive 3 and current"

OUT=$($CMD diff --from testdata/4 --repo testdir/to1 --password asdff --progress none --verbose 1 )
echo "$OUT"
if ! ( ( echo "$OUT" | grep -q 'notempty2/notemptyi2/test6 is different' ) && ( echo "$OUT" | grep -q 'filexd is different' ) && ( echo "$OUT" | grep -q 'notempty2/notemptyi2/test7 is different' ) \
 && ! ( echo "$OUT" | grep -q 'notempty2/notemptyi2/test4' ) &&  ! ( echo "$OUT" | grep -q 'notempty2/notemptyi2/test5' ) &&  ! ( echo "$OUT" | grep -q 'notemptyi2/ignoredir/testa' ) ); then
  echo "Error comparing archive last (3) and current!"
  exit 1
fi
echo "OK comparing archive last (3) and current"

OUT=$($CMD diff --from testdata/4 --repo testdir/to1 --password asdff --progress none --verbose 1 --aid ${AIDS[2]} --prefix notempty2/notemptyi2 )
echo "$OUT"
if ! ( ( echo "$OUT" | grep -q 'notempty2/notemptyi2/test6 is different' ) && ! ( echo "$OUT" | grep -q 'filexd is different' ) && ( echo "$OUT" | grep -q 'notempty2/notemptyi2/test7 is different' ) \
 && ! ( echo "$OUT" | grep -q 'notempty2/notemptyi2/test4' ) &&  ! ( echo "$OUT" | grep -q 'notempty2/notemptyi2/test5' ) &&  ! ( echo "$OUT" | grep -q 'notemptyi2/ignoredir/testa' ) ); then
  echo "Error comparing archive 3 and current!"
  exit 1
fi
echo "OK comparing archive 3 and current with prefix "

OUT=$($CMD diff --from testdata/4 --repo testdir/to1 --password asdff --progress none --verbose 1 --prefix notempty2/notemptyi2 )
echo "$OUT"
if ! ( ( echo "$OUT" | grep -q 'notempty2/notemptyi2/test6 is different' ) && ! ( echo "$OUT" | grep -q 'filexd is different' ) && ( echo "$OUT" | grep -q 'notempty2/notemptyi2/test7 is different' ) \
 && ! ( echo "$OUT" | grep -q 'notempty2/notemptyi2/test4' ) &&  ! ( echo "$OUT" | grep -q 'notempty2/notemptyi2/test5' ) &&  ! ( echo "$OUT" | grep -q 'notemptyi2/ignoredir/testa' ) ); then
  echo "Error comparing archive last (3) and current!"
  exit 1
fi
echo "OK comparing archive last (3) and current with prefix "


rm -rf testdata
rm -rf testdir

exit 0