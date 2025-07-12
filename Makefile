DEST_DIR = /usr/local/share/cog-lua

LUA_OS_SCRIPT = lua/os_detec.lua
LUA_OS_DEST = $(DEST_DIR)/os_detec.lua

LUA_UPDATE_SCRIPT = lua/update.lua
LUA_UPDATE_DEST = $(DEST_DIR)/update.lua

all:
	@gcc -o cog main.c -llua -lm -ldl -DENABLE_LUA

android:
	@gcc -o cog main.c -lm -ldl

install:
	@if [ ! -d "$(DEST_DIR)" ]; then \
		echo "Creating $(DEST_DIR) folder..."; \
		mkdir -p "$(DEST_DIR)"; \
	fi
	@echo "Loading Lua files..."
	@echo "$(LUA_OS_SCRIPT) -> $(LUA_OS_DEST)"
	@cp $(LUA_OS_SCRIPT) $(LUA_OS_DEST)
	@echo "$(LUA_UPDATE_SCRIPT) -> $(LUA_UPDATE_DEST)"
	@cp $(LUA_UPDATE_SCRIPT) $(LUA_UPDATE_DEST)
	@echo "Compiling main.c..."
	@gcc -o cog main.c -llua -lm -ldl -DENABLE_LUA
	@echo "cog -> /usr/local/bin"
	@mv cog /usr/local/bin/ 
	@echo "Installation completed."

uninstall:
	@echo "Removing cog binary file..."
	@rm -rf /usr/local/bin/cog
	@echo "Removing cog-lua..."
	@rm -rf /usr/local/share/cog-lua
