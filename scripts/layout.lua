layouts = {
		"single" = { 
				count = 1,
				positions = { { origin = { x = 0, y = 0 }, width = { x = 1, y = 1 } } }
				types = { "map" }
				players = { nil }
		}

    "dual-split" = {
				count = 2,
				positions = { 
						{ origin = { x = 0, y = 0 }, width = { x = 1, y = 0.5 } },
						{ origin = { x = 0, y = 0.5 }, width = { x = 1, y = 0.5 } }
				},
				types = { "3rd-person-follow", "3rd-person-follow" },
				players = { 0, 1 }
		}
		"four-player" = {
				count = 4,
				positions = {
						{ origin = { x = 0, y = 0 }, width = { x = 0.5, y = 0.5 } },
						{ origin = { x = 0.5, y = 0 }, width = { x = 0.5, y = 0.5 } },
						{ origin = { x = 0, y = 0.5 }, width = { x = 0.5, y = 0.5 } },
						{ origin = { x = 0.5, y = 0.5 }, width = { x = 0.5, y = 0.5 } }
				},
				types =  { "3rd-person-follow", "3rd-person-follow",
									 "3rd-person-follow", "3rd-person-follow" },
				players = { 0, 1, 2, 3 }
		}
		"cockpit-map" = {
				count = 2,
				positions = {
						{ origin = { x = 0, y = 0.2 }, width = { x = 0.6, y = 0.6 } },
						{ origin = { x = 0.6, y = 0.4 }, width = { x = 0.4, y = 0.4 } }
				},
				types = { "1st-person", "map" }
		}
}

drawGame = function()

layout = layouts[settings.visual]

for j = 1,layout.count do
	 visual = settings.visuals[j]
	 setViewport( layout.positions[i] )
	 if (layout.types[j] == "map") then
			 displayRender2D()
   else
			 displayRender3D(camera[layout.types[j]])
   end
end
							 
