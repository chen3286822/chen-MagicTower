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

function PreScene1_2()
	local scene = GetScene()
	scene:SetPlace("�ܲ��й�")
	scene:SetBackground(0)
end
