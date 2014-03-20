#include <iostream>
#include <cstdlib>
#include <getopt.h>
#include <cctype>
#include <sstream>
#include <random>
#include <cmath>
#include <unordered_map>
#include <queue>
#include <climits>

using namespace std;
struct Order{
	int time, price, quantity, duration;
	string name, offer, equity;
	unsigned int ID;
}order;
struct sellComp {
	bool operator()(const Order *a, const Order *b) const {
		if(a->price < b->price) return true;
		if(a->price == b->price){
			if(a->ID < b->ID) return true;
			}
		return false;
	}
};
struct buyComp {
	bool operator()(const Order *a, const Order *b) const {
		if(a->price > b->price) return true;
		if(a->price == b->price){
			if(a->ID < b->ID) return true;
			}
		return false;
	}
};
bool matchq(const priority_queue<Order*,vector<Order*>,sellComp> *a,
		const priority_queue<Order*,vector<Order*>,buyComp> *b);

int main(int argc, char * argv[]){
	cout.sync_with_stdio(false);
	//cout << "int limits: min: " << INT_MIN << " max: " << INT_MAX << endl;




	const struct option longopts[] =
    {
            {"verbose", 0, 0, 'v'},
            {"help", 0, 0, 'h'},
            {"midpoint", 0, 0, 'p'},
            {"median", 0, 0, 'm'},
            {"transfers", 0, 0, 't'},
            {"VWAP", 0, 0, 'w'},
            {"ttt", 0, 0, 'g'},
            {0,0,0,0},
    };
    int index;
    int iarg=0;


    //turn off getopt error message
    opterr=1;
    bool verbose, midpoint, transfers, vwap, ttt, median;
    verbose = midpoint = transfers = vwap = ttt = median = false;
    string garg;

    while(iarg != -1){
        iarg = getopt_long(argc, argv, "g:vhpmtw", longopts, &index);
        switch (iarg)
        {
            case 'h':
            	cout << "-h, --help: help message\n" << flush
            	<< "-m, --median: print current median " << flush
            	<< "match price for all equities\n" << flush
            	<< "-p, --midpoint: print current midpoint price\n" << flush
            	<< "-t, --transfers: print transfer information " << flush
            	<< "at the end of the day\n" << flush
            	<< "-v, --verbose: print verbose information " << flush
            	<< "while trades are being made\n" << flush
            	<< "-w, --VWAP: print VWAP of all equities " << flush
            	<< "at the end of the day\n" << flush
            	<< "-g, --ttt <EQUITY_SYMBOL>: prints when the best time to" << flush
            	<< " buy and sell EQUITY_SYMBOL was\n" << flush;
            	return 0;
                break;
            case 'v':
                cout << "VERBOSE OPTION SELECTED\n" << flush;
                verbose = true;
                break;
            case 'g':
                cout << "TTT OPTION SELECTED\n" << flush;
                cout << "THE OPTARG IS " << optarg << "\n" << flush;
                break;
            case 'm':
                cout << "MEDIAN OPTION SELECTED\n" << flush;
                median = true;
                break;
            case 'p':
                cout << "MIDPOINT OPTION SELECTED\n" << flush;
                midpoint = true;
                break;
            case 'w':
                cout << "VWAP OPTION SELECTED\n" << flush;
                vwap = true;
                break;
            case 't':
                cout << "TRANSFERS OPTION SELECTED\n" << flush;
                transfers = true;
                break;
            case '?':
            	cerr << "incorrect flag or no flag\n" << flush;
            	exit(1);
            	break;
        }
    }
    int currentTime = 0;
    char dollarSign, numberSign;
    string mode;
    priority_queue<Order*,vector<Order*>,sellComp> sellq;
    priority_queue<Order*,vector<Order*>,buyComp> buyq;
    unordered_map<string,priority_queue<Order*,vector<Order*>,sellComp>> sellmap;
    unordered_map<string,priority_queue<Order*,vector<Order*>,buyComp>> buymap;
    cin >> mode;
    cin.ignore(1,'\n');
    if(mode != "TL" && mode != "PR"){
    	cout << "invalid mode " << mode << "\n" << flush;
    	exit(1);
    }
	//cout << mode << "\n" << flush;
    ///////////// TL input ///////////////////
	if(mode == "TL"){
	    cin >> order.time >> order.name >> order.offer >> order.equity
	        >> dollarSign >> order.price >> numberSign >> order.quantity
	        >> order.duration;
	    order.ID = 0;
		while (!cin.fail()) {
			if (order.time < currentTime) {
				cout << "time stamps out of order\n" << flush;
				exit(1);
			}
			if(order.time > currentTime){
				currentTime = order.time;
			}
			if(order.time > INT_MAX || order.duration > INT_MAX){
				cout << "time or dur out of range\n" << flush;
				exit(1);
			}
			if(order.duration < -1){
				cout << "duration < -1\n" << flush;
				exit(1);
			}
			for (int i = 0; i < order.name.length(); ++i) {
				if (!isalnum(order.name[i]) && order.name[i] != '_') {
					cout << "name is not alphanumeric\n" << flush;
					exit(1);
				}
			}
			if (order.offer != "BUY" && order.offer != "SELL") {
				cout << "string must be BUY or SELL\n" << flush;
				exit(1);
			}
			if (order.equity.length() > 5) {
				cout << "equity symbol has too many characters\n" << flush;
				exit(1);
			}
			for (int i = 0; i < order.equity.length(); ++i) {
				if (!isalnum(order.equity[i]) && order.equity[i] != '_') {
					cout << "equity is not alphanumeric\n" << flush;
					exit(1);
				}
			}
			if(dollarSign != '$'){
				cout << "dollar sign wrong\n" << flush;
				exit(1);
			}
			if(numberSign != '#'){
				cout << "dollar sign wrong\n" << flush;
				exit(1);
			}
			if (order.price < 0) {
				cout << "price < 0\n" << flush;
				exit(1);
			}
			if (order.quantity < 0) {
				cout << "quantity < 0\n" << flush;
				exit(1);
			}

			//update queues
			if(order.offer == "BUY"){
				buyq.push(&order);
				buymap[order.equity] = buyq;
				//cout << buymap[order.equity].top()->name << endl;
			}
			if(order.offer == "SELL"){
				sellq.push(&order);
				sellmap[order.equity] = sellq;
				//cout << buymap[order.equity].top()->offer << endl;
			}
			//check for matches
			if(matchq(&sellmap[order.equity],&buymap[order.equity])){
				//cout << "found a match. sell name: "
						//<< sellmap[order.equity].top()->name << " buy name: "
						//<< buymap[order.equity].top()->name << endl;
				sellmap[order.equity].pop();
				buymap[order.equity].pop();
			}



			cin >> order.time >> order.name >> order.offer >> order.equity
			    >> dollarSign >> order.price >> numberSign >> order.quantity
			    >> order.duration;
			++order.ID;

		}
	}
	else if(mode == "PR"){
		unsigned int seed, orders, genTime = 0;
		double rate;
		char lastClient, lastEquity, trash;
		string junk, C = "C_", E = "E_";
		Order order;
		cin >> junk >> seed >> junk >> orders >> junk >> lastClient >> junk
		    >> lastEquity >> junk >> rate;
		std::mt19937 gen(seed);
		std::uniform_int_distribution<char> clients('a', lastClient);
		std::uniform_int_distribution<char> equities('A', lastEquity);
		std::exponential_distribution<> arrivals(rate);
		std::bernoulli_distribution buy_or_sell;
		std::uniform_int_distribution<> price(2,11);
		std::uniform_int_distribution<> quantity(1,30);
		std::uniform_int_distribution<> duration(-1,floor(orders/rate));

		for (unsigned int i = 0; i < orders; ++i) {
			order.ID = i;
			order.time = genTime;
			genTime += floor(arrivals(gen) + .5);
			C.push_back(clients(gen));
			order.name = C;
			C = "C_";
			order.offer = buy_or_sell(gen) ? "BUY" : "SELL";
			E.push_back(equities(gen));
			order.equity = E;
			E = "E_";
			order.price = 5 * price(gen);
			order.quantity = quantity(gen);
			order.duration = duration(gen);

			cout << order.time << " " << order.name << " " << order.offer
					<< " " << order.equity << " $" << order.price << " #"
					<< order.quantity << " " << order.duration << "\n";
		}
	}
    if(cin.eof()){
    	//cout << "end of file reached\n" << flush;
    }else if(cin.fail()){
    	cout << "end of file not reached, invalid input\n" << flush;
    	cin.clear();
    	exit(1);
    }
    if(verbose){

    }
    if(median){

    }
    if(midpoint){

    }
    if(transfers){

    }
    if(vwap){

    }
    if(ttt){

    }
	return 0;
}

bool matchq(const priority_queue<Order*,vector<Order*>,sellComp> *a,
		const priority_queue<Order*,vector<Order*>,buyComp> *b){
	if(a->empty() || b->empty()) return false;
	if(a->top()->price <= b->top()->price) return true;
	return false;
}
