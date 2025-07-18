function update()
  local function io_run(cmd)
    local handle = io.popen(cmd)
    local result = handle:read("*a")
    handle:close()
    return result
  end

  os.execute("git fetch")

  local status = io_run("git status -uno")

  if status:match("Your branch is behind") then
    print("LUA-OUT: Your repo is not updated to the latest version of the cog editor")
    return "NUPDT"
  elseif status:match("Your branch is up to date") then
    print("LUA-OUT: Your repo is updated to the latest version of the cog editor")
    return "UPDT"
  else
    print("LUA-OUT: It was not possible to determinate if your repo is updated to the latest version")
    return "UNKNOWN"
  end
end
