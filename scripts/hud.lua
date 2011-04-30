eHUD = {
	Speed = 0,
	MaskSpeed = 1,
	MaskTurbo = 2,
	Console = 3,
	Map = 4,
	Scores = 5,
	FPS = 6,
	Buster = 7,
	MaskBuster = 8,
	ElementCount = 9
}

eHUDStencil = {
	MaskSpeed = 17,
	MaskTurbo = 18,
	MaskBuster = 19,
}

-- 	drawHud: parameters
-- 	- Score (or -1 if disabled)
-- 	- AI status ("computer player" or "")
-- 	- Speed digital (absolute value)
-- 	- Speed analog (1 for default speed, > 1 during acceleration)
-- 	- Booster value (between 0 and 1)
--  - Wall buster value (between 0 and 1)

function fabs(d)
	if(d < 0) then
		return -d
	else
		return d
	end
end

function getConfig(aspect)
	best = -1
	best_aspect = 999
	for i = 1,table.getn(HUDConfig) do
		if(fabs(HUDConfig[i].aspect - aspect) < best_aspect) then
			best_aspect = fabs(HUDConfig[i].aspect - aspect)
			best = i
		end
	end
	-- io.write(string.format("aspect: %f, config: %d (%f)\n", aspect, best, HUDConfig[best].aspect))
	return HUDConfig[best]
end

function drawHUD(width, height, score, ai, speed_digital, speed_analog, booster, wall_buster, fps, 
	pause_message, pause_color_r, pause_color_g, pause_color_b)
	config = getConfig(width / height)
	-- speedometer
	if settings.show_speed == 1 and
		(settings.wall_accel_on == 1 or 
		 settings.booster_on == 1 or
		 settings.wall_buster_on) then
		c_pushMatrix()
		c_translate(config.Speed.x, config.Speed.y, 0)
		c_drawHUDSurface(eHUD.Speed)
		drawSpeedDigital(speed_digital, config.Speed_Text, HUDColors.Speed)
		c_drawHUDMask(eHUD.MaskSpeed, eHUDStencil.MaskSpeed)
		drawSpeedAnalog(speed_analog, HUDSpeedDial.circle, HUDSpeedDial.angles)
		if(settings.booster_on == 1 or settings.wall_buster_on) then
			c_drawHUDMask(eHUD.MaskTurbo, eHUDStencil.MaskTurbo)
			drawBar(booster, HUDTurbo.rect, HUDTurbo.ranges)
		end			
		c_drawHUDMask(-1, -1)
		c_popMatrix()
	end	
	-- wallbuster
	if settings.show_wall_buster == 1 and settings.wall_buster_on == 1 then
		c_pushMatrix()
		c_translate(config.Buster.x, config.Buster.y, 0)
		c_drawHUDSurface(eHUD.Buster)
		c_drawHUDMask(eHUD.MaskBuster, eHUDStencil.MaskBuster)
		-- drawBar(wall_buster, HUDBuster.rect, HUDBuster.ranges)
		c_drawHUDMask(-1,-1)
		c_popMatrix()
	end
	-- 2d map
	if settings.show_2d == 1 then
		c_pushMatrix()
		c_translate(config.MapFrame.x, config.MapFrame.y, 0)
		c_scale(settings.map_scale, settings.map_scale, settings.map_scale)
		c_drawHUDSurface(eHUD.Map)
		c_popMatrix()
		
		c_pushMatrix()
		c_translate(config.MapFrame.x, config.MapFrame.y, 0)
		c_scale(settings.map_scale, settings.map_scale, settings.map_scale)
		c_translate(config.Map.x, config.Map.y, 0)
		c_draw2D(
			config.Map.w,
			config.Map.h
			)
		c_popMatrix()
	end
	
	-- Score
	if(settings.show_scores == 1) then
		-- hack, um Score neben Map zu platzieren
		-- TODO: fix
		local xOffset = (settings.map_scale - 1) * (config.ScoreFrame.x - config.MapFrame.x)

		c_pushMatrix()
		c_translate(config.ScoreFrame.x + xOffset, config.ScoreFrame.y, 0)
		c_drawHUDSurface(eHUD.Scores)
		c_translate(config.Score.x + xOffset, config.Score.y, 0)
		c_color(HUDColors.Score.r, HUDColors.Score.g, HUDColors.Score.b, HUDColors.Score.a)
		c_drawTextFitIntoRect(string.format("%d", score),
			config.Score.w, config.Score.h,
			FontFormat.ScaleFitVertically)
		c_popMatrix()
	end

	-- AI status
	if(settings.show_ai_status == 1) then
		c_pushMatrix()
		c_translate(config.AI_Status.x, config.AI_Status.y, 0)
		c_color(HUDColors.AI_Status.r, HUDColors.AI_Status.g, HUDColors.AI_Status.b, HUDColors.AI_Status.a)
 		c_drawTextFitIntoRect(
 			ai,
 			config.AI_Status.w, config.AI_Status.h,
 			FontFormat.ScaleFitHorizontally + 
 			FontFormat.ScaleFitVertically)
		c_popMatrix()
	end

	-- Console
	if(settings.show_console == 1) then
		c_pushMatrix()
		c_translate(config.Console.x, config.Console.y, 0)
		c_drawHUDSurface(eHUD.Console)
		c_translate(config.Console_Text.x, config.Console_Text.y, 0)
		for i = 0,config.Console.lines - 1 do
			-- io.write(string.format("[console] %d: %s\n", i, console_GetLine(i)))
			c_drawTextFitIntoRect(
				console_GetLine(i),
				config.Console_Text.w,
				config.Console_Text.h,
				FontFormat.ScaleFitVertically)
			c_translate(0, config.Console_Text.h + config.Console_Text.space, 0)
		end
		c_popMatrix()
		
	end
	
	-- FPS
	if(settings.show_fps == 1) then
		c_pushMatrix()
		c_translate(config.FPS.x, config.FPS.y, 0)
		c_drawHUDSurface(eHUD.FPS)
		c_translate(config.FPS_Text.x, config.FPS_Text.y, 0)
		c_drawTextFitIntoRect(
			string.format("%d", fps),
			config.FPS_Text.w,
			config.FPS_Text.h,
			FontFormat.ScaleFitVertically)
		
		c_popMatrix()
	end
	
	-- Pause Text
	
	if(pause_message) then
		c_pushMatrix()
		c_translate(config.Pause.x, config.Pause.y, 0)
		c_color(pause_color_r, pause_color_g, pause_color_b, 1)
		c_drawTextFitIntoRect(
			pause_message,
			config.Pause.w,
			config.Pause.h,
			FontFormat.ScaleFitVertically +
			FontFormat.AlignCenter)
		c_popMatrix()
	end
end

