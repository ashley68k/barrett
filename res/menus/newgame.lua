-- newgame.lua

local difficulty_index = 1

local difficulty_icons = {
	{ "NEWG1", "NEWG2", "NEWG3", "NEWG4" },
	{ "NEWG5", "NEWG6", "NEWG7", "NEWG8" },
	{ "NEWG9", "NEWG10", "NEWG11", "NEWG12" }
}

local difficulty_labels = {
	{
		"I AM A CHEW TOY.",
		"WILL OF IRON,\nKNEES OF JELL-O.",
		"I'M IN MY\nELEMENT: LEAD.",
		"TWO WORDS:\nREAPER MAN."
	},
	{
		"THE ENEMY WILL\nDEVOUR ME.",
		"WHICH PART IS\nTHE TRIGGER?",
		"I HAVE PET NAMES\nFOR MY GRENADES.",
		"NO ONE SHALL\nLIVE."
	},
	{
		"DIG MY GRAVE.\nNOW.",
		"I THINK I LEFT\nTHE STOVE ON.",
		"I'M A FREIGHT\nTRAIN O'DEATH.",
		"THEY CALL ME\n\"THE CLEANER.\""
	}
}

local choose_difficulty = {
	title = "Choose Difficulty",
	items = {
		{
			label = difficulty_labels[difficulty_index][1],
			icon = difficulty_icons[difficulty_index][1],
			func = function()
				rott.setDifficulty(1)
				rott.startGame()
			end
		},
		{
			label = difficulty_labels[difficulty_index][2],
			icon = difficulty_icons[difficulty_index][2],
			func = function()
				rott.setDifficulty(2)
				rott.startGame()
			end
		},
		{
			label = difficulty_labels[difficulty_index][3],
			icon = difficulty_icons[difficulty_index][3],
			func = function()
				rott.setDifficulty(3)
				rott.startGame()
			end
		},
		{
			label = difficulty_labels[difficulty_index][4],
			icon = difficulty_icons[difficulty_index][4],
			func = function()
				rott.setDifficulty(4)
				rott.startGame()
			end
		}
	}
}

local character_labels = {
	"TARADINO CASSATT",
	"THI BARRETT",
	"DOUG WENDT",
	"LORELEI NI",
	"IAN PAUL FREELEY"
}

local character_icons = {
	"NAME1",
	"NAME2",
	"NAME3",
	"NAME4",
	"NAME5"
}

return {
	title = "Choose Player",
	items = {
		{
			label = character_labels[1],
			icon = character_icons[1],
			menu = choose_difficulty,
			func = function()
				difficulty_index = math.random(1, 3)
				rott.setCharacter(1)
			end
		},
		{
			label = character_labels[2],
			icon = character_icons[2],
			menu = choose_difficulty,
			func = function()
				difficulty_index = math.random(1, 3)
				rott.setCharacter(2)
			end
		},
		{
			label = character_labels[3],
			icon = character_icons[3],
			menu = choose_difficulty,
			func = function()
				difficulty_index = math.random(1, 3)
				rott.setCharacter(3)
			end
		},
		{
			label = character_labels[4],
			icon = character_icons[4],
			menu = choose_difficulty,
			func = function()
				difficulty_index = math.random(1, 3)
				rott.setCharacter(4)
			end
		},
		{
			label = character_labels[5],
			icon = character_icons[5],
			menu = choose_difficulty,
			func = function()
				difficulty_index = math.random(1, 3)
				rott.setCharacter(5)
			end
		},
	}
}
