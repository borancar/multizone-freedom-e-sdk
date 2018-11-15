# Copyright(C) 2018 Hex Five Security, Inc. - All Rights Reserved

ifneq ($(BOARD),coreplexip-e31-arty)
ifneq ($(BOARD),coreplexip-e51-arty)
ifneq ($(BOARD),freedom-e300-arty)
  $(error Unsupported board $(BOARD))
endif
endif
endif

ARCH := $(if $(findstring rv64, $(RISCV_ARCH)),--arch=rv64,--arch=rv32)

.PHONY: software
software: software_clean
	$(MAKE) -C zone1
	$(MAKE) -C zone2
	$(MAKE) -C zone3
	java -jar multizone/multizone.jar zone1/zone1.hex zone2/zone2.hex zone3/zone3.hex -o multizone_security $(ARCH)

.PHONY: software_clean
clean: software_clean
software_clean:
	$(MAKE) -C zone1 clean
	$(MAKE) -C zone2 clean
	$(MAKE) -C zone3 clean
	rm -f multizone_security

