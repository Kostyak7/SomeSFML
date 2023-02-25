#ifndef NNSNAKE_H
#define NNSNAKE_H

#include <iostream>
#include <cmath>
#include <cstdlib>
#include <vector>
#include <list>

class Net {
public:
	unsigned int in_features = 625, hidden_size_0, hidden_size_1, hidden_size_2;
	unsigned int hidden_size_3, num_classes = 4;
	Net(unsigned int In_features, unsigned int Hidden_size_0,  unsigned int Hidden_size_1,
		unsigned int Hidden_size_2, unsigned int Hidden_size_3) {
		in_features = In_features;
		hidden_size_0 = Hidden_size_0; hidden_size_1 = Hidden_size_1; 
		hidden_size_2 = Hidden_size_2; hidden_size_3 = Hidden_size_3;
	}

	int forward() {

	}

};

#endif NNSNAKE_H