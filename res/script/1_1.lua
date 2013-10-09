package.path = "res/script/?.lua;"..package.path
require "unity"
--module(...,package.seeall)

function PreFight1()
	local map = GetCurMap();
	map:SetTurns(20);
	local player1 = GetChar();
	local block = tagBlock(5,5);
	player1:Init(map:GetLevel(),34,100,1,block);
	player1:SetCamp(0);
	player1:SetMoveAbility(4,map);
	player1:SetName("Â·ÈË¼×");
	local creaMgr = GetCreature();
	creaMgr:AddFriend(player1);
end
