package.path = "res/script/?.lua;"..package.path
require "unity"
--module(...,package.seeall)

function PreFight1()
	local map = GetCurMap();
	map:SetOffXY(0,4);
	map:SetTurns(20);
	map:SetVictoryCondition(1,5)
	local player1 = GetChar();
	local block = tagBlock(5,5);
	player1:Init(map:GetLevel(),115,100,1,block);
	player1:SetCamp(0);
	player1:SetMoveAbility(4);
	player1:SetName("’≈¡…");
	player1:AddEquip(0,23);
	player1:AddEquip(1,37);
	player1:AddEquip(2,56);
	local creaMgr = GetCreature();
	creaMgr:AddFriend(player1);


	local cha1 = creaMgr:GetCreature(2);
	cha1:SetHide(true);
	local cha2 = creaMgr:GetCreature(3);
	cha2:SetHide(true);
	--local cha3 = creaMgr:GetCreature(5);
	--cha3:SetHide(true);


	--map:AddTrigger(0,"turnUp",3,-1,-1,-1,-1);
	map:AddTrigger(2,"touch",-1,100,4,-1,-1);
	map:AddTrigger(1,"location",-1,100,-1,2,3);
	map:AddTrigger(3,"kill",-1,100,5,-1,-1);
end

function turnUp()
	MyMessageBox("time is up!");
end

function touch()
	--MyMessageBox("touch the target!");
	local creaMgr = GetCreature();
	creaMgr:SetPause(true);

	local cha1 = creaMgr:GetCreature(2);
	cha1:SetHide(false);
	local cha2 = creaMgr:GetCreature(3);
	cha2:SetHide(false);
	local cha3 = creaMgr:GetCreature(5);
	cha3:SetHide(false);

	local cha = creaMgr:GetCreature(100);
	cha:Turn(3,1000);
	cha:Attack();
	cha:Crit();
	cha:Attacked(1000);
	cha:Defend(1000);
	cha:Hurt(1000);
	cha:Disappear();
	cha:Appear(1,2,0);
	cha:Step(1000);
	cha:Wait(2000);
	cha:MoveTo(3,0);
	cha:Talk(3,"π˛π˛π˛")
end

function location()
	MyMessageBox("go to the target!");
end

function kill()
	local creaMgr = GetCreature();
	creaMgr:SetPause(true);
	local cha = creaMgr:GetCreature(100);
	cha:Turn(3,1000);
	cha:Attack();
	cha:Attack();
	cha:Talk(3,"÷’”⁄…±µÙ¡À£°")
end
