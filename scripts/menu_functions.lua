MenuC = {
   type = {
      menu = 1,
      list = 2,
      slider = 3,
      key = 4,
      string = 5, -- not used yet
      number = 6, -- not used yet
      action = 7
   },
   keys = {
      player1 = {},
      player2 = {},
      player3 = {},
      player4 = {}
   }
}
	
GetMenuValue = {}
MenuAction = {}
MenuLeft = {} 
MenuRight = {}
MenuFunctions = {}

GetMenuValue[ MenuC.type.list ] = function ( menu )
   local value
   if Menu[menu].read then value = Menu[menu].read() end
   if not value then value = "undef" end

   -- script_print(menu .. " --> " .. value )
   local i
   for i=1,table.getn(Menu[menu].values) do
      if Menu[menu].values[i] == value then return Menu[menu].labels[i] end
   end
   return "unknown (" .. value .. ")"
end

GetMenuValue[ MenuC.type.key ] = function ( menu )
   local player = Menu[menu].player
   local event = Menu[menu].event
   return c_getKeyName( settings.keys[ player ][ event ] )
end

GetMenuValue[ MenuC.type.slider ] = function ( menu )
   return Menu[menu].read()
end

MenuAction[ MenuC.type.menu ] = function ( menu )
	Menu.current = menu 
	io.write("Menu.current is '", menu, "'"); io.flush();
	Menu.active = 1
	-- io.write(string.format("setting %s (%d items) as active menu\n", menu, table.getn(Menu[menu].items) ))
	local i
	for i = 1,table.getn(Menu[menu].items) do
		-- io.write(string.format("checking %s for init-function\n", Menu[menu].items[i]))
		if Menu[ Menu[menu].items[i] ].init then
			Menu[ Menu[menu].items[i] ].init(Menu[menu].items[i])
		end
	end
end

MenuAction[ MenuC.type.list ] = function ( menu )
	MenuRight[ MenuC.type.list ](menu)
end

MenuAction[ MenuC.type.action ] = function( menu )
   if Menu[menu].action then
      Menu[menu].action()
   end
end
MenuAction[ MenuC.type.slider ] = MenuAction [ MenuC.type.action ]

MenuAction[ MenuC.type.key ] = function ( menu )
   local player = Menu[menu].player
   local event = Menu[menu].event
   configure_player = player
   configure_event = event
   c_configureKeyboard()
end

function menuListCycle(menu, offset)

	local nValues = table.getn(Menu[menu].values)
	local i
	local index
	local value = Menu[menu].read()

	for i=1,nValues do
		if Menu[menu].values[i] == value then
			if (i + offset <= nValues) and (i + offset >= 1) then
				index = i + offset
			else
				if offset < 1 then
					index = nValues
				else
					index = 1
				end
			end
			Menu[menu].store(Menu[menu].values[index]) 
			return
		end
	end
	-- Menu[menu].store( Menu[menu].values[1] )
end

MenuLeft[ MenuC.type.slider ] = function( menu )
	if Menu[menu].left then
		Menu[menu].left()
	end
end

MenuLeft[ MenuC.type.list ] = function( menu )
	menuListCycle(menu, -1)
end

MenuRight[ MenuC.type.slider ] = function( menu )
	if Menu[menu].right then
		Menu[menu].right()
	end
end

MenuRight[ MenuC.type.list ] = function( menu )
	menuListCycle(menu, 1)
end

GetMenuValueWidth = function ( menu )
	if GetMenuValue[ Menu[menu].type ] then
		return string.len( GetMenuValue[ Menu[menu].type ]( menu ) )
	else
		return 0
	end
end

GetMenuValueString = function ( menu )
   -- io.write(string.format("GetMenuValueString: '%s'\n", menu));
	if GetMenuValue[ Menu[menu].type ] then
		return GetMenuValue[ Menu[menu].type ]( menu )
	else
		return ""
	end
end

MenuFunctions.SetParent = function ( main, sub )
	-- script_print("processing menu '" .. sub .. "'")
	local _,entry
	for _,entry in main[sub].items do
		if main[entry] == nil then
			script_print("menu '" .. entry .. "' does not exist")
		else 
			main[entry].parent = sub
			-- script_print("processing item '" .. entry .. "'")
			if main[entry].type == MenuC.type.menu then
				MenuFunctions.SetParent( main, entry )
			end
		end
	end
end

MenuFunctions.SetNames = function (menu)
	local name,v
	for name,v in menu do
		if type(v) == "table" then
			v.name = name
		end
	end
end

MenuFunctions.GotoParent = function ()
	io.write("Going to Parent menu of '", Menu.current, "'")
	Menu.current = Menu[Menu.current].parent
	io.write("Menu.current is '", Menu.current, "'"); io.flush();
	Menu.active = 1
end

MenuFunctions.Action = function ()
	local menu = Menu[Menu.current].items[Menu.active]
	local type = Menu[ menu  ].type
	script_print("calling action of '" .. menu .. "', type " .. type )
	MenuAction[ type ]( menu )
end

MenuFunctions.Left = function ()
	local menu = Menu[Menu.current].items[Menu.active]
	local type = Menu[ menu  ].type
	if MenuLeft[ type ] then
		MenuLeft[ type ](menu)
	end
end

MenuFunctions.Right = function ()
	local menu = Menu[Menu.current].items[Menu.active]
	local type = Menu[ menu  ].type
	if MenuRight[ type ] then
		MenuRight[ type ](menu)
	end
end


MenuFunctions.Next = function ()
	if Menu.active < table.getn(Menu[Menu.current].items) then
		Menu.active = Menu.active + 1
	else
		Menu.active = 1
	end
end

MenuFunctions.Previous = function ()
	if Menu.active > 1 then 
		Menu.active = Menu.active - 1
	else
		Menu.active = table.getn(Menu[Menu.current].items)
	end
end
