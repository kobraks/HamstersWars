cow = 
{
	Collider = 
	{
		boundingBox = 
		{
			type = box;
			size = { 10, 10, 10 };
		},

		collision = function(e, e1)

		end
	},
	Script = 
	{
		update = function (e)
		end
	},

	Graphic =
	{
		model = "cow.3DS";
		--model = { path = "c://"; }

		visible = true;
		position = 
		{
			x = 0;
			y = 0;
			z = 0;
		};

		scale = 
		{
			x = 0.005;
			y = 0.005;
			z = 0.005;
		};

		rotate = 
		{
			angle = 0;
			x = 0;
			y = 0;
			z = 0;
		};

		--mesh = 
		--{
			--id = 1;
			--texture = "" --loads texture
		--}
	}
}