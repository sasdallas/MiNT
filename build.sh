# build.sh - Build a MiNT BootCD

cmake --build build --target bootcd

# WARNING! A lot of this is hardcoded!.
if [ $1 == "minbuild" ]
then
    echo "Building a minbuild environment at $MINBUILD_FOLDER"
    if [ -f $MINBUILD_FOLDER/build.ninja ]
    then
        echo "Minbuild directory is valid, please wait..."
        
        # Rebuild if the user didn't listen
        OLD_DIR=$(pwd)
        cd $MINBUILD_FOLDER
        ninja bootcd
        
        echo "Copying MINTKRNL..."
        cp $OLD_DIR/build/mintkrnl/mintkrnl.exe ntoskrnl/ntoskrnl.exe
        
        echo "Copying HAL..."
        cp $OLD_DIR/build/dll/hal/x86/hal.dll hal/halx86/hal.dll

        echo "Copying KDCOM..."
        cp $OLD_DIR/build/dll/kdcom/kdcom.dll drivers/base/kdrosdbg/kdcom.dll

        echo "Building BootCD..."
        ninja bootcd

        echo "Copying..."
        cp bootcd.iso $OLD_DIR/build/minbuild_bootcd.iso
        
        echo "Done! BootCD created at $OLD_DIR/build/minbuild_bootcd.iso"
    else
        echo "Minbuild directory is invalid. Please check your directory."
        echo "\"$MINBUILD_FOLDER/build.ninja\" not found"
    fi  
fi