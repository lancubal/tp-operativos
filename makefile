all:
	$(MAKE) -C cpu
	$(MAKE) -C kernel
	$(MAKE) -C memoria
	$(MAKE) -C entradasalida

clean:
	$(MAKE) clean -C cpu
	$(MAKE) clean -C kernel
	$(MAKE) clean -C memoria
	$(MAKE) clean -C entradasalida