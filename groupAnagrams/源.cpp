#include<iostream>
#include<string>
#include<vector>
#include<unordered_map>
#include<ranges>
using namespace std;

/*
״̬���������֣�DELETED���ڲ��Ҵ��ڳ�ͻ������ʱ��ȷ�����߼�
����������������ϣ���ֵ��ͬ������10��20��30����5����0
��ô0��λ��10��1��λ��20��2��λ��30
���ֻ��EMPTY��OCCUPIED����ɾȥ20��1��λ״̬ΪEMPTY
��֮�����30��ʱ��ͻ���Ϊ�鵽2��λΪEMPTY��������ѯ
�����߼�����
*/
enum SlotStatus { EMPTY, OCCUPIED, DELETED };

struct HashSlot {
	char value;
	SlotStatus status;
};

struct Message {

	//��Ϣ״̬��0��ʾ�ɹ���1��ʾ�쳣 
	int status;
	//��Ϣ�ı���information̫�����Ժ���content 
	string information;
};

class HashTable {
private:
	int capacity;//���� 
	int primeFactor;//�������������������� 
	HashSlot* hashTable;

	//���ַ�ת��Ϊ��ֵ���ڼ��� 
	int hashFunction(char value) {
		return static_cast<int>(value) % capacity;
	}

	//�ж��Ƿ�Ϊ����
	bool isPrime(int number) {
		if (number <= 1)
			return false;
		if (number == 2)
			return true;
		if (number % 2 == 0)
			return false;
		for (int i = 3; i * i <= number; i += 2) {
			if (number % i == 0) {
				return false;
			}
		}
		return true;
	}

	//����������
	int getPrimeFactor(int capacity) {

		//�������������ʼУ�� 
		int start = (capacity % 2 == 0) ? capacity - 1 : capacity;

		for (int i = start; i >= 3; i -= 2) {
			if (isPrime(i)) {
				return i;
			}
		}

		return 2;
	}

public:
	explicit HashTable(int size) : capacity(size) {

		hashTable = new HashSlot[capacity];
		primeFactor = getPrimeFactor(capacity);
		for (int i = 0; i < capacity; i++) {
			hashTable[i].status = EMPTY;
		}

	}

	Message insert(char value) {
		int index = hashFunction(value);
		int flagIndex = index;
		Message message;

		//����̽����ɢ�д����ͻ 
		do {
			//���λ��Ϊ�գ������ɹ� 
			if (hashTable[index].status == EMPTY || hashTable[index].status == DELETED) {
				hashTable[index].status = OCCUPIED;
				hashTable[index].value = value;
				message.status = 0;
				message.information = "Insert completely!";
				index++;
				break;
			}
			//���λ�ò��գ�����ֵ��ͬ���򷵻��Ѵ��� 
			else if (hashTable[index].value == value) {
				message.status = 1;
				message.information = "Value has exsited!";
				break;
			}
			//���λ�ò��գ�����ֵ��ͬ�������ͻ 
			else {
				index = (index + 1) % primeFactor;
			}
		} while (flagIndex != index);

		//�������λ�ö����գ���������� 
		if (flagIndex == index) {
			message.status = 1;
			message.information = "HashTable if full!";
		}
		return message;
	}

	Message search(char value) {
		int index = hashFunction(value);
		int flagIndex = index;
		Message message;

		do {
			//��λ��Ϊ�գ����ֵ�������ڱ� 
			if (hashTable[index].status == EMPTY) {
				message.status = 1;
				message.information = "Value does not exsit!";
				break;
			}
			//��λ�ò��գ���ֵ��ͬ������ҳɹ�����λ�ü�����Ϣ���� 
			else if (hashTable[index].value == value) {
				message.status = 0;
				message.information = to_string(index);
				index++;
				break;
			}
			//��λ�ò��գ���ֵ����ͬ�������ͻ����Ų��� 
			else
				index = (index + 1) % primeFactor;
		} while (index != flagIndex);

		if (index == flagIndex) {
			message.status = 1;
			message.information = "Value does not exsit!";
		}

		return message;
	}

	Message remove(char value) {
		Message message = search(value);
		int index;
		if (message.status == 0) {
			index = stoi(message.information);
			hashTable[index].status = DELETED;
			message.information = "Delete completetly!";
		}
		else {
			message.information = "Delete failed!";
		}
		return message;
	}

	Message clear() {
		Message message;
		for (int i = 0; i < capacity; i++)
			hashTable[i].status = EMPTY;
		message.information = "HashTable has been cleared";
		message.status = 0;
		return message;
	}

	void printTable() const {
		for (int i = 0; i < capacity; ++i) {
			cout << "Index " << i << ": ";
			switch (hashTable[i].status) {
			case OCCUPIED:
				cout << hashTable[i].value;
				break;
			case DELETED:
				cout << "DELETED";
				break;
			default:
				cout << "EMPTY";
			}
			cout << endl;
		}
	}
};

class Solution {
public:
	vector<vector<string>> groupAnagrams(vector<string>& strs) {
		HashTable ht(7);
		vector<vector<string>> vectorArray;
		char chr[9];
		int length = 0, flag = 0;
		Message message;

		for (int i = 0; i < strs.size(); i++) {

			//��strs����ַ�������ȡ������vectorArray����������ַ���Ĺ�ϣ�� 
			vectorArray.push_back(vector<string>());
			vectorArray[i].push_back(strs[i]);
			length = strs[i].size();

			for (int j = 0; j < strs[i].size(); j++)
				//���մ���strs����ַ������Ϊ�ַ������ϣ�� 
				ht.insert(strs[i][j]);

			//ȡ��strs�иղŴ���vectorArray���ַ����ĺ����ַ��� 
			for (int j = i + 1; j < strs.size(); ) {
				flag = 0;//flag����ȷ����ǰ���ڴ�����ַ����Ƿ����ǰ������һ�� 
				for (int k = 0; k < strs[j].size(); k++) {
					//�����ַ�����������ĸ�Ƿ�������ڹ�ϣ���� 
					message = ht.search(strs[j][k]);
					if (message.status == 0)
						flag++;
				}
				if (flag == length)
				{
					//�����ĸ�������ڹ�ϣ���ҳ���һ�£���Ѹ��ַ��������ڴ�����vectorArray��ǰ���ĺ��� 
					vectorArray[i].push_back(strs[j]);
					//��strs��ɾ�����ַ��� 
					strs.erase(strs.begin() + j);
					continue;
				}
				j++;
			}
			ht.clear();
			//����Ӧ�ü��������ϣ��Ĵ��� 
		}
		return vectorArray;
	}
};

class Solution2 {

public:

	vector<vector<string>> groupAnagrams(vector<string>& strs) {

		unordered_map<string, vector<string>> m;

		for (string& s : strs) {

			string sorted_s = s;

			ranges::sort(sorted_s);//ԭ���������� ����Ҫ#include<ranges> 

			m[sorted_s].push_back(s); // sorted_s ��ͬ���ַ����ֵ�ͬһ��

		}



		vector<vector<string>> ans;

		ans.reserve(m.size()); // Ԥ����ռ�

		for (auto& [_, value] : m) {
			/*
			���ڷ�Χ�Ľṹ���󶨵�ѭ�������ڱ�����ϣ��ļ�ֵ��
			auto&[_,value]ʹ�ýṹ���󶨽���ϣ���ÿ����ֵ�Խ⹹Ϊ������������_��Ϊռλ������������ʾ���Դ˱�����������ʹ�á�_����������������ͻ��
			auto&��ʾ�Թ�ϣ���Ԫ��ʹ�����ã����⿽��
			���ʹ��auto����for(auto entry : m){ans.push_back(entry.second)}��ÿ�ζ����m�е�Ԫ�ظ�ֵ��entry���ٲ���
			*/
			//��ϣ��m�Ѿ�������ˣ�������һ�����ǰѹ�ϣ���ÿ��ͷ���װ��vector<vector<string>>���͵�vector������ 
			ans.push_back(value);

		}

		return ans;

	}
	//���ߣ����ɽ����
	//���ӣ�https://leetcode.cn/problems/group-anagrams/solutions/2718519/ha-xi-biao-fen-zu-jian-ji-xie-fa-pythonj-1ukv/
	//��Դ�����ۣ�LeetCode��
	//����Ȩ���������С���ҵת������ϵ���߻����Ȩ������ҵת����ע��������
};


int main() {
	//    HashTable ht(7);
	//    Message message;
	//
	//    // ����ʾ��
	//    message=ht.insert('a'); // ��ϣֵ6
	//    cout << message.information << endl;
	//    ht.insert('b'); // ��ϣֵ0
	//    ht.insert('c'); // ��ϣֵ1
	//    ht.insert('h'); // ��ϣֵ6 �� ��ͻ̽�⵽λ��0��1��2
	//
	//    cout << "Initial table:" << endl;
	//    ht.printTable();
	//
	//    // ����ʾ��
	//    message=ht.search('c');
	//    cout << "\nSearch 'c': " << message.information << endl;
	//
	//
	//    // ɾ��ʾ��
	//    ht.remove('b');
	//    cout << "\nAfter removing 'b':" << endl;
	//    ht.printTable();
	//
	//    // ����ʾ��
	//    message=ht.search('c');
	//    cout << "\nSearch 'c': " << message.information << endl;
	//
	//    // ���²�����ɾ��λ�õ�Ԫ��
	//    ht.insert('b');
	//    cout << "\nAfter reinserting 'b':" << endl;
	//    ht.printTable();
	vector<vector<string>> result;
		vector<string> vec;
	//	vec.push_back("eat");
	//	vec.push_back("tea");
	//	vec.push_back("tan");
	//	vec.push_back("ate");
	//	vec.push_back("nat");
	//	vec.push_back("bat");
	vec.push_back("a");
	vec.push_back("");
	Solution solution;
	result = solution.groupAnagrams(vec);
	cout << "[";
	for (int i = 0; i < result.size(); i++) {
		cout << "[";
		for (string element : result[i]) {
			cout << "\"" << element << "\",";
		}
		cout << "],";
	}
	cout << "]";
	return 0;
}











