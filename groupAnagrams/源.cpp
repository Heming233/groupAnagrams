#include<iostream>
#include<string>
#include<vector>
#include<unordered_map>
#include<ranges>
using namespace std;

/*
状态必须有三种，DELETED用于查找存在冲突的数据时正确运行逻辑
假设三个数经过哈希后键值相同，例如10、20、30除以5都是0
那么0号位存10，1号位存20，2号位存30
如果只有EMPTY和OCCUPIED，当删去20后，1号位状态为EMPTY
而之后查找30的时候就会因为查到2号位为EMPTY而结束查询
出现逻辑错误
*/
enum SlotStatus { EMPTY, OCCUPIED, DELETED };

struct HashSlot {
	char value;
	SlotStatus status;
};

struct Message {

	//消息状态，0表示成功，1表示异常 
	int status;
	//消息文本，information太长，以后用content 
	string information;
};

class HashTable {
private:
	int capacity;//容量 
	int primeFactor;//不大于容量的质数因子 
	HashSlot* hashTable;

	//将字符转换为数值便于计算 
	int hashFunction(char value) {
		return static_cast<int>(value) % capacity;
	}

	//判断是否为质数
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

	//计算质因子
	int getPrimeFactor(int capacity) {

		//从最近的奇数开始校验 
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

		//线性探测再散列处理冲突 
		do {
			//如果位置为空，则插入成功 
			if (hashTable[index].status == EMPTY || hashTable[index].status == DELETED) {
				hashTable[index].status = OCCUPIED;
				hashTable[index].value = value;
				message.status = 0;
				message.information = "Insert completely!";
				index++;
				break;
			}
			//如果位置不空，但数值相同，则返回已存在 
			else if (hashTable[index].value == value) {
				message.status = 1;
				message.information = "Value has exsited!";
				break;
			}
			//如果位置不空，且数值不同，则处理冲突 
			else {
				index = (index + 1) % primeFactor;
			}
		} while (flagIndex != index);

		//如果所有位置都不空，则表明表满 
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
			//若位置为空，则该值不存在于表 
			if (hashTable[index].status == EMPTY) {
				message.status = 1;
				message.information = "Value does not exsit!";
				break;
			}
			//若位置不空，且值相同，则查找成功，将位置计入消息返回 
			else if (hashTable[index].value == value) {
				message.status = 0;
				message.information = to_string(index);
				index++;
				break;
			}
			//若位置不空，但值不相同，则处理冲突后接着查找 
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

			//将strs里的字符串依次取出存入vectorArray，类似链地址法的哈希表 
			vectorArray.push_back(vector<string>());
			vectorArray[i].push_back(strs[i]);
			length = strs[i].size();

			for (int j = 0; j < strs[i].size(); j++)
				//将刚存入strs里的字符串拆解为字符存入哈希表 
				ht.insert(strs[i][j]);

			//取出strs中刚才存入vectorArray的字符串的后驱字符串 
			for (int j = i + 1; j < strs.size(); ) {
				flag = 0;//flag用于确定当前正在处理的字符串是否和其前驱长度一致 
				for (int k = 0; k < strs[j].size(); k++) {
					//检查该字符串包含的字母是否均存在于哈希表中 
					message = ht.search(strs[j][k]);
					if (message.status == 0)
						flag++;
				}
				if (flag == length)
				{
					//如果字母均存在于哈希表且长度一致，则把该字符串连接在存在于vectorArray的前驱的后面 
					vectorArray[i].push_back(strs[j]);
					//从strs中删除该字符串 
					strs.erase(strs.begin() + j);
					continue;
				}
				j++;
			}
			ht.clear();
			//这里应该加上清楚哈希表的代码 
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

			ranges::sort(sorted_s);//原地升序排序 ，需要#include<ranges> 

			m[sorted_s].push_back(s); // sorted_s 相同的字符串分到同一组

		}



		vector<vector<string>> ans;

		ans.reserve(m.size()); // 预分配空间

		for (auto& [_, value] : m) {
			/*
			基于范围的结构化绑定的循环，用于遍历哈希表的键值对
			auto&[_,value]使用结构化绑定将哈希表的每个键值对解构为两个变量，“_”为占位符变量名，表示忽略此变量（后续再使用“_”可能引发命名冲突）
			auto&表示对哈希表的元素使用引用，避免拷贝
			如果使用auto，即for(auto entry : m){ans.push_back(entry.second)}则每次都会把m中的元素赋值给entry后再操作
			*/
			//哈希表m已经分类好了，所以这一步就是把哈希表的每个头结点装进vector<vector<string>>类型的vector数组了 
			ans.push_back(value);

		}

		return ans;

	}
	//作者：灵茶山艾府
	//链接：https://leetcode.cn/problems/group-anagrams/solutions/2718519/ha-xi-biao-fen-zu-jian-ji-xie-fa-pythonj-1ukv/
	//来源：力扣（LeetCode）
	//著作权归作者所有。商业转载请联系作者获得授权，非商业转载请注明出处。
};


int main() {
	//    HashTable ht(7);
	//    Message message;
	//
	//    // 插入示例
	//    message=ht.insert('a'); // 哈希值6
	//    cout << message.information << endl;
	//    ht.insert('b'); // 哈希值0
	//    ht.insert('c'); // 哈希值1
	//    ht.insert('h'); // 哈希值6 → 冲突探测到位置0、1、2
	//
	//    cout << "Initial table:" << endl;
	//    ht.printTable();
	//
	//    // 搜索示例
	//    message=ht.search('c');
	//    cout << "\nSearch 'c': " << message.information << endl;
	//
	//
	//    // 删除示例
	//    ht.remove('b');
	//    cout << "\nAfter removing 'b':" << endl;
	//    ht.printTable();
	//
	//    // 搜索示例
	//    message=ht.search('c');
	//    cout << "\nSearch 'c': " << message.information << endl;
	//
	//    // 重新插入已删除位置的元素
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











