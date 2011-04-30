directions = { random = -1, up = 2, down = 0, right = 1, left = 3 }

level = {

version = 71,

-- the sizes in the level are all in the [0,1] range and can be
-- scaled to the appropriate arena size

scalable = 0,

-- spawn points
-- (they don't have to be sorted, they will be randomized anyway)

spawn_is_relative = 1, -- relative to bounding box
spawn = {
  { x = .25, y = .5, dir = directions.random }, 
  { x = .5, y = .25, dir = directions.random }, 
  { x = .5, y = .75, dir = directions.random }, 
  { x = .75, y = .5, dir = directions.random }
},

floor = {
	shading = {
		lit = 0, -- no lighting, only diffuse texture is applied
		textures = { 
			diffuse = {
				file = "grey_floor.png",
				-- file = "uv-debug.png",
				-- 0: nearest, 1: linear,
				mag_filter = 1,
				-- also for min_filter:
				-- 2: linear_mipmap_nearest
				-- 3: linear_mipmap_linear
				min_filter = 3,
				-- 0: clamp, 1: clamp to edge, 2: repeat
				wrap_s = 2,
				wrap_t = 2,
				-- 1: no filtering, 64: a lot of filtering
				anisotropic_filtering = 8,
				texture_scale = 240,
			}
		}
	},
	model = "floor.obj",
},

arena = {
	shading = {
		lit = 1, -- with lighting
		-- no texture
		-- passes = 1, -- no outline...
		passes = 2, -- draws a white/grey outline around the sharp edges
	},
	model = "arena.obj"
}

} -- level end
