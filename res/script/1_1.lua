package.path = "res/script/?.lua;"..package.path
require "unity"
--module(...,package.seeall)

function PreFight1()
	local map = GetCurMap();
	map:SetTurns(20);
	local player1 = GetChar();
	local block = tagBlock(5,5);
	player1:Init(map:GetLevel(),115,100,1,block);
	player1:SetCamp(0);
	player1:SetMoveAbility(4,map);
	player1:SetName("уеаи");
	local creaMgr = GetCreature();
	creaMgr:AddFriend(player1);

	--map:AddTrigger(0,"turnUp",3,-1,-1,-1,-1);
	map:AddTrigger(2,"touch",-1,100,4,-1,-1);
	map:AddTrigger(1,"location",-1,100,-1,2,3);
	map:AddTrigger(3,"kill",-1,100,3,-1,-1);
end

function turnUp()
	MyMessageBox("time is up!");
end

function touch()
	MyMessageBox("touch the target!");
	local creaMgr = GetCreature();
	creaMgr:SetPause(true);
end

function location()
	MyMessageBox("go to the target!");
end

function kill()
	MyMessageBox("kill the target!");
end
