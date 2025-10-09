-- main.lua

return {
	title = "Main Menu",
	items = {
		{
			label = "NEW GAME",
			menu = require("newgame.lua")
		},
		{
			label = "COMM-BAT\x81 GAME",
			menu = require("commbat.lua")
		},
		{
			label = "RESTORE GAME",
			menu = require("loadgame.lua")
		},
		{
			label = "SAVE GAME",
			menu = require("savegame.lua")
		},
		{
			label = "OPTIONS",
			menu = require("options.lua")
		},
		{
			label = "ORDERING INFO",
			menu = require("order.lua")
		},
		{
			label = "VIEW SCORES",
			menu = require("viewscores.lua")
		},
		{
			label = "BACK TO DEMO"
		},
		{
			label = "QUIT",
			func = rott.quit
		}
	}
}
