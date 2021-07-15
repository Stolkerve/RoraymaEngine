function queryTerminal(command)
	local success, handle = pcall(io.popen, command)
	if not success then
		return ""
	end
	result = handle:read("*a")
	handle:close()
	result = string.gsub(result, "\n$", "")
	return result
end

function getPythonPath()
	local p = queryTerminal('python3 -c "import sys; import os; print(os.path.dirname(sys.executable))"')

	p = string.gsub(p, "\\\\", "\\")
	p = string.gsub(p, "\\", "/")
	return p
end

pythonPath = getPythonPath()
pythonIncludePath = pythonPath .. "/include"
pythonLibPath = pythonPath .. "/libs"

if pythonPath == "" then
	error("Failed to find python path!")
end
