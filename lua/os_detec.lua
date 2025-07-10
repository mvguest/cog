function os_detec()
    local sep = package.config:sub(1,1)
    if sep == "\\" then
        return "Windows"
    else
        local f = io.popen("uname")
        local uname = f:read("*l")
        f:close()
        return uname or "Unknown"
    end
end
