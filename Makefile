PROJECT := zigarre

all: clean $(PROJECT).bin

flash: clean $(PROJECT).bin
	particle flash $(PROJECT) $(PROJECT).bin

$(PROJECT).bin: src/*.h src/*.cpp
	particle compile photon --saveTo $(PROJECT).bin

clean: 
	rm -f $(PROJECT).bin
