directions = { random = -1, up = 2, down = 0, right = 1, left = 3 }

level = {

version = 71,

-- the sizes in the level are all in the [0,1] range and can be
-- scaled to the appropriate arena size

scalable = 1,

-- spawn points
-- (they don't have to be sorted, they will be randomized anyway)

spawn = {
  { x = .25, y = .5, dir = directions.random }, 
  { x = .5, y = .25, dir = directions.random }, 
  { x = .5, y = .75, dir = directions.random }, 
  { x = .75, y = .5, dir = directions.random }
},

-- floor geometry is used to generate reflections, and
-- as a background for the 2d map
floor = {
	shading = {
	  lit = 1, -- no lighting, only diffuse texture is applied
	},
	model = "simple-floor.obj"
}, -- floor

} -- level
