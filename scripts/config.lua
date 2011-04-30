-- global tables
tmp = {}

settings = { }
tracks = { }

app_version = 0.718
-- debugginb
settings.debug_output = 1

-- game
-- game rules
settings.speed = 8.5                      -- lightcycle speed
settings.energy = 4.8					  -- lightcycle energy
settings.energy_increase = 0.4            -- energy increase per second

settings.booster_on = 1                   -- booster option (0=off,1=on)
settings.booster_min = 1.0                -- min. booster value so it can be activated
-- settings.booster_max = 6.5                -- fully charged booster
settings.booster_use = 1.5                -- how much speed to add per second
settings.booster_speed_factor = 1.1
-- speed_factor: multiply player speed upon usage (per second)
settings.booster_speed_increase = 1.2
-- speed_increase: increase speed by constant value per second
settings.booster_speed_decrease = 0.8
-- speed decrease per second after booster ran out
-- settings.booster_regenerate = 0.4         -- regeneration of booster when not in use

settings.wall_accel_on = 0
settings.wall_accel_max = 20.0
settings.wall_accel_use = 1.0
settings.wall_accel_decrease = 0.8

settings.wall_buster_on = 1
-- wall_buster_on:
--   1: wall buster enabled
--   0: wall buster disabled

settings.wall_buster_use = 4.8
-- amount of energy consumed per second of wall_buster use
settings.wall_buster_min = 2.4
-- amount of energy needed to trigger wall_buster usage

-- settings.wall_buster_max = 1.0
-- settings.wall_buster_regenerate = 0.2

settings.players = 1
settings.ai_opponents = 3
settings.ai_level = 2
settings.current_level = "square.lua"
-- settings.current_level = "movie.lua"
settings.erase_crashed = 1
settings.map_scale = 1

-- game settings
settings.fast_finish = 1
settings.camType = 1
settings.display_type = 3

-- video
settings.artpacks = { }
settings.current_artpack = "classic"
-- detail settings
settings.mipmap_filter = 3
settings.alpha_trails = 0
settings.show_glow = 1
settings.reflection = -1
settings.show_recognizer = 1
settings.show_impact = 1
settings.lod = 1
settings.cycle_sharp_edges = 0
-- hud settings
settings.show_fps = 0
settings.show_speed = 1
settings.show_wall_buster = 0
settings.show_console = 1
settings.show_ai_status = 1
settings.show_scores = 1
settings.show_2d = 1

-- screen options
settings.width = 800
settings.height = 600
settings.resolution = 5 -- 800x600
settings.windowMode = 1

-- other settings (not available via menu)
settings.show_gl_logo = 1

settings.fov = 90
settings.znear = 0.5
settings.light_cycles = 1
settings.turn_cycle = 1
settings.line_spacing = 20
settings.softwareRendering = 0
settings.antialias_lines = 1
settings.bitdepth_32 = 1
settings.use_stencil = 1
settings.mouse_warp = 0
settings.model_backwards = 0
settings.screenSaver = 0

settings.shadow_volumes_walls = 0
settings.shadow_volumes_cycle = 1
settings.shadow_projective_cycle_on_floor = 1
settings.shadow_projective_trails_on_floor = 1
settings.shadow_projective_recognizer_on_floor = 1

settings.arena_outlines = 0

settings.joy_threshold = 0.10

-- audio
-- settings.tracks = { }
settings.current_track = "revenge_of_cats.it"
-- music & fx settings
settings.loopMusic = 0
-- TODO: debug setting 
settings.playEffects = 0
settings.playMusic = 0
-- settings.playEffects = 1
-- settings.playMusic = 1
settings.fxVolume = 0.2
settings.musicVolume = 0.5

-- input

settings.mouse_lock_ingame = 0
settings.mouse_invert_x = 0
settings.mouse_invert_y = 0

settings.keys = { 
   { -- player 1
      left = 97, -- lower case 'a'
      right = 115, -- lower case 's'
      glance_left = 113, -- lower case 'q'
      glance_right = 119, -- lower case 'w'
      boost = 101, -- lower case 'e'
      bust = 100, -- lower case 'd'
   },
   { -- player 2
      left = 106, -- lower case 'j'
      right = 107, -- lower case 'k'
      glance_left = 117, -- lower case 'u'
      glance_right = 105, -- lower case 'i'
			boost = 108, -- lower case 'l'
			bust = 111, -- lower case 'o'
   },
   { -- player 3
      left = 276, -- cursor left
      right = 275, -- cursor right
      glance_left = 127, -- 'del'
      glance_right = 279, -- 'end'
			boost = 274, -- cursor down
			bust = 273, -- cursor up
   },
   { -- player 4
      left = 260, -- keypad 4
      right = 262, -- keypad 6
      glance_left = 263, -- keypad 7
      glance_right = 265, -- keypad 9
			boost = 261, -- keypad h
			bust = 259, -- keypad 3
   }
}

-- camera
settings.clamp_cam_r_min = 6
settings.clamp_cam_r_max =  45
settings.clamp_cam_chi_min =  3.141 / 8
settings.clamp_cam_chi_max =  3 * 3.141 / 8

settings.wireframe = 0
-- these tables are not serialized to RCNAME (yet)
-- some values are set in artpack.lua
video = {} 
video.settings = {}
