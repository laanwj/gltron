function setupSoundTrack()
   local i,name
   for i,name in tracks do
      print("[scripting audio] found track '", name, "'")
      if name == settings.current_track then
	 current_track_index = i
	 return
      end
   end
   settings.current_track = tracks[1]
   current_track_index = 1
end

function nextTrack()
   if current_track_index < table.getn(tracks) then
      current_track_index = current_track_index + 1
   else
      current_track_index = 1
   end
   settings.current_track = tracks[ current_track_index ]
   c_reloadTrack()
end

function previousTrack()
   if current_track_index > 1 then
      current_track_index = current_track_index - 1
   else
      current_track_index = table.getn(tracks) 
   end
   settings.current_track = tracks[ current_track_index ]
   c_reloadTrack()
end

function MusicVolumeUp()
   settings.musicVolume = settings.musicVolume + 0.05
   if settings.musicVolume > 1.0 then
      settings.musicVolume = 1.0
   end
   c_update_audio_volume()
end

function MusicVolumeDown()
   settings.musicVolume = settings.musicVolume - 0.05
   if settings.musicVolume < 0.0 then
      settings.musicVolume = 0.0
   end
   c_update_audio_volume()
end

function FXVolumeUp()
   settings.fxVolume = settings.fxVolume + 0.05
   if settings.fxVolume > 1.0 then
      settings.fxVolume = 1.0
   end
   c_update_audio_volume()
end

function FXVolumeDown()
   settings.fxVolume = settings.fxVolume - 0.05
   if settings.fxVolume < 0.0 then
      settings.fxVolume = 0.0
   end
   c_update_audio_volume()
end
