# $FreeBSD: head/sysutils/devcpu-data/files/Makefile 470255 2018-05-18 01:28:23Z sbruno $
INTEL_UCODE=	intel-ucode
OUTPUT_DIR=	mcodes

all:	ucode
ucode:	ucode-split
	mkdir -p $(OUTPUT_DIR)
	cd ${OUTPUT_DIR} && \
	for file in ../${INTEL_UCODE}/*; do \
		../ucode-split $$file; \
	done

# Use the host cc to compile ucode-split in case of cross-compile
ucode-split: ucode-split.c
	cc ucode-split.c -o $@

clean:
	rm -rf $(OUTPUT_DIR) ucode-split
