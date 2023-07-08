#include <iostream>
#include <vector>
#include <algorithm>
#include <string>
#include <fstream>
#include <map>
#include <sstream>
#include <iomanip>
#include <assert.h>
typedef long long ll;
using namespace std;

#define EPS 1e-6

vector<string> split(string line, char sep)
{
    vector<size_t> sepIds;
    for (size_t i = 0; i < line.size(); i++)
        if (line.at(i) == sep)
            sepIds.push_back(i);
    sepIds.push_back (line.size ());

    vector<string> wordsList;
    size_t j = 0;
    for (size_t sepId : sepIds)
    {
        wordsList.push_back(line.substr(j, sepId - j));
        j = sepId + 1;
    }

    return wordsList;
}

int main()
{
    ifstream input ("purchase_list.txt", std::ios_base::in);
    
    map<string, double> alreadySpent;
    map<string, double> shouldSpent;

    cout << "Таблица трат:\n";
    char line[1000] = "";
    input.getline (line, 1000);
    do 
    {
        vector<string> words = split(string(line), ' ');
        
        string buyer = words.front();
        double cost = atof(words.at(1).c_str());
        double n = words.size() - 2;
        string names = "";

        alreadySpent[buyer] += cost;
        for (size_t i = 2; i < words.size(); i++)
        {
            names += words.at(i) + " ";
            shouldSpent[words.at(i)] += cost / n;
        }   
        cout << buyer << " потратил(a) " << cost << " рублей. Делят: " << names << endl;

        input.getline (line, 1000);
    } while (line[0] != 0);
    input.close();

 

    typedef pair<string, double> Balance;
    vector <Balance> balance;
    for (auto curPair : shouldSpent)
    {
        string name = curPair.first;
        double spent = alreadySpent[name];
        double should = shouldSpent[name];
        balance.push_back ({curPair.first, should - spent});
    }
    
    size_t begin = 0;
    size_t end = balance.size() - 1;
    sort(balance.begin(), balance.end(), [](Balance& a, Balance& b) {
        return a.second < b.second;
    });

    cout << endl << "Баланс участников: "  << endl;
    for (auto p : balance)
        cout << p.first << " " << p.second << endl;
    cout << endl << "Рекомендации: " << endl;
    vector<string> transactions;
    auto generateTransaction = [] (string from, string to, double money) -> string
    {
        stringstream ss;
        ss  << from << " скидывает " << to << " " << money << " рублей";
        return ss.str();
    };
    while (begin < end)
    {
        string fromName = balance.at(end).first;
        string toName = balance.at(begin).first;
        double& fromBal = balance.at(end).second;
        double& toBal = balance.at(begin).second;
        assert(fromBal > 0);
        assert(toBal < 0);

        double moneyToSend = min(fromBal, abs(toBal));
        fromBal -= moneyToSend;
        toBal += moneyToSend;
        if (abs(fromBal) < EPS)
            end--;
        if (abs(toBal) < EPS)
            begin++;
            
        transactions.push_back({generateTransaction(fromName, toName, moneyToSend)});
    }

    for (string rec : transactions)
        cout << rec << endl;
    return 0;
}