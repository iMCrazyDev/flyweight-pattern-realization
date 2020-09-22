// p_test.cpp : Этот файл содержит функцию "main". Здесь начинается и заканчивается выполнение программы.
//

#include <iostream>
#include <unordered_map>
#include <string>
#include <vector>


struct SharedState
{
	static enum Type { AMV, mp4, mp3, wav, mp5, ogg };
	std::string name;
	std::vector<unsigned char> bytes;
	Type dataType;

	SharedState(const std::string& _name, std::vector<unsigned char> _bytes, const Type _dataType)
		: name(_name), bytes(_bytes), dataType(_dataType)
	{
	}
	/*friend std::ostream& operator<<(std::ostream& os, const SharedState& ss)
	{
		return os << "[ " << ss.brand_ << " , " << ss.model_ << " , " << ss.color_ << " ]";
	}*/
};


class Flyweight
{
private:
	SharedState* shared_state_;

public:
	Flyweight(const SharedState* shared_state) : shared_state_(new SharedState(*shared_state))
	{

	}
	Flyweight(const Flyweight& other) : shared_state_(new SharedState(*other.shared_state_))
	{

	}
	~Flyweight()
	{
		delete shared_state_;
	}
	SharedState* shared_state() const
	{
		return shared_state_;
	}
	//void Operation(const UniqueState& unique_state) const
	//{
	//	//TODO
	//	//std::cout << "Flyweight: Displaying shared (" << *shared_state_ << ") and unique (" << unique_state << ") state.\n";
	//}
};

class FlyweightFactory
{
	/**
	 * @var Flyweight[]
	 */
private:
	std::unordered_map<std::string, Flyweight> flyweights_;
	
	/**
	 * Возвращает хеш строки Легковеса для данного состояния.
	 */

	std::string GetKey(const SharedState& ss) const
	{
		unsigned int hash_bytes = 0;
		for (int i = 0; i < ss.bytes.size(); i++) hash_bytes += ss.bytes[i];
		return ss.name + "_" + std::to_string(((int)(ss.dataType))) + "_" + std::to_string(hash_bytes);
	}



public:
	FlyweightFactory(std::initializer_list<SharedState> share_states)
	{
		for (const SharedState& ss : share_states)
		{
			this->flyweights_.insert(std::make_pair<std::string, Flyweight>(this->GetKey(ss), Flyweight(&ss)));
		}
	}
	FlyweightFactory()
	{
	}

	void AddFlyweight(const SharedState& shared_state)
	{
		this->flyweights_.insert(std::make_pair<std::string, Flyweight>(this->GetKey(shared_state), Flyweight(&shared_state)));
	}
	/**
	 * Возвращает существующий Легковес с заданным состоянием или создает новый.
	 */
	Flyweight& GetFlyweight(const SharedState& shared_state)
	{
		std::string key = this->GetKey(shared_state);
		if (this->flyweights_.find(key) == this->flyweights_.end())
		{
			std::cout << "FlyweightFactory: Can't find a flyweight, creating new one.\n";
			this->flyweights_.insert(std::make_pair(key, Flyweight(&shared_state)));
		}
		else
		{
			std::cout << "FlyweightFactory: Reusing existing flyweight.\n";
		}
		return this->flyweights_.at(key);
	}
	void ListFlyweights() const
	{
		size_t count = this->flyweights_.size();
		std::cout << "\nFlyweightFactory: I have " << count << " flyweights:\n";
		for (std::pair<std::string, Flyweight> pair : this->flyweights_)
		{
			std::cout << pair.first << "\n";
		}
	}
};

struct UniqueState
{
	Flyweight& flyWeight;
	std::string partName;
	double startTime;
	double endTime;

	UniqueState(const std::string& _partName, double _startTime, double _endTime, Flyweight& fl)
		: partName(_partName), startTime(_startTime), endTime(_endTime), flyWeight(fl)
	{

	}

	void Play()
	{

	}

	/*friend std::ostream& operator<<(std::ostream& os, const UniqueState& us)
	{
		return os << "[ " << us.owner_ << " , " << us.plates_ << " ]";
	}*/
};

std::vector<UniqueState*> trakList;

void addInPlayMemory(FlyweightFactory& ff, SharedState::Type type, std::string name, std::vector<unsigned char> bytes)
{
	ff.AddFlyweight({ name,bytes,type });
}

void addInPlayList(FlyweightFactory& ff, SharedState::Type type, std::string name, std::vector<unsigned char> bytes, std::string namepart, double start,double end)
{
	trakList.push_back(new UniqueState(namepart, start, end, ff.GetFlyweight({ name,bytes,type })));
}



int main()
{
	FlyweightFactory* factory = new FlyweightFactory();
	factory->ListFlyweights();
	addInPlayMemory(*factory, SharedState::AMV, "bmth - suicide season", std::vector<unsigned char>());
	factory->ListFlyweights();
	addInPlayList(*factory, SharedState::AMV, "bmth - suicide season", std::vector<unsigned char>(), "start", 0, 60.3); //зачем писать одно и то же?)

    std::cout << "Hello World!\n";
}

// Запуск программы: CTRL+F5 или меню "Отладка" > "Запуск без отладки"
// Отладка программы: F5 или меню "Отладка" > "Запустить отладку"

// Советы по началу работы 
//   1. В окне обозревателя решений можно добавлять файлы и управлять ими.
//   2. В окне Team Explorer можно подключиться к системе управления версиями.
//   3. В окне "Выходные данные" можно просматривать выходные данные сборки и другие сообщения.
//   4. В окне "Список ошибок" можно просматривать ошибки.
//   5. Последовательно выберите пункты меню "Проект" > "Добавить новый элемент", чтобы создать файлы кода, или "Проект" > "Добавить существующий элемент", чтобы добавить в проект существующие файлы кода.
//   6. Чтобы снова открыть этот проект позже, выберите пункты меню "Файл" > "Открыть" > "Проект" и выберите SLN-файл.
