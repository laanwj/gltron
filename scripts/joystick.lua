function JoyThresholdUp()
   settings.joy_threshold = settings.joy_threshold + 0.05
   if settings.joy_threshold > 0.95 then
      settings.joy_threshold = 0.95
   end
end

function JoyThresholdDown()
   settings.joy_threshold = settings.joy_threshold - 0.05
   if settings.joy_threshold < 0.0 then
      settings.joy_threshold = 0.0
   end
end
