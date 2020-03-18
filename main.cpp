#include <iostream>
using namespace std;

class Container
{
public:
    // Виртуальные методы, должны быть реализованы вашим контейнером
    virtual void insert(int value) = 0;
    virtual bool exists(int value) = 0;
    virtual void remove(int value) = 0;

    // И этот тоже, хотя к нему потом ещё вернёмся
    virtual void print() = 0;
    
    // Виртуальный деструктор (пока просто поверьте, что он нужен)
    virtual ~Container() { };
};

class Vessel
{
private:
	int id;
	int level;
	Vessel* parent;
	Vessel* left;
	Vessel* right;
public:
	Vessel(int i) {
		this->id = i;
		this->level = 0;
		this->parent = NULL;
		this->left = NULL;
		this->right = NULL;
	}

	//~Vessel();

	int getID() {
		return this->id;
	}

	Vessel* attach_to_fleet(Vessel* ship) {
		if(this->id > ship->id) 
		{
			if (this->left == NULL) {
				this->left = ship;
				ship->parent = this;
				return this;
			}
			else this->left->attach_to_fleet(ship);
		}
		else
		{
			if (this->right == NULL) {
				this->right = ship;
				ship->parent = this;
				return this;
			}
			else this->right->attach_to_fleet(ship);
		}
		// this->skew();
		// this->split();
		return this;
	}

	// Vessel* skew() {
	// 	if (this->left != NULL)
	// 		if (this->level == this->left->level) {
	// 			Vessel* k = this->left;
	// 			if (this->parent != NULL)
	// 			{
	// 				k->parent = this->parent;
	// 				if (this == k->parent->right)
	// 					k->parent->right = k;
	// 				else k->parent->left = k;
	// 			}
	// 			this->parent = k;
	// 			this->left = k->right;
	// 			if (this->left != NULL)
	// 				this->left->parent = this;
	// 			k->right = this;
	// 			return k;
	// 		}
	// 	return this;
	// }

	// Vessel* split() {
	// 	if(this->right != NULL)
	// 		if(this->right->right != NULL)
	// 			if(this->right->right->level == this->level)
	// 			{
	// 				Vessel* r = this->right;
	// 				if (this->parent != NULL) {
	// 					r->parent = this->parent;
	// 					this->parent = r;
	// 					if (this == r->parent->left)
	// 						r->parent->left = r;
	// 					else r->parent->right = r;
	// 				}
	// 				this->right = r->left;
	// 				if (this->right != NULL)
	// 					this->right->parent = this;
	// 				r->left = this;
	// 				r->level++;
	// 				return r;
	// 			}
	// 	return this;
	// }

	bool find_vessel(int i) {
		if (this->id == i)
			return true;
		else if (this->id > i && this->left != NULL)
			this->left->find_vessel(i);
		else if (this->id < i && this->right != NULL)
			this->right->find_vessel(i);
		return false;
	}

	Vessel* detach_from_fleet(int target_id) {
		if (target_id < this->id)
			this->left->detach_from_fleet(target_id);
		else if (target_id > this->id)
			this->right->detach_from_fleet(target_id);
		else {
			if (this->left == NULL) {
				if (this->parent != NULL) {
					this->parent->right = this->right;
					if (this->right != NULL)
						this->right->parent = this->parent;
				}
				delete this;
			} else {
				Vessel* replaced = this->left;
				while (replaced->right != NULL)
					replaced = replaced->right;
				this->id = replaced->id;
				replaced->parent->right = NULL;
				delete replaced;
				// while(t != this) {
				// 	t = skew(t);
				// 	t = split(t);
				// 	t = t->parent;
				// }
			}
		}
		return this;
	}

	void report() {
		if (this->left != NULL)
			this->left->report();
		cout << this->getID() << endl;
		if(this->right != NULL)
			this->right->report();
	}

	void dismiss_fleet() {
		while (this->left != NULL)
			this->left->detach_from_fleet(this->left->id);
		while (this->right != NULL)
			this->right->detach_from_fleet(this->right->id);
	}
};

class Fleet: public Container
{
private:
	Vessel* root;
protected:

	void attach_to_fleet(Vessel* ship) {
		if (root == NULL) {
			root = ship;
			return;
		}
		this->root = root->attach_to_fleet(ship);
	}

	bool find_vessel(int i) {
		return this->root->find_vessel(i);
	}

	void detach_from_fleet(int target_id) {
		if (this->find_vessel(target_id) == true)
			this->root = root->detach_from_fleet(target_id);
	}

	void dismiss_fleet() {
		root->dismiss_fleet();
		delete root;
	}

public:
	Fleet() {
		this->root = NULL;
	}
	~Fleet() {
		dismiss_fleet();
	}

	void insert(int value) {
		Vessel* t = new Vessel(value);
		this->attach_to_fleet(t);
	}

    bool exists(int value) {
    	return this->find_vessel(value);
    }

    void remove(int value) {
    	this->root->detach_from_fleet(value);
    }

    void print() {
    	this->root->report();
    }
};


int main()
{
    Container* c = new Fleet();

    for(int i = 1; i < 10; i++)
        c->insert(i*i);

    cout << boolalpha;
    cout << "Value 25 found: " << c->exists(25) << endl;
    cout << "Value 111 found: " << c->exists(111) << endl;

    delete c;
    return 0;
}
