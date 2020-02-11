#include <iostream>
using namespace std;

class vessel
{
public:
	vessel(int i);
	//~vessel();
	vessel* attach_to_fleet(vessel* ship);
	vessel* skew();
	vessel* split();
private:
	int id;
	int level;
	vessel* parent;
	vessel* left;
	vessel* right;
};

vessel::vessel(int i)
{
	this->id = i;
	this->level = 0;
	this->parent = NULL;
	this->left = NULL;
	this->right = NULL;
}

vessel* vessel::attach_to_fleet(vessel* ship)
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

vessel* vessel::skew()
{
	if (this->left != NULL)
	if (this->level == this->left->level) {
		vessel* k = this->left;
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

vessel* vessel::split()
{
	if(this->right != NULL)
	if(this->right->right != NULL)
	if(this->right->right->level == this->level)
	{
		vessel* r = this->right;
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


class fleet
{
public:
	fleet(vessel* r);
	//~fleet();
	void attach_to_fleet(vessel* ship);
private:
	vessel* root;
};

fleet::fleet(vessel* r)
{
	this->root = r;
}

void fleet::attach_to_fleet(vessel* ship)
{
	this->root = root->attach_to_fleet(ship);
}


int main() {
	vessel* normandy = new vessel(10);
	vessel* luna = new vessel(5);
	vessel* enterprise = new vessel(15);
	fleet my_fleet(normandy);
	my_fleet.attach_to_fleet(luna);
	return 0;
}