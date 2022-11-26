//출처 : https://blog.naver.com/tipsware/221464968465
#include <iostream>
#include <string.h>


using namespace std;

//핸들 값을 위한 자료형 선언
typedef unsigned int HANDLE;

struct HandleData
{
	HANDLE h;	//핸들 값 : 0이면 사용하지 않은 상태
	char* p_string;	// 문자열이 저장된 메모리를 가리킬 포인터;
};

class HandleManager
{
private:
	// 최대 핸들 개수를 저장할 변수와 핸들 번호 생성을 위한 변수 선언
	unsigned int m_max_count, m_gen_value;
	// 핸들값과 문자열 주소를 함께 저장할 메모리의 주소를 저장할 포인터
	HandleData* mp_data;

public:
	HandleManager(unsigned int a_max_count = 50){
		m_gen_value = 1; // 핸들 생성 기준 값을 1로 설정
		m_max_count = a_max_count; // 최대 핸들 개수 저장
		//핸들 값과 문자열 주소를 저장할 메모리를 생성
		mp_data = new HandleData[m_max_count];
		//생성된 메모리를 0으로 초기화
		memset(mp_data, 0, sizeof(HandleData) * m_max_count);
		cout << "핸들 관리자가 생성되었습니다!!" << endl;
	}

	~HandleManager(){
		HandleData* p = mp_data;
		for (unsigned int i = 0; i < m_max_count; i++) {
			// 핸들 값이 0이 아니라는 뜻은 해당 위치에 문자열이 할당되어 있다는 것
			//p_string이 가리키는 메모리를 해제해야 합니다.
			if (p->h) delete[] p->p_string;
			p++;
		}

		delete[] mp_data;
		cout << "핸들 관리자가 문자열을 모두 제거했습니다." << endl;
	}

	HANDLE InsertString(const char* ap_string) {
		HandleData* p = mp_data;
		unsigned int i;
		//아직 핸들 값이 사용되지 않는 공간 탐색
		for (i = 0; i, m_max_count; i++) {
			if (p->h == 0) break;
			i++;
		}

		if (i < m_max_count) {
			//핸들 공간이 남는 경우
			p->h = m_gen_value++;
			int len = strlen(ap_string) + 1;
			p->p_string = new char[len];
			memcpy(p->p_string, ap_string, len);
			cout << p->p_string << " : 문자열 추가!!" << endl;

			return p->h;
		}

		cout << "더 이상 문자열을 추가할 수 없습니다!!" << endl;
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

		cout << "유효하지 않은 핸들 값을 사용하였습니다!" << endl;
	}

	void DeleteString(HANDLE ah_string) {
		HandleData* p = mp_data;
		for (unsigned int i = 0; i < m_max_count; i++) {
			if (p->h == ah_string) {
				cout << p->p_string << " : 삭제!!" << endl;
				delete[] p->p_string;
				p->h = 0;
				return;
			}

			p++;
		}

		cout << "유효하지 않은 핸들 값을 사용했습니다!" << endl;
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

