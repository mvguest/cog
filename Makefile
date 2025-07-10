LUA_SCRIPT = lua/os_detec.lua
DEST_FILE = $(DEST_DIR)/os_detec.lua
DEST_DIR = /usr/local/share/cog-lua

all:
	@gcc -o cog main.c -llua -lm -ldl

install:
	@if [ ! -d "$(DEST_DIR)" ]; then \
		echo "Creating $(DEST_DIR)..."; \
		sudo mkdir -p "$(DEST_DIR)"; \
	fi
	@echo "$(LUA_SCRIPT) -> $(DEST_FILE)"
	@sudo cp $(LUA_SCRIPT) $(DEST_FILE)
	@echo "Compiling main.c..."
	@gcc -o cog main.c -llua -lm -ldl
	@echo "cog -> /usr/local/bin"
	@mv cog /usr/local/bin/ 
	@echo "Installation completed."

uninstall:
	@echo "Removing cog binary file..."
	@rm -rf /usr/local/bin/cog
	@echo "Removing cog-lua..."
	@rm -rf /usr/local/share/cog-lua
