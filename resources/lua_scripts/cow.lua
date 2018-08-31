cow = 
{
	Collider = 
	{

		--not important name
		boundingBox = 
		{
			type = box;
			size = { 10, 10, 10 };
			position = { 10, 10, 10 } --relative position 0, 0, 0 means center
			-- size = { x = 10, y = 10, z = 10 };
		},

		collision = function(e, e1)

		end
	},
	Script = 
	{
		on_create = function (e)
			log.print(log.debug, "creating");
		end,

		on_update = function (e)
			if keyboard.is_key_pressed(keyboard.keys.k) then
				log.print(log.debug, "Mateusz to debil");
			end
			if keyboard.is_key_pressed(keyboard.keys.x) then
				mouse.position.x = 10;
				mouse.position.y = 10;
			end
			if mouse.is_left_button_pressed() then
				log.print(log.debug4, "Mateusz to malpa");
			end
		end
	},

	Transform = 
	{
		position = 
		{
			x = 1,
			y = 2,
			z = 3
		};

		scale = 
		{
			x = 0.005;
			y = 0.005;
			z = 0.005;
		};

		rotate = 
		{
			x = 0;
			y = 0;
			z = 0;
		};
	},

	Graphic =
	{
		model = "cow.3DS";
		--model = { path = "c://"; }

		visible = true;
		ala = 1;

		--{
			--mesh = 
			--{
				--id = 1;
				--texture = "" --loads texture
			--}
		--}
	}
}