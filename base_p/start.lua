
LoadResource("Resources.xml")

LoadEffects("Bang.xml")
LoadEffects("Flame.xml")

GUI:LoadLayers("Layers.xml")
UploadResourceGroup("ShootingRangeGroup")
Screen:pushLayer(GUI:getLayer("ShootingRangeLayer"))
