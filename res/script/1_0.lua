package.path = "res/script/?.lua;"..package.path
require "unity"

--[[
��������ܲ١��ڽ����ĺ������
�ص㣺�ܲ��й�
ʱ�䣺ĳʱ��
���¹��ţ��ܲ������ڽ����ĺһͬ���ֹ���Ȱ������֮�£��ĺ��Ϊ
�����������Ͷ��������ɱ���Ծ��󻼣��ڽ���Ϊ����������֮����������
��Ϊ���Դ���������ɩɩ����...

]]

Scene_Palace = 1

ID_Caocao = 1
ID_Xiahoudun = 2
ID_Yujin = 3
ID_Zhangliao = 5

--[[��ʼ��]]
local scene = GetScene()
scene:SetPlace("�ܲ��й�")
scene:SetBackground(Scene_Palace)
local caocao = scene:AddActor(ID_Caocao)
caocao:SetPos(440,245)
caocao:SetAction(eAction_Stand,eDir_Left)
local yujin = scene:AddActor(ID_Yujin)
yujin:SetPos(410,100)
yujin:SetAction(eAction_Stand,eDir_Down)
local xiahoudun = scene:AddActor(ID_Xiahoudun)
xiahoudun:SetPos(160,240)
xiahoudun:SetAction(eAction_Stand,eDir_Up)
local zhangliao = scene:AddActor(ID_Zhangliao)
zhangliao:SetHide(true)
zhangliao:SetPos(50,40)

--[[���鿪ʼ]]
scene:PushAction(ID_Caocao,eAction_Hand,eDir_Left,100)
local num = scene:GetCurrentNum()
scene:PushWords(1,"�ܲ�","����Ѵ�Һ�������Ϊ�˹��Ƴ�֮��",num)
scene:PushWords(1,"�ܲ�","�������Ҳ֪����������ʮ�־�����Ƴ����������壬һֱ��Ҫ���Ƴ��ձ�����",num)
scene:PushAction(ID_Caocao,eAction_Angry,eDir_Left,0)
scene:PushWords(3,"�ܲ�","�����������亱�е��½�����Ȼ���������֯ϯ���ĵĴ���������ˣ�ʵ�ڿɺޣ�")
scene:PushAction(ID_Caocao,eAction_Stand,eDir_Left,0)
scene:PushWords(1,"�ܲ�","���������Ƴ���Χ������������֪����������Ͷ�����ң��������������˽�ģ���һ����������������֪��λ��������ʲô�취��")
scene:PushAction(ID_Xiahoudun,eAction_Hand,eDir_Up,0)
num = scene:GetCurrentNum()
scene:PushWords(224,"�ĺ","�ϵ£�����Ϊ��������̫�����壬��Ҫ�����������ǲ����ܵġ����������󣬴������ս���������˫����֮�����ڴ������̣�������Ž������ǵ��ĸ��󻼡�",num)
scene:PushWords(224,"�ĺ","����Ϊ����˴˻���ɱ�������Ծ���",num)
scene:PushAction(ID_Xiahoudun,eAction_Stand,eDir_Up,0)
num = scene:GetCurrentNum()
scene:PushWords(1,"�ܲ�","..........",num)
scene:PushWords(1,"�ܲ�","��������ô��Ϊ��",num)
scene:PushAction(ID_Yujin,eAction_Salute,eDir_Down,0)
num = scene:GetCurrentNum()
scene:PushWords(11,"�ڽ�","��ة�࣬����������Ȼ���ң���������ة�������Ը߹ٺ�»������ÿ����ÿ�������ǰ������ס��̽�ӣ��������������Ҳ������֮�����ҳϵ���Ҳ�ᱻة����������ʿ����Ϊ���ж���",num)
scene:PushWords(11,"�ڽ�","����Ϊة�������һ�����Դ��������",num)
scene:PushAction(ID_Yujin,eAction_Stand,eDir_Down,0)
scene:PushAction(ID_Xiahoudun,eAction_Angry,eDir_Up,0)
num = scene:GetCurrentNum()
scene:PushWords(224,"�ĺ","ʲô���������ϵ�ÿ������̽��һ��������",num)
scene:PushWords(224,"�ĺ","�ϵ�����˴�Ķ������ҿ��ʲ����������",num)
scene:PushAction(ID_Xiahoudun,eAction_Stand,eDir_Up,0,0)
scene:PushWords(224,"�ĺ","�ϵ£���������ɱ�������Ժ���¾ͺ������������Ļ����˰�����������ս���������Ե�����۽�������������ɱ������",num)
scene:PushWords(190,"ʿ��","�������ɽ��������")
scene:PushWords(1,"�ܲ�","Ŷ������������")
scene:PushAction(ID_Zhangliao,eAction_Appear,eDir_Right)
scene:PushAction(ID_Zhangliao,eAction_Walk,eDir_Right,0,3)
scene:PushAction(ID_Zhangliao,eAction_Salute,eDir_Right,0)
scene:PushWords(12,"����","����ة��")
num = scene:GetCurrentNum()
scene:PushAction(ID_Caocao,eAction_Hand,eDir_Left,0,0,num)
scene:PushWords(4,"�ܲ�","��Զ�����������á�����������Ԫ�á��������������ʹ���Ƴ�Ͷ�����ң�����Ƴ�Ҳ���е㽻�飬����ʲô�취��",num)
scene:PushAction(ID_Caocao,eAction_Stand,eDir_Left,0,0)
scene:PushWords(12,"����","ة�࣬������Ϊ�˴��¶�����")
scene:PushWords(2,"�ܲ�","Ŷ�����ٵ�����")
scene:PushWords(12,"����","����˿̱�Χ��ة���֪�˿�����ĵ���ʲô��")
scene:PushWords(1,"�ܲ�","�Ƴ�������˫�������������ţ����������Ҿ죬�˿�Ӧ�������ɩɩ�İ�Σ�ɡ�")
num = scene:GetCurrentNum()
scene:PushWords(12,"����","ة��߼�������Թ�����������ΪҪЮ�����ǲ���Ͷ���ģ��������������ɩɩ�İ�Σ���������һ���ḩ�׳Ƴ���",num)
scene:PushWords(12,"����","���併��ة��ӱ��ź�������������������Ĵ��������������ܹ����������֮������������Ϣ���ö���֮��ʯ��Ҳ�����Ϊة������",num)
scene:PushAction(ID_Caocao,eAction_Hand,eDir_Left,0,0)
scene:PushWords(4,"�ܲ�","��Զ���ԣ��������ġ����¾ͽ�����죬��ȥ�ٻأ�")
scene:PushWords(12,"����","�ǣ��������ȥ˵������")
scene:PushAction(ID_Caocao,eAction_Stand,eDir_Left)
scene:PushAction(ID_Zhangliao,eAction_Walk,eDir_Left,0,3)
scene:PushAction(ID_Zhangliao,eAction_Hide,eDir_Left)
scene:PushAction(ID_Xiahoudun,eAction_Salute,eDir_Up,0,0)
scene:PushWords(224,"�ĺ","���ҵ�Ҳ����ˡ�")
scene:PushWords(1,"�ܲ�","�ţ��Ǿ����°ɡ�")
num = scene:GetCurrentNum()
scene:PushAction(ID_Xiahoudun,eAction_Walk,eDir_Left,0,2,num)
scene:PushAction(ID_Yujin,eAction_Walk,eDir_Left,0,2,num)
scene:PushAction(ID_Xiahoudun,eAction_Hide,eDir_Left)
scene:PushAction(ID_Yujin,eAction_Hide,eDir_Left)
