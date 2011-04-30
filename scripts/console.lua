console = { lines = {}, max = 10, cur = 0, fill = 0 }

function console_AddLine(line)
	-- store line in buffer
	console.lines [console.cur] = line
	console.cur = console.cur + 1
	console.fill = console.fill + 1
	-- wrap around buffer to throw the last line away
	if(console.cur == console.max) then
		console.cur = 0
	end
end

function console_Clear()
	console.cur = 0
	console.fill = 0
end

function console_GetLine(offset)
	if(offset > console.fill - 1) then
		return ""
	else
		index = (console.cur - 1 - offset)
		if(index < 0) then index = index + console.max end
		if(console.lines[index]) then
			return console.lines[index]
		else
			return "<invalid>"
		end
	end
end

function console_SeekRelative(offset)
	-- TODO: implement something
end