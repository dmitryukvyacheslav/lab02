#include <trie.hpp>
#include <iostream>

int main(void){
	Trie* t = new Trie();
	/*t->insert("ayumu", 156);
	auto query = t->lookup("ayumu");
	if (query)
		printf("Key 'ayumu' exists [%d]", query->value);
	else
		printf("Key not found");
	std::cout << std::endl;
	*/
	bool quit = false;

	/**
	* a <key> <value>
	* r <key>
	* l <key>
	**/
	char command;
	std::string key;
	int value;
	struct Trie::trie* query = nullptr;
	while (!quit) {
		std::cout << "d7trie> ";
		std::cin >> command;
		switch (command) {
			case ('a'):
				std::cin >> key;
				std::cin >> value;
				t->insert(key.c_str(), value);
				printf("Key '%s'[%d] added.", key.c_str(), value);
			break;

			case('l'):
				std::cin >> key;
				query = t->lookup(key.c_str());
				if (query != nullptr && query->value)
					printf("Key '%s' exists [%d].", key.c_str(), query->value);
				else
					printf("Key not found.");
			break;

			case('r'):
				std::cin >> key;
				query = t->lookup(key.c_str());
				if (query != nullptr && query->value) {
					int temp = query->value;
					t->remove(key.c_str());
					printf("Key '%s'[%d] removed.", key.c_str(), temp);
				}
				else
					printf("Key not found.");
			break;
			case('p'):
				t->print();
				break;
			case('q'):
				quit = true;
		}
		std::cout << std::endl;
	}
	delete t;
	return 0;
}