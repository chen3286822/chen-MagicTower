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
	player1:SetName("张辽");
	player1:AddEquip(0,23);
	player1:AddEquip(1,37);
	player1:AddEquip(2,56);
	local creaMgr = GetCreature();
	creaMgr:AddFriend(player1);

	local player2 = GetChar();
	local block2 = tagBlock(8,5);
	player2:Init(map:GetLevel(),115,101,1,block2);
	player2:SetCamp(0);
	player2:SetMoveAbility(4);
	player2:SetName("张辽");
	player2:AddEquip(0,23);
	player2:AddEquip(1,37);
	player2:AddEquip(2,56);
	local creaMgr = GetCreature();
	creaMgr:AddFriend(player2);

	local player4 = GetChar();
	local block4 = tagBlock(11,9);
	player4:Init(map:GetLevel(),115,103,1,block4);
	player4:SetCamp(0);
	player4:SetMoveAbility(4);
	player4:SetName("张辽");
	player4:AddEquip(0,23);
	player4:AddEquip(1,37);
	local creaMgr = GetCreature();
	creaMgr:AddFriend(player4);

	local player5 = GetChar();
	local block5 = tagBlock(13,9);
	player5:Init(map:GetLevel(),115,104,1,block5);
	player5:SetCamp(1);
	player5:SetMoveAbility(4);
	player5:SetName("张辽");
	player5:AddEquip(0,23);
	player5:AddEquip(1,37);
	local creaMgr = GetCreature();
	player5:SetAIStrategy(1);
	creaMgr:AddEnemy(player5);

	local player3 = GetChar();
	local block3 = tagBlock(12,9);
	player3:Init(map:GetLevel(),115,102,1,block3);
	player3:SetCamp(1);
	player3:SetMoveAbility(4);
	player3:SetName("张辽");
	player3:AddEquip(0,23);
	player3:AddEquip(1,37);
	--player3:AddEquip(2,56);
	player3:SetAIStrategy(1);
	local creaMgr = GetCreature();
	creaMgr:AddEnemy(player3);


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
	cha:Talk(3,"哈哈哈")
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
	cha:Talk(3,"终于杀掉了！")
end
