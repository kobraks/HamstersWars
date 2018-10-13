local config = {
	up = keyboard.keys.w;
	down = keyboard.keys.w;
	left = keyboard.keys.w;
	right = keyboard.keys.w;
	
	window = {
		title = "Hamsters Wars";
		size = { 800, 600 };
		position = window.position.center;

		style = window.default;
	};

	debug = true;

	shader = {
		texture = "texture";
		some = "2d";
	};

	max_updates = 5;
	update_rate = 16;

	open_gl = 
	{
		version = { 3, 3 };
		depth_bits = 24;
		stencil_bits = 8;

		--antyaliasing = 0; -- not supported
	};


	entity_list_file = "entities.lua";
}

return config;