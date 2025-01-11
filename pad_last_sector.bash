FILE_SIZE=$(stat -L -c %s os.bin)
PAD_LEN=$((512 - ($FILE_SIZE % 512)))
if [[ $PAD_LEN -gt 0 ]] && [[ $PAD_LEN -lt 512 ]]
then
    dd if=/dev/zero of=os.bin oflag=append bs=1 seek=$FILE_SIZE count=$PAD_LEN conv=notrunc status=none
fi