#include <iostream>;
#include <format>;
#include "LinkedList.h"
#include "Node.h"


int main() {
	std::cout << "Hello World\n";
	LinkedList ll;

	ll.insertAtBeginning(1);
	ll.insertAtBeginning(2);
	ll.insertAtBeginning(3);
	ll.insertAtBeginning(4);

	ll.display();

	return 0;
}