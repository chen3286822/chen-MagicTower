package.path = "res/script/?.lua;"..package.path
require "unity"
--module(...,package.seeall)

--[[
ս������������ɽ
����������ɣ�����
սǰ���飺����ң������Χ��������ǰȥȰ��
�������飺���ɽӴ�����󴥷�����ʼȰ������
�������飺��
]]

ID_Caocao = 1

function PostScene1_1()
--[[��ʼ��]]
	local scene = GetScene()
	scene:SetPlace("�ܲ��й�")
	scene:SetBackground(0)
	local caocao = scene:AddActor(ID_Caocao)
	caocao:SetPos(440,245)
	caocao:SetAction(eAction_Stand,eDir_Left)

--[[���鿪ʼ]]
	scene:PushAction(ID_Caocao,eAction_Hand,eDir_Left,100)
	num = scene:GetCurrentNum()
	scene:PushAction(ID_Caocao,eAction_Walk,eDir_Left,0,2,num)
end
