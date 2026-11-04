#include "knapsack-dp.h"
#include <iostream>
#include <numeric>

////////////////////////////////////////////////////////////
Item::Item( int const& weight, int const& value ) 
	: weight(weight), value(value) 
{
}

////////////////////////////////////////////////////////////
Item::Item( Item const& original ) 
	: weight(original.weight), value(original.value)
{
}

////////////////////////////////////////////////////////////
std::ostream& operator<< (std::ostream& os, Item const& item) {
	os << "(" << item.weight << " , " << item.value << ") ";
	return os;
}

////////////////////////////////////////////////////////////
std::istream& operator>> (std::istream& os, Item & item) {
	os >> item.weight >> item.value;
	return os;
}

////////////////////////////////////////////////////////////
typedef std::vector< std::vector<int> > Table; //2-dimensional table

////////////////////////////////////////////////////////////
//the returned value is a vector of indices
std::vector<int> knapsackDP( std::vector<Item> const& items, int const& W ) {
	int num_items = items.size();

    /*  ........... */
    Table table(W + 1, std::vector<int>(num_items + 1, 0));


    int itemMatrixNum = table[0].size();
    int weightMatrixNum = table.size();

    for(int w = 1; w < weightMatrixNum; ++w)
    {
        for(int i = 1; i < itemMatrixNum; ++i)
        {
        
            if(w < items[i-1].weight)
            {
                table[w][i] = table[w][i-1];
            }
            else
            {
                int estimateWeight = table[w-items[i-1].weight][i-1] + items[i-1].value;
                table[w][i] = table[w][i-1] > estimateWeight ? table[w][i-1] : estimateWeight;
            }
        }
    }

	//print final table - for debugging?
    //do not delete this code
    if ( num_items + W < 50 ) { //print only if table is not too big
        std::cout << "   ";
        for ( int n=0; n<=num_items; ++n) { std::cout << n << "     "; }
        std::cout << "  items\n        ";
        for ( int n=0; n<num_items; ++n) { std::cout << items[n].weight << "," << items[n].value<<"   "; }
        std::cout << "\n   ";
        for ( int n=0; n<=num_items; ++n) { std::cout << "------"; }
        std::cout << std::endl;

        for ( int w=0; w<=W; ++w) {
            std::cout << w << "| ";
            for ( int n=0; n<=num_items; ++n) {
                std::cout << table[w][n] << "     ";
            }
            std::cout << std::endl;
        }
    }
    //end do not delete this code 


	//figure out which items are in the bag based on the table
	std::vector<int> bag;
    /*  ........... */
    int weightIdx = W;
    int itemIdx = num_items;
    while(table[weightIdx][itemIdx] != 0)
    {
        if(table[weightIdx][itemIdx] != table[weightIdx][itemIdx - 1])
        {
            bag.push_back(itemIdx-1);
            weightIdx -= items[itemIdx - 1].weight;
        }
        --itemIdx;
    }
	return bag;
}

////////////////////////////////////////////////////////////
int valueBag( std::vector<Item> const& items, std::vector<int> const& bag ) {
	std::vector<int>::const_iterator it   = bag.begin(),
		                          it_e = bag.end();

	int accum = 0;
	//std::cout << "Bag ";
	for ( ; it != it_e; ++it) { 
		accum += items[ *it ].value; 
		//std::cout << *it << " ";
	}
	//std::cout << std::endl;
	return accum;
}

////////////////////////////////////////////////////////////
//prototype
//notice that auxiliary function returns value of the vector of items
//the actual vector is determined later from the table (similar to DP solution)
int knapsackRecMemAux( std::vector<Item> const&, int const&, int, Table& );

////////////////////////////////////////////////////////////
//function to kick start
std::vector<int> knapsackRecMem( std::vector<Item> const& items, int const& W ) {
	int num_items = items.size();

    /* ........... */
    Table table(W + 1, std::vector<int>(num_items + 1, -1));
    for(int i = 0; i <= num_items; ++i)
    {
        table[0][i] = 0;
    }
    for(int i = 0; i <= W; ++i)
    {
        table[i][0] = 0;
    }

    knapsackRecMemAux(items, W, num_items, table);

	//print table - debugging?
    //do not delete this code
    if ( num_items + W < 50 ) { //print only if table is not too big
        std::cout << "   ";
        for ( int n=0; n<=num_items; ++n) { std::cout << n << "     "; }
        std::cout << "  items\n        ";
        for ( int n=0; n<num_items; ++n) { 
            std::cout << items[n].weight << "," << items[n].value<<"   "; 
        }
        std::cout << "\n   ";
        for ( int n=0; n<=num_items; ++n) { std::cout << "------"; }
        std::cout << std::endl;

        for ( int w=0; w<=W; ++w) {
            std::cout << w << "| ";
            for ( int n=0; n<=num_items; ++n) {
                std::cout << table[w][n] << "     ";
            }
            std::cout << std::endl;
        }
    }
    //end do not delete this code 

	//figure out which items are in the bag based on the table
	std::vector<int> bag;
    /* ........... */
    int weightIdx = W;
    int itemIdx = num_items;
    while(table[weightIdx][itemIdx] != 0)
    {
        if(table[weightIdx][itemIdx] != table[weightIdx][itemIdx - 1])
        {
            bag.push_back(itemIdx-1);
            weightIdx -= items[itemIdx - 1].weight;
        }
        --itemIdx;
    }

	return bag;
}

////////////////////////////////////////////////////////////
//the real recursive function
int knapsackRecMemAux( std::vector<Item> const& items, int const& W, int index, Table & table ) {
    /* ........... */
    if(table[W][index] != -1) return table[W][index];

    if(W < items[index - 1].weight)
    {
        table[W][index] = knapsackRecMemAux(items, W, index - 1, table);
    }
    else
    {
        int notPutItem = knapsackRecMemAux(items, W, index - 1, table);
        int putItem = knapsackRecMemAux(items, W - items[index - 1].weight, index - 1, table) + items[index - 1].value;
        table[W][index] = putItem > notPutItem ? putItem : notPutItem;
    }
    return table[W][index];
}
////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////
