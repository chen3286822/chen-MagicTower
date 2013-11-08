#include "AIMgr.h"
#include "Character.h"

AIMgr g_AIMgr;

AIMgr::AIMgr()
{
	m_pCurAI = NULL;
	m_eStrategy = eAIStrategy_AttakTarget;
}

AIMgr::~AIMgr()
{

}

void AIMgr::SetCurCharacter(Character* cha)
{
	//�Ѿ����ù�
	if(m_pCurAI)
		return;

	m_pCurAI = cha;
}

Character* AIMgr::GetCurCharacter()
{
	return m_pCurAI;
}

bool AIMgr::DoAction()
{
	if(!m_pCurAI)
		return false;

	//���ݲ���ȷ���ж�
	/*
	*	��������ȹ���Ŀ����ԣ����ʧ�������Ǽ���ȫ������Ŀ�굥λ�����б��е�һ����λ����
	*	A���ҳ��ƶ�+������Χ�����Կ��Թ������ĵз���λ��
	*	�����Ŀ�굥λ���򹥻�֮(Ϊ��������������﹥��ʱ�ж������ʹ�ü�����ʹ������˺����ֶ�)�������ж��Ƿ���������������
	*	�ĵ�λ�����򹥻�֮�����򹥻���������ֵĵ�λ�����򹥻�
	*	Ѫ����͵�λ
	*	B���Ҳ����з���λ����ѡ����Ŀ�굥λǰ����ǰ����ʹ�����漼�ܣ�û�������
	*	��������ȱ����Լ����ԣ�
	*	A�������Լ��Ƿ��ڵз�Ŀ�깥����Χ�����Ƿ�ᱻ��������(���ڲ�ͬ���֣���ȡ��ͬ�㷨����
	*	�������˺���ħ��ϵȡ���ͷŵ�����˺�����)����Ļ�������ѡ��·���뿪����ʹ������ħ����
	*	���������Ļ�����ִ�����ȹ���Ŀ���A����
	*	B���Լ����ڰ�ȫ���������ȵȴ�������λǰ����ʹ�Լ����ں���ǰ���������ȶ��Լ��ͷ�����ħ�������
	*	��������ȱ���Ŀ����ԣ�
	*	A�����Ҫ������Ŀ���Ƿ��ڱ�������Χ���ǵĻ����Լ��Ƿ���Թ����������в�ĵ�λ�����ԵĻ�����ȥ������
	*	����ѡ����һ��в��λ��ִ���������裬�����еĻ����������в��λǰ��
	*	B������Ŀ�괦�ڰ�ȫ��Χ��ֱ�ӿ����������򱣻�Ŀ��ʩ������ħ�������ܵĻ�ֱ�Ӵ���
	*/
	switch(m_eStrategy)
	{
	case eAIStrategy_AttakTarget:
		{

		}
		break;
	case eAIStrategy_ProtectSelf:
		{

		}
		break;
	case eAIStrategy_ProtectTarget:
		{

		}
		break;
	}
	return true;
}

bool AIMgr::SelectRoute()
{
	if (!m_pCurAI)
		return false;

	
	return true;
}