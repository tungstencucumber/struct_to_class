#include <iostream>
using namespace std;

class Vehicle
{
public:
    // Виртуальные методы, должны быть реализованы вашим контейнером
    virtual void create(int id) = 0;
    virtual bool exists(int id) = 0;
    virtual void remove(int id) = 0;

    // И этот тоже, хотя к нему потом ещё вернёмся
    virtual void report() = 0;
    
    // Виртуальный деструктор (пока просто поверьте, что он нужен)
    virtual ~Vehicle() { };
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
	Vessel(int i);
	//~Vessel();
	Vessel* attach_to_fleet(Vessel* ship);
	Vessel* skew();
	Vessel* split();
	bool find_vessel(int i);
};

Vessel::Vessel(int i)
{
	this->id = i;
	this->level = 0;
	this->parent = NULL;
	this->left = NULL;
	this->right = NULL;
}

Vessel* Vessel::attach_to_fleet(Vessel* ship)
{
	if(this->id > ship->id) 
	{
		if (this->left == NULL) {
			this->left = ship;
			ship->parent = this;
			return this;
		}
		this->left->attach_to_fleet(ship);
	}
	else
	{
		if (this->right == NULL) {
			this->right = ship;
			ship->parent = this;
			return this;
		}
		this->right->attach_to_fleet(ship);
	}
	this->skew();
	this->split();
}

Vessel* Vessel::skew()
{
	if (this->left != NULL)
	if (this->level == this->left->level) {
		Vessel* k = this->left;
		if (this->parent != NULL)
		{
			k->parent = this->parent;
			if (this == k->parent->right)
				k->parent->right == k;
			else k->parent->left == k;
		}
		this->parent = k;
		this->left = k->right;
		if (this->left != NULL)
			this->left->parent = this;
		k->right = this;
		return k;
	}
	return this;
}

Vessel* Vessel::split()
{
	if(this->right != NULL)
	if(this->right->right != NULL)
	if(this->right->right->level == this->level)
	{
		Vessel* r = this->right;
		if (this->parent != NULL) {
			r->parent = this->parent;
			this->parent = r;
			if (this == r->parent->left)
				r->parent->left = r;
			else r->parent->right = r;
		}
		this->right = r->left;
		if (this->right != NULL)
			this->right->parent = this;
		r->left = this;
		r->level++;
		return r;
	}
	return this;
}


bool Vessel::find_vessel(int i) {
	if (this->id == i)
		return true;
	if (this->id > i && this->left != NULL)
		this->left->find_vessel(i);
	if (this->id < i && this->right != NULL)
		this->right->find_vessel(i);
	return false;
}

Vessel* Vessel::detach_from_fleet(Vessel* ship) {
	if (ship->id < this->id)
		this->left->detach_from_fleet(ship);
	else if (ship->id > this->id)
		this->right->detach_from_fleet(ship);
	else {
		if (this->left == NULL) {
			if (this->parent != NULL) {
				this->parent->right = this->right;
				if (this->right != NULL)
					this->right->parent = this->parent;
			}
			delete this;
		} else {
			vessel *replaced = this->left;
			while (replaced->right != NULL)
				replaced = replaced->right;
			vessel* t = replaced->parent;
			this->id = replaced->id;
			replaced->parent->right = NULL;
			delete replaced;
			while(t != this) {
				t = skew(t);
				t = split(t);
				t = t->parent;
			}
		}
	}
	return this;
}


class Fleet: public Vehicle
{
public:
	Fleet(Vessel* r);
	//~fleet();
	void attach_to_fleet(Vessel* ship);
	bool find_vessel(int i);
	void detach_from_fleet(Vessel* ship);

	void create(int value) {
		Vessel t(value) = new Vessel;
		this->attach_to_fleet(t);
	}

    bool exists(int value) {
    	return this->find_vessel(value);
    }

    void remove(int value) {
    	this->root->detach_from_fleet(value);
    }

private:
	Vessel* root;
};

Fleet::Fleet(Vessel* r)
{
	this->root = r;
}

void Fleet::attach_to_fleet(Vessel* ship)
{
	this->root = root->attach_to_fleet(ship);
}

bool Fleet::find_vessel(int i)
{
	return this->root->find_vessel(i);
}

void Fleet::detach_from_fleet(Vessel* ship) {
	if (this->find_vessel(ship) == true)
		this->root = root->detach_from_fleet(ship);
}

int main() {

	return 0;
}