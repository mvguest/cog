DEST_DIR = /usr/local/share/cog-lua

LUA_OS_SCRIPT = lua/os_detec.lua
LUA_OS_DEST = $(DEST_DIR)/os_detec.lua

all:
	@gcc -o cog main.c -llua -lm -ldl -DENABLE_LUA

android:
	@gcc -o cog main.c -lm -ldl

winstall:
	@set USERNAME=%USERNAME% && \
	set DEST_WIN=C:/Users/$$USERNAME/cog && \
	set LUA_DIR=$$DEST_WIN/lua && \
	if not exist $$DEST_WIN ( \
		echo Creating $$DEST_WIN ... && \
		mkdir $$DEST_WIN \
	) && \
	if not exist $$LUA_DIR ( \
		echo Creating $$LUA_DIR ... && \
		mkdir $$LUA_DIR \
	) && \
	echo Copying Lua scripts... && \
	copy $(LUA_OS_SCRIPT) $$LUA_DIR\\os_detec.lua >nul && \
	echo Compiling main.c... && \
	gcc -o cog.exe main.c -llua -lm -ldl -DENABLE_LUA && \
	move cog.exe $$DEST_WIN >nul && \
	echo Installation completed at $$DEST_WIN

install:
	@if [ ! -d "$(DEST_DIR)" ]; then \
		echo "Creating $(DEST_DIR) folder..."; \
		mkdir -p "$(DEST_DIR)"; \
	fi
	@echo "Loading Lua files..."
	@echo "$(LUA_OS_SCRIPT) -> $(LUA_OS_DEST)"
	@cp $(LUA_OS_SCRIPT) $(LUA_OS_DEST)
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
