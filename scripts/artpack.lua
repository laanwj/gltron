-- The following information is mostly useful for artpack authors:
-- 
-- This is the default configuration file for all artpacks.
-- It gets loaded whenever an artpack is changed, and before the
-- artpack.lua file contained in the new artpack is executed

-- You can use it to
-- - configure the (cycling) colors for the menu font

-- menu colors
menu_item = { 1, 1, 1, 1 }
menu_item_active1 = { .5, 1, 0, 1 }
menu_item_active2 = { 1, 0, 0, 1 }

-- texture settings
video.settings.show_floor_texture = 1
video.settings.show_wall = 1
video.settings.show_crash_texture = 1
video.settings.show_skybox = 1
video.settings.show_decals = 1
video.settings.stretch_textures = 1

-- lightcycle colors
model_diffuse_0 = { 1.000, 0.550, 0.140, 1.000 }
model_diffuse_1 = { 0.750, 0.020, 0.020, 1.000 }
model_diffuse_2 = { 0.120, 0.520, 0.600, 1.000 }
model_diffuse_3 = { 0.800, 0.800, 0.800, 1.000 }

model_specular_0 = { 0.500, 0.500, 0.000, 1.000 }
model_specular_1 = { 0.750, 0.020, 0.020, 1.000 }
model_specular_2 = { 0.120, 0.520, 0.600, 1.000 }
model_specular_3 = { 1.000, 1.000, 1.000, 1.000 }

trail_diffuse_0 = { 1.000, 0.850, 0.140, 0.600 }
trail_diffuse_1 = { 0.750, 0.020, 0.020, 0.600 }
trail_diffuse_2 = { 0.120, 0.520, 0.600, 0.600 }
trail_diffuse_3 = { 0.700, 0.700, 0.700, 0.600 }

clear_color = { 0, 0, 1, 0 }

video.settings.reflection = 0.0

