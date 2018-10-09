local config = {
	up = keyboard.keys.w;
	down = keyboard.keys.w;
	left = keyboard.keys.w;
	right = keyboard.keys.w;
	
	window = {
		title = "Hamsters Wars";
		size = { 800, 600 };
		position = center;
	}

	debug = true;

	shader = {
		texture = "texture";
		some = "2d";
	};

	max_updates = 5;
	update_rate = 16;
}

return config;