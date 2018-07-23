cow = 
{
	Collider = 
	{
		boundingBox = 
		{
			type = box;
			size = { 10, 10, 10 };
			-- size = { x = 10, y = 10, z = 10 };
		},

		collision = function(e, e1)

		end
	},
	Script = 
	{
		update = function (e)
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