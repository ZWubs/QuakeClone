

Sprite {
	image *Image
	view_direction Float (degrees) (We likely won't need this much detail, we might be able to get away with just an int.)
	state String
	frame_number int
}

Entities {
	position Vector
	rotation Float (degrees)
	sprite_state string
	sprite_sheet [Sprite]
	update(event) // Updates position and rotation, can spawn other entities and create / handle events
}

/*	No draw function?, each entity will have one sprite sheet,
	and a sprite state value to control which one gets used.
	The larger render function will pick the right sprite
	based on tag and view direction.
*/

Render(entity) {
	Find sprite's screen position (left right + scale)
	Test if sprite is on screen. if it is, continue.
	Find correct sprite:
		Limit sheet by state. Eg: Walking
		Get difference between payer view direction and sprite direction. Eg: Walking, 90deg
		Get max frame number. Eg: Walking, 90deg, frame X/4
		Calculate current sprite frame. (modulus with world frame count?) Eg: Walking, 90deg, frame 3/4.
	Draw final sprite at correct screen position and scale.
}
