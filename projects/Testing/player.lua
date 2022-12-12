Player = {}

function Player.Start() 
    print("Start")
end

function Player.Update()
    print(Camera.mouseX)
end

function Player.FixedUpdate()
    print("FixedUpdate")
end

return Player