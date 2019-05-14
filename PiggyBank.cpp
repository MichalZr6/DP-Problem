// https://www.spoj.com/problems/PIGBANK/

#include <iostream>
#include <vector>
#include <algorithm>
#include <string>

#define MAX long(2600000)
#define TEST

std::vector<long> dp;

struct Currency
{
    Currency(int v, int w) : value(v), weight(w) { }

    int weight;
    int value;
};

class PBank
{
public:

    bool init_pb_prop();
    bool init_currencies();
    bool evaluate();

private:
    int empty_pb, full_pb;
    std::vector<Currency> currencies;
};

int main()
{
    int n_tests;
    std::vector<PBank> vpb;

#ifdef TEST
    std::cout << "Number of tests: ";
#endif // TEST

    std::cin >> n_tests;
    vpb.resize(n_tests);

    for (auto &pb : vpb)
    {
        if (pb.init_pb_prop() && pb.init_currencies() && pb.evaluate())
            continue;
        else
            std::cout << "This is impossible.\n";
    }

    return 0;

}

bool PBank::init_pb_prop()
{
#ifdef TEST
    std::cout << "\nEmpty and full pb weight: ";
#endif // TEST

    std::cin >> empty_pb >> full_pb;

    if (empty_pb < 1 || empty_pb > full_pb || full_pb > 10000)
        return false;
    else
    {
        dp.resize(full_pb - empty_pb + 1);
        return true;
    }
}

bool PBank::init_currencies()
{
    int curr_container_size, curr_val, curr_weight;

#ifdef TEST
    std::cout << "\nCurrencies number: ";
#endif // TEST

    std::cin >> curr_container_size;
    if (curr_container_size < 1 || curr_container_size > 500)
        return false;

    for (int j = 0; j < curr_container_size; j++)
    {
#ifdef TEST
        std::cout << "\nCurrency nr " << j + 1 << ": ";
#endif // TEST
        std::cin >> curr_val >> curr_weight;
        if (curr_val < 1 || curr_val > 50000 || curr_weight < 1 || curr_weight > 10000)
            return false;
        else
            currencies.push_back(Currency(curr_val, curr_weight));
    }

    std::sort(currencies.begin(), currencies.end(),
        [](const Currency &c1, const Currency &c2)
    {
        return (static_cast<double>(c1.value) / c1.weight) > (static_cast<double>(c2.value) / c2.weight);
    });

    return true;
}

bool PBank::evaluate()
{
    int div;

    std::vector<long> prev_dp(full_pb - empty_pb + 1);

    for (int wt = 0; wt <= full_pb - empty_pb; wt++)
    {
        for (auto i = 0; i < currencies.size(); i++)
        {
            if (wt == 0)
            {
                dp[wt] = 0;
                prev_dp[wt] = 0;
                continue;
            }

            div = wt / currencies[i].weight;

            if (i == 0)
            {
                if (wt % currencies[i].weight == 0)
                    dp[wt] = div * currencies[i].value;
                else
                    dp[wt] = MAX;

                prev_dp[wt] = dp[wt];

                continue;
            }

            if (wt - currencies[i].weight < 0)
                dp[wt] = std::min(prev_dp[wt], MAX);
            else
            {
                if (div > 1 && wt >= currencies[i].weight * div)
                    dp[wt] = std::min(prev_dp[wt], dp[wt - currencies[i].weight * div] + currencies[i].value * div);
                else
                    dp[wt] = std::min(prev_dp[wt], dp[wt - currencies[i].weight] + currencies[i].value);

                dp[wt] = std::min(dp[wt], prev_dp[wt - currencies[i].weight] + currencies[i].value);
            }
            prev_dp[wt] = dp[wt];
        }
    }
#ifdef TEST
    for (int c = 0; c < dp.size(); c++)
    {
        std::cout << dp[c] << "\t";
    }

    std::cout << std::endl;
#endif // TEST
    int last = dp[full_pb - empty_pb];
    if (last == MAX)
        return false;
    else
        std::cout << "The minimum amount of money in the piggy-bank is " << last << ".\n";

    return true;
}

