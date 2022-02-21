
sysroot-dir			:= _sysroot
sysroot-source		:= pi:

clean-all:
	rm -rf _build _install

fetch-sysroot: | .mkdir-sysroot
	rsync -vzaRL --safe-links \
        $(sysroot-source)/./usr/include \
        $(sysroot-source)/./usr/src \
        $(sysroot-source)/./usr/lib/arm-linux-gnueabihf \
        $(sysroot-source)/./usr/lib/gcc/arm-linux-gnueabihf \
        $(sysroot-source)/./lib/arm-linux-gnueabihf \
        $(sysroot-dir)/
.PHONY: fetch-sysroot

.mkdir-sysroot:
	mkdir -p $(sysroot-dir)
.PHONY: .mkdir-sysroot
