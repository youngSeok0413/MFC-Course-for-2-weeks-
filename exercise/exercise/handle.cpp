//��ó : https://blog.naver.com/tipsware/221464968465
#include <iostream>
#include <string.h>


using namespace std;

//�ڵ� ���� ���� �ڷ��� ����
typedef unsigned int HANDLE;

struct HandleData
{
	HANDLE h;	//�ڵ� �� : 0�̸� ������� ���� ����
	char* p_string;	// ���ڿ��� ����� �޸𸮸� ����ų ������;
};

class HandleManager
{
private:
	// �ִ� �ڵ� ������ ������ ������ �ڵ� ��ȣ ������ ���� ���� ����
	unsigned int m_max_count, m_gen_value;
	// �ڵ鰪�� ���ڿ� �ּҸ� �Բ� ������ �޸��� �ּҸ� ������ ������
	HandleData* mp_data;

public:
	HandleManager(unsigned int a_max_count = 50){
		m_gen_value = 1; // �ڵ� ���� ���� ���� 1�� ����
		m_max_count = a_max_count; // �ִ� �ڵ� ���� ����
		//�ڵ� ���� ���ڿ� �ּҸ� ������ �޸𸮸� ����
		mp_data = new HandleData[m_max_count];
		//������ �޸𸮸� 0���� �ʱ�ȭ
		memset(mp_data, 0, sizeof(HandleData) * m_max_count);
		cout << "�ڵ� �����ڰ� �����Ǿ����ϴ�!!" << endl;
	}

	~HandleManager(){
		HandleData* p = mp_data;
		for (unsigned int i = 0; i < m_max_count; i++) {
			// �ڵ� ���� 0�� �ƴ϶�� ���� �ش� ��ġ�� ���ڿ��� �Ҵ�Ǿ� �ִٴ� ��
			//p_string�� ����Ű�� �޸𸮸� �����ؾ� �մϴ�.
			if (p->h) delete[] p->p_string;
			p++;
		}

		delete[] mp_data;
		cout << "�ڵ� �����ڰ� ���ڿ��� ��� �����߽��ϴ�." << endl;
	}

	HANDLE InsertString(const char* ap_string) {
		HandleData* p = mp_data;
		unsigned int i;
		//���� �ڵ� ���� ������ �ʴ� ���� Ž��
		for (i = 0; i, m_max_count; i++) {
			if (p->h == 0) break;
			i++;
		}

		if (i < m_max_count) {
			//�ڵ� ������ ���� ���
			p->h = m_gen_value++;
			int len = strlen(ap_string) + 1;
			p->p_string = new char[len];
			memcpy(p->p_string, ap_string, len);
			cout << p->p_string << " : ���ڿ� �߰�!!" << endl;

			return p->h;
		}

		cout << "�� �̻� ���ڿ��� �߰��� �� �����ϴ�!!" << endl;
		return 0xFFFFFFFF;
	}

	void PrintString(HANDLE ah_string) {
		HandleData* p = mp_data;

		for (unsigned int i = 0; i < m_max_count; i++) {
			if (p->h == ah_string) {
				cout << p->p_string << endl;
				return;
			}

			p++;
		}

		cout << "��ȿ���� ���� �ڵ� ���� ����Ͽ����ϴ�!" << endl;
	}

	void DeleteString(HANDLE ah_string) {
		HandleData* p = mp_data;
		for (unsigned int i = 0; i < m_max_count; i++) {
			if (p->h == ah_string) {
				cout << p->p_string << " : ����!!" << endl;
				delete[] p->p_string;
				p->h = 0;
				return;
			}

			p++;
		}

		cout << "��ȿ���� ���� �ڵ� ���� ����߽��ϴ�!" << endl;
	}
};

int main() {
	HandleManager str_list;

	HANDLE h_str = str_list.InsertString("Hello~ tipsware!!");

	if (h_str != 0xFFFFFFFF) {
		str_list.PrintString(h_str);
		str_list.DeleteString(h_str);
	}

	str_list.PrintString(h_str);

	return 0;
}

