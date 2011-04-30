-- very similiar to setupSoundTracks() in audio.lua

function setupArtpackPaths()
	artpacks = c_loadDirectory(ePath.ART);
	local i,name
	for i,name in artpacks do
		io.write("found artpack: ", name)
		io.write("\n")
	end
	for i,name in artpacks do
		if name == settings.current_artpack then
			current_artpack_index = i
			c_setArtPath()
			return
		end
	end
	settings.current_artpack = artpacks[1]
	current_artpack_index = 1
	c_setArtPath()
end

function updateArtpack()
	settings.current_artpack = artpacks[ current_artpack_index ]
	c_setArtPath()
	c_reloadArtpack()
end

function nextArtpack()
	if current_artpack_index < table.getn(artpacks) then
		current_artpack_index = current_artpack_index + 1
	else
		current_artpack_index = 1
	end
	updateArtpack()
end

function previousArtpack()
	if current_artpack_index > 1 then
		current_artpack_index = current_artpack_index - 1
	else
		current_artpack_index = table.getn(artpacks) 
	end
	updateArtpack()
end

-- copy-paste from setup/next/previous Artpack
function setupLevels()
	levels = c_loadDirectory(ePath.LEVEL);
	local i,name
		for i,name in levels do
			io.write("found level: ", name)
			io.write("\n")
		end
	for i,name in levels do
		if name == settings.current_level then
					current_level_index = i
					return
		end
	end
	settings.current_level = levels[1]
	current_level_index = 1
end

function nextLevel()
	last_level_index = current_level_index
	last_level = levels[ last_level_index ]
	while last_level_index == current_level_index or
		levels[ current_level_index ] == "" do	
		if current_level_index < table.getn(levels) then
			current_level_index = current_level_index + 1
		else
			current_level_index = 1
		end
	end
	settings.current_level = levels[ current_level_index ]
	c_cancelGame()
end

function previousLevel()
	last_level_index = current_level_index
	last_level = levels[ last_level_index ]
	while last_level_index == current_level_index or
		levels[ current_level_index ] == "" do
		if current_level_index > 1 then
			current_level_index = current_level_index - 1
		else
			current_level_index = table.getn(levels) 
		end
	end
	settings.current_level = levels[ current_level_index ]
	c_cancelGame()
end


