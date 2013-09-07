package.path = "res/script/?.lua;"..package.path
require "unity"

--[[
出场人物：曹操、于禁、夏侯惇、张辽
地点：曹操行宫
时间：某时间
故事梗概：曹操召来于禁和夏侯惇一同商讨关于劝降关羽之事，夏侯惇认为
关羽根本不会投降，主张杀掉以绝后患；于禁认为可以以利诱之；而张辽则
认为可以从他的两个嫂嫂下手...

]]

Scene_Palace = 1

ID_Caocao = 1
ID_Xiahoudun = 2
ID_Yujin = 3
ID_Zhangliao = 5

--[[初始化]]
local scene = GetScene()
scene:SetPlace("曹操行宫")
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

--[[剧情开始]]
scene:PushAction(ID_Caocao,eAction_Hand,eDir_Left,100)
local num = scene:GetCurrentNum()
scene:PushWords(1,"曹操","今天把大家喊来就是为了关云长之事",num)
scene:PushWords(1,"曹操","想必你们也知道，我素来十分敬佩关云长的武勇忠义，一直想要把云长收编麾下",num)
scene:PushAction(ID_Caocao,eAction_Angry,eDir_Left,0)
scene:PushWords(3,"曹操","但是这样世间罕有的勇将，竟然被刘备这个织席贩履的大耳贼忽悠了，实在可恨！")
scene:PushAction(ID_Caocao,eAction_Stand,eDir_Left,0)
scene:PushWords(1,"曹操","现在天幸云长被围，刘备生死不知，我想让其投降于我，但是这人我是了解的，他一定是宁死不降，不知两位爱将可有什么办法？")
scene:PushAction(ID_Xiahoudun,eAction_Hand,eDir_Up,0)
num = scene:GetCurrentNum()
scene:PushWords(224,"夏侯惇","孟德，我认为关羽这人太过忠义，想要降服他的心是不可能的。自吕布死后，此人武艺近乎天下无双，加之其善于带兵打仗，如果活着将是我们的心腹大患。",num)
scene:PushWords(224,"夏侯惇","我认为不如乘此机会杀了他，以绝后患",num)
scene:PushAction(ID_Xiahoudun,eAction_Stand,eDir_Up,0)
num = scene:GetCurrentNum()
scene:PushWords(1,"曹操","..........",num)
scene:PushWords(1,"曹操","文则你怎么认为？",num)
scene:PushAction(ID_Yujin,eAction_Salute,eDir_Down,0)
num = scene:GetCurrentNum()
scene:PushWords(11,"于禁","禀丞相，关羽这人虽然愚忠，但是我想丞相先许以高官厚禄，接着每天最好可以亲自前往关羽住处探视，殷勤相待，这样也许数月之后，再忠诚的人也会被丞相这礼贤下士的行为所感动。",num)
scene:PushWords(11,"于禁","我认为丞相的心意一定可以传达给关羽！",num)
scene:PushAction(ID_Yujin,eAction_Stand,eDir_Down,0)
scene:PushAction(ID_Xiahoudun,eAction_Angry,eDir_Up,0)
num = scene:GetCurrentNum()
scene:PushWords(224,"夏侯惇","什么！你想让孟德每天屈身探望一个降将？",num)
scene:PushWords(224,"夏侯惇","孟德有如此大的度量，我可咽不下这口气！",num)
scene:PushAction(ID_Xiahoudun,eAction_Stand,eDir_Up,0,0)
scene:PushWords(224,"夏侯惇","孟德，这次如果不杀了他，以后恐怕就很难再有这样的机会了啊！与其他日战场上与他对敌损兵折将，不如下令射杀了他！",num)
scene:PushWords(190,"士兵","报！张辽将军求见！")
scene:PushWords(1,"曹操","哦，请他进来。")
scene:PushAction(ID_Zhangliao,eAction_Appear,eDir_Right)
scene:PushAction(ID_Zhangliao,eAction_Walk,eDir_Right,0,3)
scene:PushAction(ID_Zhangliao,eAction_Salute,eDir_Right,0)
scene:PushWords(12,"张辽","见过丞相")
num = scene:GetCurrentNum()
scene:PushAction(ID_Caocao,eAction_Hand,eDir_Left,0,0,num)
scene:PushWords(4,"曹操","文远，你来的正好。方才我正与元让、文则商讨如何能使关云长投降于我，你和云长也算有点交情，你有什么办法？",num)
scene:PushAction(ID_Caocao,eAction_Stand,eDir_Left,0,0)
scene:PushWords(12,"张辽","丞相，我正是为了此事而来。")
scene:PushWords(2,"曹操","哦？速速道来。")
scene:PushWords(12,"张辽","关羽此刻被围，丞相可知此刻他最担心的是什么？")
scene:PushWords(1,"曹操","云长骁勇无双，并不惧死。嗯，他身负刘备家眷，此刻应是最担心他嫂嫂的安危吧。")
num = scene:GetCurrentNum()
scene:PushWords(12,"张辽","丞相高见，如果以关羽自身性命为要挟，他是不会投降的，但是如果换成其嫂嫂的安危，我想关羽一定会俯首称臣。",num)
scene:PushWords(12,"张辽","待其降后，丞相加倍优厚对他，想他跟着刘备四处奔波，哪里享受过富贵生活，加之封锁刘备的消息，久而久之，石人也会诚心为丞相卖力",num)
scene:PushAction(ID_Caocao,eAction_Hand,eDir_Left,0,0)
scene:PushWords(4,"曹操","文远所言，甚合我心。此事就交由你办，速去速回！")
scene:PushWords(12,"张辽","是，属下这就去说服关羽")
scene:PushAction(ID_Caocao,eAction_Stand,eDir_Left)
scene:PushAction(ID_Zhangliao,eAction_Walk,eDir_Left,0,3)
scene:PushAction(ID_Zhangliao,eAction_Hide,eDir_Left)
scene:PushAction(ID_Xiahoudun,eAction_Salute,eDir_Up,0,0)
scene:PushWords(224,"夏侯惇","那我等也告辞了。")
scene:PushWords(1,"曹操","嗯，那就退下吧。")
num = scene:GetCurrentNum()
scene:PushAction(ID_Xiahoudun,eAction_Walk,eDir_Left,0,2,num)
scene:PushAction(ID_Yujin,eAction_Walk,eDir_Left,0,2,num)
scene:PushAction(ID_Xiahoudun,eAction_Hide,eDir_Left)
scene:PushAction(ID_Yujin,eAction_Hide,eDir_Left)
