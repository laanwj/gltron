-- You can edit this file to e.g. change the HUD layout, or the colors that are used for the speed dial)

M_PI = 3.14159265358979323846

function angle_MathFromClock360(angle)
	-- convert from [0..360] to [0..2PI]
	angle = angle * 2 * M_PI / 360
	-- move 0 to 'east', and reverse direction
	angle = M_PI / 2 - angle
	return angle
end

-- Configuration of the HUD Layout
-- HUD Elements themselves can't be scaled in width/height (yet)
-- Numbers are Pixel sizes, assuming a screensize of 1024x768
-- (the optimal resolution, where HUD elements are rendered unscaled)

-- There are configs for different aspect ratios, e.g. 4:3 (aspect == 1.33, for example 800x600)
-- or 2.6:1 (e.g. two-player splitscreen with a 4:3 screen size

HUDConfig = {
	{
		aspect = 1.333, -- aspect ratio
		Speed = { x = 776, y = 0 }, -- the speed dial graphic starts here
		Speed_Text = { x = 150, y = 60, w = 44, h = 28 }, -- that's where the digital speed number gets written to
		Buster = { x = 776, y = 41 }, -- the wall_buster status
		MapFrame = { x = 10, y = 10 }, -- the 2D map frame
		Map = { x = 14.5, y = 14, w = 187, h = 187 }, -- the 2D map itself, relative to the frame
		ScoreFrame = { x = 220, y = 10 }, -- the frame for the score
		Score = { x = 10, y = 4, w = 30, h = 24 }, -- the player's score number
		AI_Status = { x = 400, y = 728, w = 228, h = 30 }, -- the 'AI' text when in splitscreen, using computer opponents
		Console = { x = 10, y = 645, lines = 4 }, -- the Console (with win & crash messages, etc.)
		Console_Text = { x = 20, y = 5, w = 200, h = 25, space = 2 }, -- the console text relative to the frame, linue size & spacing
		FPS = { x = 850, y = 692 }, -- the FPS frame
		FPS_Text = { x = 30, y = 0, w  = 80, h = 60 }, -- the actual FPS number
		-- Pause = { x = 312, y = 400, w = 400, h = 100 }
		Pause = { x = 12, y = 600, w = 1000, h = 40 } -- the pause message
	},
	{
		aspect = 1.6,
		Speed = { x = 776, y = 0 },
		Speed_Text = { x = 150, y = 60, w = 44, h = 28 },
		Buster = { x = 776, y = 41 }, -- the wall_buster status
		MapFrame = { x = 10, y = 10 },
		Map = { x = 10, y = 10, w = 190, h = 190 },
		-- Speed = { x = 0, y = 0 },
		-- MapFrame = { x = 824, y = 10 },
		-- Map = { x = 824, y = 10, w = 190, h = 190 },
		ScoreFrame = { x = 220, y = 10 },
		Score = { x = 10, y = 4, w = 30, h = 24 },
		AI_Status = { x = 400, y = 610, w = 228, h = 30 },
		Console = { x = 10, y = 515, lines = 4 },
		Console_Text = { x = 20, y = 5, w = 200, h = 25, space = 2 },
		FPS = { x = 850, y = 557 },
		FPS_Text = { x = 30, y = 0, w  = 80, h = 60 },
		-- Pause = { x = 312, y = 400, w = 400, h = 100 }
		Pause = { x = 12, y = 450, w = 1000, h = 40 }
	},
	{
		aspect = 2.6,
		Speed = { x = 776, y = 0 },
		Speed_Text = { x = 150, y = 60, w = 44, h = 28 },
		Buster = { x = 776, y = 41 }, -- the wall_buster status
		MapFrame = { x = 10, y = 10 },
		Map = { x = 10, y = 10, w = 190, h = 190 },
		-- Speed = { x = 0, y = 0 },
		-- MapFrame = { x = 824, y = 10 },
		-- Map = { x = 824, y = 10, w = 190, h = 190 },
		ScoreFrame = { x = 220, y = 10 },
		Score = { x = 10, y = 4, w = 30, h = 24 },
		AI_Status = { x = 400, y = 350, w = 228, h = 30 },
		Console = { x = 10, y = 250, lines = 4 },
		Console_Text = { x = 20, y = 5, w = 200, h = 25, space = 2 },
		FPS = { x = 850, y = 300 },
		FPS_Text = { x = 30, y = 0, w  = 40, h = 40 },
		Pause = { x = 12, y = 200, w = 1000, h = 40 }
	},
	{
		aspect = 3.13,
		Speed = { x = 776, y = 0 },
		Speed_Text = { x = 150, y = 60, w = 44, h = 28 },
		Buster = { x = 776, y = 41 }, -- the wall_buster status
		MapFrame = { x = 10, y = 3 },
		Map = { x = 10, y = 10, w = 190, h = 190 },
		-- Speed = { x = 0, y = 0 },
		-- MapFrame = { x = 824, y = 10 },
		-- Map = { x = 824, y = 10, w = 160, h = 190 },
		ScoreFrame = { x = 220, y = 10 },
		Score = { x = 10, y = 4, w = 30, h = 24 },
		AI_Status = { x = 400, y = 300, w = 228, h = 30 },
		Console = { x = 10, y = 210, lines = 4 },
		Console_Text = { x = 20, y = 5, w = 200, h = 25, space = 2 },
		FPS = { x = 850, y = 245 },
		FPS_Text = { x = 30, y = 0, w  = 40, h = 40 },
		Pause = { x = 312, y = 200, w = 400, h = 40 }
	}

}

HUDColors = {
	Speed = { r = 1, g = 1, b = 1, a = 1 }, -- Speed (text)
	Score = { r = 1, g = 1, b = 1, a = 1 }, -- Score (text)
	AI_Status = { r = 1, g = 1, b = 1, a = 1 }, -- AI message (text)
	FPS = { r = 1, g = 1, b = 1, a = 1 } -- FPS number (text)
}
-- TODO: console

-- I suggest you only touch the color attributes here
-- the Analog speed dial consists of several arc segment, where you
-- can adjust the color at the start *and* end (thus allowing you to make
-- color gradients, e.g. fading from green to yellow)

HUDSpeedDial = {
	circle = {
		center = { x = 174, y = 75 },
		inner_radius = 23,
		radius = 66
	},
	angles = {
	-- angles, and speed range for green area
		{
		arc_start = {
			angle = angle_MathFromClock360(0),
			color = { r = 0, g = .6, b = 0 },
			speed = 0,
		},
	 	arc_end = { 
			angle = angle_MathFromClock360(160),
			color = { r = 0, g = 1, b = 0 },
			speed = 0.5,
		},
		},
	-- angles, and speed range for green to yellow fade
		{
		arc_start = {
			angle = angle_MathFromClock360(160),
			color = { r = 0, g = 1, b = 0 },
			speed = 0.5,
		},
	 	arc_end = { 
			angle = angle_MathFromClock360(190),
			color = { r = 0, g = 1, b = 0 },
			speed = 0.7,
		},
		},
	-- angles, and speed range for yellow area
		{
		arc_start = {
			angle = angle_MathFromClock360(190),
			color = { r = .7, g = .7, b = 0 },
			speed = 0.7,
		},
	 	arc_end = { 
			angle = angle_MathFromClock360(270),
			color = { r = 1, g = 1, b = 0 },
			speed = 0.8,
		},
		},
	-- angles, and speed range for yellow to red fade
		{
		arc_start = {
			angle = angle_MathFromClock360(270),
			color = { r = 1, g = 1, b = 0 },
			speed = 0.8,
		},
	 	arc_end = { 
			angle = angle_MathFromClock360(313),
			color = { r = 1, g = 1, b = 0 },
			speed = 0.9,
		},
		},
	-- angles, and speed range for red area
		{
		arc_start = {
		angle = angle_MathFromClock360(313),
		color = { r = .6, g = .1, b = 0 },
		speed = 0.9,
		},
	 	arc_end = { 
			angle = angle_MathFromClock360(359),
			color = { r = 1, g = .1, b = 0 },
			speed = 1.0,
		},
		},
	}
}

-- Same settings for the turbo booster bar

HUDTurbo = {
	rect = { top = 5, left = 5, right = 130, bottom = 45 },
	ranges = {
		{
		-- width, and charge range for red area
			range_start = {
				color = { r = .3, g = 0, b = 0 },
				charge = 0.0,
				width = 0.0,
			},
			range_end = {
				color = { r = .3, g = 0, b = 0 },
				charge = 0.3,
				width = 0.3,
			},
		},
		-- width, and charge range for yellow to red fade
		{
			range_start = {
				color = { r = .3, g = 0, b = .0 },
				charge = 0.3,
				width = 0.3,
			},
			range_end = {
				color = { r = 1, g = 0, b = .0 },
				charge = 1,
				width = 1,
			},
		},
	}
}

-- Same settings for the wall_buster bar

HUDBuster = {
	rect = { top = 5, left = 5, right = 130, bottom = 45 },
	ranges = {
		{
		-- width, and charge range for red area
			range_start = {
				color = { r = .3, g = 0, b = 0 },
				charge = 0.0,
				width = 0.0,
			},
			range_end = {
				color = { r = .3, g = 0, b = 0 },
				charge = 0.3,
				width = 0.3,
			},
		},
		-- width, and charge range for yellow to red fade
		{
			range_start = {
				color = { r = .3, g = 0, b = .0 },
				charge = 0.3,
				width = 0.3,
			},
			range_end = {
				color = { r = 1, g = 0, b = .0 },
				charge = 1,
				width = 1,
			},
		},
	}
}
