package.path = "res/script/?.lua;"..package.path
require "unity"
--module(...,package.seeall)

--[[
战斗场景：吞吐山
剧情人物：张辽，关羽
战前剧情：张辽遥望关羽被围，急着上前去劝降
触发剧情：张辽接触关羽后触发，开始劝降关羽
结束剧情：无
]]

ID_Caocao = 1

function PostScene1_1()
--[[初始化]]
	local scene = GetScene()
	scene:SetPlace("曹操行宫")
	scene:SetBackground(0)
	local caocao = scene:AddActor(ID_Caocao)
	caocao:SetPos(440,245)
	caocao:SetAction(eAction_Stand,eDir_Left)

--[[剧情开始]]
	scene:PushAction(ID_Caocao,eAction_Hand,eDir_Left,100)
	num = scene:GetCurrentNum()
	scene:PushAction(ID_Caocao,eAction_Walk,eDir_Left,0,2,num)
end
