FILE_SIZE=$(stat -L -c %s os.bin)
SECTORS_COUNT=$((FILE_SIZE / 512))
echo total of $SECTORS_COUNT sectors. if more then 256 - update mbr hard disk reading
SECTORS_COUNT_BYTE=$(printf $(printf '\\x%s' $(printf '%x' $SECTORS_COUNT)))
echo $SECTORS_COUNT_BYTE | dd of=os.bin bs=1 seek=509 count=1 conv=notrunc status=none