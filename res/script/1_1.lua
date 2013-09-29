package.path = "res/script/?.lua;"..package.path
require "unity"
--module(...,package.seeall)

function PreFight()
	SetCurrentMap(1);
	local map = GetCurMap();
	map:SetTurns(20);
end
