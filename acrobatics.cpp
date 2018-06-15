#include <iostream>
#include <string>
#include <map>
#include <vector>
#include <algorithm>
using namespace std;

int gcd(int a, int b) { return (a % b ? gcd(b, a % b) : b); }

// 翻手算法 将一个n元一维向量向左旋转i个位置
void reverStr(string& str, int begin, int end) {
    char tmp;
    while (begin < end) {
        swap(str[begin], str[end]);
        begin++;
        end--;
    }
}

void rev(string& str, int i) {
    reverStr(str, 0, i - 1);
    cout << "1: " << str << endl;
    reverStr(str, i, str.size() - 1);
    cout << "2: " << str << endl;
    reverStr(str, 0, str.size() - 1);
    cout << "3: " << str << endl;
}

//给定一个英语字典 找出其中所有的变位词集合
void addWord(map<string, vector<string> >& dict, string word) {
    string w = word;
    sort(w.begin(), w.end());
    dict[w].push_back(word);
}
void lookup(map<string, vector<string> >& dict, string word) {
    sort(word.begin(), word.end());
    if (dict.count(word) == 0) {
        cout << "no such result" << endl;
        return;
    }

    for (auto item : dict[word]) {
        cout << item << endl;
    }
}

int partition(vector<int>& vec, int low, int high) {
    if (vec.empty())
        return -1;
    int flag = vec[low];
    while (low < high) {
        while (low < high && vec[high] >= flag) high--;
        vec[low] = vec[high];
        while (low < high && vec[low] <= flag) low++;
        vec[high] = vec[low];
    }
    vec[low] = flag;
    return low;
}

int getTopk(vector<int>& vec, int k) {
    if (vec.empty())
        return -1;
    int low = 0;
    int high = vec.size() - 1;
    while (high > low) {
        int index = partition(vec, low, high);
        cout << "index: " << index <<endl;
        if (k - 1 > index) {
            low = index + 1;
        }
        else if (k - 1 < index) {
            high = index - 1;
        }
        else {
            return index;
        }
    }
}


int main() {
    string str = "abcdefghijklmn";
    rev(str, 3);
    cout << "after reverse:" << str << endl;


    map<string, vector<string> > dictory;
    addWord(dictory, "abcd");
    addWord(dictory, "bacd");
    addWord(dictory, "cabd");
    addWord(dictory, "dcva");
    lookup(dictory, "abcd");

    int arr[] = {2, 4, 7, 5, 11, 3, 5, 6, 32, 5, 2, 4};
    vector<int> vec(arr, arr + 12);

    int kth = getTopk(vec, 3);
    cout << "index:" << kth << " kthNum: " << vec[kth] << endl;
    for (auto item : vec) {
        cout << item << " ";
    }
    cout << endl;
    return 0;
}
